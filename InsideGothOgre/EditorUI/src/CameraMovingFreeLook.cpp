#include "Precompiled.h"
#include "CameraMovingFreeLook.h"

namespace GUISystem
{
	//------------------------------------------------------------------------
	class CrossHairLayout : public BaseLayout
	{
	public:
		CrossHairLayout(const String& _layoutName, int _absCenterX, int _absCenterY)
		{
			setLayoutName(_layoutName);
			initialiseByAttributes(this);

			MyGUI::StaticImage* image = getMainWidget()->castType<MyGUI::StaticImage>();
			MyGUI::IntSize imageSize = image->getSize();
			image->setCoord(_absCenterX - imageSize.width / 2,
				_absCenterY - imageSize.height / 2, 
				imageSize.width, imageSize.height);
		}
	};
	//------------------------------------------------------------------------
	CameraMovingFreeLook::CameraMovingFreeLook()
		: mCamera(nullptr), mCrossHair(nullptr)
	{
		// only exit key can be pressed for now
		mSpeedUpKeyPressed = false;
		mMoveForwardKeyPressed = false;
		mMoveBackwardKeyPressed = false;
		mFlyUpKeyPressed = false;
		mFlyDownKeyPressed = false;
		mStrafeLeftKeyPressed = false;
		mStrafeRightKeyPressed = false;
		mCenterViewKeyPressed = false;

		// get camera
		World* world = WorldManager::getSingleton().getActiveWorld();
		if(!world)
			return;

		mCamera = world->getCamera();

		// clip and hide cursor
		MyGUI::PointerManager::getInstance().setVisible(false);
		InputSystem& in = InputSystem::getSingleton();
		in.getCursorPos(&mStoredCursorPosX, &mStoredCursorPosY);
		ViewportAbsCoord absCoord = world->getViewportAbsCoord();
		in.clipCursor(absCoord);

		// align cursor to center of the main viewport
		mViewportWidth   = absCoord.getWidth();
		mViewportHeight  = absCoord.getHeight();
		mViewportCenterX = absCoord.getLeft() + mViewportWidth / 2;
		mViewportCenterY = absCoord.getTop() + mViewportHeight / 2;
		InputSystem::getSingleton().setCursorPos(mViewportCenterX, mViewportCenterY, false);
	
		registerControlSettingsListener();
		registerKeyListener(InputPriority::TOOL);
		registerMouseListener(InputPriority::TOOL_HIGH);
		registerFrameListener();
	}
	//------------------------------------------------------------------------
	CameraMovingFreeLook::~CameraMovingFreeLook()
	{
		// free and show cursor
		MyGUI::PointerManager::getInstance().setVisible(true);
		InputSystem& in = InputSystem::getSingleton();
		in.freeCursor();
		in.setCursorPos(mStoredCursorPosX, mStoredCursorPosY, false /** don't call mouseMoved */);

		// unload layout for crosshair
		if(mCrossHair)
			delete mCrossHair;
	}
	//------------------------------------------------------------------------
	void CameraMovingFreeLook::setCrossHairLayout(const String& _layoutName)
	{
		if(mCrossHairLayoutName != _layoutName)
		{
			mCrossHairLayoutName = _layoutName;
			if(mCrossHair)
			{
				delete mCrossHair;
				mCrossHair = nullptr;
			}
			mCrossHair = new CrossHairLayout(_layoutName, mViewportCenterX, mViewportCenterY);
		}
	}
	//------------------------------------------------------------------------
	void CameraMovingFreeLook::controlSettingsChanged(const ConfigSettingsEvent& _evt)
	{
		ControlSettings& cs = ControlSettings::getSingleton();
		mKeySpeedUp      = cs.getSetting("SpeedUp", "Editor", KeyList(KeyCode::RSHIFT, KeyCode::LSHIFT));
		mKeyMoveForward  = cs.getSetting("MoveForward", "Editor", KeyList(KeyCode::UP));
		mKeyMoveBackward = cs.getSetting("MoveBackward", "Editor", KeyList(KeyCode::DOWN));
		mKeyFlyUp        = cs.getSetting("FlyUp", "Editor", KeyList(KeyCode::Y));
		mKeyFlyDown      = cs.getSetting("FlyDown", "Editor", KeyList(KeyCode::A));
		mKeyCenterView   = cs.getSetting("CenterView", "Editor", KeyList(KeyCode::END));
		mKeyStrafeLeft   = cs.getSetting("FreeLook_StrafeLeft", "Editor", KeyList(KeyCode::LEFT));
		mKeyStrafeRight  = cs.getSetting("FreeLook_StrafeRight", "Editor", KeyList(KeyCode::RIGHT));
		mLinearSpeed     = cs.getSetting("LinearSpeed", "Editor", 300.0f);
		mAngularSpeed    = (Degree) cs.getSetting("AngularSpeed", "Editor", 60.0f);		
		mSpeedUpCoef     = cs.getSetting("SpeedUpCoef", "Editor", 10.0f);
	}
	//---------------------------------------------------------------------------
	KeyEventResult CameraMovingFreeLook::keyPressed( const KeyEvent& _evt )
	{
		processKeys();
		return KeyEventResult::NOT_PROCESSED; // may be more handlers
	}
	//---------------------------------------------------------------------------
	KeyEventResult CameraMovingFreeLook::keyReleased( const KeyEvent& _evt )
	{
		processKeys();
		return KeyEventResult::NOT_PROCESSED; // may be more handlers
	}
	//---------------------------------------------------------------------------
	void CameraMovingFreeLook::processKeys()
	{
		InputSystem& in = InputSystem::getSingleton();
		mSpeedUpKeyPressed = in.isKeyDown(mKeySpeedUp);
		mFlyUpKeyPressed = in.isKeyDown(mKeyFlyUp);
		mFlyDownKeyPressed = in.isKeyDown(mKeyFlyDown);
		mMoveForwardKeyPressed = in.isKeyDown(mKeyMoveForward);
		mMoveBackwardKeyPressed = in.isKeyDown(mKeyMoveBackward);
		mStrafeLeftKeyPressed = in.isKeyDown(mKeyStrafeLeft);
		mStrafeRightKeyPressed = in.isKeyDown(mKeyStrafeRight);
		mCenterViewKeyPressed = in.isKeyDown(mKeyCenterView);
	}
	//---------------------------------------------------------------------------
	void CameraMovingFreeLook::frameRenderingQueued(const FrameEvent& evt)
	{
		Real ofs = mLinearSpeed * evt.timeSinceLastFrame;
		Radian rot = mAngularSpeed * evt.timeSinceLastFrame;

		if(mSpeedUpKeyPressed)
		{
			ofs *= mSpeedUpCoef;
			rot *= mSpeedUpCoef;
		}

		if(mFlyUpKeyPressed)
		{
			mCamera->move(Vector3(0, 0, ofs));
		}
		if(mFlyDownKeyPressed)
		{
			mCamera->move(Vector3(0, 0, -ofs));
		}
		if(mMoveForwardKeyPressed)
		{
			Vector3 dir = ofs * mCamera->getDirection();
			mCamera->move(dir);
		}
		if(mMoveBackwardKeyPressed)
		{
			Vector3 dir = ofs * mCamera->getDirection();
			mCamera->move(-dir);
		}
		if(mStrafeLeftKeyPressed)
		{
			Vector3 right = ofs * mCamera->getRight();
			mCamera->move(-right);
		}
		if(mStrafeRightKeyPressed)
		{
			Vector3 right = ofs * mCamera->getRight();
			mCamera->move(right);
		}

		// center view
		if(mCenterViewKeyPressed)
		{
			// calculate a new direction - make projection of the current direction
			// to XY plane
			Vector3 dir = mCamera->getDirection();
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
			mCamera->setFixedYawAxis(true, Vector3::UNIT_Z);

			mCamera->lookAt(mCamera->getPosition() + dir);
		}
	}
	//---------------------------------------------------------------------------
	MouseEventResult CameraMovingFreeLook::mouseMoved( const MouseEvent& _evt )
	{
		Real propX = Real(_evt.getAbsX() - mViewportCenterX) / mViewportWidth;
		Real propY = Real(_evt.getAbsY() - mViewportCenterY) / mViewportHeight;

		Radian rotX = propX * mCamera->getFOVy();
		Radian rotY = propY * mCamera->getFOVy() * mCamera->getAspectRatio();
		mCamera->setFixedYawAxis(true, Vector3::UNIT_Z);
		mCamera->yaw(-rotX);
		mCamera->pitch(-rotY);

		InputSystem::getSingleton().setCursorPos(mViewportCenterX, mViewportCenterY, false);
		return MouseEventResult::PROCESSED;
	}

} // namespace GUISystem
