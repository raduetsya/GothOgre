#include "Precompiled.h"
#include "3dsFileLoader.h"
#include "3dsFileLoadContext.h"


namespace GothOgre
{

	_3dsFileLoader::_3dsFileLoader()
		: MeshLoader("_3dsFileLoader", Priority::LOWEST)
	{
	}
	//---------------------------------------------------------------------------------
	const String& _3dsFileLoader::getFileExt() const
	{
		static String sFileExt = ".3ds";
		return sFileExt;
	}
	//---------------------------------------------------------------------------------
	ResourceLoadContext* _3dsFileLoader::createLoadContext()
	{
		return new _3dsFileLoadContext;
	}

} // namespace GothOgre