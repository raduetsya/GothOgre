#include "GothOgre_Precompiled.h"
#include "GothOgre_SceneManagerExEnumerator.h"
#include "GothOgre_SceneManagerEx.h"


//-----------------------------------------------------------------------------
GothOgre::SceneManagerExEnumerator* 
	Ogre::Singleton<GothOgre::SceneManagerExEnumerator>::ms_Singleton 
	= nullptr;
//-----------------------------------------------------------------------------


namespace GothOgre
{
	//-------------------------------------------------------------------------
	SceneManagerExEnumerator::SceneManagerExEnumerator()
		: mNameGenerator( "SceneManagerEx" ),
		  mActiveSceneManager( nullptr )
	{
	}
	//-------------------------------------------------------------------------
	SceneManagerExEnumerator::~SceneManagerExEnumerator()
	{
		for(SceneManagerList::iterator it = mSceneManagers.begin(); 
			it != mSceneManagers.end(); ++it)
		{
			SceneManager* sm = *it;
			Root::getSingleton().destroySceneManager(sm);
		}
	}
	//-------------------------------------------------------------------------
	SceneManagerEx* SceneManagerExEnumerator::_createSceneManager()
	{
		String instanceName = mNameGenerator.generate();
		SceneManagerEx* sm = (SceneManagerEx*) Root::getSingleton().createSceneManager( ST_GOTHOGRE, instanceName );
		GOTHOGRE_ASSERT(sm && sm->getTypeName() != DefaultSceneManagerFactory::FACTORY_TYPE_NAME, "An implementation of the SceneManagerEx class wasn't found.");
		mSceneManagers.push_back(sm);
		return sm;
	}
	//-------------------------------------------------------------------------
	void SceneManagerExEnumerator::_destroySceneManager(SceneManagerEx* _sm)
	{
		if(mActiveSceneManager == _sm)
			mActiveSceneManager = nullptr;

		SceneManagerList::iterator it = std::find(mSceneManagers.begin(), mSceneManagers.end(), _sm);
		if(it != mSceneManagers.end())
			mSceneManagers.erase(it);

		Root::getSingleton().destroySceneManager(_sm);
	}
	//-------------------------------------------------------------------------
	void SceneManagerExEnumerator::_setActiveSceneManager(SceneManagerEx* _sm) 
	{
		mActiveSceneManager = _sm;
	}


} // namespace GothOgre