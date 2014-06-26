#ifndef GOTHOGRE_EXACT_RAY_QUERY_H
#define GOTHOGRE_EXACT_RAY_QUERY_H


namespace GothOgre
{

	class GOTHOGRE_EXPORT ExactRayQuery
	{
	public:
		ExactRayQuery();
		ExactRayQuery(Ogre::SceneManager* _sceneManager, const Ogre::Ray& _ray);	
		ExactRayQuery(const Ogre::Ray& _ray, Ogre::SceneManager* _sceneManager);	
		ExactRayQuery(Real _relx, Real _rely, Ogre::Camera* _camera);
		ExactRayQuery(Real _relx, Real _rely, Ogre::Viewport* _viewport);
		ExactRayQuery(int _x, int _y, Ogre::RenderTarget* _renderTarget);
		
		~ExactRayQuery();

		void setSceneManager(Ogre::SceneManager* _sceneManager);
		SceneManager* getSceneManager() const;
		
		void setRay(const Ogre::Ray& _ray);
		void setRay(Real _relx, Real _rely, Ogre::Camera* _camera);
		void setRay(Real _relx, Real _rely, Ogre::Viewport* _viewport);
		void setRay(int _x, int _y, Ogre::RenderTarget* _renderTarget);
		const Ogre::Ray& getRay() const;

		void setCullingMode(Ogre::CullingMode _cullingMode);
		Ogre::CullingMode getCullingMode() const;

		Ogre::RaySceneQueryResult& execute();

		const Ogre::RaySceneQueryResult& getLastResults() const;

		Ogre::Entity* getNearestEntity() const;
		Ogre::Real    getNearestDistance() const;

		void clearResults();

	private:
		void init();
		void createRaySceneQuery();
		void destroyRaySceneQuery();

		static Ogre::Ray transformToLocalSpace( const Ogre::Ray& _ray, Ogre::SceneNode* _sceneNode );

		static std::pair<bool, Real> intersects( const Ogre::Ray& _ray,
			Ogre::Entity* _entity, Ogre::CullingMode _cullingMode );

		static std::pair<bool, Real> intersects( const Ogre::Ray& _ray,
			const void* _indexBuffer, size_t _indexStart, size_t _indexCount, size_t _indexSize,
			const void* _vertexBuffer, size_t _vertexStart, size_t _vertexCount,
			size_t _vertexSize, size_t _vertexPositionOffset,
			bool _positiveSide, bool _negativeSide);

		static bool isFullyTransparent(const Ogre::MaterialPtr& _material);

	private:
		Ogre::SceneManager*			mSceneManager;
		Ogre::RaySceneQuery*		mRaySceneQuery;
		Ogre::RaySceneQueryResult	mExactResult;
		Ogre::CullingMode			mCullingMode;
	};

} // namespace GothOgre

#endif // GOTHOGRE_EXACT_RAY_QUERY_H