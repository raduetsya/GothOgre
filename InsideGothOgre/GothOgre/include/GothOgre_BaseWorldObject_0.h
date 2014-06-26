#ifndef GOTHOGRE_BASE_WORLD_OBJECT_0_H
#define GOTHOGRE_BASE_WORLD_OBJECT_0_H

#include "GothOgre_WorldObject.h"

namespace GothOgre
{
	class GOTHOGRE_EXPORT BaseWorldObject_0 : public WorldObject
	{
	public:
		/** General constructor */
		BaseWorldObject_0();

		/** Destructor */
		virtual ~BaseWorldObject_0();

		/** Returns the name of the world object. */
		virtual const String& getName() const;

		/** Returns the combined (name + type) name of the world object. */
		virtual String getCombinedName() const;

		/** Returns the position of the world object relative to it's parent. */
		virtual const Vector3& getPosition() const;

		/** Returns a quaternion representing the world object's orientation.  */
		virtual const Quaternion& getOrientation() const;

		/** Returns the scaling factor applied to this world object. */
		virtual const Vector3& getScale() const;

		/** Gets the orientation of the world object as derived from all parents. */
		virtual const Quaternion& getDerivedOrientation() const;

		/** Gets the position of the world object as derived from all parents. */
		virtual const Vector3& getDerivedPosition() const;

		/** Gets the scaling factor of the world object as derived from all parents. */
		virtual const Vector3& getDerivedScale() const;

		/** Gets the full transformation matrix for this world object. */
		virtual const Matrix4& getFullTransform() const;

		/** Returns this world object's parent 
		(null pointer is retrieved if this is the root).  */
		virtual WorldObject* getParent() const;

		/** Returns the list of all the children of the world object. */
		virtual const Children& getAllChildren() const;

		/** Returns an iterator to enumerate all the children of the world object. */
		virtual ChildIterator getChildIterator() const;

		/** Searchs a child by name. The function returns the first object
		with matching name. The null pointer is returned if not found. */
		virtual WorldObjectPtr getChild(const String& _name) const;

		/** Returns a pointer to a world this object attacked to. */
		virtual World* getWorld() const;

		/** Whether the world object is selected. */
		virtual bool isSelected() const;

		/** Returns a pointer to scene node attached to the world object. */
		virtual SceneNode* getSceneNode() const;

	public:
		/** Internal function.
		Changes the name without firing world listeners. */
		virtual void _changeName(const String& _name);

		/** Internal function.
		Changes the position without firing world listeners. */
		virtual void _changePosition(const Vector3& _position);

		/** Internal function.
		Changes the orientation without firing world listeners. */
		virtual void _changeOrientation(const Quaternion& _orientation);

		/** Internal function.
		Changes the scaling factor without firing world listeners. */
		virtual void _changeScale(const Vector3& _scale);

		/** Internal function.
		Changes the final world position without firing world listeners. */
		virtual void _changeDerivedPosition(const Vector3& _derivedPosition);

		/** Internal function.
		Changes the final world orientation without firing world listeners. */
		virtual void _changeDerivedOrientation(const Quaternion& _derivedOrientation);

		/** Internal function.
		Adds a specified object to the list of children of this object. */
		virtual void _addToListOfChildren(const WorldObjectPtr& _child, bool _keepTransform);

		/** Internal function.
		Removes a specified object from the list of children of this object. */
		virtual void _removeFromListOfChildren(const WorldObjectPtr& _child, bool _keepTransform);
		
		/** Internal function.
		Sets the parent of this object without editing list of children 
		of this parent and without firing world listeners. */
		virtual void _changeParent(WorldObject* _parent);

		/** Internal function.
		Changes the selection state of this object 
		without notifying the world about it. */
		virtual void _changeSelectState(bool _selectState);

		/** Internal function.
		Shows the selection state of this object. */
		virtual void _showSelectState(bool _selectState);

	protected:
		SceneNode*					mSceneNode;
		World*                      mWorld;

	private:
		String						mName;
		Children					mChildren;
		WorldObject*				mParent;
		bool						mSelected;
	};

} // namespace GothOgre

#endif // GOTHOGRE_BASE_WORLD_OBJECT_0_H