#include "Precompiled.h"
#include "RZip.h"

namespace GothOgre
{
	RZipArchiveFactory::RZipArchiveFactory()
	{
	}
	//-----------------------------------------------------------------------------
	RZipArchiveFactory::~RZipArchiveFactory()
	{
	}
	//-----------------------------------------------------------------------------
	const String& RZipArchiveFactory::getType() const
	{
		static const String sType = "ZIP";
		return sType;
	}
	//-----------------------------------------------------------------------------
	Archive* RZipArchiveFactory::createInstance( const String& _name )
	{
		return OGRE_NEW RZipArchive(_name, getType());		
	}
	//-----------------------------------------------------------------------------
	void RZipArchiveFactory::destroyInstance( Archive* _arch)
	{
		delete _arch;
	}

} // namespace GothOgre