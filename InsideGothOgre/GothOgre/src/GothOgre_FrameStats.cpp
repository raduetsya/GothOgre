#include "GothOgre_Precompiled.h"
#include "GothOgre_StandardFrameStatsValues.h"
#include "GothOgre_FrameStats.h"

//-------------------------------------------------------------------------
GothOgre::FrameStats* 
	Ogre::Singleton<GothOgre::FrameStats>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------

namespace GothOgre
{
	FrameStatsValue::FrameStatsValue(const String& _key, bool _addToDefaultTextTemplate)
	{
		mKey = _key;
		FrameStats::getSingleton().addFrameStatsValue(this, _addToDefaultTextTemplate);
	}

	FrameStatsValue::~FrameStatsValue()
	{
		FrameStats::getSingleton().removeFrameStatsValue(this);
	}
	//-------------------------------------------------------------------------------------------------
	FrameStats::FrameStats()
	{
		mStandardValues = new StandardFrameStatsValues();
	}
	//-------------------------------------------------------------------------------------------------
	FrameStats::~FrameStats()
	{
		delete mStandardValues;
	}
	//-------------------------------------------------------------------------------------------------
	void FrameStats::addFrameStatsValue(FrameStatsValue* _value, bool _addToDefaultTextTemplate)
	{
		const String& key = _value->getKey();
		KeyValueMap::iterator it = mKeyValueMap.find(key);
		if(it == mKeyValueMap.end())
		{
			mKeyValueMap.insert( KeyValuePair(key, _value) );
		}
		if(_addToDefaultTextTemplate)
		{
			String tt = getDefaultTextTemplate();
			if(!tt.empty()) tt += "\n";
			tt += key;
			tt += ": <";
			tt += key;
			tt += ">";
			setDefaultTextTemplate(tt);
		}
	}
	//-------------------------------------------------------------------------------------------------
	void FrameStats::removeFrameStatsValue(FrameStatsValue* _value)
	{
		const String& key = _value->getKey();
		KeyValueMap::iterator it = mKeyValueMap.find(key);
		if(it != mKeyValueMap.end())
		{
			mKeyValueMap.erase(it);
		}
	}
	//-------------------------------------------------------------------------------------------------
	String FrameStats::getValue(const String& _key) const
	{
		KeyValueMap::const_iterator it = mKeyValueMap.find(_key);
		if(it != mKeyValueMap.end())
		{
			FrameStatsValue* value = it->second;
			return value->getValue();
		}
		else
		{
			return StringUtil::BLANK;
		}
	}
	//-------------------------------------------------------------------------------------------------
	String FrameStats::getText(const String& _textTemplate) const
	{
		return getText(_textTemplate, StringUtil::BLANK, StringUtil::BLANK);
	}
	//-------------------------------------------------------------------------------------------------
	String FrameStats::getText(const String& _textTemplate, const String& _fmtValueBegin, const String& _fmtValueEnd) const
	{
		String ret;
		String curKey;
		size_t i = 0;
		while(i != _textTemplate.length())
		{
			size_t j = _textTemplate.find('<', i);
			size_t k = (j != String::npos) ? _textTemplate.find('>', j + 1) : String::npos;
			if(j != String::npos && k != String::npos)
			{
				curKey = _textTemplate.substr(j + 1, k - j - 1);
				ret += _textTemplate.substr(i, j - i);
				ret += _fmtValueBegin;
				ret += getValue(curKey);
				ret += _fmtValueEnd;
				i = k + 1;
			}
			else
			{
				ret += _textTemplate.substr(i);
				break;
			}
		}
		return ret;
	}

} // namespace GothOgre