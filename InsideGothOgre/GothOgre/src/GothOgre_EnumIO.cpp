#include "GothOgre_Precompiled.h"
#include "GothOgre_EnumIO.h"


namespace GothOgre
{
	//---------------------------------------------------------------------------
	void EnumUtil::output(StrStream& _ss, int _value, const ValueNameMap& _valueNameMap)
	{
		ValueNameMap::const_iterator it = _valueNameMap.find(_value);
		if(it != _valueNameMap.end())
			_ss << it->second;
		else
			_ss << _value;
	}
	//---------------------------------------------------------------------------
	void EnumUtil::input(StrStream& _ss, int& _value, const NameValueMap& _nameValueMap)
	{
		if(_ss.fail())
			return;

		// Skip spaces
		int c;
		while((c = _ss.peek()) == ' ')
			_ss.ignore(1);
		
		if(isdigit(c))
		{
			// a number, not a name
			_ss >> _value;
			return;
		}
		
		String tempStr;
		if(c == '\'' || c == '\"')
		{
			// Name is delimited by quotes
			int delim = c;
			int numDelims = 0;
			while(true)
			{
				c = _ss.get();
				if(!_ss.gcount())
				{
					_ss.clear(std::ios_base::eofbit); // eofbit could raise failbit too, clearing
					break;
				}
				tempStr.push_back(c);
				if(c == delim)
				{
					if(++numDelims == 2)
						break;
				}
			}
		}
		else
		{
			// Name is a string containing letters, digits, and underscores
			while(true)
			{
				c = _ss.get();
				if(!_ss.gcount())
				{
					_ss.clear(std::ios_base::eofbit); // eofbit could raise failbit too, clearing
					break;
				}
				if(isalnum(c) || c == '_')
				{
					tempStr.push_back(c);
				}
				else
				{
					_ss.unget();
					break;
				}
			}
		}

		// Searching the read name in the map
		NameValueMap::const_iterator it = _nameValueMap.find(tempStr);
		if(it == _nameValueMap.end())
			_ss.seekg(-1); // set error state
		else
			_value = it->second;
	}

} // namespace GothOgre