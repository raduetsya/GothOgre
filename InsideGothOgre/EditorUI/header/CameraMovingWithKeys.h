#ifndef CAMERA_MOVING_WITH_KEYS_H
#define CAMERA_MOVING_WITH_KEYS_H


namespace GUISystem
{
	class CameraMovingWithKeys
	{
	public:
		CameraMovingWithKeys();
		~CameraMovingWithKeys();
	
	private:
		GOTHOGRE_DECLARE_KEY_PRESSED_RELEASED( CameraMovingWithKeys );
		GOTHOGRE_DECLARE_FRAME_RENDERING_QUEUED( CameraMovingWithKeys );
		GUISYSTEM_DECLARE_CONTROL_SETTINGS_CHANGED( CameraMovingWithKeys );
		void processKeys();

	private:
		// Keys to move camera
		KeyList  mKeySpeedUp;
		KeyList  mKeyMoveForward;
		KeyList  mKeyMoveBackward;
		KeyList  mKeyFlyUp;
		KeyList  mKeyFlyDown;
		KeyList  mKeyRotateLeft;
		KeyList  mKeyRotateRight;
		KeyList  mKeyRotateUp;
		KeyList  mKeyRotateDown;
		KeyList  mKeyCenterView;

		// Whether keys are pressed
		bool mSpeedUpKeyPressed;
		bool mMoveForwardKeyPressed;
		bool mMoveBackwardKeyPressed;
		bool mFlyUpKeyPressed;
		bool mFlyDownKeyPressed;
		bool mRotateLeftKeyPressed;
		bool mRotateRightKeyPressed;
		bool mRotateUpKeyPressed;
		bool mRotateDownKeyPressed;
		bool mCenterViewKeyPressed;

		// Camera normal movement speed, in cm/s
		Real     mLinearSpeed;

		// Rotation camera speed, in degree/s
		Degree   mAngularSpeed;

		// Coefficient to scale camera movement, dimensionless
		Real     mSpeedUpCoef;
	};
	
} // namespace GUISystem
	
#endif // CAMERA_MOVING_WITH_KEYS_H