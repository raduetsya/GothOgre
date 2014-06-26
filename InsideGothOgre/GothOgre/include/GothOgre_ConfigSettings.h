#ifndef GOTHOGRE_CONFIG_SETTINGS_H
#define GOTHOGRE_CONFIG_SETTINGS_H

#include "GothOgre_ConfigFileEx.h"
#include "GothOgre_ConfigSettingsListener.h"
#include "GothOgre_SafeList.h"


namespace GothOgre
{
	//-------------------------------------------------------------------------
	/** Class to keep configuration settings.
	@remark
	This is a common base class for some classes-singletons
	which operate with different *.cfg files. */
	class GOTHOGRE_EXPORT ConfigSettings : public ConfigFileEx
	{
	public:
		ConfigSettings(const String& _filename, ConfigFileExFlags _flags = ConfigFileExFlags::DEFAULT);
		virtual ~ConfigSettings();

		/** Loads settings from the file. */
		void initialise();
		
		/** Saves settings to the file. */
		void shutdown();

		void beginBatch();
		void endBatch(bool _fireIfChanged = true);

	private:
		void changed();
		void settingNotFound(const String& _key, const String& _section);
		void loaded();
		void saved();

	private:
		void fireEvent(ConfigSettingsEvent::Reason _reason, ConfigSettingsListener* _singleListener = nullptr);

	private:
		friend class ConfigSettingsListener;
		void _addListener(ConfigSettingsListener* _listener);
		void _removeListener(ConfigSettingsListener* _listener);

	private:
		typedef SafeList<ConfigSettingsListener*> Listeners;

		Listeners	      mListeners;
		String            mFilename;
		ConfigFileExFlags mFlags;
		size_t            mUpdateCounter;
		size_t            mBeforeBatchUpdateCounter;
		size_t            mBatchCounter;
		bool              mFiring;
		bool              mChanged;
	};

} // namespace GothOgre

#endif // GOTHOGRE_CONFIG_SETTINGS_H