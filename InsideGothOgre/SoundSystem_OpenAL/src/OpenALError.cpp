#include "Precompiled.h"
#include "OpenALError.h"


namespace GothOgre
{
	//-----------------------------------------------------------------------
	void Error::checkALState(const char* _file, long _line)
	{
		ALenum err = alGetError();
		if(err != AL_NO_ERROR)
		{
			const char* desc;
			switch(err)
			{
				case AL_INVALID_NAME:      desc = "A bad name (ID) was passed to an OpenAL function."; break;
				case AL_INVALID_ENUM:      desc = "An invalid enum value was passed to an OpenAL function."; break;
				case AL_INVALID_VALUE:     desc = "An invalid value was passed to an OpenAL function."; break;
				case AL_INVALID_OPERATION: desc = "The requested operation is not valid."; break;
				case AL_OUT_OF_MEMORY:     desc = "The requested operation resulted in OpenAL running out of memory."; break;
				default:                   desc = "Unknown error.";
			}
			GOTHOGRE_EXCEPT2( desc, _file, _line );
		}
	}
	//-----------------------------------------------------------------------
	void Error::checkALCState(ALCdevice* _device, const char* _file, long _line)
	{
		ALenum err = alcGetError(_device);
		if(err != ALC_NO_ERROR)
		{
			const char* desc;
			switch(err)
			{
				case ALC_INVALID_DEVICE:  desc = "A bad device was passed to an OpenAL function."; break;
				case ALC_INVALID_CONTEXT: desc = "A bad context was passed to an OpenAL function."; break;
				case ALC_INVALID_ENUM:    desc = "An unknown enum value was passed to an OpenAL function."; break;
				case ALC_INVALID_VALUE:   desc = "An invalid value was passed to an OpenAL function."; break;
				case ALC_OUT_OF_MEMORY:   desc = "The requested operation resulted in OpenAL running out of memory."; break;
				default:                  desc = "Unknown error.";
			}
			GOTHOGRE_EXCEPT2( desc, _file, _line );
		}
	}
}
