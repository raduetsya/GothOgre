#ifndef GOTHOGRE_WORLD_OBJECT_LISTENER_H
#define GOTHOGRE_WORLD_OBJECT_LISTENER_H

#include "GothOgre_SafeList.h"
#include "GothOgre_WorldObject.h"


namespace GothOgre
{
	//-------------------------------------------------------------------------------
	/** World listener is a set of callbacks which is called when world is changed.
	    This class contains data which are sent to the handler. */
	class WorldObjectEvent
	{
	public:
		WorldObjectEvent() 
			: mWorld(nullptr), mWorldObject(nullptr), mSelected(false), mParamName(nullptr) {}

		WorldObjectEvent(World* _world, WorldObject* _worldObject) 
			: mWorld(_world), mWorldObject(_worldObject), mSelected(false), mParamName(nullptr) {}

		WorldObjectEvent(World* _world, WorldObject* _worldObject, bool _selected) 
			: mWorld(_world), mWorldObject(_worldObject), mSelected(_selected), mParamName(nullptr) {}
		
		WorldObjectEvent(World* _world, WorldObject* _worldObject, const String& _paramName) 
			: mWorld(_world), mWorldObject(_worldObject), mSelected(false), mParamName(&_paramName) {}

		World* getWorld() const {return mWorld;}
		WorldObject* getWorldObject() const {return mWorldObject;}
		const String& getParamName() const {return *mParamName;}
		bool isSelected() const {return mSelected;}

	private:
		World*          mWorld;
		WorldObject*	mWorldObject;
		bool            mSelected;
		const String*   mParamName;
	};


	//--------------------------------------------------------------------------
	/** Internal base class for listener. */
	class GOTHOGRE_EXPORT WorldObjectListener
	{
	public:
		/** A world object has been added as a child of another world object.
		This function is called by the "WorldObject::addChild" function. */
		virtual void worldObjectAdded(const WorldObjectEvent& _evt) = 0;

		/** A world object has been removed from children of its parent.
		This function is called by the "WorldObject::removeChild" function. */
		virtual void worldObjectRemoved(const WorldObjectEvent& _evt) = 0;

		/** A world object has been changed. 
		This function is called by several functions:
		WorldObject::setName, WorldObject::setPosition, WorldObject::setOrientation,
		WorldObject::setParameter, and so on. */
		virtual void worldObjectChanged(const WorldObjectEvent& _evt) = 0;

		/** A world object has been selected or deselected.
		Use the "_evt.isSelected" function to check what's happened.
		This function is called by several functions:
		WorldObject::setSelected, World::selectAll, World::invertSelection, and so on. */
		virtual void worldObjectSelectChanged(const WorldObjectEvent& _evt) = 0;

	public:
		WorldObjectListener();
		~WorldObjectListener();

		void registerListener();
		void unregisterListener();

	private:
		friend class WorldManager;
		typedef SafeList<WorldObjectListener*>::ElementHandle ElementHandle;
		ElementHandle  mElementHandle;
	};


	// The macros is designed to be used as a base only. Should not be used directly.
#	define GOTHOGRE_DECLARE_WORLD_OBJECT_BASE( \
		worldObjectAddedFunc, worldObjectRemovedFunc, \
		worldObjectChangedFunc, worldObjectSelectChangedFunc, \
		registerFunc, unregisterFunc, classname) \
	public: \
		void registerFunc() \
		{ \
			mPrivateWorldObjectListener_##registerFunc.registerListener(this); \
		} \
		void unregisterFunc() \
		{ \
			mPrivateWorldObjectListener_##registerFunc.unregisterListener(); \
		} \
	private: \
		void worldObjectAdded_defaultImpl_##registerFunc(const WorldObjectEvent& _evt) \
		{ \
		} \
		void worldObjectRemoved_defaultImpl_##registerFunc(const WorldObjectEvent& _evt) \
		{ \
		} \
		void worldObjectChanged_defaultImpl_##registerFunc(const WorldObjectEvent& _evt) \
		{ \
		} \
		void worldObjectSelectChanged_defaultImpl_##registerFunc(const WorldObjectEvent& _evt) \
		{ \
		} \
		\
		class PrivateWorldObjectListener_##registerFunc : public WorldObjectListener \
		{ \
		public: \
			void worldObjectAdded(const WorldObjectEvent& _evt) \
			{ \
				return mOwner->worldObjectAddedFunc(_evt); \
			} \
			void worldObjectRemoved(const WorldObjectEvent& _evt) \
			{ \
				return mOwner->worldObjectRemovedFunc(_evt); \
			} \
			void worldObjectChanged(const WorldObjectEvent& _evt) \
			{ \
				return mOwner->worldObjectChangedFunc(_evt); \
			} \
			void worldObjectSelectChanged(const WorldObjectEvent& _evt) \
			{ \
				return mOwner->worldObjectSelectChangedFunc(_evt); \
			} \
			\
			void registerListener(classname* _owner) \
			{ \
				mOwner = _owner; \
				WorldObjectListener::registerListener(); \
			} \
		\
		private: \
			classname* mOwner; \
		}; \
		friend class PrivateWorldObjectListener_##registerFunc; \
		PrivateWorldObjectListener_##registerFunc mPrivateWorldObjectListener_##registerFunc;


	//----------------------------------------------------------------------------
	// WorldObjectListener.
	// Macroses to inject world listening functionality to any class.
	//----------------------------------------------------------------------------
#	define GOTHOGRE_DECLARE_WORLD_OBJECT_SELECT_CHANGED_EX( \
		worldObjectSelectChangedFunc, \
		registerFunc, unregisterFunc, classname) \
		void worldObjectSelectChangedFunc(const WorldObjectEvent& _evt); \
		GOTHOGRE_DECLARE_WORLD_OBJECT_BASE( \
			worldObjectAdded_defaultImpl_##registerFunc, \
			worldObjectRemoved_defaultImpl_##registerFunc, \
			worldObjectChanged_defaultImpl_##registerFunc, \
			worldObjectSelectChangedFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WORLD_OBJECT_CHANGED_SELECT_CHANGED_EX( \
		worldObjectChangedFunc, worldObjectSelectChangedFunc, \
		registerFunc, unregisterFunc, classname) \
		void worldObjectChangedFunc(const WorldObjectEvent& _evt); \
		void worldObjectSelectChangedFunc(const WorldObjectEvent& _evt); \
		GOTHOGRE_DECLARE_WORLD_OBJECT_BASE( \
			worldObjectAdded_defaultImpl_##registerFunc, \
			worldObjectRemoved_defaultImpl_##registerFunc, \
			worldObjectChangedFunc, \
			worldObjectSelectChangedFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WORLD_OBJECT_ADDED_REMOVED_CHANGED_EX( \
		worldObjectAddedFunc, worldObjectRemovedFunc, worldObjectChangedFunc, \
		registerFunc, unregisterFunc, classname) \
		void worldObjectAddedFunc(const WorldObjectEvent& _evt); \
		void worldObjectRemovedFunc(const WorldObjectEvent& _evt); \
		void worldObjectChangedFunc(const WorldObjectEvent& _evt); \
		GOTHOGRE_DECLARE_WORLD_OBJECT_BASE( \
			worldObjectAddedFunc, \
			worldObjectRemovedFunc, \
			worldObjectChangedFunc, \
			worldObjectSelectChanged_defaultImpl_##registerFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WORLD_OBJECT_ADDED_REMOVED_CHANGED_SELECT_CHANGED_EX( \
		worldObjectAddedFunc, worldObjectRemovedFunc, \
		worldObjectChangedFunc, worldObjectSelectChangedFunc, \
		registerFunc, unregisterFunc, classname) \
		void worldObjectAddedFunc(const WorldObjectEvent& _evt); \
		void worldObjectRemovedFunc(const WorldObjectEvent& _evt); \
		void worldObjectChangedFunc(const WorldObjectEvent& _evt); \
		void worldObjectSelectChangedFunc(const WorldObjectEvent& _evt); \
		GOTHOGRE_DECLARE_WORLD_OBJECT_BASE( \
			worldObjectAddedFunc, \
			worldObjectRemovedFunc, \
			worldObjectChangedFunc, \
			worldObjectSelectChangedFunc, \
			registerFunc, unregisterFunc, classname)


#	define GOTHOGRE_DECLARE_WORLD_OBJECT_SELECT_CHANGED( classname ) \
		GOTHOGRE_DECLARE_WORLD_OBJECT_SELECT_CHANGED_EX( \
			worldObjectSelectChanged, \
			registerWorldObjectListener, unregisterWorldObjectListener, classname )

#	define GOTHOGRE_DECLARE_WORLD_OBJECT_CHANGED_SELECT_CHANGED( classname ) \
		GOTHOGRE_DECLARE_WORLD_OBJECT_CHANGED_SELECT_CHANGED_EX( \
			worldObjectChanged, worldObjectSelectChanged, \
			registerWorldObjectListener, unregisterWorldObjectListener, classname )

#	define GOTHOGRE_DECLARE_WORLD_OBJECT_ADDED_REMOVED_CHANGED( classname ) \
		GOTHOGRE_DECLARE_WORLD_OBJECT_ADDED_REMOVED_CHANGED_EX( \
			worldObjectAdded, worldObjectRemoved, worldObjectChanged, \
			registerWorldObjectListener, unregisterWorldObjectListener, classname )

#	define GOTHOGRE_DECLARE_WORLD_OBJECT_ADDED_REMOVED_CHANGED_SELECT_CHANGED( classname ) \
		GOTHOGRE_DECLARE_WORLD_OBJECT_ADDED_REMOVED_CHANGED_SELECT_CHANGED_EX( \
			worldObjectAdded, worldObjectRemoved, worldObjectChanged, worldObjectSelectChanged, \
			registerWorldObjectListener, unregisterWorldObjectListener, classname )
	
} // namespace GothOgre

#endif // GOTHOGRE_WORLD_OBJECT_LISTENER_H