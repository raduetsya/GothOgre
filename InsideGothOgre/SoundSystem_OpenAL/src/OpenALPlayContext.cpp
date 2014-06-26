#include "Precompiled.h"
#include "OpenALError.h"
#include "OpenALPlayContext.h"
#include "AudioCodec.h"


namespace GothOgre
{
	//--------------------------------------------------------------------------
	OpenALPlayContext::OpenALPlayContext(AudioCodec* _codec)
	{
		mAudioCodec = _codec;
		mBufferTime = SoundSettings::getSingleton().getSetting("BufferTime", "OpenAL", 0.1f);
		mMaxNumBuffers = SoundSettings::getSingleton().getSetting("NumBuffers", "OpenAL", 2);
		
		alGenSources(1, &mSource); CHECK_AL_ERROR_STATE;
		alSourcef(mSource, AL_REFERENCE_DISTANCE, 0); CHECK_AL_ERROR_STATE;
		// AL_ROLLOFF_FACTOR = 1 by default and it's ok

		mLooping = false;
		mPlaying = false;
		mPaused = false;
	}
	//--------------------------------------------------------------------------
	OpenALPlayContext::~OpenALPlayContext()
	{
		stop();
		alDeleteSources(1, &mSource);
		if(!mBuffers.empty())
			alDeleteBuffers(mBuffers.size(), &mBuffers[0]);
		if(mAudioCodec)
			delete mAudioCodec;
	}
	//--------------------------------------------------------------------------
	void OpenALPlayContext::play(bool _3D, const String& _filename, const String& _resourceGroup)
	{
		// Read the file's header.
		mAudioCodec->open(_filename, _resourceGroup);
		
		const AudioCodec::SoundInfo& info = mAudioCodec->getSoundInfo();
		size_t bufferSize = (size_t) (mBufferTime * info.framesPerSec * info.channels);
		mWaveData.resize(0);
		mWaveData.resize(bufferSize);

		// Sets the source parameters if it's 2D sound
		alSourcei(mSource, AL_SOURCE_RELATIVE, !_3D); CHECK_AL_ERROR_STATE;
		alSource3f(mSource, AL_POSITION, 0.0f, 0.0f, 0.0f); CHECK_AL_ERROR_STATE;

		_play(0);
	}
	//--------------------------------------------------------------------------
	void OpenALPlayContext::_play(size_t _startFrame)
	{
		// Reset OpenAL source (That's necessary because the source could be used before).
		_stop();

		// Rewind to the beginning
		alSourceRewind(mSource); CHECK_AL_ERROR_STATE;

		const AudioCodec::SoundInfo& info = mAudioCodec->getSoundInfo();
		mStartFrame = _startFrame;
		mAudioCodec->setOffset(_startFrame);

		// Start the playing.
		mWaitMoreData = true;
		mPlaying = true;
		mPaused = false;
		mNumStopsAtFinalBlock = 0;

		// Read the first portion of data
		update();
	}
	//--------------------------------------------------------------------------
	void OpenALPlayContext::update()
	{
		if(!mPlaying)
			return;

		size_t unqueued  = unqueueProcessedBuffers();
		size_t queued    = queueNewBuffers();
		bool justStopped = checkStopPlaying();

		/*Debug (output debug string) 
		StrStream ss;
		ss << "Playing = " << mPlaying
			<< ", playpos = " << getPlayPosition() << " / " << getPlayLength()
			<< ", WaitMoreData = " << mWaitMoreData
			<< ", unqueued " << unqueued
			<< ", queued " << queued
			<< ", total_queued " << mBuffers.size() - mFreeBuffers.size();
		if(justStopped)
			ss << ", just stopped!!!";
		ss << "\n";
		OutputDebugString(ss.str().c_str());
		//*/	
	}
	//--------------------------------------------------------------------------
	size_t OpenALPlayContext::unqueueProcessedBuffers()
	{
		const AudioCodec::SoundInfo& info = mAudioCodec->getSoundInfo();
		
		ALint processed;
		alGetSourcei( mSource, AL_BUFFERS_PROCESSED, &processed); CHECK_AL_ERROR_STATE;
		size_t numUnqueued;
		for(numUnqueued = 0; numUnqueued != processed; ++numUnqueued)
		{
			ALuint buffer;
			alSourceUnqueueBuffers( mSource, 1, &buffer); CHECK_AL_ERROR_STATE;
			mFreeBuffers.push_back(buffer);
			
			ALint size;
			alGetBufferi(buffer, AL_SIZE, &size); CHECK_AL_ERROR_STATE;
			mStartFrame += size / info.channels / 2;
			mStartFrame %= info.frames;
		}
		return numUnqueued;
	}
	//--------------------------------------------------------------------------
	size_t OpenALPlayContext::queueNewBuffers()
	{
		if(!mWaitMoreData)
			return 0;

		// Buffer limit reached
		size_t numBusyBuffers = mBuffers.size() - mFreeBuffers.size();
		if(numBusyBuffers == mMaxNumBuffers)
			return 0;

		// Get a free buffer
		ALuint buffer; 
		if(!mFreeBuffers.empty())
		{
			// free buffer is already allocated
			buffer = mFreeBuffers.back();
			mFreeBuffers.pop_back();
		}
		else
		{
			// Generate a new buffer if a free buffer was not found.
			alGenBuffers(1, &buffer);
			mBuffers.push_back(buffer);
		}

		const AudioCodec::SoundInfo& info = mAudioCodec->getSoundInfo();

		// Try to read and decode sound data from the file.
		size_t maxFrames = mWaveData.size() / info.channels;
		size_t readFrames = mAudioCodec->readData(&mWaveData[0], maxFrames);
		if(!readFrames)
		{
			if(!mLooping || !mStartFrame)
			{
				mWaitMoreData = false;
				mAudioCodec->close(); // no data to play more
				return 0;
			}

			// Looping - go to the data' beginning.
			mAudioCodec->setOffset(0);

			// Try to read and decode sound data again.
			readFrames = mAudioCodec->readData(&mWaveData[0], maxFrames);
			if(!readFrames)
			{
				mWaitMoreData = false;
				mAudioCodec->close();
				return 0;
			}
		}

		// Fill the OpenAL buffer with audio data
		ALenum alFormat;
		if(info.channels == 1)
			alFormat = AL_FORMAT_MONO16;
		else
			alFormat = AL_FORMAT_STEREO16;

		ALsizei freq = info.framesPerSec;
		const ALvoid* data = (const ALvoid*) &mWaveData[0];
		ALsizei size = readFrames * info.channels * 2;
		alBufferData(buffer, alFormat, data, size, freq);  CHECK_AL_ERROR_STATE;

		// Queue the buffer with new sound data on the source.
		alSourceQueueBuffers(mSource, 1, &buffer); CHECK_AL_ERROR_STATE;
		return 1;
	}
	//--------------------------------------------------------------------------
	bool OpenALPlayContext::checkStopPlaying()
	{
		if(!mPlaying)
			return false; // was stopped before

		ALint state;
		alGetSourcei(mSource, AL_SOURCE_STATE, &state); CHECK_AL_ERROR_STATE;
		if(state == AL_PLAYING)
			return false; // nothing to do if it's being playing yet
			
		// It's possible it's an incorrect stop
		// because data in a single buffer has been played
		// before new data was provided.
		size_t numBusyBuffers = mBuffers.size() - mFreeBuffers.size();
		if(mWaitMoreData || numBusyBuffers > 1)
		{
			alSourcePlay(mSource); CHECK_AL_ERROR_STATE;
			return false;
		}
		
		// It's possible it's incorrent stop at the beginning of the final buffer
		if(numBusyBuffers == 1)
		{
			if(mNumStopsAtFinalBlock++ == 0)
			{
				alSourcePlay(mSource); CHECK_AL_ERROR_STATE;
				return false;
			}
		}

		// all buffers have been played, 
		// clear queue and mark that reached the stop state
		mPlaying = false;
		clearQueue();
		return true; // stopped just now
	}
	//--------------------------------------------------------------------------
	void OpenALPlayContext::clearQueue()
	{
		// Clear the queue
		alSourcei(mSource, AL_BUFFER, 0); CHECK_AL_ERROR_STATE;
		// All buffers are free now
		mFreeBuffers = mBuffers;
	}
	//--------------------------------------------------------------------------
	void OpenALPlayContext::stop()
	{
		mAudioCodec->close();
		_stop();
	}
	//--------------------------------------------------------------------------
	void OpenALPlayContext::_stop()
	{
		alSourceStop(mSource); CHECK_AL_ERROR_STATE;
		clearQueue();
		mWaitMoreData = false;
		mPlaying = false;
		mPaused = false;
	}
	//--------------------------------------------------------------------------
	bool OpenALPlayContext::isPlaying()
	{
		return mPlaying;
	}
	//--------------------------------------------------------------------------
	void OpenALPlayContext::setLooping(bool _looping)
	{
		mLooping = _looping;
	}
	//--------------------------------------------------------------------------
	void OpenALPlayContext::setVolume(Real _volume)
	{
		alSourcef(mSource, AL_GAIN, _volume); CHECK_AL_ERROR_STATE;
	}
	//--------------------------------------------------------------------------
	void OpenALPlayContext::setPosition(const Vector3& _position)
	{
		alSource3f(mSource, AL_POSITION, (float) _position.x, (float) _position.y, (float) _position.z); CHECK_AL_ERROR_STATE;	
	}
	//--------------------------------------------------------------------------
	void OpenALPlayContext::setMaxDistance(Real _radius)
	{
		alSourcef(mSource, AL_MAX_DISTANCE, (float) _radius); CHECK_AL_ERROR_STATE;
	}
	//--------------------------------------------------------------------------
	void OpenALPlayContext::pause()
	{
		if(!mPaused && mPlaying)
		{
			alSourcePause(mSource); CHECK_AL_ERROR_STATE;
			mPaused = true;
			mPlaying = false;
		}
	}
	//--------------------------------------------------------------------------
	void OpenALPlayContext::unpause()
	{
		if(mPaused)
		{
			alSourcePlay(mSource); CHECK_AL_ERROR_STATE;
			mPaused = false;
			mPlaying = true;
		}
	}
	//--------------------------------------------------------------------------
	bool OpenALPlayContext::isPaused()
	{
		return mPaused;
	}
	//--------------------------------------------------------------------------
	Real OpenALPlayContext::getPlayLength()
	{
		const AudioCodec::SoundInfo& info = mAudioCodec->getSoundInfo();
		return (Real) info.frames / (Real) info.framesPerSec;
	}
	//--------------------------------------------------------------------------
	Real OpenALPlayContext::getPlayPosition()
	{
		if(mPlaying || mPaused)
		{
			ALint frameOffset;
			alGetSourcei(mSource, AL_SAMPLE_OFFSET, &frameOffset); CHECK_AL_ERROR_STATE;
			size_t curFrame = frameOffset + mStartFrame;
			
			const AudioCodec::SoundInfo& info = mAudioCodec->getSoundInfo();
			curFrame %= info.frames;

			Real secOffset = Real(curFrame) / Real(info.framesPerSec);
			return secOffset;
		}
		else
		{
			return 0;
		}
	}
	//--------------------------------------------------------------------------
	void OpenALPlayContext::setPlayPosition(Real _position)
	{
		if(mPlaying || mPaused)
		{
			bool pauseUsed = mPaused;
			
			const AudioCodec::SoundInfo& info = mAudioCodec->getSoundInfo();
			size_t startFrame = size_t( _position * info.framesPerSec );
			_play( startFrame );
			
			if(pauseUsed)
				pause();
		}
	}

} // namespace GothOgre