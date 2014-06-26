#include "Precompiled.h"
#include "CameraMovingWithKeys.h"

namespace GUISystem
{
	CameraMovingWithKeys::CameraMovingWithKeys()
	{
		// all buttons are released
		mSpeedUpKeyPressed = false;
		mMoveForwardKeyPressed = false;
		mMoveBackwardKeyPressed = false;
		mFlyUpKeyPressed = false;
		mFlyDownKeyPressed = false;
		mCenterViewKeyPressed = false;
		mRotateLeftKeyPressed = false;
		mRotateRightKeyPressed = false;
		mRotateUpKeyPressed = false;
		mRotateDownKeyPressed = false;

		registerControlSettingsListener();
		registerFrameListener();
		registerKeyListener(InputPriority::TOOL);
	}
	//------------------------------------------------------------------------------------
	CameraMovingWithKeys::~CameraMovingWithKeys()
	{
	}
	//------------------------------------------------------------------------------------
	void CameraMovingWithKeys::controlSettingsChanged(const ConfigSettingsEvent& _evt)
	{
		ControlSettings& cs = ControlSettings::getSingleton();
		mKeySpeedUp       = cs.getSetting("SpeedUp", "Editor", KeyList(KeyCode::LSHIFT, KeyCode::RSHIFT));
		mKeyMoveForward   = cs.getSetting("MoveForward", "Editor", KeyList(KeyCode::UP));
		mKeyMoveBackward  = cs.getSetting("MoveBackward", "Editor", KeyList(KeyCode::DOWN));
		mKeyFlyUp         = cs.getSetting("FlyUp", "Editor", KeyList(KeyCode::Y));
		mKeyFlyDown       = cs.getSetting("FlyDown", "Editor", KeyList(KeyCode::A));
		mKeyCenterView    = cs.getSetting("CenterView", "Editor", KeyList(KeyCode::END));
		mKeyRotateLeft    = cs.getSetting("RotateLeft", "Editor", KeyList(KeyCode::LEFT));
		mKeyRotateRight   = cs.getSetting("RotateRight", "Editor", KeyList(KeyCode::RIGHT));
		mKeyRotateUp      = cs.getSetting("RotateUp", "Editor", KeyList(KeyCode::PRIOR));
		mKeyRotateDown    = cs.getSetting("RotateDown", "Editor", KeyList(KeyCode::NEXT));
		mLinearSpeed      = cs.getSetting("LinearSpeed", "Editor", 300.0f);
		mAngularSpeed     = (Degree) cs.getSetting("AngularSpeed", "Editor", 60.0f);
		mSpeedUpCoef      = cs.getSetting("SpeedUpCoef", "Editor", 10.0f);
	}
	//---------------------------------------------------------------------------
	KeyEventResult CameraMovingWithKeys::keyPressed( const KeyEvent& _evt )
	{
		processKeys();
		return KeyEventResult::NOT_PROCESSED; // may be more handlers
	}
	//---------------------------------------------------------------------------
	KeyEventResult CameraMovingWithKeys::keyReleased( const KeyEvent& _evt )
	{
		processKeys();
		return KeyEventResult::NOT_PROCESSED; // may be more handlers
	}
	//---------------------------------------------------------------------------
	void CameraMovingWithKeys::processKeys()
	{
		InputSystem& in = InputSystem::getSingleton();
		mSpeedUpKeyPressed = in.isKeyDown(mKeySpeedUp);
		mFlyUpKeyPressed = in.isKeyDown(mKeyFlyUp);
		mFlyDownKeyPressed = in.isKeyDown(mKeyFlyDown);
		mMoveForwardKeyPressed = in.isKeyDown(mKeyMoveForward);
		mMoveBackwardKeyPressed = in.isKeyDown(mKeyMoveBackward);
		mCenterViewKeyPressed = in.isKeyDown(mKeyCenterView);		
		mRotateLeftKeyPressed = in.isKeyDown(mKeyRotateLeft);
		mRotateRightKeyPressed = in.isKeyDown(mKeyRotateRight);
		mRotateUpKeyPressed = in.isKeyDown(mKeyRotateUp);
		mRotateDownKeyPressed = in.isKeyDown(mKeyRotateDown);
	}
	//---------------------------------------------------------------------------
	void CameraMovingWithKeys::frameRenderingQueued(const FrameEvent& evt)
	{
		World* world = WorldManager::getSingleton().getActiveWorld();
		if(!world)
			return;

		Camera* camera = world->getCamera();

		float ofs = evt.timeSinceLastFrame * mLinearSpeed;
		Radian rot(evt.timeSinceLastFrame * mAngularSpeed);

		if(mSpeedUpKeyPressed)
		{
			ofs *= mSpeedUpCoef;
			rot *= mSpeedUpCoef;
		}

		if(mFlyUpKeyPressed)
		{
			camera->move(Vector3(0, 0, ofs));
		}
		if(mFlyDownKeyPressed)
		{
			camera->move(Vector3(0, 0, -ofs));
		}
		if(mMoveForwardKeyPressed)
		{
			Vector3 dir = ofs * camera->getDirection();
			camera->move(Vector3(dir.x, dir.y, 0));
		}
		if(mMoveBackwardKeyPressed)
		{
			Vector3 dir = ofs * camera->getDirection();
			camera->move(Vector3(-dir.x, -dir.y, 0));
		}
		if(mRotateLeftKeyPressed)
		{
			camera->setFixedYawAxis(true, Vector3::UNIT_Z);
			camera->yaw(rot);
		}
		if(mRotateRightKeyPressed)
		{
			camera->setFixedYawAxis(true, Vector3::UNIT_Z);
			camera->yaw(-rot);
		}
		if(mRotateUpKeyPressed)
		{
			camera->pitch(rot);
		}
		if(mRotateDownKeyPressed)
		{
			camera->pitch(-rot);
		}

		// center view
		if(mCenterViewKeyPressed)
		{
			// calculate a new direction - make projection of the current direction
			// to XY plane
			Vector3 dir = camera->getDirection();
			if(dir.x != 0 && dir.y != 0)
			{
				dir.z = 0;
				dir.normalise();
			}
			else
			{
				dir = Vector3(1, 0, 0);
			}

			// yaw axis is used by lookAt to determine the up direction
			camera->setFixedYawAxis(true, Vector3::UNIT_Z);

			camera->lookAt(camera->getPosition() + dir);
		}
	}
	
} // namespace GUISystem
