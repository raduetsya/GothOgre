#include "GothOgre_Precompiled.h"
#include "GothOgre_ZenObject.h"
#include "GothOgre_ZenFactory.h"


namespace GothOgre
{
	//-----------------------------------------------------------------------
	// ZenObject
	//-----------------------------------------------------------------------
	ZenObject::ZenObject()
		: mCreator(nullptr), mUseCount(0)
	{
	}
	//-----------------------------------------------------------------------
	void ZenObject::_init(ZenFactory* _creator)
	{
		mCreator = _creator;
		mCreator->objectCreated(this);
	}
	//-----------------------------------------------------------------------
	ZenObject::~ZenObject()
	{
		if(mCreator)
		{
			mCreator->objectDestroyed(this);
			if(mUseCount != 0)
				GOTHOGRE_ERROR("There are existing references to a destroying object of type '" << getTypeName() << "'");
		}
	}
	//-----------------------------------------------------------------------
	const String& ZenObject::getTypeName() const 
	{
		return mCreator->getTypeName();
	}
	//-----------------------------------------------------------------------
	const vector<String>::type& ZenObject::getSupportedTypeNames() const
	{
		return getCreator()->getSupportedTypeNames();
	}
	//-----------------------------------------------------------------------
	const String& ZenObject::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getCreator()->getCompatibleTypeName(_zenArchive);
	}
	//-----------------------------------------------------------------------
	const vector<ushort>::type& ZenObject::getSupportedVersions() const
	{
		return getCreator()->getSupportedVersions();
	}
	//-----------------------------------------------------------------------
	ushort ZenObject::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getCreator()->getCompatibleVersion(_zenArchive);
	}



	//----------------------------------------------------------------------------
	// ZenObjectPtr
	//----------------------------------------------------------------------------
	void ZenObjectPtr::bind(ZenObject* _p)
	{
		if(mZenObject != _p)
		{
			release();
			mZenObject = _p;
			if(mZenObject)
				++mZenObject->mUseCount;
		}
	}
	//----------------------------------------------------------------------------
	unsigned int ZenObjectPtr::useCount () const
	{
		return mZenObject ? mZenObject->mUseCount : 0;
	}
	//----------------------------------------------------------------------------
	void ZenObjectPtr::release()
	{
		if(mZenObject)
		{
			if(!(--mZenObject->mUseCount))
				destroy();
		}
	}
	//----------------------------------------------------------------------------
	void ZenObjectPtr::destroy()
	{
		delete mZenObject;
	}

} // namespace GothOgre
