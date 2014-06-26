#include "Precompiled.h"
#include "ZenBspVisualDestroyer.h"


namespace GothOgre
{
	//-------------------------------------------------------------------------
	ZenBspVisualDestroyer::ZenBspVisualDestroyer(SceneNode* _mainSceneNode)
	{
		mMainSceneNode = _mainSceneNode;
	}
	//-------------------------------------------------------------------------
	void ZenBspVisualDestroyer::execute()
	{
		// Get a list of meshes and destroy movable objects
		list<MeshPtr>::type meshes;
		
		SceneNode::ChildNodeIterator itN = mMainSceneNode->getChildIterator();
		while(itN.hasMoreElements())
		{
			SceneNode* child = (SceneNode*) itN.getNext();
			SceneNode::ObjectIterator itO = child->getAttachedObjectIterator();
			while(itO.hasMoreElements())
			{
				MovableObject* movable = itO.getNext();
				child->detachObject(movable);
				if(movable->getTypeFlags() & SceneManager::ENTITY_TYPE_MASK)
				{
					Entity* entity = (Entity*) movable;
					meshes.push_back(entity->getMesh());
				}
				movable->_getManager()->destroyMovableObject(movable);
			}
		}

		// Destroy scene nodes
		mMainSceneNode->removeAndDestroyAllChildren();

		// Remove meshes
		for(list<MeshPtr>::type::iterator it = meshes.begin(); 
			it != meshes.end(); ++it)
		{
			MeshPtr mesh = *it;
			if(!mesh.isNull())
			{
				// The same vertex data can be shared between lots of meshes,
				// so if we don't remove we should remove excess links,
				// or MeshManager::remove causes fault.
				VertexData* vertexData = mesh->sharedVertexData;
				if(vertexData != nullptr)
				{
					// Remove other links to the same vertex data
					list<MeshPtr>::type::iterator it2 = it; ++it2;
					for(; it2 != meshes.end(); ++it2)
					{
						MeshPtr othermesh = *it2;
						if(othermesh->sharedVertexData == vertexData)
							othermesh->sharedVertexData = nullptr;
					}
				}
				MeshManagerEx::getSingleton().remove(mesh);
			}
		}
	}

} // namespace GothOgre
