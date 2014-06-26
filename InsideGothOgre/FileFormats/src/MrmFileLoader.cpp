#include "Precompiled.h"
#include "MrmFileLoader.h"
#include "MrmFileLoadContext.h"

namespace GothOgre
{

	MrmFileLoader::MrmFileLoader()
		: MeshLoader("MrmFileLoader", Priority::NORMAL)
	{
	}
	//---------------------------------------------------------------------------------
	ResourceLoadContext* MrmFileLoader::createLoadContext()
	{
		return new MrmFileLoadContext;
	}
	//---------------------------------------------------------------------------------
	bool MrmFileLoader::canLoad(String& _filename, const String& _resourceGroup) const
	{
		// Can be loaded instead of *.3ds
		size_t dotpos = _filename.rfind('.');
		String fileext = (dotpos != String::npos) ? _filename.substr(dotpos) : StringUtil::BLANK;
		if(!StrUtil::equalsNoCase(fileext, ".3ds"))
			return false;

		String newfilename = _filename.substr(0, dotpos) + ".mrm";
		if(!ResourceGroupManager::getSingleton().resourceExists(_resourceGroup, newfilename))
			return false;

		_filename = newfilename;
		return true;		
	}

} // namespace GothOgre