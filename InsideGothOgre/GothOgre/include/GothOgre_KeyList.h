#ifndef GOTHOGRE_KEY_LIST_H
#define GOTHOGRE_KEY_LIST_H

#include "GothOgre_KeyComb.h"

namespace GothOgre
{
	/** Class represents a list of alternative KeyCombs (combinations of keys).
	Any of these combination can be used to do some action.
	@remark
	The class supports reading from string and writing to string
	in readable form. 
	Example: 
	"LMENU + X | ESCAPE | F10". 
	*/
	class GOTHOGRE_EXPORT KeyList
	{
	public:
		/** Constructor */
		KeyList() {}
		
		KeyList(const KeyCode& _keyCode) 
		{
			add(_keyCode);
		}
		
		KeyList(KeyCode::Enum _value)
		{
			add(KeyCode(_value));
		}

		KeyList(const MouseButton& _mouseButton) 
		{
			add(_mouseButton);
		}

		KeyList(MouseButton::Enum _value)
		{
			add(MouseButton(_value));
		}

		KeyList(const KeyComb& _keyComb) 
		{
			add(_keyComb);
		}

		KeyList(const KeyList& _keyList) 
		{
			join(_keyList);
		}

		KeyList(const KeyList& _keyList1, const KeyList& _keyList2) 
		{
			join(_keyList1);
			join(_keyList2);
		}

		/** Assign operator */
		const KeyList& operator =(const KeyList& _keyList)
		{
			clear();
			join(_keyList);
			return *this;
		}

		/** Compare operator */
		bool operator ==(const KeyList& _other) const;

		bool operator !=(const KeyList& _other) const
		{
			return !(*this == _other);
		}

		/** Erases all of the key codes and mouse button identifiers. */
		void clear()
		{
			mKeyCombs.clear();
		}

		/** Returns the number of key combinations */
		size_t size() const
		{
			return mKeyCombs.size();
		}

		/** true if the key list's size is 0. */
		bool empty() const
		{
			return mKeyCombs.empty();
		}

		/** Inserts a key combination to the key list's size. 
		@remark
		If the key list already contains the specified key combination
		then the function does nothing.
		*/
		void add(const KeyComb& _keyComb);

		/** Removes a key combination from the key list.
		If the key list does not contain the specified key combination
		then the function does nothing. */
		void remove(const KeyComb& _keyComb);

		/** Joins the current key list and specified one.
		Repeated values are ignored. */
		void join(const KeyList& _keyList);

		/** Returns the number of key combinations */
		size_t getNumKeyCombs() const
		{
			return mKeyCombs.size();
		}

		/** Returns the key code by an index */
		const KeyComb& getKeyComb(size_t _index) const
		{
			return mKeyCombs[_index];
		}

		const KeyComb& operator[](size_t _index) const
		{
			return getKeyComb(_index);
		}

	private:
		vector<KeyComb>::type	 mKeyCombs;
	};

	/** Converts to a string */
	GOTHOGRE_EXPORT StrStream& operator <<(StrStream& _ss, const KeyList& _keyList);

	/** Converts from a string */
	GOTHOGRE_EXPORT StrStream& operator >>(StrStream& _ss, KeyList& _keyList);

}


#endif // GOTHOGRE_KEY_LIST_H