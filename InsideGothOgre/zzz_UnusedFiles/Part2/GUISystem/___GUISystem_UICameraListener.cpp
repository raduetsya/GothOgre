#include "GUISystem_Precompiled.h"
#include "GUISystem_UICameraListener.h"


namespace GUISystem
{
	//-------------------------------------------------------------------------
	UICameraListener::UICameraListener(Camera* _camera)
	{
		mCamera = _camera;
		mCamera->addListener(this);
	}
	//-------------------------------------------------------------------------
	UICameraListener::~UICameraListener()
	{
		if(mCamera)
			mCamera->removeListener(this);
	}
	//-------------------------------------------------------------------------
	void UICameraListener::cameraDestroyed( Camera* _camera )
	{
		mCamera = nullptr;
	}
	//-------------------------------------------------------------------------
	void UICameraListener::cameraPreRenderScene( Camera* _camera )
	{
		mProfilerEnabled = Profiler::getSingleton().getEnabled();
		//if(mProfilerEnabled)
		//	Profiler::getSingleton().setEnabled(false);
	}
	//-------------------------------------------------------------------------
	void UICameraListener::cameraPostRenderScene( Camera* _camera )
	{
		//if(mProfilerEnabled)
		//	Profiler::getSingleton().setEnabled(true);
	}

} // namespace GUISystem