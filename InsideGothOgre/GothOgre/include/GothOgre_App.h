#ifndef GOTHOGRE_APP_H
#define GOTHOGRE_APP_H


namespace GothOgre
{
	class AppImpl;


	/** The main application class */
	class GOTHOGRE_EXPORT App
	{
	public:
		/** Initialises the application, and creates all static objects,
		and initialises the Ogre and MyGUI engines */ 
		App(int _argc, char** _argv);
		App(char* _commandLine);

		/** Main rendering loop */
		void run();

		/** Frees all resources */
		~App();

	private:
		AppImpl* mImpl;
	};

} // namespace GothOgre

#endif // GOTHOGRE_APP_H