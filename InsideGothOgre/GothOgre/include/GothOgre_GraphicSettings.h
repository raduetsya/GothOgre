#ifndef GOTHOGRE_GRAPHIC_SETTINGS_H
#define GOTHOGRE_GRAPHIC_SETTINGS_H

#include "GothOgre_ConfigSettings.h"

namespace GothOgre
{
	//--------------------------------------------------------------------------
	/** Class to keep graphic settings.
	@remark
	The class is designed to operate with graphic settings.
	@par
	Once an instance has been created, the same instance is accessible throughout 
	the life of that object by using GraphicSettings::getSingleton (as a reference) 
	or GraphicSettings::getSingletonPtr (as a pointer). */
	class GOTHOGRE_EXPORT GraphicSettings : public Singleton<GraphicSettings>, public ConfigSettings
	{
	public:
		GraphicSettings(const String& _filename);
	};

	// Macro to inject config settings' listening functionality to any class.
#	define GOTHOGRE_DECLARE_GRAPHIC_SETTINGS_CHANGED_EX( \
		graphicSettingsChangedFunc, \
		registerFunc, unregisterFunc, classname) \
		GOTHOGRE_DECLARE_CONFIG_SETTINGS_CHANGED_EX( \
			graphicSettingsChanged, \
			registerFunc, unregisterFunc, \
			classname, GraphicSettings)

#	define GOTHOGRE_DECLARE_GRAPHIC_SETTINGS_CHANGED(classname) \
		GOTHOGRE_DECLARE_GRAPHIC_SETTINGS_CHANGED_EX( \
			graphicSettingsChanged, \
			registerGraphicSettingsListener, unregisterGraphicSettingsListener, \
			classname)

	//------------------------------------------------------------------------
	/* Example of usage:
	class MyClass
	{
	public:
		MyClass();
		~MyClass();

	private:
		GOTHOGRE_DECLARE_GRAPHIC_SETTINGS_CHANGED(MyClass)
	};
	
	MyClass::MyClass()
	{
		registerGraphicSettingsListener(); // Start handling of events
	}

	MyClass::~MyClass()
	{
		// It's not necessary to call unregisterGraphicSettingsListener
		// - it's called automatically
	}

	void MyClass::graphicSettingsChanged(const ConfigSettingsEvent& _evt)
	{
		// my event-handling code
	}

	*/

} // namespace GothOgre

#endif // GOTHOGRE_GRAPHIC_SETTINGS_H