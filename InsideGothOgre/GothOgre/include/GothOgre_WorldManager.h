#ifndef GOTHOGRE_WORLD_MANAGER_H
#define GOTHOGRE_WORLD_MANAGER_H

#include "GothOgre_ZenObject.h"
#include "GothOgre_ZenArchive.h"
#include "GothOgre_SafeList.h"


namespace GothOgre
{
	class World;


	/** A singleton which stores pointers to all using worlds. */
	class GOTHOGRE_EXPORT WorldManager : public Singleton<WorldManager>
	{
	public:
		typedef list<ZenObjectPtr>::type WorldList;

		/** Iterator for iteration through all created worlds. */
		class WorldIterator : public IteratorWrapper<WorldList, WorldList::const_iterator, World*>
		{
		public:
			WorldIterator(const WorldList& _container)
				: IteratorWrapper<WorldList, WorldList::const_iterator, World*>(_container.begin(), _container.end())
			{}

			World* peekNext()
			{
				return (World*) (*mCurrent).getPointer();
			}

			World* getNext()
			{
				return (World*) (*mCurrent++).getPointer();
			}
		};

	public:
		WorldManager();
		~WorldManager();

		/** Returns an iterator for iterating through all created worlds. */
		WorldIterator getWorldIterator() const;

		/** Returns a pointer to the active world. 
		This function can return nullptr if there is no active world now. */
		World* getActiveWorld() const;

		/** Sets the active world. */
		void setActiveWorld(World* _world);

		/** Sets the name of the type which must be used for worlds' creating. 
		This function allows plugins to implement own world class. */
		void setWorldTypeName(const String& _typeName);

		/** Returns the name of the type which must be used for worlds' creating. */
		const String& getWorldTypeName() const;

		/** Creates a new world, and makes it active. */
		World* createWorld(const String& _name = StringUtil::BLANK);

		/** Destroys a world. */
		void destroyWorld(World* _world);

		/** Destroys all worlds. */
		void destroyAllWorlds();

		/** Loads a world from zen archive. */
		World* loadWorld(const String& _filename, const String& _resourceGroup, ZenArchiveFlags _flags = ZenArchiveFlags::DEFAULT);
		/** Loads a world from zen archive. */
		World* loadWorld(const DataStreamExPtr& _dataStream, ZenArchiveFlags _flags = ZenArchiveFlags::DEFAULT);
		/** Loads a world from zen archive. */
		World* loadWorld(ZenArchive* _zenArchive);

		/** Merges a world with a world which is read from zen archive. */
		void mergeWorld(World* _world, const String& _filename, const String& _resourceGroup, ZenArchiveFlags _flags = ZenArchiveFlags::DEFAULT);
		/** Merges a world with a world which is read from zen archive. */
		void mergeWorld(World* _world, const DataStreamExPtr& _dataStream, ZenArchiveFlags _flags = ZenArchiveFlags::DEFAULT);
		/** Merges a world with a world which is read from zen archive. */
		void mergeWorld(World* _world, ZenArchive* _zenArchive);

		/** Saves a world to zen archive. */
		void saveWorld(World* _world, const String& _filename, const String& _resourceGroup, ZenArchiveType _type, ZenArchiveFlags _flags = ZenArchiveFlags::DEFAULT);
		/** Saves a world to zen archive. */
		void saveWorld(World* _world, const DataStreamExPtr& _dataStream, ZenArchiveType _type, ZenArchiveFlags _flags = ZenArchiveFlags::DEFAULT);
		/** Saves a world to zen archive. */
		void saveWorld(World* _world, ZenArchive* _zenArchive);

		enum CreateMode
		{
			CREATE_NEW_WORLD,
			RETRIEVE_ACTIVE_WORLD
		};

		/** Sets the creation mode for worlds.
		This function is internally used by zen factory,
		which type name is specified by the setWorldTypeName function. */
		void _setCreateMode(CreateMode _createMode);

		/** Returns the creation mode for worlds. */
		CreateMode _getCreateMode() const;

	private:
		friend class World;
		void _setActiveWorldOverride(World* _world);

		friend class WorldListener;
		void _initListener(WorldListener* _listener);
		void _addListener(WorldListener* _listener);
		void _removeListener(WorldListener* _listener);
		typedef SafeList<WorldListener*> Listeners;
		
		friend class WorldObjectListener;
		void _initListener(WorldObjectListener* _listener);
		void _addListener(WorldObjectListener* _listener);
		void _removeListener(WorldObjectListener* _listener);
		typedef SafeList<WorldObjectListener*> ObjectListeners;

		friend class WorldObject;
		void _worldObjectCreated(WorldObject* _worldObject);
		void _worldObjectDestroyed(WorldObject* _worldObject);
		void _worldObjectAdded(WorldObject* _worldObject);
		void _worldObjectRemoved(WorldObject* _worldObject);
		void _worldObjectChanged(WorldObject* _worldObject, const String& _paramName);
		void _worldObjectSelectChanged(WorldObject* _worldObject, bool _selected);

	private:
		void fireWorldActiveChanged(World* _world, bool _activated);
		void fireWorldCreated(World* _world);
		void fireWorldDestroyed(World* _world);
		void fireWorldLoaded(World* _world);
		void fireWorldMerged(World* _world);
		void fireWorldSaved(World* _world);
		void fireWorldObjectAdded(World* _world, WorldObject* _worldObject);
		void fireWorldObjectRemoved(World* _world, WorldObject* _worldObject);
		void fireWorldObjectChanged(World* _world, WorldObject* _worldObject);
		void fireWorldObjectSelectChanged(World* _world, WorldObject* _worldObject, bool _selected);

	private:
		void mergeWorld2(World* _world, ZenArchive* _zenArchive);
		void resetCreatedNotAdded();
		void resetAddedNotFired();

	private:
		String           mWorldTypeName;
		WorldList        mWorlds;
		World*           mActiveWorld;
		World*           mActiveWorldOverride; // Used in the constructor of the "World" class. (It's necessary because the root world object of a new world must be able to get the new world.)
		CreateMode       mCreateMode;       // Create mode - used by ZenWorldFactory only.
		Listeners        mListeners;
		ObjectListeners  mObjectListeners;
		size_t           mNameAutoGenIndex; // Index for auto-generating names like "Unnamed03.zen".
		bool             mLoading;          // Is a world loading or merging now?
		WorldObject*     mCreatedNotAdded;  // For optimization purposes - 
		WorldObject*     mAddedNotFired;    // - to fire listeners not very frequently when a world is loading. 
	};

} // namespace GothOgre

#endif // GOTHOGRE_WORLD_MANAGER_H