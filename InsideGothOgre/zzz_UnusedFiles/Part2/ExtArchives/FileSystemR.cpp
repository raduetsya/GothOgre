#include "ExtArchives_Precompiled.h"
#include "ExtArchives_CommonInclude.h"
#include "FileSystemR.h"

namespace GothOgre
{
	FileSystemRArchiveFactory::FileSystemRArchiveFactory()
	{
	}
	//-----------------------------------------------------------------------------
	FileSystemRArchiveFactory::~FileSystemRArchiveFactory()
	{
	}
	//-----------------------------------------------------------------------------
	const String& FileSystemRArchiveFactory::getType() const
	{
		static const String sType = "FileSystemR";
		return sType;
	}
	//-----------------------------------------------------------------------------
	Archive* FileSystemRArchiveFactory::createInstance( const String& _name )
	{
		return OGRE_NEW FileSystemRArchive(_name, getType());		
	}
	//-----------------------------------------------------------------------------
	void FileSystemRArchiveFactory::destroyInstance( Archive* _arch)
	{
		delete _arch;
	}

} // namespace GothOgre