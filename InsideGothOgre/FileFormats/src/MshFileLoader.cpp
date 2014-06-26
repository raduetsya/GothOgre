#include "Precompiled.h"
#include "MshFileLoader.h"
#include "MshFileLoadContext.h"

namespace GothOgre
{
	MshFileLoader::MshFileLoader()
		: MeshLoader("MshFileLoader", Priority::BELOW_NORMAL)
	{
	}
	//---------------------------------------------------------------------------------
	ResourceLoadContext* MshFileLoader::createLoadContext()
	{
		return new MshFileLoadContext;
	}
	//---------------------------------------------------------------------------------
	bool MshFileLoader::canLoad(String& _filename, const String& _resourceGroup) const
	{
		size_t dotpos = _filename.rfind('.');
		String fileext = (dotpos != String::npos) ? _filename.substr(dotpos) : StringUtil::BLANK;
		if(!StrUtil::equalsNoCase(fileext, ".3ds"))
			return false;

		String newfilename = _filename.substr(0, dotpos) + ".msh";
		if(!ResourceGroupManager::getSingleton().resourceExists(_resourceGroup, newfilename))
			return false;

		_filename = newfilename;
		return true;		
	}

} // namespace GothOgre