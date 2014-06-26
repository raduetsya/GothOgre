#include "Precompiled.h"
#include "SkyFileLoader.h"
#include "SkyFileLoadContext.h"

namespace GothOgre
{
	SkyFileLoader::SkyFileLoader()
		: SkyDefLoader("SkyFileLoader", Priority::LOWEST)
	{
	}
	//---------------------------------------------------------------------------------
	ResourceLoadContext* SkyFileLoader::createLoadContext()
	{
		return new SkyFileLoadContext;
	}
	//---------------------------------------------------------------------------------
	bool SkyFileLoader::canLoad(String& _filename, const String& _resourceGroup) const
	{
		size_t dotpos = _filename.rfind('.');
		String fileext = (dotpos != String::npos) ? _filename.substr(dotpos) : StringUtil::BLANK;
		if(!UnicodeUtil::equalsNoCase(fileext, ".sky"))
			return false;

		//String newfilename = _filename.substr(0, dotpos) + ".skb";
		//if(!ResourceGroupManager::getSingleton().resourceExists(_resourceGroup, newfilename))
		//	return false;
		//
		//_filename = newfilename;

		return true;		
	}

} // namespace GothOgre