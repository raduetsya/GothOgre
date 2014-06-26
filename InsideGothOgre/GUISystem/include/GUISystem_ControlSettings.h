#ifndef GUISYSTEM_CONTROL_SETTINGS_H
#define GUISYSTEM_CONTROL_SETTINGS_H


namespace GUISystem
{
	//--------------------------------------------------------------------------
	/** Class to keep control settings.
	@remark
	The class is designed to operate with control settings.
	The control settings mean keys for various operations, mouse speed, etc. 
	@par
	Once an instance has been created, the same instance is accessible throughout 
	the life of that object by using ControlSettings::getSingleton (as a reference) 
	or ControlSettings::getSingletonPtr (as a pointer). */
	class GUISYSTEM_EXPORT ControlSettings : public Singleton<ControlSettings>, public ConfigSettings
	{
	public:
		ControlSettings(const String& _filename);
	};

	// Macro to inject config settings' listening functionality to any class.
#	define GUISYSTEM_DECLARE_CONTROL_SETTINGS_CHANGED_EX( \
		controlSettingsChangedFunc, \
		registerFunc, unregisterFunc, classname) \
		GOTHOGRE_DECLARE_CONFIG_SETTINGS_CHANGED_EX( \
			controlSettingsChanged, \
			registerFunc, unregisterFunc, classname, ControlSettings)

#	define GUISYSTEM_DECLARE_CONTROL_SETTINGS_CHANGED( classname ) \
		GUISYSTEM_DECLARE_CONTROL_SETTINGS_CHANGED_EX( \
			controlSettingsChanged, \
			registerControlSettingsListener, unregisterControlSettingsListener, classname )

} // namespace GUISystem

#endif // GUISYSTEM_CONTROL_SETTINGS_H