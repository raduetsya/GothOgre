#include "Precompiled.h"
#include "SelectingObjectsWithMouse.h"

namespace GUISystem
{
	SelectingObjectsWithMouse::SelectingObjectsWithMouse()
	{
		registerMouseListener(InputPriority::TOOL);
	}
	//------------------------------------------------------------------------------------
	SelectingObjectsWithMouse::~SelectingObjectsWithMouse()
	{
	}
	//---------------------------------------------------------------------------
	MouseEventResult SelectingObjectsWithMouse::mousePressed( const MouseEvent& _evt )
	{
		World* world = WorldManager::getSingleton().getActiveWorld();
		if(!world)
			return MouseEventResult::NOT_PROCESSED;

		Camera* camera = world->getCamera();
		SceneManagerEx* sm = world->getSceneManager();

		RaySceneQueryExPtr q = sm->createRayQueryEx(_evt.getAbsX(), _evt.getAbsY(), world->getViewportAbsCoord(), camera);
		q->setQueryTypeMask( SceneQueryMask::VOB | SceneQueryMask::INVISIBLE_IN_GAME );
		q->setMaxResults(1);
		RaySceneQueryExResult& result = q->execute();

		world->deselectAll();

		if(!result.empty())
		{
			WorldObject* worldObject = WorldObject::getFromMovableObject(result.front().movable);
			if(worldObject)
				worldObject->select();
		}

		return MouseEventResult::PROCESSED;
	}
	
} // namespace GUISystem
