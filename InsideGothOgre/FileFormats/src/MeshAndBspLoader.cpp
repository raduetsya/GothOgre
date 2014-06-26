#include "Precompiled.h"
#include "MeshAndBspLoader.h"
#include "MeshAndBspLoadContext.h"

namespace GothOgre
{
	//---------------------------------------------------------------------------------
	MeshAndBspLoader::MeshAndBspLoader()
		: MeshLoader("MeshAndBspLoader")
	{
	}
	//---------------------------------------------------------------------------------
	ResourceLoadContext* MeshAndBspLoader::createLoadContext()
	{
		return new MeshAndBspLoadContext;
	}
	//---------------------------------------------------------------------------------
	bool MeshAndBspLoader::canLoad(String& _filename, const String& _resourceGroup) const
	{
		// Cannot be automatically created from entire file, 
		// it can be load from datastream only.
		return false;
	}

} // namespace GothOgre