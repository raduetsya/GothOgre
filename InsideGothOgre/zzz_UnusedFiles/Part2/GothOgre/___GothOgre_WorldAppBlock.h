#ifndef GOTHOGRE_WORLD_APP_BLOCK_H
#define GOTHOGRE_WORLD_APP_BLOCK_H

#include "GothOgre_AppBlock.h"
#include "GothOgre_ZenManager.h"
#include "GothOgre_World.h"

namespace GothOgre
{

	class WorldAppBlock : public AppBlock
	{
	public:
		WorldAppBlock()
			: mMainViewport(nullptr)
		{
		}

		~WorldAppBlock()
		{
		}

		void initialise()
		{
			// Create world and scene manager
			mWorldPtr = ZenManager::getSingleton().createZenObject(World::getWorldTypeName());

			// Create viewport
			mViewportManage


			// initialise camera and viewport
			Camera* camera = mWorldPtr->getCamera();
			RenderWindow* window = Ogre::Root::getSingleton().getAutoCreatedWindow();
			mMainViewport = window->addViewport(camera);
			mMainViewport->setBackgroundColour(ColourValue(0,0,0));
			camera->setAutoAspectRatio(true);
		}

		void shutdown()
		{
			// destroy world
			mWorldPtr.setNull();
		}

	private:
		WorldPtr                     mWorldPtr;
		Viewport*                    mMainViewport;
	};

}

#endif // GOTHOGRE_WORLD_APP_BLOCK_H