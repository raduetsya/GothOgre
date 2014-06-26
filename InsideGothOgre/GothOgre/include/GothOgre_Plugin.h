#ifndef GOTHOGRE_PLUGIN_H
#define GOTHOGRE_PLUGIN_H

// This file contains an interface for future plugins.


//----------------------------------------------------------------------------
// GothOgre plugin is Ogre plugin

#include "OgrePlugin.h"


//----------------------------------------------------------------------------
// Platform dependent attribute to export functions

// Windows Settings
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 
#   define GOTHOGRE_PLUGIN_EXPORT __declspec( dllexport )
#endif


//----------------------------------------------------------------------------
// Macros "GOTHOGRE_PLUGIN_ENTRY_POINT" must be written in a plugin's source file

#define GOTHOGRE_PLUGIN_ENTRY_POINT(PluginType) \
	static PluginType* sPlugin = nullptr; \
	extern "C" void GOTHOGRE_PLUGIN_EXPORT dllStartPlugin() \
	{ \
		sPlugin = OGRE_NEW PluginType(); \
		Ogre::Root::getSingleton().installPlugin(sPlugin); \
	} \
	extern "C" void GOTHOGRE_PLUGIN_EXPORT dllStopPlugin() \
	{ \
		Root::getSingleton().uninstallPlugin(sPlugin); \
		OGRE_DELETE sPlugin; \
	}

#endif