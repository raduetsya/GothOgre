#ifndef GOTHOGRE_USER_INTERFACE_APP_BLOCK_H
#define GOTHOGRE_USER_INTERFACE_APP_BLOCK_H

#include "GothOgre_AppBlock.h"
#include "GothOgre_UIModeManager.h"
#include "GothOgre_FrameStats.h"
#include "GothOgre_OgreFrameStats.h"
#include "GothOgre_Progress.h"
#include "GothOgre_ViewportManager.h"


namespace GothOgre
{
	class UserInterfaceAppBlock : public AppBlock
	{
	public:
		UserInterfaceAppBlock()
			: mUIModeManager(nullptr), mFrameStats(nullptr),
			  mOgreFrameStats(nullptr), mProgress(nullptr),
			  mViewportManager(nullptr)
		{
			mUIModeManager = new UIModeManager(UIMode::INITIALISATION);
			mProgress = new Progress;
			mFrameStats = new FrameStats;
			mViewportManager = new ViewportManager;
		}

		~UserInterfaceAppBlock()
		{
			if(mProgress)
				delete mProgress;

			if(mUIModeManager)
				delete mUIModeManager;

			if(mViewportManager)
				delete mViewportManager;

			if(mFrameStats)
				delete mFrameStats;
		}
	
		void initialise()
		{
			RenderWindow* window = Ogre::Root::getSingleton().getAutoCreatedWindow();
			mViewportManager->setRenderWindow(window);
			Viewport* mainViewport = window->getViewport(0);
			mViewportManager->setMainViewport(mainViewport);

			mOgreFrameStats = new OgreFrameStats(mainViewport);

			if(UIModeManager::getSingleton().getUIMode() == UIMode::INITIALISATION)
				UIModeManager::getSingleton().setUIMode(UIMode::EDITOR);
		}

		void shutdown()
		{
			mUIModeManager->setUIMode(UIMode::SHUTDOWN);

			if(mOgreFrameStats)
			{
				delete mOgreFrameStats;
				mOgreFrameStats = nullptr;
			}
		}

	private:
		UIModeManager*                mUIModeManager;
		FrameStats*					  mFrameStats;
		OgreFrameStats*               mOgreFrameStats;
		Progress*                     mProgress;
		ViewportManager*              mViewportManager;
	};

} // namespace GothOgre

#endif // GOTHOGRE_USER_INTERFACE_APP_BLOCK_H