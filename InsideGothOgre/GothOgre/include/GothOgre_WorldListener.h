#ifndef GOTHOGRE_WORLD_LISTENER_H
#define GOTHOGRE_WORLD_LISTENER_H

#include "GothOgre_SafeList.h"
#include "GothOgre_World.h"


namespace GothOgre
{
	//-------------------------------------------------------------------------------
	/** World listener is a set of callbacks which is called when world is changed.
	    This class contains data which are sent to the handler. */
	class WorldEvent
	{
	public:
		WorldEvent() 
			: mWorld(nullptr), mActivated(false) {}

		WorldEvent(World* _world) 
			: mWorld(_world), mActivated(false) {}

		WorldEvent(World* _world, bool _activated) 
			: mWorld(_world), mActivated(_activated) {}

		World* getWorld() const {return mWorld;}
		bool isActivated() const {return mActivated;}

	private:
		World*          mWorld;
		bool            mActivated;
	};


	//--------------------------------------------------------------------------
	/** Internal base class for listener. */
	class GOTHOGRE_EXPORT WorldListener
	{
	public:
		/** World has been created. 
		This function is called by the "WorldManager::createWorld" function. */
		virtual void worldCreated(const WorldEvent& _evt) = 0;

		/** World has been destroyed. 
		This function is called by the "WorldManager::destroyWorld" function. */
		virtual void worldDestroyed(const WorldEvent& _evt) = 0;

		/** World has been loaded. 
		This function is called by the "WorldManager::loadWorld" function. */
		virtual void worldLoaded(const WorldEvent& _evt) = 0;

		/** World has been merged. 
		This function is called by the "WorldManager::mergeWorld" function. */
		virtual void worldMerged(const WorldEvent& _evt) = 0;

		/** World has been saved. 
		This function is called by the "WorldManager::saveWorld" function. */
		virtual void worldSaved(const WorldEvent& _evt) = 0;

		/** World has been activated (made active) or deactivated.
		Use the "_evt.isActivated" function to check what's happened.
		This function is called by the "WorldManager::setActiveWorld" function. */
		virtual void worldActiveChanged(const WorldEvent& _evt) = 0;

	public:
		WorldListener();
		~WorldListener();

		void registerListener();
		void unregisterListener();

	private:
		friend class WorldManager;
		typedef SafeList<WorldListener*>::ElementHandle ElementHandle;
		ElementHandle  mElementHandle;
	};


	// The macros is designed to be used as a base only. Should not be used directly.
#	define GOTHOGRE_DECLARE_WORLD_BASE( \
		worldCreatedFunc, worldDestroyedFunc, worldLoadedFunc, \
		worldMergedFunc, worldSavedFunc, worldActiveChangedFunc, \
		registerFunc, unregisterFunc, classname) \
	public: \
		void registerFunc() \
		{ \
			mPrivateWorldListener_##registerFunc.registerListener(this); \
		} \
		void unregisterFunc() \
		{ \
			mPrivateWorldListener_##registerFunc.unregisterListener(); \
		} \
	private: \
		void worldCreated_defaultImpl_##registerFunc(const WorldEvent& _evt) \
		{ \
		} \
		void worldDestroyed_defaultImpl_##registerFunc(const WorldEvent& _evt) \
		{ \
		} \
		void worldLoaded_defaultImpl_##registerFunc(const WorldEvent& _evt) \
		{ \
		} \
		void worldMerged_defaultImpl_##registerFunc(const WorldEvent& _evt) \
		{ \
		} \
		void worldSaved_defaultImpl_##registerFunc(const WorldEvent& _evt) \
		{ \
		} \
		void worldActiveChanged_defaultImpl_##registerFunc(const WorldEvent& _evt) \
		{ \
		} \
		\
		class PrivateWorldListener_##registerFunc : public WorldListener \
		{ \
		public: \
			void worldCreated(const WorldEvent& _evt) \
			{ \
				return mOwner->worldCreatedFunc(_evt); \
			} \
			void worldDestroyed(const WorldEvent& _evt) \
			{ \
				return mOwner->worldDestroyedFunc(_evt); \
			} \
			void worldLoaded(const WorldEvent& _evt) \
			{ \
				return mOwner->worldLoadedFunc(_evt); \
			} \
			void worldMerged(const WorldEvent& _evt) \
			{ \
				return mOwner->worldMergedFunc(_evt); \
			} \
			void worldSaved(const WorldEvent& _evt) \
			{ \
				return mOwner->worldSavedFunc(_evt); \
			} \
			void worldActiveChanged(const WorldEvent& _evt) \
			{ \
				return mOwner->worldActiveChangedFunc(_evt); \
			} \
			\
			void registerListener(classname* _owner) \
			{ \
				mOwner = _owner; \
				WorldListener::registerListener(); \
			} \
		\
		private: \
			classname* mOwner; \
		}; \
		friend class PrivateWorldListener_##registerFunc; \
		PrivateWorldListener_##registerFunc mPrivateWorldListener_##registerFunc;


	//----------------------------------------------------------------------------
	// WorldTimeListener.
	// Macroses to inject world listening functionality to any class.
	//----------------------------------------------------------------------------
#	define GOTHOGRE_DECLARE_WORLD_CREATED_EX( \
		worldCreatedFunc, \
		registerFunc, unregisterFunc, classname) \
		void worldCreatedFunc(const WorldEvent& _evt); \
		GOTHOGRE_DECLARE_WORLD_BASE( \
			worldCreatedFunc, \
			worldDestroyed_defaultImpl_##registerFunc, \
			worldLoaded_defaultImpl_##registerFunc, \
			worldMerged_defaultImpl_##registerFunc, \
			worldSaved_defaultImpl_##registerFunc, \
			worldActiveChanged_defaultImpl_##registerFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WORLD_DESTROYED_EX( \
		worldDestroyedFunc, \
		registerFunc, unregisterFunc, classname) \
		void worldDestroyedFunc(const WorldEvent& _evt); \
		GOTHOGRE_DECLARE_WORLD_BASE( \
			worldCreated_defaultImpl_##registerFunc, \
			worldDestroyedFunc, \
			worldLoaded_defaultImpl_##registerFunc, \
			worldMerged_defaultImpl_##registerFunc, \
			worldSaved_defaultImpl_##registerFunc, \
			worldActiveChanged_defaultImpl_##registerFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WORLD_LOADED_EX( \
		worldLoadedFunc, \
		registerFunc, unregisterFunc, classname) \
		void worldLoadedFunc(const WorldEvent& _evt); \
		GOTHOGRE_DECLARE_WORLD_BASE( \
			worldCreated_defaultImpl_##registerFunc, \
			worldDestroyed_defaultImpl_##registerFunc, \
			worldLoadedFunc, \
			worldMerged_defaultImpl_##registerFunc, \
			worldSaved_defaultImpl_##registerFunc, \
			worldActiveChanged_defaultImpl_##registerFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WORLD_MERGED_EX( \
		worldMergedFunc, \
		registerFunc, unregisterFunc, classname) \
		void worldMergedFunc(const WorldEvent& _evt); \
		GOTHOGRE_DECLARE_WORLD_BASE( \
			worldCreated_defaultImpl_##registerFunc, \
			worldDestroyed_defaultImpl_##registerFunc, \
			worldLoaded_defaultImpl_##registerFunc, \
			worldMergedFunc, \
			worldSaved_defaultImpl_##registerFunc, \
			worldActiveChanged_defaultImpl_##registerFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WORLD_SAVED_EX( \
		worldSavedFunc, \
		registerFunc, unregisterFunc, classname) \
		void worldMergedFunc(const WorldEvent& _evt); \
		GOTHOGRE_DECLARE_WORLD_BASE( \
			worldCreated_defaultImpl_##registerFunc, \
			worldDestroyed_defaultImpl_##registerFunc, \
			worldLoaded_defaultImpl_##registerFunc, \
			worldMerged_defaultImpl_##registerFunc, \
			worldSavedFunc, \
			worldActiveChanged_defaultImpl_##registerFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WORLD_ACTIVE_CHANGED_EX( \
		worldActiveChangedFunc, \
		registerFunc, unregisterFunc, classname) \
		void worldActiveChangedFunc(const WorldEvent& _evt); \
		GOTHOGRE_DECLARE_WORLD_BASE( \
			worldCreated_defaultImpl_##registerFunc, \
			worldDestroyed_defaultImpl_##registerFunc, \
			worldLoaded_defaultImpl_##registerFunc, \
			worldMerged_defaultImpl_##registerFunc, \
			worldSaved_defaultImpl_##registerFunc, \
			worldActiveChangedFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WORLD_CREATED_DESTROYED_EX( \
		worldCreatedFunc, \
		registerFunc, unregisterFunc, classname) \
		void worldCreatedFunc(const WorldEvent& _evt); \
		void worldDestroyedFunc(const WorldEvent& _evt); \
		GOTHOGRE_DECLARE_WORLD_BASE( \
			worldCreatedFunc, \
			worldDestroyedFunc, \
			worldLoaded_defaultImpl_##registerFunc, \
			worldMerged_defaultImpl_##registerFunc, \
			worldSaved_defaultImpl_##registerFunc, \
			worldActiveChanged_defaultImpl_##registerFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WORLD_LOADED_MERGED_EX( \
		worldLoadedFunc, worldMergedFunc, \
		registerFunc, unregisterFunc, classname) \
		void worldLoadedFunc(const WorldEvent& _evt); \
		void worldMergedFunc(const WorldEvent& _evt); \
		GOTHOGRE_DECLARE_WORLD_BASE( \
			worldCreated_defaultImpl_##registerFunc, \
			worldDestroyed_defaultImpl_##registerFunc, \
			worldLoadedFunc, \
			worldMergedFunc, \
			worldSaved_defaultImpl_##registerFunc, \
			worldActiveChanged_defaultImpl_##registerFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WORLD_LOADED_SAVED_EX( \
		worldLoadedFunc, worldSavedFunc, \
		registerFunc, unregisterFunc, classname) \
		void worldLoadedFunc(const WorldEvent& _evt); \
		void worldSavedFunc(const WorldEvent& _evt); \
		GOTHOGRE_DECLARE_WORLD_BASE( \
			worldCreated_defaultImpl_##registerFunc, \
			worldDestroyed_defaultImpl_##registerFunc, \
			worldLoadedFunc, \
			worldMergedFunc_defaultImpl_##registerFunc, \
			worldSavedFunc, \
			worldActiveChanged_defaultImpl_##registerFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WORLD_LOADED_MERGED_SAVED_EX( \
		worldLoadedFunc, worldMergedFunc, worldSavedFunc, \
		registerFunc, unregisterFunc, classname) \
		void worldLoadedFunc(const WorldEvent& _evt); \
		void worldMergedFunc(const WorldEvent& _evt); \
		void worldSavedFunc(const WorldEvent& _evt); \
		GOTHOGRE_DECLARE_WORLD_BASE( \
			worldCreated_defaultImpl_##registerFunc, \
			worldDestroyed_defaultImpl_##registerFunc, \
			worldLoadedFunc, \
			worldMergedFunc, \
			worldSavedFunc, \
			worldActiveChanged_defaultImpl_##registerFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WORLD_CREATED_DESTROYED_ACTIVE_CHANGED_EX( \
		worldCreatedFunc, worldDestroyedFunc, worldActiveChangedFunc, \
		registerFunc, unregisterFunc, classname) \
		void worldCreatedFunc(const WorldEvent& _evt); \
		void worldDestroyedFunc(const WorldEvent& _evt); \
		void worldActiveChangedFunc(const WorldEvent& _evt); \
		GOTHOGRE_DECLARE_WORLD_BASE( \
			worldCreatedFunc, \
			worldDestroyedFunc, \
			worldLoaded_defaultImpl_##registerFunc, \
			worldMerged_defaultImpl_##registerFunc, \
			worldSaved_defaultImpl_##registerFunc, \
			worldActiveChangedFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WORLD_LOADED_SAVED_ACTIVE_CHANGED_EX( \
		worldLoadedFunc, worldSavedFunc, worldActiveChangedFunc, \
		registerFunc, unregisterFunc, classname) \
		void worldLoadedFunc(const WorldEvent& _evt); \
		void worldSavedFunc(const WorldEvent& _evt); \
		void worldActiveChangedFunc(const WorldEvent& _evt); \
		GOTHOGRE_DECLARE_WORLD_BASE( \
			worldCreated_defaultImpl_##registerFunc, \
			worldDestroyed_defaultImpl_##registerFunc, \
			worldLoadedFunc, \
			worldMerged_defaultImpl_##registerFunc, \
			worldSavedFunc, \
			worldActiveChangedFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_WORLD_CREATED_DESTROYED_LOADED_MERGED_SAVED_ACTIVE_CHANGED_EX( \
		worldCreatedFunc, worldDestroyedFunc, worldLoadedFunc, \
		worldMergedFunc, worldSavedFunc, worldActiveChangedFunc, \
		registerFunc, unregisterFunc, classname) \
		void worldCreatedFunc(const WorldEvent& _evt); \
		void worldDestroyedFunc(const WorldEvent& _evt); \
		void worldLoadedFunc(const WorldEvent& _evt); \
		void worldMergedFunc(const WorldEvent& _evt); \
		void worldSavedFunc(const WorldEvent& _evt); \
		void worldActiveChangedFunc(const WorldEvent& _evt); \
		GOTHOGRE_DECLARE_WORLD_BASE( \
			worldCreatedFunc, \
			worldDestroyedFunc, \
			worldLoadedFunc, \
			worldMergedFunc, \
			worldSavedFunc, \
			worldActiveChangedFunc, \
			registerFunc, unregisterFunc, classname)


#	define GOTHOGRE_DECLARE_WORLD_CREATED( classname ) \
		GOTHOGRE_DECLARE_WORLD_CREATED_EX( \
			worldCreated, \
			registerWorldListener, unregisterWorldListener, classname )

#	define GOTHOGRE_DECLARE_WORLD_DESTROYED( classname ) \
		GOTHOGRE_DECLARE_WORLD_DESTROYED_EX( \
			worldDestroyed, \
			registerWorldListener, unregisterWorldListener, classname )

#	define GOTHOGRE_DECLARE_WORLD_LOADED( classname ) \
		GOTHOGRE_DECLARE_WORLD_LOADED_EX( \
			worldLoaded, \
			registerWorldListener, unregisterWorldListener, classname )

#	define GOTHOGRE_DECLARE_WORLD_MERGED( classname ) \
		GOTHOGRE_DECLARE_WORLD_MERGED_EX( \
			worldMerged, \
			registerWorldListener, unregisterWorldListener, classname )

#	define GOTHOGRE_DECLARE_WORLD_SAVED( classname ) \
		GOTHOGRE_DECLARE_WORLD_SAVED_EX( \
			worldSaved, \
			registerWorldListener, unregisterWorldListener, classname )

#	define GOTHOGRE_DECLARE_WORLD_ACTIVE_CHANGED( classname ) \
		GOTHOGRE_DECLARE_WORLD_ACTIVE_CHANGED_EX( \
			worldActiveChanged, \
			registerWorldListener, unregisterWorldListener, classname )

#	define GOTHOGRE_DECLARE_WORLD_CREATED_DESTROYED( classname ) \
		GOTHOGRE_DECLARE_WORLD_CREATED_DESTROYED_EX( \
			worldCreated, worldDestroyed, \
			registerWorldListener, unregisterWorldListener, classname )

#	define GOTHOGRE_DECLARE_WORLD_LOADED_MERGED( classname ) \
		GOTHOGRE_DECLARE_WORLD_LOADED_MERGED_EX( \
			worldLoaded, worldMerged, \
			registerWorldListener, unregisterWorldListener, classname )

#	define GOTHOGRE_DECLARE_WORLD_LOADED_SAVED( classname ) \
		GOTHOGRE_DECLARE_WORLD_LOADED_SAVED_EX( \
			worldLoaded, worldSaved, \
			registerWorldListener, unregisterWorldListener, classname )

#	define GOTHOGRE_DECLARE_WORLD_LOADED_MERGED_SAVED( classname ) \
		GOTHOGRE_DECLARE_WORLD_LOADED_MERGED_SAVED_EX( \
			worldLoaded, worldMerged, worldSaved, \
			registerWorldListener, unregisterWorldListener, classname )

#	define GOTHOGRE_DECLARE_WORLD_CREATED_DESTROYED_ACTIVE_CHANGED( classname ) \
		GOTHOGRE_DECLARE_WORLD_CREATED_DESTROYED_ACTIVE_CHANGED_EX( \
			worldCreated, worldDestroyed, worldActiveChanged, \
			registerWorldListener, unregisterWorldListener, classname )

#	define GOTHOGRE_DECLARE_WORLD_LOADED_SAVED_ACTIVE_CHANGED( classname ) \
		GOTHOGRE_DECLARE_WORLD_LOADED_SAVED_ACTIVE_CHANGED_EX( \
			worldLoaded, worldSaved, worldActiveChanged, \
			registerWorldListener, unregisterWorldListener, classname )

#	define GOTHOGRE_DECLARE_WORLD_CREATED_DESTROYED_LOADED_MERGED_SAVED_ACTIVE_CHANGED( classname ) \
		GOTHOGRE_DECLARE_WORLD_CREATED_DESTROYED_LOADED_MERGED_SAVED_ACTIVE_CHANGED_EX( \
			worldCreated, worldDestroyed, Loaded, worldMerged, worldSaved, worldActiveChanged, \
			registerWorldListener, unregisterWorldListener, classname )
	
} // namespace GothOgre

#endif // GOTHOGRE_WORLD_LISTENER_H
