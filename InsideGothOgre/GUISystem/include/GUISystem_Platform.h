#ifndef GUISYSTEM_PLATFORM_H
#define GUISYSTEM_PLATFORM_H

//----------------------------------------------------------------------------
// Windows Settings
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#	if defined( GUISYSTEM_BUILD )
#      	define GUISYSTEM_EXPORT __declspec( dllexport )
#   else
#       define GUISYSTEM_EXPORT __declspec( dllimport )
#	endif
#endif

#endif // GUISYSTEM_PLATFORM_H
