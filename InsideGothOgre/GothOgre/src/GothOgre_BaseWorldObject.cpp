#include "GothOgre_Precompiled.h"
#include "GothOgre_World.h"
#include "GothOgre_BaseWorldObject.h"

namespace GothOgre
{
	//--------------------------------------------------------------------------------
	BaseWorldObject::BaseWorldObject()
	{
		SceneManagerEx* sceneMgr = mWorld->getSceneManager();
		mSceneNode = sceneMgr->createSceneNode();
		mSceneNode->setUserAny( (Any) static_cast<WorldObject*>(this) );
	}
	//--------------------------------------------------------------------------------
	BaseWorldObject::~BaseWorldObject()
	{
		deselect();
		removeAllChildren();

		if(mSceneNode)
			mSceneNode->getCreator()->destroySceneNode(mSceneNode);
	}
	//--------------------------------------------------------------------------------
	BaseWorldObjectFactory::BaseWorldObjectFactory()
		: ZenFactory("BaseWorldObject")
	{
	}
	//--------------------------------------------------------------------------------
	ZenObjectPtr BaseWorldObjectFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new BaseWorldObject();
	}

} // namespace GothOgre
