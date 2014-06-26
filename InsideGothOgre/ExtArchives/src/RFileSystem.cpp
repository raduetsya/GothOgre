#include "Precompiled.h"
#include "RFileSystem.h"

namespace GothOgre
{
	RFileSystemArchiveFactory::RFileSystemArchiveFactory()
	{
	}
	//-----------------------------------------------------------------------------
	RFileSystemArchiveFactory::~RFileSystemArchiveFactory()
	{
	}
	//-----------------------------------------------------------------------------
	const String& RFileSystemArchiveFactory::getType() const
	{
		static const String sType = "FILESYSTEM";
		return sType;
	}
	//-----------------------------------------------------------------------------
	Archive* RFileSystemArchiveFactory::createInstance( const String& _name )
	{
		return OGRE_NEW RFileSystemArchive(_name, getType());		
	}
	//-----------------------------------------------------------------------------
	void RFileSystemArchiveFactory::destroyInstance( Archive* _arch)
	{
		delete _arch;
	}

} // namespace GothOgre