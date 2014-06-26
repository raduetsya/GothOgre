#ifndef ICU_ERROR_H
#define ICU_ERROR_H

namespace GothOgre
{
	struct ICUError
	{
		static void checkCode(UErrorCode _errorCode, const char* _file, long _line);
	};

	#define CHECK_ICU_ERROR_CODE(_errorCode)   ICUError::checkCode(_errorCode, __FILE__, __LINE__)
}

#endif // ICU_ERROR_H