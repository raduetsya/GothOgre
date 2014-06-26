#include "Precompiled.h"
#include "MovableObjectDestroyer.h"
#include "SceneManagerExImpl.h"


namespace GothOgre
{
	//--------------------------------------------------------------------------
	MovableObjectDestroyer::MovableObjectDestroyer(SceneManagerExImpl* _sm)
		: mSceneManager(_sm)
	{
	}
	//--------------------------------------------------------------------------
	MovableObjectDestroyer::~MovableObjectDestroyer()
	{
	}
	//--------------------------------------------------------------------------
	void MovableObjectDestroyer::destroyAllAttachedObjects(SceneNode* _sceneNode)
	{
		_destroyAllAttachedObjects(_sceneNode);
		destroyCollectedVertexDatas();
	}
	//--------------------------------------------------------------------------
	void MovableObjectDestroyer::_destroyAllAttachedObjects(SceneNode* _sceneNode)
	{
		while(true)
		{
			SceneNode::ObjectIterator movableIt = _sceneNode->getAttachedObjectIterator();
			if(!movableIt.hasMoreElements())
				break;
			MovableObject* movable = movableIt.getNext();
			destroyMovableObject(movable);
		}

		SceneNode::ChildNodeIterator childIt = _sceneNode->getChildIterator();
		while(childIt.hasMoreElements())
		{
			SceneNode* child = static_cast<SceneNode*>( childIt.getNext() );
			_destroyAllAttachedObjects(child);
		}
	}
	//--------------------------------------------------------------------------
	void MovableObjectDestroyer::destroyMovableObject(MovableObject* _movable)
	{
		if((_movable->getTypeFlags() & SceneManager::ENTITY_TYPE_MASK) != 0)
		{
			Entity* entity = static_cast<Entity*>(_movable);
			destroyEntity(entity);
		}
		else
		{
			mSceneManager->destroyMovableObject(_movable);
		}
	}
	//--------------------------------------------------------------------------
	void MovableObjectDestroyer::destroyEntity(Entity* _entity)
	{
		// Remove child objects (child objects can be attached to entity)
		while(true)
		{
			Entity::ChildObjectListIterator it = _entity->getAttachedObjectIterator();
			if(!it.hasMoreElements())
				break;
			MovableObject* movable2 = it.getNext();
			destroyMovableObject(movable2);
		}

		// Get mesh (used later)
		MeshPtr mesh = _entity->getMesh();

		// Destroy entity
		mSceneManager->destroyEntity(_entity);

		// Destroy mesh if no more references
		if(mesh.useCount() > 1 + ResourceGroupManager::RESOURCE_SYSTEM_NUM_REFERENCE_COUNTS)
			return;

		// Collect vertex datas
		VertexData* vd = mesh->sharedVertexData;
		if(vd)
		{
			mVertexDataCollection.insert(vd);
			mesh->sharedVertexData = nullptr;
		}
		Mesh::SubMeshIterator it2 = mesh->getSubMeshIterator(); 
		while(it2.hasMoreElements())
		{
			SubMesh* subMesh = it2.getNext();
			vd = subMesh->vertexData;
			if(vd)
			{
				mVertexDataCollection.insert(vd);
				subMesh->vertexData = nullptr;
			}
		}

		ResourcePtr resource = mesh; 
		mesh.setNull();
		MeshManager::getSingleton().remove( resource );
	}
	//--------------------------------------------------------------------------
	void MovableObjectDestroyer::destroyCollectedVertexDatas()
	{
		// Destroy all vertex datas
		for(VertexDataCollection::iterator it = mVertexDataCollection.begin();
			it != mVertexDataCollection.end(); ++it)
		{
			VertexData* vd = *it;
			OGRE_DELETE vd;
		}

		// Clear a list
		mVertexDataCollection.clear();
	}

} // namespace GothOgre
