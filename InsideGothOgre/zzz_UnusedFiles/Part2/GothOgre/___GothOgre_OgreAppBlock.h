#ifndef GOTHOGRE_OGRE_APP_BLOCK_H
#define GOTHOGRE_OGRE_APP_BLOCK_H

#include "GothOgre_AppBlock.h"


namespace GothOgre
{

	class OgreAppBlock : public AppBlock
	{
	public:
		OgreAppBlock() 
			: mRoot(nullptr), mWindow(nullptr) 
		{
			GOTHOGRE_INFO("Creating Ogre::Root and loading plugins, using pathes: ");

			String pluginsCfg = CommandLine::getSingleton().getAbsolutePath("plugins.cfg");
			String ogreCfg    = CommandLine::getSingleton().getAbsolutePath("ogre.cfg");
			String ogreLog    = CommandLine::getSingleton().getAbsolutePath("ogre.log");

			GOTHOGRE_INFO(pluginsCfg << " - path to plugins.cfg,");
			GOTHOGRE_INFO(ogreCfg << " - path to ogre.cfg.");

			mRoot = new Root(pluginsCfg, ogreCfg, ogreLog);
			GOTHOGRE_ASSERT(mRoot != nullptr, "Failed to create Ogre::Root.");
			GOTHOGRE_INFO("Ogre::Root created successfully, plugins loaded.");

			GOTHOGRE_INFO("Looking for a valid Ogre's saved configuration from a previous run:");
			if (mRoot->restoreConfig())
			{
				GOTHOGRE_INFO("Valid Ogre's configuration was found.");
			}
			else
			{
				GOTHOGRE_INFO("Valid Ogre's configuration was not found, displaying a dialog asking the user to choose system settings.");
				if (mRoot->showConfigDialog()) 
				{
					GOTHOGRE_INFO("The user clicked 'OK'.");
				}
				else
				{
					GOTHOGRE_INFO("The user clicked 'Cancel'.");
					return;
				}
			}

			GOTHOGRE_INFO("Creating a rendering window");
			mWindow = mRoot->initialise(true);
			GOTHOGRE_ASSERT(mWindow != nullptr, "Could not create RenderWindow");
			GOTHOGRE_INFO("Rendering window created successfully");

			setWindowIcon();
		}

		~OgreAppBlock()
		{
			if(!mRoot)
				return;

			GOTHOGRE_INFO("Shutting down Ogre");
			
			// Unload archives to prevent destructor ArchiveManager::~ArchiveManager
			// from doint it. The destructor cannot safe delete archives
			// which uses each others.
			vector<Archive*>::type archives; 
			ArchiveManager::ArchiveMapIterator it = ArchiveManager::getSingleton().getArchiveIterator();
			while(it.hasMoreElements())
			{
				archives.push_back(it.getNext());
			}
			while(!archives.empty())
			{
				ArchiveManager::getSingleton().unload(archives.back());
				archives.pop_back();
			}

			// Destroy window and Root.
			mWindow = nullptr;
			OGRE_DELETE mRoot;
			mRoot = nullptr;
			GOTHOGRE_INFO("Ogre shutted down successfully");
		}

	private:
		void setWindowIcon()
		{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			// load icon from the application's Win32 resources.
			// name of icon resource: "appicon"
			HINSTANCE instance = ::GetModuleHandleA(NULL);
			HICON hIcon = ::LoadIconA(instance, "appicon");
			if (hIcon)
			{
				HWND hwnd;
				mWindow->getCustomAttribute("WINDOW", &hwnd);
				::SendMessageA( hwnd, WM_SETICON, ICON_BIG,   (LPARAM)hIcon );
				::SendMessageA( hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon );
			}
#endif
		}

	private:
		Ogre::Root*     mRoot;
		RenderWindow*   mWindow;
	};

}

#endif // GOTHOGRE_OGRE_APP_BLOCK_H