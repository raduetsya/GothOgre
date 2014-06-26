#ifndef GOTHOGRE_CONFIG_SETTINGS_LISTENER_H
#define GOTHOGRE_CONFIG_SETTINGS_LISTENER_H

#include "GothOgre_SafeList.h"


namespace GothOgre
{
	class ConfigSettings;

	//-------------------------------------------------------------------------
	/** Event sending by class ControlSettings */
	class GOTHOGRE_EXPORT ConfigSettingsEvent
	{
	public:
		/** What's the reason for the event */
		enum Reason
		{
			/** New listener was added. */
			LISTENER_ADDED,

			/** Single setting has been changed */
			SETTING_CHANGED,

			/** Batch of settings has been changed */
			BATCH_CHANGED,

			/** A file has been loaded */
			FILE_LOADED,
		};

		ConfigSettingsEvent(Reason _reason, size_t _counter) {mReason = _reason; mCounter = _counter;}
		Reason getReason() const {return mReason;}
		size_t getCounter() const {return mCounter;}

	private:
		Reason mReason;
		size_t mCounter;
	};


	//-------------------------------------------------------------------------
	/** Internal base class for handlers of event ControlSettingsEvent. */
	class GOTHOGRE_EXPORT ConfigSettingsListener
	{
	public:
		virtual void configSettingsChanged( const ConfigSettingsEvent& _evt) = 0;

	public:
		ConfigSettingsListener();
		virtual ~ConfigSettingsListener();

		void registerListener(ConfigSettings* _configSettings);
		void unregisterListener();

	private:
		friend class ConfigSettings;
		typedef SafeList<ConfigSettingsListener*>::ElementHandle ElementHandle;
		ElementHandle   mElementHandle;
		ConfigSettings* mConfigSettings;
	};

	// The macros is designed to be used as a base only. Should not be used directly.
#	define GOTHOGRE_DECLARE_CONFIG_SETTINGS_BASE(configSettingsChangedFunc, registerFunc, unregisterFunc, classname, configSettingsSingletonName) \
		void registerFunc() \
		{ \
			mPrivateConfigSettingsListener_##registerFunc.registerListener(this); \
		} \
		void unregisterFunc() \
		{ \
			mPrivateConfigSettingsListener_##registerFunc.unregisterListener(); \
		} \
		\
	private: \
		void configSettingsChanged_defaultImpl_##registerFunc(const ConfigSettingsEvent& _evt) \
		{ \
		} \
		\
		class PrivateConfigSettingsListener_##registerFunc : public ConfigSettingsListener \
		{ \
		public: \
			void configSettingsChanged(const ConfigSettingsEvent& _evt) \
			{ \
				return mOwner->configSettingsChangedFunc(_evt); \
			} \
			\
			void registerListener(classname* _owner) \
			{ \
				mOwner = _owner; \
				ConfigSettings* configSettings = configSettingsSingletonName::getSingletonPtr(); \
				ConfigSettingsListener::registerListener(configSettings); \
			} \
		\
		private: \
			classname* mOwner; \
		}; \
		friend class PrivateConfigSettingsListener_##registerFunc; \
		PrivateConfigSettingsListener_##registerFunc  mPrivateConfigSettingsListener_##registerFunc;



	//----------------------------------------------------------------------------
	// ConfigSettingsListener.
	// Macro to inject config settings' listening functionality to any class.
	//----------------------------------------------------------------------------
	// This macros is used to define useful macroses: 
	// GOTHOGRE_DECLARE_SOUND_SETTINGS_CHANGED, 
	// GOTHOGRE_DECLARE_ENCODING_SETTINGS_CHANGED, and so on.
#	define GOTHOGRE_DECLARE_CONFIG_SETTINGS_CHANGED_EX( \
		configSettingsChangedFunc, \
		registerFunc, unregisterFunc, classname, configSettingsSingletonName) \
		void configSettingsChangedFunc(const ConfigSettingsEvent& _evt); \
		GOTHOGRE_DECLARE_CONFIG_SETTINGS_BASE( \
			configSettingsChangedFunc, \
			registerFunc, unregisterFunc, classname, configSettingsSingletonName)

} // namespace GothOgre

#endif // GOTHOGRE_CONFIG_SETTINGS_LISTENER_H