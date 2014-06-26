#include "GothOgre_Precompiled.h"
#include "GothOgre_ZenFactory.h"
#include "GothOgre_ZenObject.h"
#include "GothOgre_ZenManager.h"


namespace GothOgre
{
	ZenFactory::ZenFactory(const String& _typeName)
	{
		mTypeName = _typeName;
		ZenManager::getSingleton().addZenFactory(this);

		mNumCreatedObjects = 0;
		mNumLivingObjects = 0;
	}
	//-----------------------------------------------------------------
	ZenFactory::~ZenFactory()
	{
		ZenManager::getSingleton().removeZenFactory(this);
	}
	//-----------------------------------------------------------------
	void ZenFactory::objectCreated(ZenObject* _zenObject)
	{
		++mNumCreatedObjects;
		++mNumLivingObjects;
	}
	//-----------------------------------------------------------------
	void ZenFactory::objectDestroyed(ZenObject* _zenObject)
	{
		GOTHOGRE_ASSERT(mNumLivingObjects != 0, getTypeName() << ": numCreatedObjects must not be less than numDestroyedObjects");
		--mNumLivingObjects;
	}
	//-----------------------------------------------------------------
	const vector<String>::type& ZenFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		return sTypeNames;
	}
	//-----------------------------------------------------------------
	const String& ZenFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return StringUtil::BLANK;
	}
	//-----------------------------------------------------------------
	const vector<ushort>::type& ZenFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		return sVersions;
	}
	//-----------------------------------------------------------------
	ushort ZenFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return 0;
	}
	//-----------------------------------------------------------------
	ZenObjectPtr ZenFactory::createZenObject()
	{
		ZenObjectPtr obj = createZenObjectImpl();
		obj->_init(this);
		return obj;
	}

} // namespace GothOgre