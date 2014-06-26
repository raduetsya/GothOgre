#include "GothOgre_Precompiled.h"
#include "GothOgre_EncodingSettings.h"
#include "GothOgre_StrUtil.h"

//-------------------------------------------------------------------------
GothOgre::EncodingSettings* 
Ogre::Singleton<GothOgre::EncodingSettings>::ms_Singleton 
= nullptr;
//-------------------------------------------------------------------------

namespace GothOgre
{
	//-------------------------------------------------------------------------------------------------
	const String EncodingSettings::DEFAULT_ENCODING = "Windows-1251";
	static String sEncodingsToGuess[] = {"UTF-8", "UTF16-LE", "UTF16-BE", "UTF32-LE", "UTF32-BE"};
	//-------------------------------------------------------------------------------------------------
	EncodingSettings::EncodingSettings(const String& _filename)
		: ConfigSettings(_filename)
	{
	}
	//-------------------------------------------------------------------------------------------------
	String EncodingSettings::getEncoding(const String& _filename, const String& _defaultEncoding)
	{
		static const String ENCODING = "Encoding";
		String r;
		if(hasSetting(ENCODING, _filename, &r))
			return r;

		size_t dotpos = _filename.rfind('.');
		if(dotpos != String::npos)
		{
			String filemask = "*" + _filename.substr(dotpos);
			StrUtil::toUpperCase(filemask);
			r = getSetting<String>(ENCODING, filemask, _defaultEncoding);
			return r;
		}

		return _defaultEncoding;
	}
	//-------------------------------------------------------------------------------------------------
	void EncodingSettings::setupStreamEncoding(const DataStreamExPtr& _dataStream, const String& _defaultEncoding)
	{
		String encoding = _dataStream->getEncoding();
		if(encoding.empty())
		{
			String encoding = getEncoding(_dataStream->getName(), _defaultEncoding);
			_dataStream->setEncoding(encoding);
		}
	}
	//-------------------------------------------------------------------------------------------------
	void EncodingSettings::readStringAndGuessEncoding(const DataStreamExPtr& _dataStream, const String& _stringExpected, size_t _sizeInBytes)
	{
		setupStreamEncoding(_dataStream);

		size_t ofs = _dataStream->tellg();
		String stringRead;
		
		size_t index = -1;
		while(true)
		{
			if(index != -1)
			{
				if(index == sizeof(sEncodingsToGuess) / sizeof(sEncodingsToGuess[0]))
					GOTHOGRE_EXCEPT(_dataStream->getName() << 
						" - File corrupted, text '" << _stringExpected 
						<< "' not found");

				_dataStream->setEncoding(sEncodingsToGuess[index]);
			}

			++index;

			try
			{
				_dataStream->readString(stringRead, _sizeInBytes);
				if(stringRead == _stringExpected)
					return;
			}
			catch(...)
			{
			}
			_dataStream->seekg(ofs);
		}
	}
	//-------------------------------------------------------------------------------------------------
	void EncodingSettings::readLineAndGuessEncoding(const DataStreamExPtr& _dataStream, const String& _lineExpected, EOL _eol)
	{
		setupStreamEncoding(_dataStream);

		size_t ofs = _dataStream->tellg();
		String lineRead;
		
		size_t index = -1;
		while(true)
		{
			if(index != -1)
			{
				if(index == sizeof(sEncodingsToGuess) / sizeof(sEncodingsToGuess[0]))
					GOTHOGRE_EXCEPT(_dataStream->getName() << 
						" - File corrupted, text '" << _lineExpected 
						<< "' not found.");

				_dataStream->setEncoding(sEncodingsToGuess[index]);
			}

			++index;

			try
			{
				_dataStream->readLine(lineRead, _eol);
				if(lineRead == _lineExpected)
					return;
			}
			catch(...)
			{
			}
			_dataStream->seekg(ofs);
		}		
	}


} // namespace GothOgre