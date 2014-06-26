#include "GothOgre_Precompiled.h"
#include "GothOgre_WorldManager.h"
#include "GothOgre_World.h"
#include "GothOgre_WorldListener.h"
#include "GothOgre_WorldObjectListener.h"
#include "GothOgre_ZenManager.h"
#include "GothOgre_Progress.h"
#include "GothOgre_ResourceLocationScanner.h"
#include "GothOgre_StrConv.h"


//-------------------------------------------------------------------------
GothOgre::WorldManager* 
	Ogre::Singleton<GothOgre::WorldManager>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------


namespace GothOgre
{
	//----------------------------------------------------------------------------
	WorldManager::WorldManager()
		: mActiveWorld(nullptr), mActiveWorldOverride(nullptr),
		  mCreateMode(RETRIEVE_ACTIVE_WORLD),
		  mNameAutoGenIndex(1), mLoading(false), 
		  mCreatedNotAdded(nullptr), mAddedNotFired(nullptr)
	{
	}
	//----------------------------------------------------------------------------
	WorldManager::~WorldManager()
	{
	}
	//----------------------------------------------------------------------------
	WorldManager::WorldIterator WorldManager::getWorldIterator() const
	{
		return WorldIterator(mWorlds);
	}
	//----------------------------------------------------------------------------
	World* WorldManager::getActiveWorld() const
	{
		return mActiveWorldOverride ? mActiveWorldOverride : mActiveWorld;
	}
	//----------------------------------------------------------------------------
	void WorldManager::setActiveWorld(World* _world)
	{
		if(mActiveWorld != _world)
		{
			if(mActiveWorld)
			{
				World* oldActiveWorld = mActiveWorld;
				mActiveWorld->_setActive(false);
				mActiveWorld = nullptr;
				fireWorldActiveChanged(oldActiveWorld, false);
			}

			if(_world)
			{
				mActiveWorld = _world;
				mActiveWorld->_setActive(true);
				fireWorldActiveChanged(_world, true);
			}
		}
		mActiveWorldOverride = nullptr;
	}
	//----------------------------------------------------------------------------
	void WorldManager::_setActiveWorldOverride(World* _world)
	{
		mActiveWorldOverride = _world;
	}
	//----------------------------------------------------------------------------
	void WorldManager::setWorldTypeName(const String& _typeName)
	{
		mWorldTypeName = _typeName;
	}
	//----------------------------------------------------------------------------
	const String& WorldManager::getWorldTypeName() const
	{
		return mWorldTypeName;
	}
	//----------------------------------------------------------------------------
	World* WorldManager::createWorld(const String& _name)
	{
		if(mWorldTypeName.empty())
			GOTHOGRE_EXCEPT("ZenWorld is not registered.");

		_setCreateMode(CREATE_NEW_WORLD);
		ZenObjectPtr ptr = ZenManager::getSingleton().createZenObject(mWorldTypeName);
		_setActiveWorldOverride(nullptr);
		
		mWorlds.push_back(ptr);
		World* world = (World*) ptr.getPointer();
		world->_setChanged(false);

		if(!_name.empty())
			world->_setName(_name);
		else
		{
			String sidx = StrConv::toString(mNameAutoGenIndex++);
			if(sidx.length() < 2)
				sidx.insert(0, 2 - sidx.length(), '0');
			String name = "Unnamed" + sidx + ".zen";
			world->_setName(name);
		}

		fireWorldCreated(world);
		setActiveWorld(world);
		return world;
	}
	//----------------------------------------------------------------------------
	void WorldManager::destroyWorld(World* _world)
	{
		WorldList::iterator itErase;
		for(itErase = mWorlds.begin(); itErase != mWorlds.end(); ++itErase)
		{
			const ZenObjectPtr& ptr = *itErase;
			if(ptr.getPointer() == _world)
				break;
		}

		if(_world == mActiveWorld)
		{
			WorldList::iterator itNewActive = itErase;
			if(itNewActive != mWorlds.end())
			{
				if(itNewActive != mWorlds.begin())
					--itNewActive;
				else
					++itNewActive;
			}
			if(itNewActive != mWorlds.end())
				setActiveWorld( (World*)((*itNewActive).getPointer()) );
			else
				setActiveWorld( nullptr );
		}

		fireWorldDestroyed(_world);
		mWorlds.erase(itErase);
	}
	//----------------------------------------------------------------------------
	void WorldManager::destroyAllWorlds()
	{
		while(true)
		{
			WorldIterator wit = getWorldIterator();
			if(!wit.hasMoreElements())
				break;
			World* world = wit.getNext();
			destroyWorld(world);
		}
	}
	//----------------------------------------------------------------------------
	World* WorldManager::loadWorld(const String& _filename, const String& _resourceGroup, ZenArchiveFlags _flags)
	{
		ZenArchive za;
		za.readHeader(_filename, _resourceGroup, _flags);
		World* world = loadWorld(&za);
		za.readFooter();
		return world;
	}
	//----------------------------------------------------------------------------
	World* WorldManager::loadWorld(const DataStreamExPtr& _dataStream, ZenArchiveFlags _flags)
	{
		ZenArchive za;
		za.readHeader(_dataStream, _flags);
		World* world = loadWorld(&za);
		za.readFooter();
		return world;
	}
	//----------------------------------------------------------------------------
	World* WorldManager::loadWorld(ZenArchive* _zenArchive)
	{
		Progress& progress = Progress::getSingleton();
		progress.begin();
		progress.setCaption("#{Loading}");
		progress.setMessage(_zenArchive->getName());

		World* world = createWorld(_zenArchive->getName());
		{
			world->freezeChangedState();
			mergeWorld2(world, _zenArchive);
			world->unfreezeChangedState();
		}
		fireWorldLoaded(world);

		progress.end();
		return world;
	}
	//----------------------------------------------------------------------------
	void WorldManager::mergeWorld(World* _world, const String& _filename, const String& _resourceGroup, ZenArchiveFlags _flags)
	{
		ZenArchive za;
		za.readHeader(_filename, _resourceGroup, _flags);
		mergeWorld(_world, &za);
		za.readFooter();
	}
	//----------------------------------------------------------------------------
	void WorldManager::mergeWorld(World* _world, const DataStreamExPtr& _dataStream, ZenArchiveFlags _flags)
	{
		ZenArchive za;
		za.readHeader(_dataStream, _flags);
		mergeWorld(_world, &za);
		za.readFooter();
	}
	//----------------------------------------------------------------------------
	void WorldManager::mergeWorld(World* _world, ZenArchive* _zenArchive)
	{
		Progress& progress = Progress::getSingleton();
		progress.begin();
		progress.setCaption("#{Merging}");
		progress.setMessage(_zenArchive->getName());

		mergeWorld2(_world, _zenArchive);
		fireWorldMerged(_world);

		progress.end();
	}
	//----------------------------------------------------------------------------
	void WorldManager::mergeWorld2(World* _world, ZenArchive* _zenArchive)
	{
		GOTHOGRE_ASSERT(_world, "Cannot merge NULL world.");

		Progress& progress = Progress::getSingleton();
		progress.begin();
		ResourceLocationScanner::getSingleton().scanAll();

		setActiveWorld(_world);

		mLoading = true;

		_setCreateMode(RETRIEVE_ACTIVE_WORLD);
		_zenArchive->readObject();

		resetCreatedNotAdded();
		resetAddedNotFired();
		mLoading = false;

		progress.end();
	}
	//----------------------------------------------------------------------------
	void WorldManager::saveWorld(World* _world, const String& _filename, const String& _resourceGroup, ZenArchiveType _type, ZenArchiveFlags _flags)
	{
		ZenArchive za;
		za.writeHeader(_filename, _resourceGroup, _type, _flags);
		saveWorld(_world, &za);
		za.readFooter();
	}
	//----------------------------------------------------------------------------
	void WorldManager::saveWorld(World* _world, const DataStreamExPtr& _dataStream, ZenArchiveType _type, ZenArchiveFlags _flags)
	{
		ZenArchive za;
		za.writeHeader(_dataStream, _type, _flags);
		saveWorld(_world, &za);
		za.writeFooter();
	}
	//----------------------------------------------------------------------------
	void WorldManager::saveWorld(World* _world, ZenArchive* _zenArchive)
	{
		GOTHOGRE_ASSERT(_world, "Cannot save NULL world.");

		Progress& progress = Progress::getSingleton();
		progress.begin();
		progress.setCaption("#{Saving}");
		progress.setMessage(_zenArchive->getName());

		_zenArchive->writeObject(_world);

		progress.end();

		// 
		_world->_setName(_zenArchive->getName());

		//
		_world->_setChanged(false);

		fireWorldSaved(_world);
	}
	//----------------------------------------------------------------------------
	void WorldManager::_setCreateMode(CreateMode _createMode)
	{
		mCreateMode = _createMode;
	}
	//----------------------------------------------------------------------------
	WorldManager::CreateMode WorldManager::_getCreateMode() const
	{
		return mCreateMode;
	}
	//----------------------------------------------------------------------------
	void WorldManager::resetCreatedNotAdded()
	{
		mCreatedNotAdded = nullptr;
	}
	//----------------------------------------------------------------------------
	void WorldManager::resetAddedNotFired()
	{
		if(mAddedNotFired)
		{
			World* world = mAddedNotFired->getWorld();
			world->_setChanged(true);
			fireWorldObjectAdded(world, mAddedNotFired);
			mAddedNotFired = nullptr;
		}
	}
	//----------------------------------------------------------------------------
	void WorldManager::_worldObjectCreated(WorldObject* _worldObject)
	{
		mCreatedNotAdded = _worldObject;
	}
	//----------------------------------------------------------------------------
	void WorldManager::_worldObjectDestroyed(WorldObject* _worldObject)
	{
		if(mCreatedNotAdded == _worldObject)
			mCreatedNotAdded = nullptr;
		if(mAddedNotFired == _worldObject)
			mAddedNotFired = nullptr;
	}
	//----------------------------------------------------------------------------
	void WorldManager::_worldObjectAdded(WorldObject* _worldObject)
	{
		resetCreatedNotAdded();
		resetAddedNotFired();

		mAddedNotFired = _worldObject;
		
		if(!mLoading)
			resetAddedNotFired();
	}
	//----------------------------------------------------------------------------
	void WorldManager::_worldObjectRemoved(WorldObject* _worldObject)
	{
		if(mAddedNotFired)
		{
			if(mAddedNotFired == _worldObject)
			{
				mAddedNotFired = nullptr;
				return;
			}
			if(mAddedNotFired->getSceneNode() == _worldObject->getSceneNode())
			{
				mAddedNotFired = nullptr;
				return;
			}
		}

		resetCreatedNotAdded();
		resetAddedNotFired();

		World* world = _worldObject->getWorld();
		world->_setChanged(true);
		fireWorldObjectRemoved(world, _worldObject);
	}
	//----------------------------------------------------------------------------
	void WorldManager::_worldObjectChanged(WorldObject* _worldObject, const String& _paramName)
	{
		if(mCreatedNotAdded)
		{
			if(mCreatedNotAdded == _worldObject)
				return;
			if(mCreatedNotAdded->getSceneNode() == _worldObject->getSceneNode())
			{
				mCreatedNotAdded = _worldObject;
				return;
			}
			resetCreatedNotAdded();
		}

		if(mAddedNotFired)
		{
			if(mAddedNotFired == _worldObject)
				return;
			if(mAddedNotFired->getSceneNode() == _worldObject->getSceneNode())
			{
				mAddedNotFired = _worldObject;
				return;
			}
			resetAddedNotFired();
		}

		World* world = _worldObject->getWorld();
		world->_setChanged(true);
		fireWorldObjectChanged(world, _worldObject);
	}
	//----------------------------------------------------------------------------
	void WorldManager::_worldObjectSelectChanged(WorldObject* _worldObject, bool _selected)
	{
		World* world = _worldObject->getWorld();
		if(_selected)
			world->_addToSelection(_worldObject);
		else
			world->_removeFromSelection(_worldObject);

		fireWorldObjectSelectChanged(world, _worldObject, _selected);
	}
	//----------------------------------------------------------------------------
	void WorldManager::_initListener(WorldListener* _listener)
	{
		_listener->mElementHandle = mListeners.getNullHandle();
	}
	//----------------------------------------------------------------------------
	void WorldManager::_addListener(WorldListener* _listener)
	{
		if(_listener->mElementHandle == mListeners.getNullHandle())
		{
			_listener->mElementHandle = mListeners.add(_listener);
		}
	}
	//----------------------------------------------------------------------------
	void WorldManager::_removeListener(WorldListener* _listener)
	{
		if(_listener->mElementHandle != mListeners.getNullHandle())
		{
			mListeners.remove(_listener->mElementHandle);
			_listener->mElementHandle = mListeners.getNullHandle();
		}
	}
	//----------------------------------------------------------------------------
	void WorldManager::_initListener(WorldObjectListener* _listener)
	{
		_listener->mElementHandle = mObjectListeners.getNullHandle();
	}
	//----------------------------------------------------------------------------
	void WorldManager::_addListener(WorldObjectListener* _listener)
	{
		if(_listener->mElementHandle == mObjectListeners.getNullHandle())
		{
			_listener->mElementHandle = mObjectListeners.add(_listener);
		}
	}
	//----------------------------------------------------------------------------
	void WorldManager::_removeListener(WorldObjectListener* _listener)
	{
		if(_listener->mElementHandle != mObjectListeners.getNullHandle())
		{
			mObjectListeners.remove(_listener->mElementHandle);
			_listener->mElementHandle = mObjectListeners.getNullHandle();
		}
	}
	//----------------------------------------------------------------------------
	void WorldManager::fireWorldActiveChanged(World* _world, bool _activated)
	{
		WorldEvent evt(_world, _activated);
		Listeners::Iterator it = mListeners.getIterator();
		while(it.hasMoreElements())
		{
			WorldListener* listener = it.getNext();
			listener->worldActiveChanged(evt);
		}
	}
	//----------------------------------------------------------------------------
	void WorldManager::fireWorldCreated(World* _world)
	{
		WorldEvent evt(_world);
		Listeners::Iterator it = mListeners.getIterator();
		while(it.hasMoreElements())
		{
			WorldListener* listener = it.getNext();
			listener->worldCreated(evt);
		}
	}
	//----------------------------------------------------------------------------
	void WorldManager::fireWorldDestroyed(World* _world)
	{
		WorldEvent evt(_world);
		Listeners::Iterator it = mListeners.getIterator();
		while(it.hasMoreElements())
		{
			WorldListener* listener = it.getNext();
			listener->worldDestroyed(evt);
		}
	}
	//----------------------------------------------------------------------------
	void WorldManager::fireWorldLoaded(World* _world)
	{
		WorldEvent evt(_world);
		Listeners::Iterator it = mListeners.getIterator();
		while(it.hasMoreElements())
		{
			WorldListener* listener = it.getNext();
			listener->worldLoaded(evt);
		}
	}
	//----------------------------------------------------------------------------
	void WorldManager::fireWorldMerged(World* _world)
	{
		WorldEvent evt(_world);
		Listeners::Iterator it = mListeners.getIterator();
		while(it.hasMoreElements())
		{
			WorldListener* listener = it.getNext();
			listener->worldMerged(evt);
		}
	}
	//----------------------------------------------------------------------------
	void WorldManager::fireWorldSaved(World* _world)
	{
		WorldEvent evt(_world);
		Listeners::Iterator it = mListeners.getIterator();
		while(it.hasMoreElements())
		{
			WorldListener* listener = it.getNext();
			listener->worldSaved(evt);
		}
	}
	//--------------------------------------------------------------------------------
	void WorldManager::fireWorldObjectAdded(World* _world, WorldObject* _worldObject)
	{
		WorldObjectEvent evt(_world, _worldObject);
		ObjectListeners::Iterator it = mObjectListeners.getIterator();
		while(it.hasMoreElements())
		{
			WorldObjectListener* listener = it.getNext();
			listener->worldObjectAdded(evt);
		}
	}
	//--------------------------------------------------------------------------------
	void WorldManager::fireWorldObjectRemoved(World* _world, WorldObject* _worldObject)
	{
		WorldObjectEvent evt(_world, _worldObject);
		ObjectListeners::Iterator it = mObjectListeners.getIterator();
		while(it.hasMoreElements())
		{
			WorldObjectListener* listener = it.getNext();
			listener->worldObjectRemoved(evt);
		}
	}
	//--------------------------------------------------------------------------------
	void WorldManager::fireWorldObjectChanged(World* _world, WorldObject* _worldObject)
	{
		WorldObjectEvent evt(_world, _worldObject);
		ObjectListeners::Iterator it = mObjectListeners.getIterator();
		while(it.hasMoreElements())
		{
			WorldObjectListener* listener = it.getNext();
			listener->worldObjectChanged(evt);
		}
	}
	//--------------------------------------------------------------------------------
	void WorldManager::fireWorldObjectSelectChanged(World* _world, WorldObject* _worldObject, bool _selected)
	{
		WorldObjectEvent evt(_world, _worldObject, _selected);
		ObjectListeners::Iterator it = mObjectListeners.getIterator();
		while(it.hasMoreElements())
		{
			WorldObjectListener* listener = it.getNext();
			listener->worldObjectSelectChanged(evt);
		}
	}

} // namespace GothOgre