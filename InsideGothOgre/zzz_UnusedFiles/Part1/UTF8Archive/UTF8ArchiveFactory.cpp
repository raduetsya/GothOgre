#include "FlatArchives_StdHeaders.h"
#include "Utf8ArchiveFactory.h"
#include "Utf8Archive.h"

namespace GothOgre
{

	Utf8ArchiveFactory::Utf8ArchiveFactory(const String& _archType, const String& _baseArchType) 
	{
		mArchType = _archType; 
		mBaseArchType = _baseArchType;
	}
    //--------------------------------------------------------------------------------------
	Utf8ArchiveFactory::~Utf8ArchiveFactory() 
	{
	}
    //--------------------------------------------------------------------------------------
	const String& Utf8ArchiveFactory::getType() const 
	{
		return mArchType;
	}
    //--------------------------------------------------------------------------------------
	Archive* Utf8ArchiveFactory::createInstance( const String& _name ) 
    {
		return OGRE_NEW Utf8Archive(_name, mArchType, mBaseArchType);
    }
    //--------------------------------------------------------------------------------------
	void Utf8ArchiveFactory::destroyInstance( Archive* _arch) 
	{ 
		delete _arch; 
	}

} // namespace GothOgre