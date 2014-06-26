#ifndef GOTHOGRE_H
#define GOTHOGRE_H

// Main
#include "GothOgre_Platform.h"
#include "GothOgre_Prerequest.h"
#include "GothOgre_Using.h"
#include "GothOgre_App.h"

// Diagnostic
#include "GothOgre_Diagnostic.h"
#include "GothOgre_LogSystem.h"
#include "GothOgre_LogListener.h"
#include "GothOgre_Exception.h"

// Interface for future plugins
#include "GothOgre_Plugin.h"

// Command line
#include "GothOgre_CommandLine.h"

// Configuration Files
#include "GothOgre_ConfigFileEx.h"
#include "GothOgre_ConfigSettings.h"
#include "GothOgre_GraphicSettings.h"
#include "GothOgre_SoundSettings.h"
#include "GothOgre_EncodingSettings.h"
#include "GothOgre_ResourceLocationScanner.h"

// Input System
#include "GothOgre_MouseButton.h"
#include "GothOgre_KeyCode.h"
#include "GothOgre_KeyList.h"
#include "GothOgre_KeyComb.h"
#include "GothOgre_InputEventResult.h"
#include "GothOgre_MouseListener.h"
#include "GothOgre_KeyListener.h"
#include "GothOgre_WindowEventListener.h"
#include "GothOgre_FrameListener.h"
#include "GothOgre_InputSupport.h"
#include "GothOgre_InputSystem.h"

// Resources & Managers
#include "GothOgre_ResourceGroup.h"
#include "GothOgre_ResourceLoader.h"
#include "GothOgre_ResourceSaver.h"
#include "GothOgre_TextureManagerEx.h"
#include "GothOgre_Mtl.h"
#include "GothOgre_MtlManagerEx.h"
#include "GothOgre_MtlNameUtil.h"
#include "GothOgre_MatGenParams.h"
#include "GothOgre_MeshManagerEx.h"
#include "GothOgre_MeshReadHelper.h"
#include "GothOgre_MeshExReadHelper.h"

// Scripting
#include "GothOgre_ScriptLoaderEx.h"

// Time
#include "GothOgre_Time.h"
#include "GothOgre_SystemTime.h"
#include "GothOgre_WorldTime.h"
#include "GothOgre_WorldTimeListener.h"

// Sounds
#include "GothOgre_SoundDef.h"
#include "GothOgre_SoundDefManagerEx.h"
#include "GothOgre_SoundPlayer.h"
#include "GothOgre_SoundGroup.h"
#include "GothOgre_SoundSystem.h"

// Skies
#include "GothOgre_SkyDef.h"
#include "GothOgre_SkyDefManagerEx.h"
#include "GothOgre_SkySystem.h"

// Portals
#include "GothOgre_PortalSystem.h"

// Scene
#include "GothOgre_SceneManagerEx.h"
#include "GothOgre_SceneManagerExEnumerator.h"
#include "GothOgre_RaySceneQueryEx.h"
#include "GothOgre_SceneQueryMask.h"

// Zen archive
#include "GothOgre_ZenStringInterface.h"
#include "GothOgre_ZenStringInterface2.h"
#include "GothOgre_ZenObject.h"
#include "GothOgre_ZenFactory.h"
#include "GothOgre_ZenManager.h"
#include "GothOgre_ZenArchive.h"

// World
#include "GothOgre_World.h"
#include "GothOgre_WorldManager.h"
#include "GothOgre_WorldListener.h"
#include "GothOgre_WorldObjectListener.h"
#include "GothOgre_WorldObject.h"
#include "GothOgre_RootWorldObject.h"
#include "GothOgre_BaseWorldObject.h"
#include "GothOgre_DerivedWorldObject.h"

// User Interface
#include "GothOgre_UIModeManager.h"
#include "GothOgre_UIModeListener.h"
#include "GothOgre_Progress.h"
#include "GothOgre_ProgressListener.h"
#include "GothOgre_FrameStats.h"

// Strings & streams
#include "GothOgre_StrStream.h"
#include "GothOgre_EnumIO.h"
#include "GothOgre_StdIO.h"
#include "GothOgre_OgreTypeIO.h"
#include "GothOgre_StrConv.h"
#include "GothOgre_StrUtil.h"
#include "GothOgre_UnicodeUtil.h"
#include "GothOgre_UnicodeSupport.h"
#include "GothOgre_CodepageConverter.h"
#include "GothOgre_DataStreamEx.h"

// Containers
#include "GothOgre_SafeList.h"
#include "GothOgre_SafeOrderedList.h"
#include "GothOgre_Priority.h"
#include "GothOgre_BoundingSphereTree.h"
#include "GothOgre_FlagSet.h"
#include "GothOgre_MemoryPool.h"
#include "GothOgre_MemoryPoolPtr.h"
#include "GothOgre_MemoryPoolAllocator.h"
#include "GothOgre_AnyLite.h"

// Math
#include "GothOgre_Math.h"
#include "GothOgre_Interpolation.h"


#endif // GOTHOGRE_H