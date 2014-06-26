#include "ExtArchives_Precompiled.h"
#include "ExtArchives_CommonInclude.h"
#include "FlatArchiveFactory.h"
#include "FlatArchive.h"

namespace GothOgre
{

	FlatArchiveFactory::FlatArchiveFactory(const String& _archType, const String& _baseArchType) 
	{
		mArchType = _archType; 
		mBaseArchType = _baseArchType;
	}
    //--------------------------------------------------------------------------------------
	FlatArchiveFactory::~FlatArchiveFactory() 
	{
	}
    //--------------------------------------------------------------------------------------
	const String& FlatArchiveFactory::getType() const 
	{
		return mArchType;
	}
    //--------------------------------------------------------------------------------------
	Archive* FlatArchiveFactory::createInstance( const String& _name ) 
    {
		return OGRE_NEW FlatArchive(_name, mArchType, mBaseArchType);
    }
    //--------------------------------------------------------------------------------------
	void FlatArchiveFactory::destroyInstance( Archive* _arch) 
	{ 
		delete _arch; 
	}

} // namespace GothOgre