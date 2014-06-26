#ifndef DXERROR_H
#define DXERROR_H


namespace GothOgre
{
	struct DXError
	{
		static void check(HRESULT _hr);
	};
}

#endif // DXERROR_H