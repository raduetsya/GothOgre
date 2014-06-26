#ifndef GOTHOGRE_WORLD_OBJECT_H
#define GOTHOGRE_WORLD_OBJECT_H

#include "GothOgre_ZenObject.h"

namespace GothOgre
{
	class GraphicSettingsEvent;
	class WorldObjectPtr;
	class World;


	//--------------------------------------------------------------------------
	/** WorldObject represents a union of two concepts: a scene node in Ogre
	and an object which can be written to/read from a zen archive.
	
	@par WorldObject is a superclass. There is not a factory for this class
	because objects of class WorldObject must not be created.
	Create subclasses of this class instead. */
	class GOTHOGRE_EXPORT WorldObject : public ZenObject
	{
	public:
		/** General constructor */
		WorldObject();

		/** Destructor */
		virtual ~WorldObject();

		/** Sets the name of the world object. */
		void setName(const String& _name);

		/** Returns the name of the world object. */
		virtual const String& getName() const = 0;

		/** Returns the combined (name + type) name of the world object. */
		virtual String getCombinedName() const = 0;

		/** Sets the position of the world object relative to it's parent. */
		void setPosition(const Vector3& _position);

		/** Returns the position of the world object relative to it's parent. */
		virtual const Vector3& getPosition() const = 0;

		/** Sets the orientation of this object via a quaternion. */
		void setOrientation(const Quaternion& _orientation);

		/** Returns a quaternion representing the world object's orientation.  */
		virtual const Quaternion& getOrientation() const = 0;

		/** Sets the scaling factor applied to this world object. */
		void setScale(const Vector3& _scale);

		/** Returns the scaling factor applied to this world object. */
		virtual const Vector3& getScale() const = 0;

		/** Sets the final world position of the world object directly. */
		void setDerivedPosition(const Vector3& _derivedPosition);

		/** Sets the final world orientation of the world object directly. */
		void setDerivedOrientation(const Quaternion& _derivedOrientation);

		/** Gets the orientation of the world object as derived from all parents. */
		virtual const Quaternion& getDerivedOrientation() const = 0;

		/** Gets the position of the world object as derived from all parents. */
		virtual const Vector3& getDerivedPosition() const = 0;

		/** Gets the scaling factor of the world object as derived from all parents. */
		virtual const Vector3& getDerivedScale() const = 0;

		/** Gets the full transformation matrix for this world object. */
		virtual const Matrix4& getFullTransform() const = 0;

		/** Returns this world object's parent 
		(null pointer is retrieved if this is the root).  */
		virtual WorldObject* getParent() const = 0;

		/** Appends a child to this world object */
		void addChild(const WorldObjectPtr& _child, bool _keepTransform = false);

		/** Removes a child from this world object.	
		@return a shared pointer to the removed child. */
		WorldObjectPtr removeChild(const WorldObjectPtr& _child, bool _keepTransform = false);

		typedef list<WorldObjectPtr>::type		Children;
		typedef ConstVectorIterator<Children>	ChildIterator;

		/** Removes all children of this world object.
		The function returns list of the removed children. */
		Children removeAllChildren(bool _keepTransform);

		/** Removes all children of this world object.
		If the removed children have no more references, they will be destroyed. */
		void removeAllChildren();

		/** Returns the list of all the children of the world object. */
		virtual const Children& getAllChildren() const = 0;

		/** Returns number of all the children of the world object. */
		size_t numChildren() const;

		/** Returns a child by index (0 .. (numChildren()-1)). */
		WorldObjectPtr getChild(ushort _index) const;

		/** Searchs a child by name. The function returns the first object
		with matching name. The null pointer is returned if not found. */
		virtual WorldObjectPtr getChild(const String& _name) const = 0;

		/** Returns an iterator to enumerate all the children of the world object. */
		virtual ChildIterator getChildIterator() const = 0;

		/** Returns true if the world object is added to any parent. */
		bool isInWorld() const;

		/** Returns a pointer to a world this object attacked to. */
		virtual World* getWorld() const = 0;
		
		/** Returns a pointer to scene node attached to the world object. */
		virtual SceneNode* getSceneNode() const = 0;

		/** Returns a pointer to a world object (if it exists)
		which is linked to a specified scene node. */
		static WorldObject* getFromSceneNode(SceneNode* _node);
		static WorldObject* getFromMovableObject(MovableObject* _movable);

		/** Returns the type's name of the world object. */
		const String& getWorldObjectTypeName() const;

		/** Selects or deselects the world object. */
		void setSelected(bool _select);
		
		/** Selects the world object. */
		inline void select() {setSelected(true);}

		/** Deselects the world object. */
		inline void deselect() {setSelected(false);}
		
		/** Whether the world object is selected. */
		virtual bool isSelected() const = 0;

		/** Called when a parameter changed via ZenStringInterface. */
		virtual void parameterChanged(const String& _paramName);

	public:
		/** Internal function.
		Changes the name without firing world listeners. */
		virtual void _changeName(const String& _name) = 0;

		/** Internal function.
		Changes the position without firing world listeners. */
		virtual void _changePosition(const Vector3& _position) = 0;

		/** Internal function.
		Changes the orientation without firing world listeners. */
		virtual void _changeOrientation(const Quaternion& _orientation) = 0;

		/** Internal function.
		Changes the scaling factor without firing world listeners. */
		virtual void _changeScale(const Vector3& _scale) = 0;

		/** Internal function.
		Changes the final world position without firing world listeners. */
		virtual void _changeDerivedPosition(const Vector3& _derivedPosition) = 0;

		/** Internal function.
		Changes the final world orientation without firing world listeners. */
		virtual void _changeDerivedOrientation(const Quaternion& _derivedOrientation) = 0;

		/** Internal function.
		Adds a specified object to the list of children of this object. */
		virtual void _addToListOfChildren(const WorldObjectPtr& _child, bool _keepTransform) = 0;

		/** Internal function.
		Removes a specified object from the list of children of this object. */
		virtual void _removeFromListOfChildren(const WorldObjectPtr& _child, bool _keepTransform) = 0;
		
		/** Internal function.
		Sets the parent of this object without editing list of children 
		of this parent and without firing world listeners. */
		virtual void _changeParent(WorldObject* _parent) = 0;

		/** Internal function.
		Changes the selection state of this object 
		without notifying the world about it. */
		virtual void _changeSelectState(bool _selectState) = 0;

		/** Internal function.
		Shows the selection state of this object. */
		virtual void _showSelectState(bool _selectState) = 0;
	};



	//--------------------------------------------------------------------------
	/** Shared pointer to an instance of the WorldObject class */
	class GOTHOGRE_EXPORT WorldObjectPtr : public ZenObjectPtr
	{
	public:
		WorldObjectPtr() : ZenObjectPtr() {}
		WorldObjectPtr(WorldObject* _p);
		WorldObjectPtr(SceneNode* _sceneNode);
		WorldObjectPtr(const WorldObjectPtr& _ptr) : ZenObjectPtr(_ptr) {}
		WorldObjectPtr(const ZenObjectPtr& _ptr) : ZenObjectPtr(_ptr) {}

		const WorldObjectPtr& operator =(const WorldObjectPtr& _ptr) {ZenObjectPtr::operator =(_ptr); return *this;}
		const WorldObjectPtr& operator =(const ZenObjectPtr& _ptr) {ZenObjectPtr::operator =(_ptr); return *this;}

		WorldObject& operator*() const { return *(operator ->()); }
		WorldObject* operator->() const;
		WorldObject* get() const;
		WorldObject* getPointer() const { return get(); }
	};

} // namespace GothOgre

#endif // GOTHOGRE_WORLD_OBJECT_H