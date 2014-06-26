#ifndef GOTHOGRE_DERIVED_WORLD_OBJECT_H
#define GOTHOGRE_DERIVED_WORLD_OBJECT_H

#include "GothOgre_WorldObject.h"


namespace GothOgre
{
	/** This class provides an interface to create a WorldObject's subclass 
	based on other subclass.
	Thereby an object of the first subclass can use data 
	of the second subclass. A hierarchy of subclasses can be created.
	This is an alternative way to inherit subclasses from another subclasses.
	(The normal C++ inheritance cannot be used here because
	some classes can be provided in plugins and a compiler will not see
	their definitions.) */
	class GOTHOGRE_EXPORT DerivedWorldObject : public WorldObject
	{
	public:
		/** Constructor */
		DerivedWorldObject(const String& _baseTypeName);

		/** Destructor */
		virtual ~DerivedWorldObject();

		/** Returns the base world object for this. */
		WorldObjectPtr getBase() const {return mBaseData;}

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

		/** Returns a pointer to scene node attached to the world object. */
		virtual SceneNode* getSceneNode() const;

		/** Loads the object from a specified zen archive */
		virtual void read(ZenArchive* _zenArchive, ushort _version);
		
		/** Saves the object to a specified zen archive */
		virtual void write(ZenArchive* _zenArchive, ushort _version);

		/** Returns a pointer to a world this object attacked to. */
		virtual World* getWorld() const;

		/** Whether the world object is selected. */
		virtual bool isSelected() const;

		ZenParamNameList getParamNameList() const;
		ZenParamDef* findParamDef(const String& _name, void** _targetPtr);

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

	private:
		WorldObjectPtr mBaseData;
	};

} // namespace GothOgre

#endif // GOTHOGRE_DERIVED_WORLD_OBJECT_H