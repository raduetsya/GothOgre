#ifndef GOTHOGRE_SCENE_MANAGER_EX_ENUMERATOR_H
#define GOTHOGRE_SCENE_MANAGER_EX_ENUMERATOR_H


namespace GothOgre
{
	class SceneManagerEx;


	//-------------------------------------------------------------------------
	/** Enumerates the instances of the SceneManagerEx class. */
	class GOTHOGRE_EXPORT SceneManagerExEnumerator : public Singleton<SceneManagerExEnumerator>
	{
	public:
		SceneManagerExEnumerator();
		~SceneManagerExEnumerator();

		typedef vector<SceneManagerEx*>::type SceneManagerList;
		typedef VectorIterator<SceneManagerList> SceneManagerIterator;

		/** Returns an iterator over all the existing SceneManager instances. */
		SceneManagerIterator getSceneManagerIterator() {return (SceneManagerIterator) mSceneManagers;}

		/** Returns a pointer to the active scene manager, 
		or nullptr if there is no active scene manager. */
		SceneManagerEx* getActiveSceneManager() const {return mActiveSceneManager;}

	private:
		friend class World;

		/** Creates a scene manager 
		(have to be called by the constructor of the World class. */
		SceneManagerEx* _createSceneManager();

		/** Destroys a scene manager 
		(have to be called by the destructor of the World class. */
		void _destroySceneManager(SceneManagerEx* _sm);

		/** Changes a pointer to the active scene manager 
		(have to be called by the World::_setActive function. */
		void _setActiveSceneManager(SceneManagerEx* _sm);

	private:
		SceneManagerList  mSceneManagers;
		NameGenerator     mNameGenerator;
		SceneManagerEx*   mActiveSceneManager;
	};

} // namespace GothOgre

#endif // GOTHOGRE_SCENE_MANAGER_EX_ENUMERATOR_H