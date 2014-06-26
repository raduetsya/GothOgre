#ifndef GOTHOGRE_CONFIG_FILE_EX_H
#define GOTHOGRE_CONFIG_FILE_EX_H

#include "GothOgre_FlagSet.h"
#include "GothOgre_DataStreamEx.h"
#include "GothOgre_StrStream.h"


namespace GothOgre
{
	struct ConfigFileExFlags : public FlagSet
	{
		enum Enum
		{
			/** Don't trim leading and trailing whitespaces. */
			NO_TRIM_WHITESPACES  = 0x01,

			/** Don't clear settings before loading. */
			MERGE                = 0x02,

			/** Use multivalues: one key can have several values. */
			MULTI_SETTINGS       = 0x04,

			/** The default flags: trim whitespaces, clear before loading,
			don't use multivalues. */
			DEFAULT = 0
		};
		ConfigFileExFlags(int _val = DEFAULT) : FlagSet(_val) {}
	};



	//-----------------------------------------------------------------------
	/** Class extends the Ogre::ConfigFile class.
        @remarks
            This class can do anything what ConfigFile can do.
			See documentation in Ogre API Reference.
        @par
            It also makes possible to save file.
		@par
			The load functions get an additional parameter "comments".
			This parameter is a string any character of it can be used to start a comment.
        @par
            Functions to get and to set settings are templatized, 
			so they can work with any type.
    */
	class GOTHOGRE_EXPORT ConfigFileEx
	{
	public:
		static const String DEFAULT_SEPARATORS;
		static const String DEFAULT_COMMENTS;
		enum {DEFAULT_FLAGS = ConfigFileExFlags::DEFAULT};

		ConfigFileEx();
		virtual ~ConfigFileEx();

		/** Load from a file (not using resource group locations).
		@param _separators The string of characters. 
		Any of the character can be used to separate key and setting.
		@param _trimWhitespace If true the function will trim leading 
		and trailing whitespaces from any setting.
		@param _comments The string any character of it can be used to start a comment.	*/
		void load(const String& _filename, const String& _separators = DEFAULT_SEPARATORS, const String& _comments = DEFAULT_COMMENTS, ConfigFileExFlags _flags = DEFAULT_FLAGS);
		
		/** Load from a file (using resource group locations). */
		void load(const String& _filename, const String& _resourceGroup, const String& _separators = DEFAULT_SEPARATORS, const String& _comments = DEFAULT_COMMENTS, ConfigFileExFlags _flags = DEFAULT_FLAGS);
		
		/** Load from a data stream. */
		void load(const DataStreamExPtr& _dataStream, const String& _separators = DEFAULT_SEPARATORS, const String& _comments = DEFAULT_COMMENTS, ConfigFileExFlags _flags = DEFAULT_FLAGS);
		
		/** Load from a file (not using resource group locations). */
		void loadDirect(const String& _filename, const String& _separators = DEFAULT_SEPARATORS, const String& _comments = DEFAULT_COMMENTS, ConfigFileExFlags _flags = DEFAULT_FLAGS);
		
		/** Load from a file (using resource group locations). */
		void loadFromResourceSystem(const String& _filename, const String& _resourceGroup, const String& _separators = DEFAULT_SEPARATORS, const String& _comments = DEFAULT_COMMENTS, ConfigFileExFlags _flags = DEFAULT_FLAGS);

		/** Save to a file (not using resource group locations). */
		void save(const String& _filename, const String& _separator = "=", const String& _comments = DEFAULT_COMMENTS);

		/** Save to a file (not using resource group locations). */
		void save(const DataStreamExPtr& _dataStream, const String& _separator = "=", const String& _comments = DEFAULT_COMMENTS);

		/** Whether if there is a section. 
		@param _section The name of the section. */
		bool hasSection(const String& _section) const;

		/** Whether if there is a setting from the file with the named key. 
		@param _key The name of the setting
		@param _section The name of the section it must be in */
		bool hasSetting(const String& _key, const String& _section) const;

		/** Whether if there is a setting from the file with the named key. 
		@param _key The name of the setting
		@param _section The name of the section it must be in 
		@param _section The value of the settings if it found. */
		template<typename T>
		bool hasSetting(const String& _key, const String& _section, T* _pValue) const;

		/** Gets the first setting from the file with the named key. 
		The function returns T() if the setting is not found.
		@param _key The name of the setting
		@param _section The name of the section it must be in */
		template<typename T>
		T getSetting(const String& _key, const String& _section);

		/** Gets the first setting from the file with the named key. 
		@param _key The name of the setting
		@param _section The name of the section it must be in
		@param _defaultValue The value to return if the key will not found **/
		template<typename T>
		T getSetting(const String& _key, const String& _section, T _defaultValue);

		/** Gets all settings from the file with the named key. */
		template<typename T>
		typename vector<T>::type getMultiSetting(const String& _key, const String& _section) const;

		/** Appends one setting to the file with the named key. */
		template<typename T>
		void appendSetting(const String& _key, const String& _section, T _value);

		/** Sets the first setting with the named key and deletes 
		the rest settings with the same key. */
		template<typename T>
		void setSetting(const String& _key, const String& _section, T _value);

		/** Sets all the settings with the named key. */
		template<typename T>
		void setMultiSetting(const String& _key, const String& _section, const typename vector<T>::type& _multiValue);

		/** Clears the settings with the named key. */
		void clearMultiSetting(const String& _key, const String& _section);

		/** Clears all the settings. */
		void clear();

        typedef multimap<String, String>::type SettingsMultiMap;
        typedef ConstMapIterator<SettingsMultiMap> SettingsIterator;
       
		/** Gets an iterator for stepping through all the keys / values in the file. */
        typedef map<String, SettingsMultiMap*>::type SettingsBySection;
        typedef ConstMapIterator<SettingsBySection> SectionIterator;
        
		/** Get an iterator over all the available sections in the config file */
        SectionIterator getSectionIterator() const;
        
		/** Get an iterator over all the available settings in a section */
        SettingsIterator getSettingsIterator(const String& section = StringUtil::BLANK) const;

	protected:
		virtual void changed() {}
		virtual void settingNotFound(const String& _key, const String& _section) {}
		virtual void loaded() {}
		virtual void saved() {}

	private:
		template<typename T>
		bool _setMultiSetting(const String& _key, const String& _section, const typename vector<T>::type& _multiValue);

		template<typename T>
		bool _setSetting(const String& _key, const String& _section, T _value);

		template<typename T>
		bool _appendSetting(const String& _key, const String& _section, T _value);

		bool _clearMultiSetting(const String& _key, const String& _section);
		bool _clear();

		static String convertSpecialChars(const String& _str);
		static String deconvertSpecialChars(const String& _str);

		template<typename T>
		static String printToString(T _value);

		template<typename T>
		static T extractFromString(const String& _str, bool* _pSuccess);

		template<>
		static String extractFromString<String>(const String& _str, bool* _pSuccess);

	private:
		SettingsBySection	mSettings;
	};


	//-----------------------------------------------------------------------
	template<typename T>
	String ConfigFileEx::printToString(T _value)
	{
		StrStream ss;
		ss << _value;
		return ss.str();
	}
	//-----------------------------------------------------------------------
	template<typename T>
	T ConfigFileEx::extractFromString(const String& _str, bool* _pSuccess)
	{
		StrStream ss(_str, StrStream::DONT_COPY);
		T value;
		ss >> value;
		*_pSuccess = !ss.fail();
		return value;
	}
	//-----------------------------------------------------------------------
	template<>
	String ConfigFileEx::extractFromString<String>(const String& _str, bool* _pSuccess)
	{
		*_pSuccess = true;
		return _str;
	}
	//-----------------------------------------------------------------------
	template<typename T>
	bool ConfigFileEx::hasSetting(const String& _key, const String& _section, T* _pValue) const
	{
		SettingsBySection::const_iterator secIt = mSettings.find(_section);
		if (secIt == mSettings.end())
			return false;

		SettingsMultiMap* sec = secIt->second;
		SettingsMultiMap::const_iterator it = sec->find(_key);
		if (it == sec->end())
			return false;
		
		bool ok;
		*_pValue = extractFromString<T>(it->second, &ok);
		return ok;
	}
	//-----------------------------------------------------------------------
	template<typename T>
	T ConfigFileEx::getSetting(const String& _key, const String& _section)
	{
		T defaultValue = T();
		return getSetting(_key, _section, defaultValue);
	}
	//-----------------------------------------------------------------------
	template<typename T>
	T ConfigFileEx::getSetting(const String& _key, const String& _section, T _defaultValue)
	{
		SettingsBySection::iterator secIt = mSettings.find(_section);

		if (secIt == mSettings.end())
		{
			SettingsMultiMap* newSec = OGRE_NEW_T(SettingsMultiMap, MEMCATEGORY_GENERAL)();
			secIt = mSettings.insert(
				SettingsBySection::value_type(_section, newSec)).first;
		}

		SettingsMultiMap* sec = secIt->second;
		T value;
		bool ok = false;
		
		SettingsMultiMap::iterator it = sec->find(_key);
		if (it != sec->end())
			value = extractFromString<T>(it->second, &ok);

		if (!ok)
		{
			value = _defaultValue;
			it = sec->insert(SettingsMultiMap::value_type(_key, printToString(value)));
			settingNotFound(_key, _section);
		}
		return value;
	}
	//-----------------------------------------------------------------------
	template<typename T>
	typename vector<T>::type ConfigFileEx::getMultiSetting(const String& _key, const String& _section) const
	{
		vector<T>::type ret;
		SettingsBySection::const_iterator secIt = mSettings.find(_section);
		if (secIt != mSettings.end())
		{
			SettingsMultiMap* sec = secIt->second;
			std::pair<SettingsMultiMap::iterator, SettingsMultiMap::iterator> range;
			range = sec->equal_range(_key);

			// Iterate over matches
			for( SettingsMultiMap::iterator it = range.first; it != range.second; ++it)
			{
				const String& str = it->second;
				bool ok;
				T value = extractFromString<T>(str, &ok);
				if(ok)				
					ret.push_back(value);
			}
		}
		return ret;
	}
	//-----------------------------------------------------------------------
	template<typename T>
	void ConfigFileEx::appendSetting(const String& _key, const String& _section, T _value)
	{
		if(_appendSetting(_key, _section, _value))
			changed();
	}
	//-----------------------------------------------------------------------
	template<typename T>
	bool ConfigFileEx::_appendSetting(const String& _key, const String& _section, T _value)
	{
		SettingsBySection::iterator secIt = mSettings.find(_section);

		if (secIt == mSettings.end())
		{
			SettingsMultiMap* newSec = OGRE_NEW_T(SettingsMultiMap, MEMCATEGORY_GENERAL)();
			secIt = mSettings.insert(
				SettingsBySection::value_type(_section, newSec)).first;
		}
		
		SettingsMultiMap* sec = secIt->second;
		String strValue = printToString<T>(_value);
		sec->insert(SettingsMultiMap::value_type(_key, strValue));
		return true;
	}
	//-----------------------------------------------------------------------
	template<typename T>
	void ConfigFileEx::setSetting(const String& _key, const String& _section, T _value)
	{
		if(_setSetting(_key, _section, _value))
			changed();
	}
	//-----------------------------------------------------------------------
	template<typename T>
	bool ConfigFileEx::_setSetting(const String& _key, const String& _section, T _value)
	{
		bool wasChanged = false;
		String strValue = printToString<T>(_value);

		SettingsBySection::iterator secIt = mSettings.find(_section);
		if (secIt == mSettings.end())
		{
			SettingsMultiMap* newSec = OGRE_NEW_T(SettingsMultiMap, MEMCATEGORY_GENERAL)();
			secIt = mSettings.insert(
				SettingsBySection::value_type(_section, newSec)).first;
			wasChanged = true;
		}
		SettingsMultiMap* sec = secIt->second;

		std::pair<SettingsMultiMap::iterator, SettingsMultiMap::iterator> range;
		range = sec->equal_range(_key);
		if(range.first == range.second)
		{
			sec->insert(SettingsMultiMap::value_type(_key, strValue));
			wasChanged = true;
		}
		else
		{
			SettingsMultiMap::iterator setIt = range.first++;
			if(range.first != range.second)
			{
				sec->erase(range.first, range.second);
				wasChanged = true;
			}
			if(setIt->second != strValue)
			{
				setIt->second = strValue;
				wasChanged = true;
			}
		}

		return wasChanged;
	}
	//-----------------------------------------------------------------------
	template<typename T>
	void ConfigFileEx::setMultiSetting(const String& _key, const String& _section, const typename vector<T>::type& _values)
	{
		if(_setMultiSetting(_key, _section, _values))
			changed();
	}
	//-----------------------------------------------------------------------
	template<typename T>
	bool ConfigFileEx::_setMultiSetting(const String& _key, const String& _section, const typename vector<T>::type& _values)
	{
		bool wasChanged = false;

		SettingsBySection::iterator secIt = mSettings.find(_section);
		if (secIt == mSettings.end())
		{
			SettingsMultiMap* newSec = OGRE_NEW_T(SettingsMultiMap, MEMCATEGORY_GENERAL)();
			secIt = mSettings.insert(
				SettingsBySection::value_type(_section, newSec)).first;
			wasChanged = true;
		}
		SettingsMultiMap* sec = secIt->second;

		std::pair<SettingsMultiMap::iterator, SettingsMultiMap::iterator> range;
		range = sec->equal_range(_key);
		
		size_t index = 0;
		SettingsMultiMap::iterator it = range.first;
		for(; it != range.second && index != _values.size(); ++it, ++index)
		{
			String strValue = printToString<T>(_values[index]);
			if(it->second != strValue)
			{
				it->second = strValue;
				wasChanged = true;
			}
		}

		if(it != range.second)
		{
			sec->erase(it, range.second);
			wasChanged = true;
		}
		else
		{
			for(; index != _values.size(); ++index)
			{
				String strValue = printToString<T>(_values[index]);
				it = sec->insert(it, SettingsMultiMap::value_type(_key, strValue));
				wasChanged = true;
			}
		}

		return wasChanged;
	}

} // namespace GothOgre

#endif // GOTHOGRE_CONFIG_FILE_EX_H