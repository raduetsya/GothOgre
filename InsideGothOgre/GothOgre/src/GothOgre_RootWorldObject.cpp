#include "GothOgre_Precompiled.h"
#include "GothOgre_RootWorldObject.h"
#include "GothOgre_World.h"

namespace GothOgre
{
	//--------------------------------------------------------------------------------
	RootWorldObject::RootWorldObject()
	{
		SceneManagerEx* sceneMgr = mWorld->getSceneManager();
		mSceneNode = sceneMgr->getRootSceneNode();
		mSceneNode->setUserAny( (Any) static_cast<WorldObject*>(this) );
		setName("Root");
	}
	//--------------------------------------------------------------------------------
	RootWorldObject::~RootWorldObject()
	{
	}
	//--------------------------------------------------------------------------------
	RootWorldObjectFactory::RootWorldObjectFactory()
		: ZenFactory("RootWorldObject")
	{
	}
	//--------------------------------------------------------------------------------
	ZenObjectPtr RootWorldObjectFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new RootWorldObject();
	}

} // namespace GothOgre
