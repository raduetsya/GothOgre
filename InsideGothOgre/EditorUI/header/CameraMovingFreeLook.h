#ifndef CAMERA_MOVING_FREELOOK_H
#define CAMERA_MOVING_FREELOOK_H

namespace GUISystem
{
	class CameraMovingFreeLook
	{
	public:
		CameraMovingFreeLook();
		~CameraMovingFreeLook();

		void setCrossHairLayout(const String& _layoutName);
	
	private:
		GUISYSTEM_DECLARE_CONTROL_SETTINGS_CHANGED( CameraMovingFreeLook );
		GOTHOGRE_DECLARE_FRAME_RENDERING_QUEUED( CameraMovingFreeLook );
		GOTHOGRE_DECLARE_MOUSE_MOVED( CameraMovingFreeLook );
		GOTHOGRE_DECLARE_KEY_PRESSED_RELEASED( CameraMovingFreeLook );

		void processKeys();

	private:
		// Keys to move camera
		KeyList mKeySpeedUp;
		KeyList mKeyMoveForward;
		KeyList mKeyMoveBackward;
		KeyList mKeyFlyUp;
		KeyList mKeyFlyDown;
		KeyList mKeyStrafeLeft;
		KeyList mKeyStrafeRight;
		KeyList mKeyCenterView;

		// Whether keys are pressed
		bool mSpeedUpKeyPressed;
		bool mMoveForwardKeyPressed;
		bool mMoveBackwardKeyPressed;
		bool mFlyUpKeyPressed;
		bool mFlyDownKeyPressed;
		bool mStrafeLeftKeyPressed;
		bool mStrafeRightKeyPressed;
		bool mCenterViewKeyPressed;

		// Camera normal movement speed, in cm/s
		Real	mLinearSpeed;

		// Rotation camera speed, in degree/s
		Degree	mAngularSpeed;

		// Coefficient to scale camera movement, dimensionless
		Real	mSpeedUpCoef;

		// Main viewport's attributes
		int     mViewportWidth;
		int     mViewportHeight;
		int     mViewportCenterX;
		int     mViewportCenterY;
		Camera* mCamera;

		// The cursor's coordinates stored on activation of this tool
		int		mStoredCursorPosX;
		int		mStoredCursorPosY;

		// Layout for crosshair
		String       mCrossHairLayoutName;
		BaseLayout*  mCrossHair;
	};
}

#endif // CAMERA_MOVING_FREELOOK_H