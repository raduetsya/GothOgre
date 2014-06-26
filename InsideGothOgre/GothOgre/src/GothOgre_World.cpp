#include "GothOgre_Precompiled.h"
#include "GothOgre_World.h"
#include "GothOgre_WorldManager.h"
#include "GothOgre_ZenManager.h"
#include "GothOgre_RootWorldObject.h"
#include "GothOgre_SoundSystem.h"
#include "GothOgre_StrConv.h"
#include "GothOgre_SceneManagerExEnumerator.h"


namespace GothOgre
{
	//--------------------------------------------------------------------------------
	// World
	//--------------------------------------------------------------------------------
	World::World()
		: mCamera(nullptr), mSceneManager(nullptr), mViewport(nullptr),
		  mActive(false), mChanged(false), mChangedFreezeCount(0)

	{
		// create a scene manager
		mSceneManager = SceneManagerExEnumerator::getSingleton()._createSceneManager();

		// create a camera
		mCamera = mSceneManager->createCamera("WorldCamera");
		mCamera->setNearClipDistance(5); // 5cm
		mCamera->setFixedYawAxis(true, Vector3::UNIT_Z);

		// reset camera position
		mCamera->setPosition(Vector3(0,0,0));
		mCamera->lookAt(mSceneManager->getEastDirection());
		Vector3 up = mCamera->getUp();

		// create the root world object
		WorldManager::getSingleton()._setActiveWorldOverride(this);
		mRootWorldObject = ZenManager::getSingleton().createZenObject("RootWorldObject");
	}
	//--------------------------------------------------------------------------------
	World::~World() 
	{
		// destroy all objects before destroying scene manager and camera
		mRootWorldObject.setNull();

		// destroy scene manager and camera
		if(mSceneManager)
		{
			if(mCamera)
				mSceneManager->destroyCamera(mCamera);

			SceneManagerExEnumerator::getSingleton()._destroySceneManager(mSceneManager);
		}		
	}
	//--------------------------------------------------------------------------------
	void World::_setChanged(bool _changed)
	{
		if(mChangedFreezeCount <= 0)
			mChanged = _changed;
	}
	//--------------------------------------------------------------------------------
	void World::freezeChangedState()
	{
		++mChangedFreezeCount;
	}
	//--------------------------------------------------------------------------------
	void World::unfreezeChangedState()
	{
		--mChangedFreezeCount;
	}
	//--------------------------------------------------------------------------------
	bool World::isChanged() const
	{
		return mChanged;
	}
	//--------------------------------------------------------------------------------
	void World::_setActive(bool _active)
	{
		mActive = _active;
		SoundSystem::getSingleton().setSoundListener(mActive ? mCamera : nullptr);
		SceneManagerExEnumerator::getSingleton()._setActiveSceneManager(mActive ? mSceneManager : nullptr);
		if(mViewport)
			mViewport->setAutoUpdated(mActive);
	}
	//--------------------------------------------------------------------------------
	bool World::isActive() const
	{
		return mActive;
	}
	//--------------------------------------------------------------------------------
	void World::setViewport(Viewport* _viewport)
	{
		mViewport = _viewport;
		if(mViewport)
			mViewport->setAutoUpdated(isActive());
	}
	//--------------------------------------------------------------------------------
	Viewport* World::getViewport() const
	{
		return mViewport;
	}
	//--------------------------------------------------------------------------------
	void World::setViewportAbsCoord(const ViewportAbsCoord& _vpAbsCoord)
	{
		mViewportAbsCoord = _vpAbsCoord;
		mViewportAbsCoord.setWidth(std::max(mViewportAbsCoord.getWidth(), 1));
		mViewportAbsCoord.setHeight(std::max(mViewportAbsCoord.getHeight(), 1));

		Real aspectRatio = (Real) mViewportAbsCoord.getWidth() / (Real) mViewportAbsCoord.getHeight();
		mCamera->setAspectRatio(aspectRatio);
	}
	//--------------------------------------------------------------------------------
	const ViewportAbsCoord& World::getViewportAbsCoord() const
	{
		return mViewportAbsCoord;
	}
	//--------------------------------------------------------------------------------
	void World::_setName(const String& _name)
	{
		mName = _name;

		// Get the name without folder path
		size_t i = mName.find_last_of("\\/");
		if(i != String::npos)
			mShortName = mName.substr(i + 1);
		else
			mShortName = mName;
	}
	//--------------------------------------------------------------------------------
	const String& World::getName() const
	{
		return mName;
	}
	//--------------------------------------------------------------------------------
	const String& World::getShortName() const
	{
		return mShortName;
	}
	//--------------------------------------------------------------------------------
	WorldObjectPtr World::findWorldObject(const String& _name) const
	{
		return findWorldObject(_name, StringUtil::BLANK, getRootWorldObject());
	}
	//--------------------------------------------------------------------------------
	WorldObjectPtr World::findWorldObject(const String& _name, const WorldObjectPtr& _subtreeRoot) const
	{
		return findWorldObject(_name, StringUtil::BLANK, _subtreeRoot);
	}
	//--------------------------------------------------------------------------------
	WorldObjectPtr World::findWorldObject(const String& _name, const String& _typeName) const
	{
		return findWorldObject(_name, _typeName, getRootWorldObject());
	}
	//--------------------------------------------------------------------------------
	WorldObjectPtr World::findWorldObject(const String& _name, const String& _typeName, const WorldObjectPtr& _subtreeRoot) const
	{
		WorldObjectPtr ret;
		if(_subtreeRoot->getName() == _name && 
			(_typeName.empty() || _typeName == _subtreeRoot->getWorldObjectTypeName()))
		{
			ret = _subtreeRoot;
		}
		else
		{
			WorldObject::ChildIterator it = _subtreeRoot->getChildIterator();
			while(it.hasMoreElements())
			{
				WorldObjectPtr child = it.getNext();
				ret = findWorldObject(_name, child);
				if(!ret.isNull())
					break;
			}
		}
		return ret;
	}
	//--------------------------------------------------------------------------------
	World::SelectionIterator World::getSelectionIterator() const
	{
		return SelectionIterator(mSelection);
	}
	//--------------------------------------------------------------------------------
	void World::selectAll()
	{
		selectSubtree(getRootWorldObject(), false);
	}
	//--------------------------------------------------------------------------------
	void World::selectSubtree(const WorldObjectPtr& _subtreeRoot, bool _includingSubtreeRoot)
	{
		_selectSubtree(_subtreeRoot, _includingSubtreeRoot);
	}
	//--------------------------------------------------------------------------------
	void World::_selectSubtree(const WorldObjectPtr& _subtreeRoot, bool _includingSubtreeRoot)
	{
		if(_includingSubtreeRoot && !_subtreeRoot->isSelected())
		{
			_subtreeRoot->setSelected(true);
		}
		
		WorldObject::ChildIterator it = _subtreeRoot->getChildIterator();
		while(it.hasMoreElements())
		{
			WorldObjectPtr child = it.getNext();
			_selectSubtree(child, true);
		}
	}
	//--------------------------------------------------------------------------------
	void World::deselectAll()
	{
		deselectSubtree(getRootWorldObject(), false);
	}
	//--------------------------------------------------------------------------------
	void World::deselectSubtree(const WorldObjectPtr& _subtreeRoot, bool _includingSubtreeRoot)
	{
		_deselectSubtree(_subtreeRoot, _includingSubtreeRoot);
	}
	//--------------------------------------------------------------------------------
	void World::_deselectSubtree(const WorldObjectPtr& _subtreeRoot, bool _includingSubtreeRoot)
	{
		if(_includingSubtreeRoot && _subtreeRoot->isSelected())
		{
			_subtreeRoot->setSelected(false);
		}
		
		WorldObject::ChildIterator it = _subtreeRoot->getChildIterator();
		while(it.hasMoreElements())
		{
			WorldObjectPtr child = it.getNext();
			_deselectSubtree(child, true);
		}
	}
	//--------------------------------------------------------------------------------
	void World::invertSelection()
	{
		invertSelection(getRootWorldObject(), false);
	}
	//--------------------------------------------------------------------------------
	void World::invertSelection(const WorldObjectPtr& _subtreeRoot, bool _includingSubtreeRoot)
	{
		_invertSelection(_subtreeRoot, _includingSubtreeRoot);
	}
	//--------------------------------------------------------------------------------
	void World::_invertSelection(const WorldObjectPtr& _subtreeRoot, bool _includingSubtreeRoot)
	{
		if(_includingSubtreeRoot)
		{
			_subtreeRoot->setSelected(!_subtreeRoot->isSelected());
		}
		
		WorldObject::ChildIterator it = _subtreeRoot->getChildIterator();
		while(it.hasMoreElements())
		{
			WorldObjectPtr child = it.getNext();
			_invertSelection(child, true);
		}
	}
	//--------------------------------------------------------------------------------
	void World::_addToSelection(WorldObject* _worldObject)
	{
		mSelection.push_back(_worldObject);
	}
	//--------------------------------------------------------------------------------
	void World::_removeFromSelection(WorldObject* _worldObject)
	{
		mSelection.remove(_worldObject);
	}

} // namespace GothOgre
