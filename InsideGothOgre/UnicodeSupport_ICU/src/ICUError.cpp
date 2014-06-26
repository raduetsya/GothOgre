#include "Precompiled.h"
#include "ICUError.h"


namespace GothOgre
{
	//----------------------------------------------------------------------------
	void ICUError::checkCode(UErrorCode _errorCode, const char* _file, long _line)
	{
		if(U_FAILURE(_errorCode))
			GOTHOGRE_EXCEPT2(u_errorName(_errorCode), _file, _line);
		else if(_errorCode <= U_ERROR_WARNING_LIMIT)
			GOTHOGRE_WARNING(u_errorName(_errorCode));
	}
}