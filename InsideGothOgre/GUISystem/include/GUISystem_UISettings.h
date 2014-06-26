#ifndef GUISYSTEM_UI_SETTINGS_H
#define GUISYSTEM_UI_SETTINGS_H


namespace GUISystem
{
	//--------------------------------------------------------------------------
	/** Class to keep ui settings.
	@remark
	The class is designed to operate with ui settings.
	The ui settings mean keys for various operations, mouse speed, etc. 
	@par
	Once an instance has been created, the same instance is accessible throughout 
	the life of that object by using UISettings::getSingleton (as a reference) 
	or UISettings::getSingletonPtr (as a pointer). */
	class GUISYSTEM_EXPORT UISettings : public Singleton<UISettings>, public ConfigSettings
	{
	public:
		UISettings(const String& _filename);
	};

	// Macro to inject config settings' listening functionality to any class.
#	define GUISYSTEM_DECLARE_UI_SETTINGS_CHANGED_EX( \
		uiSettingsChangedFunc, \
		registerFunc, unregisterFunc, classname) \
		GOTHOGRE_DECLARE_CONFIG_SETTINGS_CHANGED_EX( \
			uiSettingsChanged, \
			registerFunc, unregisterFunc, classname, UISettings)

#	define GUISYSTEM_DECLARE_UI_SETTINGS_CHANGED( classname ) \
		GUISYSTEM_DECLARE_UI_SETTINGS_CHANGED_EX( \
			uiSettingsChanged, \
			registerUISettingsListener, unregisterUISettingsListener, classname )

} // namespace GUISystem

#endif // GUISYSTEM_UI_SETTINGS_H
