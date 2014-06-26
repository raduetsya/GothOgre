#ifndef GOTHOGRE_APP_BLOCK_WORLD_SCENE_H
#define GOTHOGRE_APP_BLOCK_WORLD_SCENE_H

#include "GothOgre_AppBlock.h
#include "GothOgre_ZenManager.h"
#include "GothOgre_ZenArchive.h"
#include "GothOgre_World.h"
#include "GothOgre_BaseWorldObject.h"
#include "GothOgre_RootWorldObject.h"
#include "GothOgre_World.h"
#include "GothOgre_DefaultSceneManager.h"

namespace GothOgre
{

	class WorldSceneAppBlock : public AppBlock
	{
	public:
		WorldSceneAppBlock()
			: mDefaultSceneManagerFactory(nullptr), mMainViewport(nullptr)
		{
			mZenManager = new ZenManager;
			mBaseWorldObjectFactory = new BaseWorldObjectFactory;
			mRootWorldObjectFactory = new RootWorldObjectFactory;

			mDefaultSceneManagerFactory = new DefaultSceneManagerFactory;
			mWorldPtr = ZenManager::getSingleton().createZenObject(World::getWorldTypeName());

			Camera* camera = mWorldPtr->getCamera();
			RenderWindow* window = Ogre::Root::getSingleton().getAutoCreatedWindow();
			mMainViewport = window->addViewport(camera);
			mMainViewport->setBackgroundColour(ColourValue(0,0,0));
			camera->setAutoAspectRatio(true);
		}

		~WorldSceneAppBlock()
		{
			mWorldPtr.setNull();
			if(mDefaultSceneManagerFactory)
				delete mDefaultSceneManagerFactory;

			if(mRootWorldObjectFactory)
				delete mRootWorldObjectFactory;

			if(mBaseWorldObjectFactory)
				delete mBaseWorldObjectFactory;
		
			if(mZenManager)
				delete mZenManager;
		}

	private:
		ZenManager*                  mZenManager;
		BaseWorldObjectFactory*      mBaseWorldObjectFactory;
		RootWorldObjectFactory*      mRootWorldObjectFactory;
		DefaultSceneManagerFactory*  mDefaultSceneManagerFactory;
		WorldPtr                     mWorldPtr;
		Viewport*                    mMainViewport;
	};

}

#endif // GOTHOGRE_APP_BLOCK_WORLD_SCENE_H