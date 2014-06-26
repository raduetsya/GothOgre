#include "GUISystem_Precompiled.h"
#include "GUISystem_ViewportManager.h"

//-------------------------------------------------------------------------
GUISystem::ViewportManager* 
	Ogre::Singleton<GUISystem::ViewportManager>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------

namespace GUISystem
{
	//----------------------------------------------------------------------
	ViewportManager::ViewportManager()
		: mBackgroundViewport(nullptr), 
		  mUIViewport(nullptr)
	{
	}
	//----------------------------------------------------------------------
	ViewportManager::~ViewportManager()
	{
	}


} // namespace GUISystem

