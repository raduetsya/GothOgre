#include "Precompiled.h"
#include "DirectMusicPlayer.h"
#include "DirectMusicPlayContext.h"
#include "DXError.h"


namespace GothOgre
{
	//-------------------------------------------------------------------------
	DirectMusicPlayer::DirectMusicPlayer()
		: SoundPlayer("DirectMusicPlayer")
	{
		mPerformance = nullptr;
		mLoader = nullptr;
		mEvent = NULL;
	}
	//-------------------------------------------------------------------------
	DirectMusicPlayer::~DirectMusicPlayer()
	{
	}
	//-------------------------------------------------------------------------
	void DirectMusicPlayer::initialise()
	{
		// Initialize COM
		DXError::check( 
			CoInitialize( NULL ) );

		// Create the central object of any DirectMusic application, 
		// which manages the playback of segments
		DXError::check(
			CoCreateInstance(
				CLSID_DirectMusicPerformance,
				NULL,
				CLSCTX_INPROC, 
				IID_IDirectMusicPerformance8,
				(void**) &mPerformance ) );

		// Initialise the performance
		DXError::check( 
			mPerformance->InitAudio(
				NULL,  // IDirectMusic interface not needed.
				NULL,  // IDirectSound interface not needed.
				NULL,  // Window handle.
				0,     // Default audiopath type: no default audio path.
				0,     // Number of performance channels; not used because the previos value is zero.
				DMUS_AUDIOF_ALL, // Requested features: all features.
				NULL ) ); // Parameters for the synthesizer: default parameters.

		// In order to load any object from disk, 
		// it's necessary to create the DirectMusicLoader object
		DXError::check( 
			CoCreateInstance(
				CLSID_DirectMusicLoader,
				NULL,
				CLSCTX_INPROC, 
				IID_IDirectMusicLoader8,
				(void**)&mLoader ) );

		mEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		mPerformance->SetNotificationHandle(mEvent, 0);
		mPerformance->AddNotificationType(GUID_NOTIFICATION_SEGMENT);
	}
	//-------------------------------------------------------------------------
	void DirectMusicPlayer::shutdown()
	{
		if(mPerformance)
		{
			mPerformance->SetNotificationHandle(NULL, 0);

			// If there is any music playing, stop it. This is 
			// not really necessary, because the music will stop when
			// the instruments are unloaded or the performance is
			// closed down.
			DXError::check(
				mPerformance->Stop( 
					NULL, // Stop all segments.
					NULL, // Stop all segment states.
					0,    // Do it immediately.
					0 ) );  // Flags.
		 
			// Close down the performance object.
			DXError::check( 
				mPerformance->CloseDown() );

			// Release the performance object.
			mPerformance->Release();
			mPerformance = nullptr;
		}

		if(mEvent)
		{
			::CloseHandle(mEvent);
			mEvent = NULL;
		}

		// Release the loader object.
		if(mLoader)
		{
			mLoader->Release();
			mLoader = nullptr;
		}
	 
		// Release COM.
		CoUninitialize();
	}
	//-------------------------------------------------------------------------
	void DirectMusicPlayer::update(const Vector3& _listenerPosition, 
		const Quaternion& _listenerOrientation)
	{
		// DirectMusic can produce 2D sounds only,
		// so the listener's position is not used.
	}
	//-------------------------------------------------------------------------
	SoundPlayContext* DirectMusicPlayer::createPlayContext()
	{
		return new DirectMusicPlayContext(mPerformance, mLoader, mEvent);
	}
	//-------------------------------------------------------------------------
	bool DirectMusicPlayer::canPlay(bool _3D, String& _filename, const String& _resourceGroup) const
	{
		if(_3D)
			return false;

		size_t dotpos = _filename.rfind('.');
		String fileext = (dotpos != String::npos) ? _filename.substr(dotpos) : StringUtil::BLANK;
		if(!StrUtil::equalsNoCase(fileext, ".sgt"))
			return false;

		if(!ResourceGroupManager::getSingleton().resourceExists(_resourceGroup, _filename))
			return false;

		return true;		
	}

} // namespace GothOgre