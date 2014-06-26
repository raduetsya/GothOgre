#ifndef GOTHOGRE_WORLD_H
#define GOTHOGRE_WORLD_H

#include "GothOgre_ZenObject.h"
#include "GothOgre_WorldObject.h"
#include "GothOgre_SafeList.h"
#include "GothOgre_WorldListener.h"
#include "GothOgre_SceneManagerEx.h"


namespace GothOgre
{
	class ZenFactory;


	//-------------------------------------------------------------------------
	/** Central World class. */
	class GOTHOGRE_EXPORT World : public ZenObject
	{
	public:
		/** Returns the name of the world. */
		const String& getName() const;

		/** Returns the short name of the world (without folder path). */
		const String& getShortName() const;

		/** Whether the world is active. */
		bool isActive() const;

		/** Whether the world is changed after it has been loaded or saved. */
		bool isChanged() const;

		/** Freezes changing of the "changed" internal variable. */
		void freezeChangedState();

		/** Unfreezes changing of the "changed" internal variable. */
		void unfreezeChangedState();

		/** Sets a pointer to used viewport. 
		A created world initially hasn't a viewport, it must be created
		separately and assigned to the world by the function. */
		void setViewport(Viewport* _viewport);

		/** Returns a pointer to used viewport. */
		Viewport* getViewport() const;

		/** Sets absolute coordinates of the viewport attached to this world. 
		See description of the "ViewportAbsCoord" class for details. */
		void setViewportAbsCoord(const ViewportAbsCoord& _vpAbsCoord);

		/** Returns absolute coordinates of the viewport attached to this world. */
		const ViewportAbsCoord& getViewportAbsCoord() const;

		/** Returns the root world object (it is attached to the root scene node). */
		WorldObjectPtr getRootWorldObject() const {return mRootWorldObject;}

		/** Searchs a world object by name. The first found object is returned.
		If there is no object with the specified name, the null pointer returned. */
		WorldObjectPtr findWorldObject(const String& _name) const;

		/** Searchs a world object by name, starting from the specified object. */
		WorldObjectPtr findWorldObject(const String& _name, const WorldObjectPtr& _subtreeRoot) const;

		/** Searchs a world object by name and type. The first found object is returned.
		If there is no object with the specified name, the null pointer returned. */
		WorldObjectPtr findWorldObject(const String& _name, const String& _typeName) const;

		/** Searchs a world object by name and type, starting from the specified object. */
		WorldObjectPtr findWorldObject(const String& _name, const String& _typeName, const WorldObjectPtr& _subtreeRoot) const;

		typedef ConstVectorIterator<list<WorldObject*>::type> SelectionIterator;
		
		/** Returns iterator for all selected world objects. */
		SelectionIterator getSelectionIterator() const;

		/** Selects all world objects, excluding the world root. */
		void selectAll();

		/** Selects all world objects in a specified subtree. */
		void selectSubtree(const WorldObjectPtr& _subtreeRoot, bool _includingSubtreeRoot = true);

		/** Deselects all world objects. */
		void deselectAll();

		/** Deselects all world objects in a specified subtree. */
		void deselectSubtree(const WorldObjectPtr& _subtreeRoot, bool _includingSubtreeRoot = true);

		/** Inverts the selection of all world objects,
		excluding the world root. */
		void invertSelection();

		/** Invert the selection of all world objects in a specified subtree. */
		void invertSelection(const WorldObjectPtr& _subtreeRoot, bool _includingSubtreeRoot = true);

		/** Returns the scene manager created by function World::create. */
		SceneManagerEx* getSceneManager() const {return mSceneManager;}

		/** Returns the world camera created by function World::create. */
		Camera* getCamera() const {return mCamera;}

		/** Resets the position of the world camera,
		i.e. moves it to (0, 0, 0). */
		void resetCameraPosition();

	private:
		friend class WorldManager;
		/** Internal function.
		Sets the name of the world. */
		void _setName(const String& _name);

		/** Internal function.
		Sets after this world has been activated or deactivated. */
		void _setActive(bool _active);

		/** Sets whether this world has been changed. 
		If the changing is freezed (see the "freezeChanged" function)
		then this function does nothing. */
		void _setChanged(bool _changed);

		/** Internal function. 
		Adds a specified world object to selection list
		without firing listeners and without changing selection state
		of the specified world object itself. */
		void _addToSelection(WorldObject* _worldObject);

		/** Internal function. 
		Removes a specified world object from selection list
		without firing listeners and without changing selection state
		of the specified world object itself. */
		void _removeFromSelection(WorldObject* _worldObject);

	protected:
		World();
		virtual ~World();

	private:
		void _selectSubtree(const WorldObjectPtr& _subtreeRoot, bool _includingSubtreeRoot);
		void _deselectSubtree(const WorldObjectPtr& _subtreeRoot, bool _includingSubtreeRoot);
		void _invertSelection(const WorldObjectPtr& _subtreeRoot, bool _includingSubtreeRoot);

	private:
		String							mName;
		String                          mShortName;
		SceneManagerEx*					mSceneManager;
		Camera*							mCamera;
		WorldObjectPtr					mRootWorldObject;
		list<WorldObject*>::type		mSelection;
		bool                            mActive;
		bool                            mChanged;
		int                             mChangedFreezeCount;
		Viewport*                       mViewport;
		ViewportAbsCoord                mViewportAbsCoord;
	};

} // namespace GothOgre

#endif // GOTHOGRE_WORLD_H
