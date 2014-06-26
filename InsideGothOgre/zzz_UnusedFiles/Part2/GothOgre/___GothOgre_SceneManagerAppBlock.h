#ifndef GOTHOGRE_SCENE_MANAGER_APP_BLOCK_H
#define GOTHOGRE_SCENE_MANAGER_APP_BLOCK_H

#include "GothOgre_AppBlock.h"
#include "GothOgre_DefaultSceneManager.h"
#include "GothOgre_EntitySubTypes.h"

namespace GothOgre
{

	class SceneManagerAppBlock : public AppBlock
	{
	public:
		SceneManagerAppBlock()
			: mDefaultSceneManagerFactory(nullptr),
			  mBspEntityFactory(nullptr),
			  mLightEntityFactory(nullptr),
			  mSoundEntityFactory(nullptr),
			  mWaynetEntityFactory(nullptr)
		{
		}

		~SceneManagerAppBlock()
		{
		}

		void initialise()
		{
			// instantiate and register movable factories
			mBspEntityFactory = new BspEntityFactory();
			mLightEntityFactory = new LightEntityFactory();
			mSoundEntityFactory = new SoundEntityFactory();
			mWaynetEntityFactory = new WaynetEntityFactory();

			// register default scene manager factory
			mDefaultSceneManagerFactory = new DefaultSceneManagerFactory;
		}

		void shutdown()
		{
			// remove default scene manager factory
			if(mDefaultSceneManagerFactory)
			{
				delete mDefaultSceneManagerFactory;
				mDefaultSceneManagerFactory = nullptr;
			}

			// remove movable factories
			if(mBspEntityFactory)
			{
				delete mBspEntityFactory;
				mDefaultSceneManagerFactory = nullptr;
			}
			
			if(mLightEntityFactory)
			{
				delete mLightEntityFactory;
				mLightEntityFactory = nullptr;
			}

			if(mSoundEntityFactory)
			{
				delete mSoundEntityFactory;
				mSoundEntityFactory = nullptr;
			}

			if(mWaynetEntityFactory)
			{
				delete mWaynetEntityFactory;
				mWaynetEntityFactory = nullptr;
			}
		}

	private:
		BspEntityFactory*            mBspEntityFactory;
		LightEntityFactory*          mLightEntityFactory;
		SoundEntityFactory*          mSoundEntityFactory;
		WaynetEntityFactory*         mWaynetEntityFactory;
		DefaultSceneManagerFactory*  mDefaultSceneManagerFactory;
	};

}

#endif // GOTHOGRE_SCENE_MANAGER_APP_BLOCK_H