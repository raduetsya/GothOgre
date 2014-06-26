#include "GothOgre_Precompiled.h"
#include "GothOgre_Mesh.h"

namespace GothOgre
{
	Mesh::Mesh() 
		: Ogre::Mesh(nullptr, "", 0, "") 
	{
		GOTHOGRE_CRITICAL("Mesh::Mesh() never should be called!");
	}
	//----------------------------------------------------------------------------
	Mesh::~Mesh() 
	{
		GOTHOGRE_CRITICAL("Mesh::~Mesh() never should be called!");
	}

} //  namespace GothOgre