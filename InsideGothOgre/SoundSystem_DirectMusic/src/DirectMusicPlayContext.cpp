#include "Precompiled.h"
#include "DirectMusicPlayContext.h"
#include "DirectMusicPlayer.h"
#include "DXError.h"


namespace GothOgre
{
	//--------------------------------------------------------------------------
	// Internal class which stores a map 
	// IDirectMusicSegmentState* -> DirectMusicPlayContext
	//--------------------------------------------------------------------------
	class SegmentStatePlayContextMap
	{
	public:
		SegmentStatePlayContextMap()
		{
			mMutex = ::CreateMutex(NULL, FALSE, NULL);
		}

		~SegmentStatePlayContextMap()
		{
			::CloseHandle(mMutex);
		}

		void insert(IDirectMusicSegmentState* _state, DirectMusicPlayContext* _context)
		{
			::WaitForSingleObject(mMutex, INFINITE);
			mMap.insert(std::make_pair(_state, _context));
			::ReleaseMutex(mMutex);
		}

		void remove(IDirectMusicSegmentState* _state)
		{
			::WaitForSingleObject(mMutex, INFINITE);
			mMap.erase(_state);
			::ReleaseMutex(mMutex);
		}

		DirectMusicPlayContext* find(IDirectMusicSegmentState* _state)
		{
			DirectMusicPlayContext* context = nullptr;
			::WaitForSingleObject(mMutex, INFINITE);
			Map::iterator it = mMap.find(_state);
			if(it != mMap.end())
				context = it->second;
			::ReleaseMutex(mMutex);
			return context;
		}
		
	private:
		typedef map<IDirectMusicSegmentState*, DirectMusicPlayContext*>::type Map;
		Map     mMap;
		HANDLE  mMutex;
	};
	static SegmentStatePlayContextMap sSSPCM;





	//--------------------------------------------------------------------------
	// DirectMusicPlayContext
	//--------------------------------------------------------------------------
	DirectMusicPlayContext::DirectMusicPlayContext(IDirectMusicPerformance8* _performance, IDirectMusicLoader8* _loader, HANDLE _event)
	{
		mPerformance = _performance;
		mLoader = _loader;
		mEvent = _event;
		mAudioPath = nullptr;
		mSegment = nullptr;
		mSegmentState = nullptr;
		mPlaying = false;
		mLooping = false;
		mStartMusicTime = 0;
		mEndMusicTime = 0;

		DXError::check(
			mPerformance->CreateStandardAudioPath(
				DMUS_APATH_SHARED_STEREOPLUSREVERB, // Type of the path
				64,   // Number of performance channels
				TRUE, // Activate the path on creation
				&mAudioPath ) ); // Receiver for a pointer to IDirectMusicAudioPath interface
	}
	//--------------------------------------------------------------------------
	DirectMusicPlayContext::~DirectMusicPlayContext()
	{
		stop();

		if(mAudioPath)
			mAudioPath->Release();
	}
	//--------------------------------------------------------------------------
	void DirectMusicPlayContext::play(bool _3D, const String& _filename, const String& _resourceGroup)
	{
		// Stop if played already
		stop();

		// Find the full path to the file.
		FileInfoListPtr fileinfos = ResourceGroupManager::getSingleton().findResourceFileInfo(_resourceGroup, _filename);
		if(fileinfos->empty())
			GOTHOGRE_EXCEPT(_filename << " - File not found.");
		FileInfo& fi = (*fileinfos)[0];
		String path = fi.archive->getName() + "/" + fi.path;

		// Convert the full path and the filename to UTF-16LE.
		CodepageConverter* utf16Conv = UnicodeUtil::openConverter("UTF-16LE");
		CodepageConverter::ByteBuffer bbFileName, bbPath;
		utf16Conv->unicodeToBuffer(bbFileName, fi.basename);
		utf16Conv->unicodeToBuffer(bbPath, path);
		bbPath.push_back(0);
		bbPath.push_back(0);
		bbFileName.push_back(0);
		bbFileName.push_back(0);
		WCHAR* wszPath = (WCHAR*) bbPath.data();
		WCHAR* wszFileName = (WCHAR*) bbFileName.data();

		// Set the search path.
		DXError::check(
			mLoader->SetSearchDirectory(
				GUID_DirectMusicAllTypes, // Types of files sought.
				wszPath, // Where to look.
				FALSE ) ); // Don't clear object data
		
		// Create a segment from the file.
		DXError::check(
			mLoader->LoadObjectFromFile(
				CLSID_DirectMusicSegment, // Class identifier.
				IID_IDirectMusicSegment8, // ID of desired interface.
				wszFileName,              // Filename.
				(LPVOID*) &mSegment ) );    // Pointer that receives interface.

		// Download instrumental data (DLS) to the audiopath.
		DXError::check(
			mSegment->Download( mAudioPath ) );

		// Begin playback of the segment.
		DXError::check(
			mPerformance->PlaySegmentEx(
				mSegment,       // Segment to play.
				NULL,           // Used for songs; not implemented.
				NULL,           // For transitions. 
				0,              // Flags.
				0,              // Start time; 0 is immediate.
				&mSegmentState, // Pointer that receives segment state.
				NULL,           // Object to stop.
				mAudioPath ) ); // Audiopath, if not default.

		// Write the new segment state to the static list
		sSSPCM.insert(mSegmentState, this);

		mPlaying = true;

		// Get playback length
		MUSIC_TIME mtLength;
		DXError::check( 
			mSegment->GetLength(&mtLength) );

		DXError::check( 
			mSegment->GetStartPoint(&mStartMusicTime) );

		mEndMusicTime = mtLength + mStartMusicTime;
	}
	//--------------------------------------------------------------------------
	void DirectMusicPlayContext::update()
	{
		if(::WaitForSingleObject(mEvent, 0) != WAIT_OBJECT_0)
			return;

		// Event signaled

		// Retrieve the notification event
		DMUS_NOTIFICATION_PMSG* message;
		DXError::check( 
			mPerformance->GetNotificationPMsg(&message) );

		if(message && 
			message->guidNotificationType == GUID_NOTIFICATION_SEGMENT && 
			message->punkUser != NULL)
		{
			// get a pointer to segment state
			IDirectMusicSegmentState* state = nullptr;
			DXError::check( 
				message->punkUser->QueryInterface(IID_IDirectMusicSegmentState8, (void**) &state) );
			
			// send the notification to corrent DirectMusicPlayContext
			DirectMusicPlayContext* context = sSSPCM.find(state);
			if(context)
				context->_update(message);
		}
	}
	//--------------------------------------------------------------------------
	void DirectMusicPlayContext::_update(DMUS_NOTIFICATION_PMSG* _message)
	{
		DWORD option = _message->dwNotificationOption;
		if(option == DMUS_NOTIFICATION_SEGLOOP)
		{
			if(!mLooping)
				stop();
		}
		else if(option == DMUS_NOTIFICATION_SEGEND || option == DMUS_NOTIFICATION_SEGABORT)
		{
			stop();
		}

		/*Debug (output debug string) 
		StrStream ss;
		ss << "playpos = " << getPlayPosition() << " / " << getPlayLength();
		ss << "\n";
		OutputDebugString(ss.str().c_str());
		//*/	
	}
	//--------------------------------------------------------------------------
	void DirectMusicPlayContext::stop()
	{
		mPlaying = false;

		// Release the segment state
		if(mSegmentState)
		{
			sSSPCM.remove(mSegmentState);

			// Stop playing
			DXError::check( 
				mPerformance->StopEx(
					mSegmentState, // Object to stop
					0,        // Stop time
					0 ) );    // Flags

			mSegmentState->Release();
			mSegmentState = nullptr;
		}

		// Unload and release the segment.
		if(mSegment)
		{
			// Unload instrumental data.
			// Unload must be called before releasing the segment.
			DXError::check(
				mSegment->Unload( mAudioPath ) );

			// Release the segment.
			mSegment->Release();
			mSegment = nullptr;
		}
	}
	//--------------------------------------------------------------------------
	bool DirectMusicPlayContext::isPlaying()
	{
		return mPlaying;
	}
	//--------------------------------------------------------------------------
	void DirectMusicPlayContext::setLooping(bool _looping)
	{
		mLooping = _looping;
		DWORD dwRepeats = _looping ? DMUS_SEG_REPEAT_INFINITE : 0;
		DXError::check(
			mSegment->SetRepeats( dwRepeats ) );
	}
	//--------------------------------------------------------------------------
	void DirectMusicPlayContext::setVolume(Real _volume)
	{
		long lVolume = _volume > 0.001f ? long( log10f(_volume) * 2000 ) : -9600;
		if(lVolume < -9600)
			lVolume = -9600;

		DXError::check(
			mAudioPath->SetVolume( lVolume, 0 ) );
	}
	//--------------------------------------------------------------------------
	Real DirectMusicPlayContext::getPlayLength()
	{
		/*
		return musicTimeToSeconds(mEndMusicTime - mStartMusicTime);
		//*/
		return 0;
	}
	//--------------------------------------------------------------------------
	Real DirectMusicPlayContext::getPlayPosition()
	{
		/*
		if(mSegmentState)
		{
			MUSIC_TIME currentMusicTime;
			DXError::check( 
				mSegmentState->GetSeek(&currentMusicTime) );

			return musicTimeToSeconds(currentMusicTime - mStartMusicTime);
		}
		//*/
		return 0;
	}
	//--------------------------------------------------------------------------
	Real DirectMusicPlayContext::musicTimeToSeconds(MUSIC_TIME mtTime)
	{
		REFERENCE_TIME rtTime;
		DXError::check(
			mPerformance->MusicToReferenceTime(mtTime, &rtTime) );

		return (Real) rtTime / (Real) 10000000.;
	}

} // namespace GothOgre