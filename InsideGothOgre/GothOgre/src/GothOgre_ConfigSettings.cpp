#include "GothOgre_Precompiled.h"
#include "GothOgre_ConfigSettings.h"
#include "GothOgre_ConfigSettingsListener.h"
#include "GothOgre_CommandLine.h"

namespace GothOgre
{
	//------------------------------------------------------------------------------
	ConfigSettings::ConfigSettings(const String& _filename, ConfigFileExFlags _flags)
	{
		mFilename = CommandLine::getSingleton().getAbsolutePath(_filename);
		mFlags = _flags;
		mUpdateCounter = 0;
		mBeforeBatchUpdateCounter = 0;
		mBatchCounter  = 0;
		mFiring = false;
		mChanged = false;
	}
	//------------------------------------------------------------------------------
	ConfigSettings::~ConfigSettings()
	{
	}
	//------------------------------------------------------------------------------
	void ConfigSettings::initialise()
	{
		// file can be not found and it's not a problem
		std::ifstream s;
		s.open(mFilename.c_str(), std::ios::in | std::ios::binary);
		if(s.fail())
		{
			GOTHOGRE_WARNING(mFilename << " - File not found");
		}
		else
		{
			GOTHOGRE_INFO(mFilename << " - Loading file");
			s.close();
			ConfigFileEx::load(mFilename, 
				ConfigFileEx::DEFAULT_SEPARATORS, ConfigFileEx::DEFAULT_COMMENTS, mFlags);
		}		
	}
	//------------------------------------------------------------------------------
	void ConfigSettings::shutdown()
	{
		GOTHOGRE_INFO(mFilename << " - Saving file");
		if(mChanged)
			ConfigFileEx::save(mFilename);
	}
	//------------------------------------------------------------------------------
	void ConfigSettings::_addListener(ConfigSettingsListener* _listener)
	{
		_listener->mElementHandle = mListeners.add(_listener);
		fireEvent(ConfigSettingsEvent::LISTENER_ADDED, _listener);
	}
	//------------------------------------------------------------------------------
	void ConfigSettings::_removeListener(ConfigSettingsListener* _listener)
	{
		mListeners.remove(_listener->mElementHandle);
	}
	//------------------------------------------------------------------------------
	void ConfigSettings::beginBatch()
	{
		if(!mBatchCounter)
			mBeforeBatchUpdateCounter = mUpdateCounter;

		++mBatchCounter;
	}
	//------------------------------------------------------------------------------
	void ConfigSettings::endBatch(bool _fireIfChanged /* = true */)
	{
		--mBatchCounter;
		
		if(!mBatchCounter && mUpdateCounter != mBeforeBatchUpdateCounter && _fireIfChanged)
			fireEvent(ConfigSettingsEvent::BATCH_CHANGED);
	}
	//------------------------------------------------------------------------------
	void ConfigSettings::changed()
	{
		mChanged = true;
		++mUpdateCounter;
		if(!mBatchCounter)
		{
			fireEvent(ConfigSettingsEvent::SETTING_CHANGED);
		}
	}
	//------------------------------------------------------------------------------
	void ConfigSettings::settingNotFound(const String& _key, const String& _section)
	{
		mChanged = true;
		++mUpdateCounter;
	}
	//------------------------------------------------------------------------------
	void ConfigSettings::loaded()
	{
		mChanged = false;
		++mUpdateCounter;
		if(!mBatchCounter)
		{
			fireEvent(ConfigSettingsEvent::FILE_LOADED);
		}
	}
	//------------------------------------------------------------------------------
	void ConfigSettings::saved()
	{
		mChanged = false;
	}
	//------------------------------------------------------------------------------
	void ConfigSettings::fireEvent( ConfigSettingsEvent::Reason _reason, ConfigSettingsListener* _singleListener )
	{
		if(mFiring)
			return; // Recursion is not allowed!

		mFiring = true;
		ConfigSettingsEvent evt( _reason, mUpdateCounter );

		if(_singleListener)
			_singleListener->configSettingsChanged(evt);
		else
		{
			Listeners::Iterator it = mListeners.getIterator();
			while(it.hasMoreElements())
			{
				ConfigSettingsListener* listener = it.getNext();
				listener->configSettingsChanged(evt);
			}
		}

		mFiring = false;
	}

} // namespace GothOgre