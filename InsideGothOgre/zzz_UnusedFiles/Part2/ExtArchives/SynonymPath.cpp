#include "ExtArchives_Precompiled.h"
#include "ExtArchives_CommonInclude.h"
#include "SynonymPath.h"

namespace GothOgre
{

	String SynonymPath::calculate(const String& _srcPath)
	{
		// This name and base archive's name are the same directory.
		// However the names must be different to allow 
		// to find archives by unique name.
		String retPath = _srcPath;
		size_t slashPos = retPath.rfind('\\');
		if(slashPos != String.npos)
		{
			retPath.insert(slashPos, "\\");
		}
		else
		{
			slashPos = retPath.rfind('/');
			if(slashPos != String.npos)
			{
				retPath.insert(slashPos, "/");
			}
		}
		GOTHOGRE_ASSERT(retPath != _srcPath, "Pathes must differs!");
		return retPath;
	}

} // namespace GothOgre