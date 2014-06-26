#ifndef EDITOR_UI_PLUGIN_H
#define EDITOR_UI_PLUGIN_H


namespace GUISystem
{
	class EditorInitSkin;
	class EditorInitBeforeTools;
	class EditorInitTools;


	/** Plugin instance for EditorUI */
	class EditorUIPlugin : public Plugin
	{
	public:
		EditorUIPlugin();
		const String& getName() const;
		void install();
		void initialise();
		void shutdown();
		void uninstall();

	private:
		typedef list<ToolFactory*>::type ToolFactories;
		ToolFactories               mToolFactories;

		EditorInitSkin*         mInitSkin;
		EditorInitBeforeTools*  mInitBeforeTools;
		EditorInitTools*        mInitTools;
	};

} // namespace GUISystem

#endif // EDITOR_UI_PLUGIN_H