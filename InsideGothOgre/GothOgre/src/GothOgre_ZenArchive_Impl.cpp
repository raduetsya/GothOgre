#include "GothOgre_Precompiled.h"
#include "GothOgre_CommonInclude.h"
#include "GothOgre_ZenArchive_Impl.h"

namespace GothOgre
{
	ZenArchiveImpl::ZenArchiveImpl() 
	{
	}
	//---------------------------------------------------------------------------------------
	ZenArchiveImpl::~ZenArchiveImpl() 
	{
	}
	//---------------------------------------------------------------------------------------
	void ZenArchiveImpl::init( Ogre::DataStreamPtr _dataStream, int _flags, ZenArchive::ObjectReadList* _objectReadList )
	{

	}
	//---------------------------------------------------------------------------------------
	void ZenArchiveImpl::init( Ogre::DataStreamPtr _dataStream, int _flags, ZenArchive::ObjectWriteMap* _objectWriteMap )
	{
		mDataStream = _dataStream;
		mName = mDataStream->getName();
		mFlags = _flags;
		mObjectWriteMap = _objectWriteMap;
	}

} // namespace GothOgre