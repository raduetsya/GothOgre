#include "GothOgre_Precompiled.h"
#include "GothOgre_BaseWorldObject_0.h"
#include "GothOgre_World.h"
#include "GothOgre_WorldManager.h"


namespace GothOgre
{
	//-------------------------------------------------------------------
	BaseWorldObject_0::BaseWorldObject_0()
	: mSceneNode( nullptr ), mParent( nullptr ), mWorld(nullptr), mSelected(false)
	{
		mWorld = WorldManager::getSingleton().getActiveWorld();
	}
	//---------------------------------------------------------------------------------
	BaseWorldObject_0::~BaseWorldObject_0()
	{
	}
	//---------------------------------------------------------------------------------
	const String& BaseWorldObject_0::getName() const
	{
		return mName;
	}
	//---------------------------------------------------------------------------------
	String BaseWorldObject_0::getCombinedName() const
	{
		String str = getName();
		if(!str.empty())
			str += ' ';

		str += '(';
		str += getWorldObjectTypeName();
		str += ')';
		return str;
	}
	//---------------------------------------------------------------------------------
	const Vector3& BaseWorldObject_0::getPosition() const
	{
		return mSceneNode->getPosition();
	}
	//---------------------------------------------------------------------------------
	const Quaternion& BaseWorldObject_0::getOrientation() const
	{
		return mSceneNode->getOrientation();
	}
	//---------------------------------------------------------------------------------
	const Vector3& BaseWorldObject_0::getScale() const
	{
		return mSceneNode->getScale();
	}
	//---------------------------------------------------------------------------------
	const Quaternion& BaseWorldObject_0::getDerivedOrientation() const
	{
		return mSceneNode->_getDerivedOrientation();
	}
	//---------------------------------------------------------------------------------
	const Vector3& BaseWorldObject_0::getDerivedPosition() const
	{
		return mSceneNode->_getDerivedPosition();
	}
	//---------------------------------------------------------------------------------
	const Vector3& BaseWorldObject_0::getDerivedScale() const
	{
		return mSceneNode->_getDerivedScale();
	}
	//---------------------------------------------------------------------------------
	const Matrix4& BaseWorldObject_0::getFullTransform() const
	{
		return mSceneNode->_getFullTransform();
	}
	//---------------------------------------------------------------------------------
	WorldObject* BaseWorldObject_0::getParent() const
	{
		return mParent;
	}
	//---------------------------------------------------------------------------------
	const WorldObject::Children& BaseWorldObject_0::getAllChildren() const
	{
		return mChildren;
	}
	//---------------------------------------------------------------------------------
	WorldObject::ChildIterator BaseWorldObject_0::getChildIterator() const
	{
		return ChildIterator(mChildren);
	}
	//---------------------------------------------------------------------------------
	WorldObjectPtr BaseWorldObject_0::getChild(const String& _name) const
	{
		for(list<WorldObjectPtr>::type::const_iterator it = mChildren.begin();
			it != mChildren.end(); ++it)
		{
			if((*it)->getName() == _name)
				return *it;
		}
		return WorldObjectPtr();
	}
	//---------------------------------------------------------------------------------
	SceneNode* BaseWorldObject_0::getSceneNode() const
	{
		return mSceneNode;
	}
	//---------------------------------------------------------------------------------
	bool BaseWorldObject_0::isSelected() const
	{
		return mSelected;
	}
	//---------------------------------------------------------------------------------
	World* BaseWorldObject_0::getWorld() const
	{
		return mWorld;
	}
	//---------------------------------------------------------------------------------
	void BaseWorldObject_0::_changeName(const String& _name)
	{
		mName = _name;
	}
	//---------------------------------------------------------------------------------
	void BaseWorldObject_0::_changePosition(const Vector3& _position)
	{
		mSceneNode->setPosition(_position);
	}
	//---------------------------------------------------------------------------------
	void BaseWorldObject_0::_changeOrientation(const Quaternion& _orientation)
	{
		mSceneNode->setOrientation(_orientation);
	}
	//---------------------------------------------------------------------------------
	void BaseWorldObject_0::_changeScale(const Vector3& _scale)
	{
		mSceneNode->setScale(_scale);
	}
	//---------------------------------------------------------------------------------
	void BaseWorldObject_0::_changeDerivedPosition(const Vector3& _derivedPosition)
	{
		if(mSceneNode->getParent())
			mSceneNode->_setDerivedPosition(_derivedPosition);
		else
			mSceneNode->setPosition(_derivedPosition);
	}
	//---------------------------------------------------------------------------------
	void BaseWorldObject_0::_changeDerivedOrientation(const Quaternion& _derivedOrientation)
	{
		if(mSceneNode->getParent())
			mSceneNode->_setDerivedOrientation(_derivedOrientation);
		else
			mSceneNode->setOrientation(_derivedOrientation);
	}
	//---------------------------------------------------------------------------------
	void BaseWorldObject_0::_addToListOfChildren(const WorldObjectPtr& _child, bool _keepTransform)
	{
		mChildren.push_back(_child);

		Vector3 position;
		Quaternion orientation;
		if(_keepTransform)
		{
			position = _child->getPosition();
			orientation = _child->getOrientation();
		}
		mSceneNode->addChild(_child->getSceneNode());
		if(_keepTransform)
		{
			_child->setDerivedPosition(position);
			_child->setDerivedOrientation(orientation);
		}		
	}
	//---------------------------------------------------------------------------------
	void BaseWorldObject_0::_removeFromListOfChildren(const WorldObjectPtr& _child, bool _keepTransform)
	{
		Children::iterator it = std::find(mChildren.begin(), mChildren.end(), _child);
		assert(it != mChildren.end());
		mChildren.erase(it);

		Vector3 position;
		Quaternion orientation;
		if(_keepTransform)
		{
			position = _child->getDerivedPosition();
			orientation = _child->getDerivedOrientation();
		}
		mSceneNode->removeChild(_child->getSceneNode());
		if(_keepTransform)
		{
			_child->setPosition(position);
			_child->setOrientation(orientation);
		}
	}
	//---------------------------------------------------------------------------------
	void BaseWorldObject_0::_changeParent(WorldObject* _parent)
	{
		if(_parent)
		{
			if(mParent)
				GOTHOGRE_EXCEPT("World object cannot have two parents at the same time.");
			if(mWorld != _parent->getWorld())
				GOTHOGRE_EXCEPT("World object cannot have parent from another world.");
		}
		mParent = _parent;
	}
	//---------------------------------------------------------------------------------
	void BaseWorldObject_0::_changeSelectState(bool _selectState)
	{
		mSelected = _selectState;
	}
	


	
	//---------------------------------------------------------------------------------
	// Utility class to show selection state
	//---------------------------------------------------------------------------------
	class SelectionStateShower
	{
	public:
		static void show(WorldObject* _wobj, bool _selected)
		{
			SceneNode* sceneNode = _wobj->getSceneNode();
			show( _wobj, sceneNode, _selected, _wobj->getParent()->isSelected());
		}

	private:
		static void show(WorldObject* _wobj, SceneNode* _sceneNode, bool _selected, bool _parentSelected)
		{
			/*
			if(_selected)
				sceneNode->showBoundingBox(_selected);
			//*/

			SceneNode::ObjectIterator itObj = _sceneNode->getAttachedObjectIterator();
			while(itObj.hasMoreElements())
			{
				MovableObject* movable = itObj.getNext();
				show(movable, _selected, _parentSelected);
			}

			SceneNode::ChildNodeIterator itChild = _sceneNode->getChildIterator();
			while(itChild.hasMoreElements())
			{
				SceneNode* child = (SceneNode*) (itChild.getNext());
				WorldObject* newWObj = _wobj;
				bool newSelected = _selected;
				bool newParentSelected = _parentSelected;
				Any any = child->getUserAny();
				if(any.getType() == typeid(WorldObject*))
				{
					WorldObject* newWObj = any.operator ()<WorldObject*>();
					if(newWObj != _wobj)
					{
						newSelected = newWObj->isSelected();
						newParentSelected = _selected;
					}
				}
				show(newWObj, child, newSelected, newParentSelected);
			}
		}

		static void show(MovableObject* _movable, bool _selected, bool _parentSelected)
		{
			if(_movable->getMovableType() == EntityFactory::FACTORY_TYPE_NAME)
			{
				Entity* entity = static_cast<Entity*>(_movable);
				show(entity, _selected, _parentSelected);
			}
		}

		static void show(Entity* _entity, bool _selected, bool _parentSelected)
		{
			Entity::ChildObjectListIterator it = _entity->getAttachedObjectIterator();
			while(it.hasMoreElements())
			{
				MovableObject* movable2 = it.getNext();
				show(_entity, _selected, _parentSelected);
			}

			SceneManagerEx* sceneMgr = (SceneManagerEx*) _entity->_getManager();
			for(size_t i = 0; i != _entity->getNumSubEntities(); ++i)
			{
				SubEntity* subEntity = _entity->getSubEntity(i);
				MaterialPtr material = subEntity->getMaterial();
				MatGenInfo matGenInfo = sceneMgr->getMatGenInfo(material);
				MatGenParams& matGenParams = matGenInfo.matGenParams;
				matGenParams.setSelected(_selected);
				matGenParams.setParentSelected(_parentSelected);
				MaterialPtr material2 = sceneMgr->generateMaterial(matGenInfo);
				subEntity->setMaterial(material2);
			}
		}
	};
	//---------------------------------------------------------------------------------
	void BaseWorldObject_0::_showSelectState(bool _selectState)
	{
		SelectionStateShower::show(this, _selectState);
	}

} // namespace GothOgre
