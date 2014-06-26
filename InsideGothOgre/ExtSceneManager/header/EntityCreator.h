#ifndef ENTITY_CREATOR_H
#define ENTITY_CREATOR_H


namespace GothOgre
{
	class SceneManagerExImpl;


	//----------------------------------------------------------------------
	class EntityCreator
	{
	public:
		EntityCreator( SceneManagerExImpl* _sceneManager );
		~EntityCreator();

		Entity* createEntity( const MeshExPtr& _mesh, uint32 _queryMask );

	private:
		void setupMaterialNames( const MeshExPtr& _mesh );

	private:
		SceneManagerExImpl*   mSceneManager;
		static NameGenerator  ms_NameGenerator;
	};

} // namespace GothOgre

#endif // ENTITY_CREATOR_H
