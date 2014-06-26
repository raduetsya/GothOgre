#ifndef GOTHOGRE_DEFAULT_EXT_SCENE_MANAGER_H
#define GOTHOGRE_DEFAULT_EXT_SCENE_MANAGER_H

#include "GothOgre_SceneManager.h"
#include "GothOgre_SceneQuery.h"


namespace GothOgre
{
	class DefaultExtRaySceneQuery : public ExtRaySceneQuery
	{
	public:
		DefaultExtRaySceneQuery( ExtSceneManager* _sceneManager );
		~DefaultExtRaySceneQuery();
		void execute( ExtRaySceneQueryListener* _listener );

	private:
		RaySceneQuery*  mOgreQuery;
	};


	//-----------------------------------------------------------------------
	/// Default version of ExtSceneManager.
	class DefaultExtSceneManager : public ExtSceneManager
	{
	public:
		DefaultExtSceneManager(const String& _instanceName);
		~DefaultExtSceneManager();

		const String& getTypeName() const;

		void addBsp(const BspPtr& _bsp);
		void removeBsp(const BspPtr& _bsp);
		void removeAllBsps();
		vector<BspPtr>::type getAllBsps() const;

		void setNonBspPartMaxSize(const Vector3& _maxSize);
		const Vector3& getNonBspPartMaxSize() const;

		void setFarClipDistance(const Real _farClipDistance);
		Real getFarClipDistance() const;

		ExtRaySceneQuery* createExtRayQuery();

		const String& getSector(MovableObject* _obj);
		bool isUnderwater(MovableObject* _obj);
	};


	//-----------------------------------------------------------------------
	// Factory for DefaultExtSceneManager
	class DefaultExtSceneManagerFactory : public SceneManagerFactory
	{
	public:
		DefaultExtSceneManagerFactory();
		~DefaultExtSceneManagerFactory();
		static const String FACTORY_TYPE_NAME;
		void initMetaData() const;
		SceneManager* createInstance(const String& instanceName);
		void destroyInstance(SceneManager* instance);
	};
}

#endif // GOTHOGRE_EXT_DEFAULT_SCENE_MANAGER_H
