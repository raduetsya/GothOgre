#include "Precompiled.h"
#include "SceneManagerExImpl.h"
#include "NewSceneNode.h"


namespace GothOgre
{
	//------------------------------------------------------------------------
	NewSceneNode::NewSceneNode(SceneManager* _creator)
		: SceneNode(_creator)
	{
		mBstElemHandle = SceneNodeBST::NULL_HANDLE;
	}
	//------------------------------------------------------------------------
	NewSceneNode::NewSceneNode(SceneManager* _creator, const String& _name)
		: SceneNode(_creator, _name)
	{
		mBstElemHandle = SceneNodeBST::NULL_HANDLE;
	}
	//------------------------------------------------------------------------
	NewSceneNode::~NewSceneNode()
	{
		checkNoObjects();
		checkNoChildren();
		removeFromBST();
	}
	//------------------------------------------------------------------------
	void NewSceneNode::checkNoObjects()
	{
		// Show error message if there are non-removed movable objects
		size_t num = numAttachedObjects();
		if(num != 0)
		{
			StrStream ss;
			ss << "Scene node removed with " << num << " attached movable objects. Types of objects:";
			for(size_t i = 0; i != num; ++i)
				ss << ' ' << getAttachedObject(i)->getMovableType();
			GOTHOGRE_ERROR(ss.str());
		}
	}
	//------------------------------------------------------------------------
	void NewSceneNode::checkNoChildren()
	{
		// Show error message if there are non-removed child scene nodes
		size_t num = numChildren();
		if(num != 0)
		{
			StrStream ss;
			ss << "Scene node removed with " << num << " children.";
			GOTHOGRE_ERROR(ss.str());
		}
	}
	//------------------------------------------------------------------------
	void NewSceneNode::setParent(Node* _parent)
	{
		SceneNode::setParent(_parent);

		if(!_parent)
			removeFromBST();
	}
	//------------------------------------------------------------------------
	void NewSceneNode::removeFromBST()
	{
		if(mBstElemHandle != SceneNodeBST::NULL_HANDLE)
		{
			SceneManagerExImpl* sm = (SceneManagerExImpl*) getCreator();
			SceneNodeBST& bst = sm->_getSceneNodeBST();
			bst.remove(mBstElemHandle);
			mBstElemHandle = SceneNodeBST::NULL_HANDLE;
		}
	}
	//------------------------------------------------------------------------
	void NewSceneNode::_updateBounds()
	{
		SceneNode::_updateBounds();

		const AxisAlignedBox& aab = _getWorldAABB();
		if(aab.isFinite())
		{
			Vector3 position = aab.getCenter();
			Real radius = aab.getHalfSize().length();
			
			SceneManagerExImpl* sm = (SceneManagerExImpl*) getCreator();
			SceneNodeBST& bst = sm->_getSceneNodeBST();

			if(mBstElemHandle == SceneNodeBST::NULL_HANDLE)
			{
				mBstElemHandle = bst.add(position, radius, this);
			}
			else
			{
				bst.setCenter(mBstElemHandle, position);
				bst.setRadius(mBstElemHandle, radius);
			}
		}
	}

} // namespace GothOgre

