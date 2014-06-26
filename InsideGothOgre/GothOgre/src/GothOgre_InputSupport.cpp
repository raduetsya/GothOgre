#include "GothOgre_Precompiled.h"
#include "GothOgre_InputSupport.h"

//-------------------------------------------------------------------------
GothOgre::InputSupportRegistration* 
	Ogre::Singleton<GothOgre::InputSupportRegistration>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------


namespace GothOgre
{
	//-----------------------------------------------------------------------
	// InputSupport
	//-----------------------------------------------------------------------
	InputSupport::InputSupport()
		: mWindow(nullptr)
	{
		InputSupportRegistration::getSingleton().registerSupport(this);
	}
	//-----------------------------------------------------------------------
	InputSupport::~InputSupport()
	{
		InputSupportRegistration::getSingleton().unregisterSupport(this);
	}
	//-----------------------------------------------------------------------
	void InputSupport::_setRenderWindow(RenderWindow* _window)
	{
		mWindow = _window;
	}




	//-----------------------------------------------------------------------
	// InputSupportRegistration
	//-----------------------------------------------------------------------
	InputSupportRegistration::InputSupportRegistration()
	{
		mSupport = nullptr;
	}
	//-----------------------------------------------------------------------
	InputSupportRegistration::~InputSupportRegistration()
	{
		if(mSupport)
			GOTHOGRE_ERROR("Input support is not unregistered before program exit.");
	}
	//-----------------------------------------------------------------------
	void InputSupportRegistration::registerSupport(InputSupport* _support)
	{
		if(mSupport && mSupport != _support)
			GOTHOGRE_ERROR("Input support is already registered.");
		mSupport = _support;
	}
	//-----------------------------------------------------------------------
	void InputSupportRegistration::unregisterSupport(InputSupport* _support)
	{
		if(mSupport == _support)
			mSupport = nullptr;
	}
	//-----------------------------------------------------------------------
	InputSupport* InputSupportRegistration::getSupport() const
	{
		GOTHOGRE_ASSERT(mSupport, "Input support is not registered.");
		return mSupport;
	}

} // namespace GothOgre