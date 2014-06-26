#include "GothOgre_Precompiled.h"
#include "GothOgre_ConfigFileEx.h"
#include "GothOgre_UnicodeUtil.h"

namespace GothOgre
{
	const String ConfigFileEx::DEFAULT_SEPARATORS = "\t:=";
	const String ConfigFileEx::DEFAULT_COMMENTS = "#@;";
	//-----------------------------------------------------------------------
	ConfigFileEx::ConfigFileEx()
	{
	}
	//-----------------------------------------------------------------------
	ConfigFileEx::~ConfigFileEx()
	{
		_clear();
	}
	//-----------------------------------------------------------------------
	void ConfigFileEx::load(const String& _filename, const String& _separators, const String& _comments, ConfigFileExFlags _flags)
	{
		loadDirect(_filename, _separators, _comments, _flags);
	}
	//-----------------------------------------------------------------------
	void ConfigFileEx::load(const String& _filename, const String& _resourceGroup, const String& _separators, const String& _comments, ConfigFileExFlags _flags)
	{
		loadFromResourceSystem(_filename, _resourceGroup, _separators, _comments, _flags);
	}
	//-----------------------------------------------------------------------
	void ConfigFileEx::loadDirect(const String& _filename, const String& _separators, const String& _comments, ConfigFileExFlags _flags)
	{
		/* Open the configuration file */
		std::ifstream s;
		// Always open in binary mode
		s.open(_filename.c_str(), std::ios::in | std::ios::binary);
		if(s.fail())
			GOTHOGRE_EXCEPT("ConfigFile: '" << _filename << "' file not found!");

		// Wrap as a stream
		DataStreamPtr stream(OGRE_NEW FileStreamDataStream(_filename, &s, false));
		load(stream, _separators, _comments, _flags);
	}
	//-----------------------------------------------------------------------
	void ConfigFileEx::loadFromResourceSystem(const String& _filename, const String& _resourceGroup, const String& _separators, const String& _comments, ConfigFileExFlags _flags)
	{
		DataStreamPtr stream = 
			ResourceGroupManager::getSingleton().openResource(_filename, _resourceGroup);
		load(stream, _separators, _comments, _flags);
	}
	//-----------------------------------------------------------------------
	void ConfigFileEx::load(const DataStreamExPtr& _dataStream, const String& _separators, const String& _comments, ConfigFileExFlags _flags)
	{
		if( !_flags.check( ConfigFileExFlags::MERGE ))
			_clear();

		if(_dataStream->getEncoding().empty())
			_dataStream->setEncoding("UTF-8");

		String currentSection = "";
		SettingsMultiMap* currentSettings = OGRE_NEW_T(SettingsMultiMap, MEMCATEGORY_GENERAL)();
		mSettings[currentSection] = currentSettings;

		/* Process the file line for line */
		String line, optName, optVal;
		while (!_dataStream->eof())
		{
			_dataStream->readLine(line, EOL::CRLF);
			/* Ignore comments & blanks */
			if (line.length() > 0 && line.find_first_of(_comments) != 0)
			{
				if (line.at(0) == '[' && line.at(line.length()-1) == ']')
				{
					// Section
					currentSection = line.substr(1, line.length() - 2);
					SettingsBySection::const_iterator seci = mSettings.find(currentSection);
					if (seci == mSettings.end())
					{
						currentSettings = OGRE_NEW_T(SettingsMultiMap, MEMCATEGORY_GENERAL)();
						mSettings[currentSection] = currentSettings;
					}
					else
					{
						currentSettings = seci->second;
					} 
				}
				else
				{
					/* Find the first seperator character and split the string there */
					size_t separator_pos = line.find_first_of(_separators, 0);
					if (separator_pos != String::npos)
					{
						optName = line.substr(0, separator_pos);
						/* Find the first non-seperator character following the name */
						String::size_type nonseparator_pos = line.find_first_not_of(_separators, separator_pos);
						/* ... and extract the value */
						/* Make sure we don't crash on an empty setting (it might be a valid value) */
						optVal = (nonseparator_pos == String::npos) ? "" : line.substr(nonseparator_pos);
						if(!_flags.check( ConfigFileExFlags::NO_TRIM_WHITESPACES))
						{
							StringUtil::trim(optVal);
							StringUtil::trim(optName);
						}
						optVal = deconvertSpecialChars(optVal);
						SettingsMultiMap::iterator setit = currentSettings->end();
						if( !_flags.check( ConfigFileExFlags::MULTI_SETTINGS ))
						{
							setit = currentSettings->find(optName);
						}
						if(setit == currentSettings->end())
						{
							setit = currentSettings->insert(SettingsMultiMap::value_type(optName, String()));
						}
						setit->second = optVal;
					}
				}
			}
		}
		loaded();
	}
	//-----------------------------------------------------------------------
	void ConfigFileEx::save(const String& _filename, const String& _separator, const String& _comments)
	{
		std::fstream s;
		s.open( _filename.c_str(), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc );
		if(s.fail())
			GOTHOGRE_EXCEPT("ConfigFile: '" << _filename << "' file cannot be written!");

		DataStreamPtr stream(OGRE_NEW FileStreamDataStream(_filename, &s, false));
		save(stream, _separator, _comments);
	}
	//-----------------------------------------------------------------------
	void ConfigFileEx::save(const DataStreamExPtr& _dataStream, const String& _separator, const String& _comments)
	{
		_dataStream->setEncoding("UTF-8");

		SettingsBySection::iterator secIt;
		for(secIt = mSettings.begin(); secIt!=mSettings.end(); secIt++)
		{
			if(secIt->first.size() > 0)
			{
				String line = "[" + secIt->first + "]";
				_dataStream->writeLine(line, EOL::CRLF);
			}
			SettingsMultiMap* sec = secIt->second;
			for(SettingsMultiMap::iterator it = sec->begin(); it != sec->end(); it++)
			{
				const String& key = it->first;
				const String& value = it->second;
				if(key.find_first_of(_comments) != 0)
				{
					String line = key + _separator + convertSpecialChars(value);
					_dataStream->writeLine(line, EOL::CRLF);
				}
			}
		}
		saved();
	}
	//-----------------------------------------------------------------------
	bool ConfigFileEx::hasSection(const String& _section) const
	{
		SettingsBySection::const_iterator secIt = mSettings.find(_section);
		return (secIt != mSettings.end());
	}
	//-----------------------------------------------------------------------
	bool ConfigFileEx::hasSetting(const String& _key, const String& _section) const
	{
		SettingsBySection::const_iterator secIt = mSettings.find(_section);

		if (secIt == mSettings.end())
			return false;

		SettingsMultiMap* sec = secIt->second;
		SettingsMultiMap::const_iterator it = sec->find(_key);
		return (it != sec->end());
	}
	//-----------------------------------------------------------------------
	void ConfigFileEx::clearMultiSetting(const String& _key, const String& _section)
	{
		if(_clearMultiSetting(_key, _section))
			changed();
	}
	//-----------------------------------------------------------------------
	bool ConfigFileEx::_clearMultiSetting(const String& _key, const String& _section)
	{
		bool wasChanged = false;
		SettingsBySection::iterator secIt = mSettings.find(_section);
		if (secIt != mSettings.end())
		{
			SettingsMultiMap* sec = secIt->second;
			std::pair<SettingsMultiMap::iterator, SettingsMultiMap::iterator> range;
			range = sec->equal_range(_key);
			if(range.first != range.second)
			{
				sec->erase(range.first, range.second);
				wasChanged = true;
			}
		}
		return wasChanged;
	}
	//-----------------------------------------------------------------------
	void ConfigFileEx::clear()
	{
		if(!_clear())
			changed();
	}
	//-----------------------------------------------------------------------
	bool ConfigFileEx::_clear()
	{
		bool wasChanged = false;
		if(!mSettings.empty())
		{
			for( SettingsBySection::iterator seci = mSettings.begin(); 
					seci != mSettings.end(); ++seci )
			{
				OGRE_DELETE_T(seci->second, SettingsMultiMap, MEMCATEGORY_GENERAL);
			}
			mSettings.clear();
			wasChanged = true;
		}
		return wasChanged;
	}
	//-----------------------------------------------------------------------
	ConfigFileEx::SectionIterator ConfigFileEx::getSectionIterator() const
	{
		return SectionIterator(mSettings.begin(), mSettings.end());
	}
	//-----------------------------------------------------------------------
	ConfigFileEx::SettingsIterator ConfigFileEx::getSettingsIterator(const String& section) const
	{
        SettingsBySection::const_iterator seci = mSettings.find(section);
        if (seci == mSettings.end())
        {
            GOTHOGRE_EXCEPT("Cannot find section " << section << 
                "ConfigFileEx::getSettingsIterator");
        }
        else
        {
            return SettingsIterator(seci->second->begin(), seci->second->end());
        }
	}
	//-----------------------------------------------------------------------
	String ConfigFileEx::convertSpecialChars(const String& _str)
	{
		String ret;
		ret.reserve(_str.length());
		size_t i = 0; 
		while(i != _str.length())
		{
			char ch = _str[i++];
			switch(ch)
			{
				case '\\': ret += "\\\\"; break;
				case '\n': ret += "\\n"; break;
				case '\r': ret += "\\r"; break;
				case '\t': ret += "\\t"; break;
				default:   ret += ch; 
			}
		}
		return ret;
	}
	//-----------------------------------------------------------------------
	String ConfigFileEx::deconvertSpecialChars(const String& _str)
	{
		String ret;
		ret.reserve(_str.length());
		size_t i = 0; 
		while(i != _str.length())
		{
			char ch = _str[i++];
			if(ch == '\\')
			{
				ch = _str[i++];
				switch(ch)
				{
					case 'n': ret += '\n'; break;
					case 'r': ret += '\r'; break;
					case 't': ret += '\t'; break;
					default:  ret += ch;
				}
			}
			else
			{
				ret += ch;
			}
		}
		return ret;
	}

} // namespace GothOgre