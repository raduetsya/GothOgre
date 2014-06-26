#ifndef GUISYSTEM_UICAMERA_LISTENER_H
#define GUISYSTEM_UICAMERA_LISTENER_H

namespace GUISystem
{
	class UICameraListener : public Camera::Listener
	{
	public:
		UICameraListener(Camera* _camera);
		~UICameraListener();

		void cameraPreRenderScene( Camera* _camera );
		void cameraPostRenderScene( Camera* _camera );
		void cameraDestroyed( Camera* _camera );

	private:
		Camera* mCamera;
		bool    mProfilerEnabled;
	};

}

#endif // GUISYSTEM_UICAMERA_LISTENER_H