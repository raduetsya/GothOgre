#include "GothOgre_Precompiled.h"
#include "GothOgre_ZenManager.h"
#include "GothOgre_World.h"
#include "GothOgre_DerivedWorldObject.h"

namespace GothOgre
{
	DerivedWorldObject::DerivedWorldObject( const String& _baseTypeName )
	{
		mBaseData = ZenManager::getSingleton().createZenObject(_baseTypeName);
		getSceneNode()->setUserAny( (Any) static_cast<WorldObject*>(this) );
	}
	//---------------------------------------------------------------------------------
	DerivedWorldObject::~DerivedWorldObject()
	{
		deselect();
		removeAllChildren();
	}
	//---------------------------------------------------------------------------------
	const String& DerivedWorldObject::getName() const
	{
		return mBaseData->getName();
	}
	//---------------------------------------------------------------------------------
	String DerivedWorldObject::getCombinedName() const
	{
		return mBaseData->getCombinedName();
	}
	//---------------------------------------------------------------------------------
	const Vector3& DerivedWorldObject::getPosition() const
	{
		return mBaseData->getPosition();
	}
	//---------------------------------------------------------------------------------
	const Quaternion& DerivedWorldObject::getOrientation() const
	{
		return mBaseData->getOrientation();
	}
	//---------------------------------------------------------------------------------
	const Vector3& DerivedWorldObject::getScale() const
	{
		return mBaseData->getScale();
	}
	//---------------------------------------------------------------------------------
	const Quaternion& DerivedWorldObject::getDerivedOrientation() const
	{
		return mBaseData->getDerivedOrientation();
	}
	//---------------------------------------------------------------------------------
	const Vector3& DerivedWorldObject::getDerivedPosition() const
	{
		return mBaseData->getDerivedPosition();
	}
	//---------------------------------------------------------------------------------
	const Vector3& DerivedWorldObject::getDerivedScale() const
	{
		return mBaseData->getDerivedScale();
	}
	//---------------------------------------------------------------------------------
	const Matrix4& DerivedWorldObject::getFullTransform() const
	{
		return mBaseData->getFullTransform();
	}
	//---------------------------------------------------------------------------------
	WorldObject* DerivedWorldObject::getParent() const
	{
		return mBaseData->getParent();
	}
	//---------------------------------------------------------------------------------
	const WorldObject::Children& DerivedWorldObject::getAllChildren() const
	{
		return mBaseData->getAllChildren();
	}
	//---------------------------------------------------------------------------------
	WorldObject::ChildIterator DerivedWorldObject::getChildIterator() const
	{
		return mBaseData->getChildIterator();
	}
	//---------------------------------------------------------------------------------
	WorldObjectPtr DerivedWorldObject::getChild(const String& _name) const
	{
		return mBaseData->getChild(_name);
	}
	//---------------------------------------------------------------------------------
	SceneNode* DerivedWorldObject::getSceneNode() const
	{
		return mBaseData->getSceneNode();
	}
	//---------------------------------------------------------------------------------
	World* DerivedWorldObject::getWorld() const
	{
		return mBaseData->getWorld();
	}
	//---------------------------------------------------------------------------------
	void DerivedWorldObject::read(ZenArchive* _zenArchive, ushort _version)
	{
		mBaseData->read(_zenArchive, _version);
	}
	//---------------------------------------------------------------------------------
	void DerivedWorldObject::write(ZenArchive* _zenArchive, ushort _version)
	{
		mBaseData->write(_zenArchive, _version);
	}
	//---------------------------------------------------------------------------------
	bool DerivedWorldObject::isSelected() const
	{
		return mBaseData->isSelected();
	}
	//---------------------------------------------------------------------------------
	ZenParamNameList DerivedWorldObject::getParamNameList() const
	{
		ZenParamNameList lst = mBaseData->getParamNameList();
		ZenParamNameList lst2 = ZenStringInterface::getParamNameList();
		for(ZenParamNameList::iterator it = lst2.begin(); it != lst2.end(); ++it)
		{
			if(std::find(lst.begin(), lst.end(), *it) == lst.end())
				lst.push_back(*it);
		}
		return lst;
	}
	//---------------------------------------------------------------------------------
	ZenParamDef* DerivedWorldObject::findParamDef(const String& _name, void** _targetPtr)
	{
		ZenParamDef* def = ZenStringInterface::findParamDef(_name, _targetPtr);
		if(def)
			return def;
		else
			return mBaseData->findParamDef(_name, _targetPtr);
	}
	//---------------------------------------------------------------------------------
	void DerivedWorldObject::_changeName(const String& _name)
	{
		mBaseData->_changeName(_name);
	}
	//---------------------------------------------------------------------------------
	void DerivedWorldObject::_changePosition(const Vector3& _position)
	{
		mBaseData->_changePosition(_position);
	}
	//---------------------------------------------------------------------------------
	void DerivedWorldObject::_changeOrientation(const Quaternion& _orientation)
	{
		mBaseData->_changeOrientation(_orientation);
	}
	//---------------------------------------------------------------------------------
	void DerivedWorldObject::_changeScale(const Vector3& _scale)
	{
		mBaseData->_changeScale(_scale);
	}
	//---------------------------------------------------------------------------------
	void DerivedWorldObject::_changeDerivedPosition(const Vector3& _derivedPosition)
	{
		mBaseData->_changeDerivedPosition(_derivedPosition);
	}
	//---------------------------------------------------------------------------------
	void DerivedWorldObject::_changeDerivedOrientation(const Quaternion& _derivedOrientation)
	{
		mBaseData->_changeDerivedOrientation(_derivedOrientation);
	}
	//---------------------------------------------------------------------------------
	void DerivedWorldObject::_addToListOfChildren(const WorldObjectPtr& _child, bool _keepTransform)
	{
		mBaseData->_addToListOfChildren(_child, _keepTransform);
	}
	//---------------------------------------------------------------------------------
	void DerivedWorldObject::_removeFromListOfChildren(const WorldObjectPtr& _child, bool _keepTransform)
	{
		mBaseData->_removeFromListOfChildren(_child, _keepTransform);
	}
	//---------------------------------------------------------------------------------
	void DerivedWorldObject::_changeParent(WorldObject* _parent)
	{
		mBaseData->_changeParent(_parent);
	}
	//---------------------------------------------------------------------------------
	void DerivedWorldObject::_changeSelectState(bool _selectState)
	{
		mBaseData->_changeSelectState(_selectState);
	}
	//---------------------------------------------------------------------------------
	void DerivedWorldObject::_showSelectState(bool _selectState)
	{
		mBaseData->_showSelectState(_selectState);
	}

} // namespace GothOgre