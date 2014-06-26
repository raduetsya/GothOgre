#ifndef GOTHOGRE_PLATFORM_H
#define GOTHOGRE_PLATFORM_H

//----------------------------------------------------------------------------
// Windows Settings
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#	if defined( GOTHOGRE_BUILD )
#      	define GOTHOGRE_EXPORT __declspec( dllexport )
#   else
#       define GOTHOGRE_EXPORT __declspec( dllimport )
#	endif
#endif

#endif // GOTHOGRE_PLATFORM_H
