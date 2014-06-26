#include "GothOgre_Precompiled.h"
#include "GothOgre_WorldObject.h"
#include "GothOgre_WorldManager.h"


namespace GothOgre
{
	//-------------------------------------------------------------------------
	// WorldObject
	//-------------------------------------------------------------------------
	WorldObject::WorldObject()
	{
		WorldManager::getSingleton()._worldObjectCreated(this);
	}
	//-------------------------------------------------------------------------
	WorldObject::~WorldObject() 
	{
		WorldManager::getSingleton()._worldObjectDestroyed(this);
	}
	//-------------------------------------------------------------------------
	void WorldObject::setName(const String& _name)
	{
		_changeName(_name);
		static const String sParamName = "$Name";
		WorldManager::getSingleton()._worldObjectChanged(this, sParamName);
	}
	//-------------------------------------------------------------------------
	void WorldObject::setPosition(const Vector3& _pos)
	{
		_changePosition(_pos);
		static const String sParamName = "$Position";
		WorldManager::getSingleton()._worldObjectChanged(this, sParamName);
	}
	//-------------------------------------------------------------------------
	void WorldObject::setOrientation(const Quaternion& _q)
	{
		_changeOrientation(_q);
		static const String sParamName = "$Orientation";
		WorldManager::getSingleton()._worldObjectChanged(this, sParamName);
	}
	//-------------------------------------------------------------------------
	void WorldObject::setScale(const Vector3& _scale)
	{
		_changeScale(_scale);
		static const String sParamName = "$Scale";
		WorldManager::getSingleton()._worldObjectChanged(this, sParamName);
	}
	//-------------------------------------------------------------------------
	void WorldObject::setDerivedPosition(const Vector3& _derivedPosition)
	{
		_changeDerivedPosition(_derivedPosition);
		static const String sParamName = "$DPosition";
		WorldManager::getSingleton()._worldObjectChanged(this, sParamName);
	}
	//-------------------------------------------------------------------------
	void WorldObject::setDerivedOrientation(const Quaternion& _derivedOrientation)
	{
		_changeDerivedOrientation(_derivedOrientation);
		static const String sParamName = "$DOrientation";
		WorldManager::getSingleton()._worldObjectChanged(this, sParamName);
	}
	//-------------------------------------------------------------------------
	void WorldObject::addChild(const WorldObjectPtr& _child, bool _keepTransform)
	{
		_child->_changeParent(this);
		_addToListOfChildren(_child, _keepTransform);
		WorldManager::getSingleton()._worldObjectAdded(_child.get());
	}
	//-------------------------------------------------------------------------
	WorldObjectPtr WorldObject::removeChild(const WorldObjectPtr& _child, bool _keepTransform)
	{
		WorldObjectPtr ret = _child;
		if(_child->isSelected())
			_child->setSelected(false);

		_child->_changeParent(nullptr);
		_removeFromListOfChildren(_child, _keepTransform);

		WorldManager::getSingleton()._worldObjectRemoved(_child.get());
		return ret;
	}
	//-------------------------------------------------------------------------
	WorldObject::Children WorldObject::removeAllChildren(bool _keepTransform)
	{
		Children result;
		while(true)
		{
			ChildIterator it = getChildIterator();
			if(!it.hasMoreElements()) break;
			WorldObjectPtr child = it.getNext();
			removeChild(child, _keepTransform);
			result.push_back(child);
		}
		return result;
	}
	//-------------------------------------------------------------------------
	void WorldObject::removeAllChildren()
	{
		while(true)
		{
			ChildIterator it = getChildIterator();
			if(!it.hasMoreElements()) break;
			WorldObjectPtr child = it.getNext();
			removeChild(child);
		}
	}
	//-------------------------------------------------------------------------
	size_t WorldObject::numChildren() const
	{
		size_t num = 0;
		ChildIterator it = getChildIterator();
		while(it.hasMoreElements())
		{
			WorldObjectPtr child = it.getNext();
			++num;
		}
		return num;
	}
	//-------------------------------------------------------------------------
	WorldObjectPtr WorldObject::getChild(ushort _index) const
	{
		ChildIterator it = getChildIterator();
		while(it.hasMoreElements())
		{
			WorldObjectPtr child = it.getNext();
			if(!_index--) return child;
		}
		return WorldObjectPtr();		
	}
	//-------------------------------------------------------------------------
	void WorldObject::setSelected(bool _select)
	{
		if(isSelected() != _select)
		{
			_changeSelectState(_select);
			_showSelectState(_select);
			WorldManager::getSingleton()._worldObjectSelectChanged(this, _select);
		}
	}
	//-------------------------------------------------------------------------
	void WorldObject::parameterChanged(const String& _paramName)
	{
		WorldManager::getSingleton()._worldObjectChanged(this, _paramName);
	}
	//-------------------------------------------------------------------------
	const String& WorldObject::getWorldObjectTypeName() const
	{
		SceneNode* sceneNode = getSceneNode();
		WorldObject* subtypeWorldObject = getFromSceneNode(sceneNode);
		return subtypeWorldObject->getTypeName();
	}
	//-------------------------------------------------------------------------
	WorldObject* WorldObject::getFromSceneNode(SceneNode* _node)
	{
		SceneNode* node = _node;
		while(node)
		{
			Any any = node->getUserAny();
			if(any.getType() == typeid(WorldObject*))
			{
				WorldObject* p = any.operator ()<WorldObject*>();
				if(p)
					return p;
				else
					node = node->getParentSceneNode();
			}
			else
				node = node->getParentSceneNode();
		}
		return nullptr;
	}
	//-------------------------------------------------------------------------
	WorldObject* WorldObject::getFromMovableObject(MovableObject* _movable)
	{
		if(!_movable)
			return nullptr;

		SceneNode* node = _movable->getParentSceneNode();
		return getFromSceneNode(node);
	}




	//-----------------------------------------------------------------------
	// WorldObjectPtr
	//-----------------------------------------------------------------------
	WorldObjectPtr::WorldObjectPtr(WorldObject* _p)
		: ZenObjectPtr(_p) 
	{
	}
	//-----------------------------------------------------------------------
	WorldObjectPtr::WorldObjectPtr(SceneNode* _sceneNode)
	{
		WorldObject* p = WorldObject::getFromSceneNode(_sceneNode);
		if(p)
		{
			bind(p);
		}
	}
	//-----------------------------------------------------------------------
	WorldObject* WorldObjectPtr::operator->() const 
	{ 
		return static_cast<WorldObject*>(ZenObjectPtr::operator ->());
	}
	//-----------------------------------------------------------------------
	WorldObject* WorldObjectPtr::get() const 
	{
		return static_cast<WorldObject*>(ZenObjectPtr::get()); 
	}

} // namespace GothOgre

