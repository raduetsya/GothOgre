#ifndef GUISYSTEM_WORLD_EDIT_SETTINGS_H
#define GUISYSTEM_WORLD_EDIT_SETTINGS_H


namespace GUISystem
{
	//--------------------------------------------------------------------------
	/** Class to keep world editing settings.
	@remark
	The class is designed to operate with world editing settings.
	The world editing settings mean temporary information used to edit worlds. 
	@par
	Once an instance has been created, the same instance is accessible throughout 
	the life of that object by using ControlSettings::getSingleton (as a reference) 
	or ControlSettings::getSingletonPtr (as a pointer). */
	class GUISYSTEM_EXPORT WorldEditSettings : public Singleton<WorldEditSettings>, public ConfigSettings
	{
	public:
		WorldEditSettings(const String& _filename);
	};

	// Macro to inject config settings' listening functionality to any class.
#	define GUISYSTEM_DECLARE_WORLD_EDIT_SETTINGS_CHANGED_EX( \
		worldEditSettingsChangedFunc, \
		registerFunc, unregisterFunc, classname) \
		GOTHOGRE_DECLARE_CONFIG_SETTINGS_CHANGED_EX( \
			worldEditSettingsChanged, \
			registerFunc, unregisterFunc, classname, WorldEditSettings)

#	define GUISYSTEM_DECLARE_WORLD_EDIT_SETTINGS_CHANGED( classname ) \
		GUISYSTEM_DECLARE_WORLD_EDIT_SETTINGS_CHANGED_EX( \
			worldEditSettingsChanged, \
			registerWorldEditSettingsListener, unregisterWorldEditSettingsListener, classname )

} // namespace GUISystem

#endif // GUISYSTEM_WORLD_EDIT_SETTINGS_H