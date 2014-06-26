#ifndef OPENAL_ERROR_H
#define OPENAL_ERROR_H


namespace GothOgre
{
	struct Error
	{
		static void checkALState(const char* _file, long _line);
		static void checkALCState(ALCdevice* _device, const char* _file, long _line);
	};
}

#define CHECK_AL_ERROR_STATE   Error::checkALState(__FILE__, __LINE__)
#define CHECK_ALC_ERROR_STATE(_device)  Error::checkALCState(_device, __FILE__, __LINE__)

#endif // OPENAL_ERROR_H