#ifndef GOTHOGRE_ROOT_H
#define GOTHOGRE_ROOT_H

namespace GothOgre
{
	class Root : public Ogre::Root
	{
	public:
        /** Constructor
        @param pluginFileName The file that contains plugins information.
            Defaults to "plugins.cfg", may be left blank to ignore.
		@param configFileName The file that contains the configuration to be loaded.
			Defaults to "ogre.cfg", may be left blank to load nothing.
		@param logFileName The logfile to create, defaults to Ogre.log, may be 
			left blank if you've already set up LogManager & Log yourself
		*/
        Root(const String& pluginFileName = "plugins.cfg", 
			const String& configFileName = "ogre.cfg", 
			const String& logFileName = "Ogre.log");

		~Root();

		static void addFrameListener(FrameListener* _listener);
		static void removeFrameListener(FrameListener* _listener);
	};