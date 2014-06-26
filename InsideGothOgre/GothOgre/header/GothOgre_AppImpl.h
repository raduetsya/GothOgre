#ifndef GOTHOGRE_APP_IMPL_H
#define GOTHOGRE_APP_IMPL_H

#include "GothOgre_CommandLine.h"
#include "GothOgre_LogSystem.h"
#include "GothOgre_UnicodeSupport.h"
#include "GothOgre_EncodingSettings.h"
#include "GothOgre_GraphicSettings.h"
#include "GothOgre_SoundSettings.h"
#include "GothOgre_ResourceLocationScanner.h"
#include "GothOgre_InputSupport.h"
#include "GothOgre_InputSystem.h"
#include "GothOgre_ZenManager.h"
#include "GothOgre_BaseWorldObject.h"
#include "GothOgre_RootWorldObject.h"
#include "GothOgre_SystemTime.h"
#include "GothOgre_WorldTime.h"
#include "GothOgre_UIModeManager.h"
#include "GothOgre_Progress.h"
#include "GothOgre_FrameStats.h"
#include "GothOgre_TextureManagerEx.h"
#include "GothOgre_MtlManagerEx.h"
#include "GothOgre_MeshManagerEx.h"
#include "GothOgre_SoundDefManagerEx.h"
#include "GothOgre_SoundSystem.h"
#include "GothOgre_SoundPlayer.h"
#include "GothOgre_SkyDefManagerEx.h"
#include "GothOgre_SkySystem.h"
#include "GothOgre_PortalSystem.h"
#include "GothOgre_SceneManagerExEnumerator.h"
#include "GothOgre_WorldManager.h"


namespace GothOgre
{
	//------------------------------------------------------------------------
	class AppImpl
	{
	public:
		AppImpl();
		~AppImpl();

		void initialise(int _argc, char** _argv);
		void initialise(const char* _cmdLine);
		void shutdown();

		bool initOk() const {return mInitOk;}
		void renderingLoop();

	private:
		void initialise_CommandLine(int _argc, char** _argv);
		void initialise_CommandLine(const char* _cmdLine);
		void initialise_after_CommandLine_before_Ogre();
		void initialise_Ogre();
		void initialise_after_Ogre();

		void test();

		void shutdown_before_Ogre();
		void shutdown_Ogre();
		void shutdown_after_Ogre();

		void setWindowIcon();

	private:
		bool                         mInitOk;

		CommandLine*                 mCommandLine;
		LogSystem*                   mLogSystem;
		UnicodeSupportRegistration*  mUnicodeSupportRegistration;
		EncodingSettings*            mEncodingSettings;
		GraphicSettings*             mGraphicSettings;
		SoundSettings*               mSoundSettings;
		ResourceLocationScanner*     mResourceLocationScanner;
		InputSupportRegistration*    mInputSupportRegistration;
		InputSystem*                 mInputSystem;
		UIModeManager*               mUIModeManager;
		Progress*                    mProgress;
		FrameStats*                  mFrameStats;

		Ogre::Root*                  mRoot;
		Ogre::RenderWindow*          mWindow;

		TextureManagerEx*            mTextureManagerEx;
		MtlManager*                  mMtlManager;
		MtlManagerEx*                mMtlManagerEx;
		MeshManagerEx*               mMeshManagerEx;

		SkyDefManager*               mSkyDefManager;
		SkyDefManagerEx*             mSkyDefManagerEx;
		SkySystem*                   mSkySystem;
		PortalSystem*                mPortalSystem;

		SoundDefManager*             mSoundDefManager;
		SoundDefManagerEx*           mSoundDefManagerEx;
		SoundPlayerEnumerator*       mSoundPlayerEnumerator;
		SoundSystem*                 mSoundSystem;

		SceneManagerExEnumerator*    mSceneManagerExEnumerator;

		SystemTime*                  mSystemTime;
		WorldTime*                   mWorldTime;

		ZenManager*                  mZenManager;
		BaseWorldObjectFactory*      mBaseWorldObjectFactory;
		RootWorldObjectFactory*      mRootWorldObjectFactory;
		WorldManager*                mWorldManager;
	};

} // namespace GothOgre

#endif // GOTHOGRE_APP_IMPL_H