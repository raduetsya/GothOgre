#include "Precompiled.h"
#include "DXError.h"


namespace GothOgre
{
	void DXError::check(HRESULT _hr)
	{
		if(SUCCEEDED(_hr))
			return;

		const TCHAR* errstr = DXGetErrorString8(_hr);
		const char* errdesc = DXGetErrorDescription8(_hr);
		GOTHOGRE_EXCEPT( "DirectX error " << errstr 
			<< "( " << errdesc << ")" );
	}
}