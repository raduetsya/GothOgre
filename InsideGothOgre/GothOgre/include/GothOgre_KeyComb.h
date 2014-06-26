#ifndef GOTHOGRE_KEY_COMB_H
#define GOTHOGRE_KEY_COMB_H

#include "GothOgre_KeyCode.h"
#include "GothOgre_MouseButton.h"

namespace GothOgre
{
	/** Class represents a combination of keys which must be pressed at the same time 
	to do some action. This is a list of values of types KeyCode and MouseButton.
	@remark
	The class supports reading from string and writing to string
	in readable form. 
	Examples: 
	"LMENU + X", 
	"LCONTROL + MOUSE_LEFT". 
	*/
	class GOTHOGRE_EXPORT KeyComb
	{
	public:
		/** Constructor */
		KeyComb() {}
		
		KeyComb(const KeyCode& _keyCode) 
		{
			add(_keyCode);
		}

		KeyComb(KeyCode::Enum _value)
		{
			add(KeyCode(_value));
		}

		KeyComb(const MouseButton& _mouseButton) 
		{
			add(_mouseButton);
		}

		KeyComb(MouseButton::Enum _value)
		{
			add(MouseButton(_value));
		}

		KeyComb(const KeyComb& _keyComb)
		{
			join(_keyComb);
		}

		KeyComb(const KeyComb& _keyComb1, const KeyComb& _keyComb2)
		{
			join(_keyComb1);
			join(_keyComb2);
		}

		KeyComb(const KeyComb& _keyComb1, const KeyComb& _keyComb2, const KeyComb& _keyComb3)
		{
			join(_keyComb1);
			join(_keyComb2);
			join(_keyComb3);
		}

		/** Assign operator */
		const KeyComb& operator =(const KeyComb& _keyComb)
		{
			clear();
			join(_keyComb);
			return *this;
		}

		/** Compare operator */
		bool operator ==(const KeyComb& _other) const;

		bool operator !=(const KeyComb& _other) const
		{
			return !(*this == _other);
		}

		/** Erases all of the key codes and mouse button identifiers. */
		void clear()
		{
			mKeyCodes.clear();
			mMouseButtons.clear();
		}

		/** Returns the number of key codes plus the number of mouse button identifiers */
		size_t size() const
		{
			return mKeyCodes.size() + mMouseButtons.size();
		}

		/** true if the key combination's size is 0. */
		bool empty() const
		{
			return mKeyCodes.empty() && mMouseButtons.empty();
		}

		/** Inserts a key code to the key combination. 
		@remark
		If the key combination already contains the specified key code
		then the function does nothing.
		*/
		void add(const KeyCode& _keyCode);

		/** Inserts a mouse button identifier to the key combination. 
		@remark
		If the key combination already contains the specified mouse button identifier
		then the function does nothing.
		*/
		void add(const MouseButton& _mouseButton);

		/** Removes a key code from the key combination.
		If the key combination does not contain the specified key code
		then the function does nothing. */
		void remove(const KeyCode& _keyCode);

		/** Removes a mouse button identifier from the key combination.
		If the key combination does not contain the specified mouse button identifier
		then the function does nothing. */
		void remove(const MouseButton& _mouseButton);

		/** Joins the current key combination and a specified one.
		Repeated values are ignored. */
		void join(const KeyComb& _keyComb);

		/** Returns the number of key codes */
		size_t getNumKeyCodes() const
		{
			return mKeyCodes.size();
		}

		/** Returns the key code by an index */
		KeyCode getKeyCode(size_t _index) const
		{
			return mKeyCodes[_index];
		}

		/** Returns an index of the key code or -1 if not found. */
		size_t findKeyCode(KeyCode _keyCode) const;

		/** Returns the number of mouse button identifiers */
		size_t getNumMouseButtons() const
		{
			return mMouseButtons.size();
		}

		/** Returns the mouse button identifier by an index */
		MouseButton getMouseButton(size_t _index) const
		{
			return mMouseButtons[_index];
		}

		/** Returns an index of the mouse button or -1 if not found. */
		size_t findMouseButton(MouseButton _mouseButton) const;

	private:
		vector<KeyCode>::type		mKeyCodes;
		vector<MouseButton>::type	mMouseButtons;
	};

	/** Converts to a string */
	GOTHOGRE_EXPORT StrStream& operator <<(StrStream& _ss, const KeyComb& _keyComb);

	/** Converts from a string */
	GOTHOGRE_EXPORT StrStream& operator >>(StrStream& _ss, KeyComb& _keyComb);

}


#endif // GOTHOGRE_KEY_COMB_H