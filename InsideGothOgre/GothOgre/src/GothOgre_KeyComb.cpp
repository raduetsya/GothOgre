#include "GothOgre_Precompiled.h"
#include "GothOgre_KeyComb.h"


namespace GothOgre
{
	size_t KeyComb::findKeyCode(KeyCode _keyCode) const
	{
		for(size_t i = 0; i != mKeyCodes.size(); ++i)
			if(mKeyCodes[i] == _keyCode)
				return i;
		return -1;
	}
	//-----------------------------------------------------------------------------------	
	size_t KeyComb::findMouseButton(MouseButton _mouseButton) const
	{
		for(size_t i = 0; i != mMouseButtons.size(); ++i)
			if(mMouseButtons[i] == _mouseButton)
				return i;
		return -1;
	}
	//-----------------------------------------------------------------------------------	
	void KeyComb::add(const KeyCode& _keyCode)
	{
		if(findKeyCode(_keyCode) == -1 && _keyCode != KeyCode::NONE)
		{
			mKeyCodes.push_back(_keyCode);
		}
	}
	//-----------------------------------------------------------------------------------	
	void KeyComb::add(const MouseButton& _mouseButton)
	{
		if(findMouseButton(_mouseButton) == -1 && _mouseButton != MouseButton::NONE)
		{
			mMouseButtons.push_back(_mouseButton);
		}
	}
	//-----------------------------------------------------------------------------------	
	void KeyComb::remove(const KeyCode& _keyCode)
	{
		vector<KeyCode>::type::iterator it = std::find(mKeyCodes.begin(), mKeyCodes.end(), _keyCode);
		if(it != mKeyCodes.end())
			mKeyCodes.erase(it);
	}
	//-----------------------------------------------------------------------------------	
	void KeyComb::remove(const MouseButton& _mouseButton)
	{
		vector<MouseButton>::type::iterator it = std::find(mMouseButtons.begin(), mMouseButtons.end(), _mouseButton);
		if(it != mMouseButtons.end())
			mMouseButtons.erase(it);
	}
	//-----------------------------------------------------------------------------------	
	void KeyComb::join(const KeyComb& _keyComb)
	{
		for(size_t k = 0; k != _keyComb.getNumKeyCodes(); ++k)
			add(_keyComb.getKeyCode(k));
		
		for(size_t m = 0; m != _keyComb.getNumMouseButtons(); ++m)
			add(_keyComb.getMouseButton(m));
	}
	//-----------------------------------------------------------------------------------	
	bool KeyComb::operator ==(const KeyComb& _other) const
	{
		if(mKeyCodes.size() != _other.mKeyCodes.size()
			|| mMouseButtons.size() != _other.mMouseButtons.size())
			return false;

		for(size_t i = 0; i != mKeyCodes.size(); ++i)
			if(_other.findKeyCode(mKeyCodes[i]) == -1)
				return false;

		for(size_t i = 0; i != mMouseButtons.size(); ++i)
			if(_other.findMouseButton(mMouseButtons[i]) == -1)
				return false;

		return true;
	}
	//-----------------------------------------------------------------------------------	
	StrStream& operator <<(StrStream& _ss, const KeyComb& _keyComb)
	{
		// write key codes, separating with " + "
		for(size_t i = 0; i < _keyComb.getNumKeyCodes(); ++i)
		{
			if(i != 0)
				_ss << " + ";
			_ss << _keyComb.getKeyCode(i);
		}

		// write mouse button identifiers, separating with " + "
		for(size_t j = 0; j < _keyComb.getNumMouseButtons(); ++j)
		{
			if(j != 0)
				_ss << " + ";
			_ss << "Mouse_";
			_ss << _keyComb.getMouseButton(j);
		}
		return _ss;
	}
	//-----------------------------------------------------------------------------------	
	StrStream& operator >>(StrStream& _ss, KeyComb& _keyComb)
	{
		if(_ss.fail())
			return _ss;

		_keyComb.clear();
		while(!_ss.eof())
		{
			// skip spaces and pluses (' ' and '+' characters)
			int c;
			while((c = _ss.peek()) == ' ' || c == '+')
				_ss.ignore(1);
			
			size_t pos = _ss.tellg();
			String::value_type buf[6];
			_ss.get(buf, 6);
			if(_ss.gcount() == 6 && strnicmp(buf, "Mouse_", 6) == 0)
			{
				// try to read mouse button code
				MouseButton mouseButton;
				_ss >> mouseButton;
				if(_ss.fail())
				{
					_ss.clear();
					_ss.seekg(pos);
					break;
				}
				_keyComb.add(mouseButton);
			}
			else
			{
				// try to read key code
				_ss.clear();
				_ss.seekg(pos);
				KeyCode keyCode;
				_ss >> keyCode;
				if(_ss.fail())
				{
					_ss.clear();
					_ss.seekg(pos);
					break;
				}
				_keyComb.add(keyCode);
			}
		}

		// if the code above wasn't able to read something
		// then we set error state of the input stream
		if(_keyComb.empty())
		{
			_ss.seekg(-1); // set error state
		}
		return _ss;
	}
}