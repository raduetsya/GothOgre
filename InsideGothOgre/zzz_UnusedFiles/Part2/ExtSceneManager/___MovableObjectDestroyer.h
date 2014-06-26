#ifndef MOVABLE_OBJECT_DESTROYER_H
#define MOVABLE_OBJECT_DESTROYER_H


namespace GothOgre
{
	class SceneManagerExImpl;

	/** This class implements utility functions to destroy movable objects. */
	class MovableObjectDestroyer
	{
	public:
		MovableObjectDestroyer(SceneManagerExImpl* _sm);
		~MovableObjectDestroyer();

		/** Detaches and destroys all movable objects,
		which are attached to a specified node and its children. */
		void destroyAllAttachedObjects(SceneNode* _sceneNode);

	private:
		void _destroyAllAttachedObjects(SceneNode* _sceneNode);
		void destroyMovableObject(MovableObject* _movable);
		void destroyEntity(Entity* _entity);
		void destroyCollectedVertexDatas();

	private:
		typedef set<VertexData*>::type VertexDataCollection;
		VertexDataCollection           mVertexDataCollection;
		SceneManagerExImpl*  mSceneManager;
	};

} // namespace GothOgre

#endif // MOVABLE_OBJECT_DESTROYER_H
