#include "ExtArchives_Precompiled.h"
#include "ExtArchives_CommonInclude.h"
#include "ZipR.h"

namespace GothOgre
{
	ZipRArchiveFactory::ZipRArchiveFactory()
	{
	}
	//-----------------------------------------------------------------------------
	ZipRArchiveFactory::~ZipRArchiveFactory()
	{
	}
	//-----------------------------------------------------------------------------
	const String& ZipRArchiveFactory::getType() const
	{
		static const String sType = "ZipR";
		return sType;
	}
	//-----------------------------------------------------------------------------
	Archive* ZipRArchiveFactory::createInstance( const String& _name )
	{
		return OGRE_NEW ZipRArchive(_name, getType());		
	}
	//-----------------------------------------------------------------------------
	void ZipRArchiveFactory::destroyInstance( Archive* _arch)
	{
		delete _arch;
	}

} // namespace GothOgre