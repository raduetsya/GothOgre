#include "Precompiled.h"
#include "OpenALPlayContext.h"
#include "OpenALPlayer.h"
#include "OpenALError.h"


namespace GothOgre
{
	//-------------------------------------------------------------------------
	OpenALPlayer::OpenALPlayer()
		: SoundPlayer("OpenALPlayer")
	{
		mALDevice = nullptr;
		mALContext = nullptr;
	}
	//-------------------------------------------------------------------------
	OpenALPlayer::~OpenALPlayer()
	{
	}
	//-------------------------------------------------------------------------
	void OpenALPlayer::initialise()
	{
		openALDevice();
		if(!mALDevice)
			return;

		alGetError(); // to clear initial error state

		mALContext = alcCreateContext(mALDevice, 0); CHECK_ALC_ERROR_STATE(mALDevice);
		if(!mALContext)
			GOTHOGRE_EXCEPT("Cannot open an OpenAL device context.");

		alcMakeContextCurrent(mALContext); CHECK_ALC_ERROR_STATE(mALDevice);
	
		// set OpenAL properties
		alDistanceModel(AL_LINEAR_DISTANCE);
	}
	//-------------------------------------------------------------------------
	void OpenALPlayer::openALDevice()
	{
		static CodepageConverter::ByteBuffer byteBuf;
		static CodepageConverter* utf8Conv = UnicodeUtil::openConverter("UTF-8");

		// Get name of the default device
		const ALCchar* szDefaultDeviceName = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
		byteBuf.assign( (const uint8*) szDefaultDeviceName, strlen(szDefaultDeviceName));
		String defaultDeviceName;
		utf8Conv->bufferToUnicode(defaultDeviceName, byteBuf);

		// Get device name from the configuration file.
		String deviceName = SoundSettings::getSingleton().getSetting("DeviceName", "OpenAL", defaultDeviceName);
		
		// Convert retrieved device name to UTF-8.
		byteBuf.clear();
		utf8Conv->unicodeToBuffer(byteBuf, deviceName);
		byteBuf.push_back('\0');
		const ALCchar* szDeviceName = (const ALCchar*) byteBuf.data();

		// Try to open device.
		mALDevice = alcOpenDevice(szDeviceName);

		if(!mALDevice)
		{
			// If failed, try the default device
			GOTHOGRE_WARNING("Could not open OpenAL device '" << deviceName << "'. The default device will be used.");
			mALDevice = alcOpenDevice(0);
			
			if(!mALDevice)
			{
				GOTHOGRE_ERROR("Could not open the default OpenAL device. You will not hear the sound.");
				return;
			}
		}
	}
	//-------------------------------------------------------------------------
	void OpenALPlayer::shutdown()
	{
		alcMakeContextCurrent(0);
		alcDestroyContext(mALContext);
		alcCloseDevice(mALDevice);
	}
	//-------------------------------------------------------------------------
	void OpenALPlayer::update(const Vector3& _listenerPosition, 
		const Quaternion& _listenerOrientation)
	{
		// Send the new listener's position to OpenAL
		Vector3 direction = _listenerOrientation * Vector3::NEGATIVE_UNIT_Z;
		Vector3 up = _listenerOrientation * Vector3::UNIT_Y;
		ALfloat values[] = { (float) direction.x, (float) direction.y, (float) direction.z, (float) up.x, (float) up.y, (float) up.z};
		alListenerfv(AL_ORIENTATION, values);
		alListener3f(AL_POSITION, (float) _listenerPosition.x, (float) _listenerPosition.y, (float) _listenerPosition.z);
	}
	//-------------------------------------------------------------------------
	SoundPlayContext* OpenALPlayer::createPlayContext()
	{
		return new OpenALPlayContext(createAudioCodec());
	}
	//-------------------------------------------------------------------------
	bool OpenALPlayer::canPlay(bool _3D, String& _filename, const String& _resourceGroup) const
	{
		size_t dotpos = _filename.rfind('.');
		String fileext = (dotpos != String::npos) ? _filename.substr(dotpos) : StringUtil::BLANK;
		if(!StrUtil::equalsNoCase(fileext, ".wav")
			&& !StrUtil::equalsNoCase(fileext, ".ogg")
			&& !StrUtil::equalsNoCase(fileext, ".flac"))
			return false;

		if(!ResourceGroupManager::getSingleton().resourceExists(_resourceGroup, _filename))
			return false;

		return true;		
	}

} // namespace GothOgre