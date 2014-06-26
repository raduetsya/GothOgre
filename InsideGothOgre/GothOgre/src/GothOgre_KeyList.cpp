#include "GothOgre_Precompiled.h"
#include "GothOgre_KeyList.h"


namespace GothOgre
{
	void KeyList::add(const KeyComb& _keyComb)
	{
		if(std::find(mKeyCombs.begin(), mKeyCombs.end(), _keyComb) 
			== mKeyCombs.end())
		{
			mKeyCombs.push_back(_keyComb);
		}
	}
	//-----------------------------------------------------------------------------------	
	void KeyList::remove(const KeyComb& _keyComb)
	{
		vector<KeyComb>::type::iterator it = std::find(mKeyCombs.begin(), mKeyCombs.end(), _keyComb);
		if(it != mKeyCombs.end())
			mKeyCombs.erase(it);
	}
	//-----------------------------------------------------------------------------------	
	void KeyList::join(const KeyList& _keyList)
	{
		for(size_t i = 0; i != _keyList.size(); ++i)
			add(_keyList[i]);
	}
	//-----------------------------------------------------------------------------------	
	bool KeyList::operator ==(const KeyList& _other) const
	{
		if(mKeyCombs.size() != _other.mKeyCombs.size())
			return false;

		for(size_t i = 0; i != mKeyCombs.size(); ++i)
			if(std::find(_other.mKeyCombs.begin(), _other.mKeyCombs.end(), mKeyCombs[i])
				== _other.mKeyCombs.end())
				return false;

		return true;
	}
	//-----------------------------------------------------------------------------------	
	StrStream& operator <<(StrStream& _ss, const KeyList& _keyList)
	{
		// write key codes, separating with "; "
		for(size_t i = 0; i < _keyList.getNumKeyCombs(); ++i)
		{
			if(i != 0)
				_ss << "; ";
			_ss << _keyList.getKeyComb(i);
		}
		return _ss;
	}
	//-----------------------------------------------------------------------------------	
	StrStream& operator >>(StrStream& _ss, KeyList& _keyList)
	{
		if(_ss.fail())
			return _ss;

		_keyList.clear();
		while(!_ss.eof())
		{
			// skip spaces and semicolons (' ' and ';' characters)
			int c;
			while((c = _ss.peek()) == ' ' || c == ';')
				_ss.ignore(1);
			
			size_t pos = _ss.tellg();
			// try to read keycomb
			KeyComb keyComb;
			_ss >> keyComb;
			if(_ss.fail())
			{
				_ss.clear();
				_ss.seekg(pos);
				break;
			}
			_keyList.add(keyComb);
		}

		// if the code above wasn't able to read something
		// then we set error state of the input stream
		if(_keyList.empty())
		{
			_ss.seekg(-1); // set error state
		}
		return _ss;
	}
}