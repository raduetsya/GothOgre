#ifndef GOTHOGRE_SOUND_SETTINGS_H
#define GOTHOGRE_SOUND_SETTINGS_H

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
	class GOTHOGRE_EXPORT SoundSettings : public Singleton<SoundSettings>, public ConfigSettings
	{
	public:
		SoundSettings(const String& _filename);
	};

	// Macroses to inject config settings' listening functionality to any class.
#	define GOTHOGRE_DECLARE_SOUND_SETTINGS_CHANGED_EX( \
		soundSettingsChangedFunc, \
		registerFunc, unregisterFunc, classname) \
		GOTHOGRE_DECLARE_CONFIG_SETTINGS_CHANGED_EX( \
			soundSettingsChangedFunc, \
			registerFunc, unregisterFunc, \
			classname, SoundSettings)

#	define GOTHOGRE_DECLARE_SOUND_SETTINGS_CHANGED(classname) \
		GOTHOGRE_DECLARE_SOUND_SETTINGS_CHANGED_EX( \
			soundSettingsChanged, \
			registerSoundSettingsListener, unregisterSoundSettingsListener, \
			classname)

	//------------------------------------------------------------------------
	/* Example of usage:
	class MyClass
	{
	public:
		MyClass();
		~MyClass();

	private:
		GOTHOGRE_DECLARE_SOUND_SETTINGS_CHANGED(MyClass)
	};
	
	MyClass::MyClass()
	{
		registerSoundSettingsListener(); // Start handling of events
	}

	MyClass::~MyClass()
	{
		// It's not necessary to call unregisterSoundSettingsListener
		// - it's called automatically
	}

	void MyClass::soundSettingsChanged(const ConfigSettingsEvent& _evt)
	{
		// my event-handling code
	}

	*/

} // namespace GothOgre

#endif // GOTHOGRE_SOUND_SETTINGS_H