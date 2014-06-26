#ifndef STRING_HOLDER_H
#define STRING_HOLDER_H

#include "StringHolder_ICUDefs.h"


namespace GothOgre
{
	//----------------------------------------------------------------------------
	// char_cast is a template function to convert pointers to characters
	// Example of using: 
	// const uint8* p = &a;
	// char* p2 = char_cast<char*>(p);
	template<typename TO, typename FROM>
	TO char_cast(FROM _ptr)
	{
		TO errorMaker(_ptr);
		return errorMaker;
	}

	template<>
	inline char8a* char_cast<char8a*, char8*>(char8* _ptr)
	{
		return (char8a*) _ptr;
	}

	template<>
	inline char8* char_cast<char8*, char8a*>(char8a* _ptr)
	{
		return (char8*) _ptr;
	}

	template<>
	inline char16a* char_cast<char16a*, char16*>(char16* _ptr)
	{
		return (char16a*) _ptr;
	}

	template<>
	inline char16* char_cast<char16*, char16a*>(char16a* _ptr)
	{
		return (char16*) _ptr;
	}

	template<>
	inline const char8a* char_cast<const char8a*, const char8*>(const char8* _ptr)
	{
		return (const char8a*) _ptr;
	}

	template<>
	inline const char8* char_cast<const char8*, const char8a*>(const char8a* _ptr)
	{
		return (char8*) _ptr;
	}

	template<>
	inline const char16a* char_cast<const char16a*, const char16*>(const char16* _ptr)
	{
		return (const char16a*) _ptr;
	}

	template<>
	inline const char16* char_cast<const char16*, const char16a*>(const char16a* _ptr)
	{
		return (char16*) _ptr;
	}




	//----------------------------------------------------------------------------
	// StringHolder and ConstStringHolder is template classes
	// to use instead of "String&" and "const String&"
	template<int CharSize>
	struct StringHolderTypedefs
	{
		typedef float value_type;
	};

	template<>
	struct StringHolderTypedefs<1>
	{
		typedef char8 value_type;
	};

	template<>
	struct StringHolderTypedefs<2>
	{
		typedef char16 value_type;
	};
	//----------------------------------------------------------------------------
	template<int CharSize>
	class ConstStringHolder : public StringHolderTypedefs<CharSize>
	{
	public:
		ConstStringHolder(const String& _str);
		const String& get() const;
		bool empty() const;
		size_type length() const;
		size_type size() const;
		value_type operator[](size_type _index) const;
		const value_type* c_str() const;
	};
	//----------------------------------------------------------------------------
	template<>
	class ConstStringHolder<sizeof(String::value_type)> : public StringHolderTypedefs<sizeof(String::value_type)>
	{
	public:
		ConstStringHolder(const String& _str) : mStrRef(_str) {}
		const String& get() const {return mStrRef;}
		bool empty() const {return mStrRef.empty();}
		size_type length() const {return mStrRef.length();}
		size_type size() const {return mStrRef.size();}
		value_type operator[](size_type _index) const {return mStrRef[_index];}
		const value_type* c_str() const {return char_cast<const value_type*>( mStrRef.c_str());}

	private:
		const String& mStrRef;
	};
	//----------------------------------------------------------------------------
	template<int CharSize>
	class StringHolder : public StringHolderTypedefs<CharSize>
	{
	public:
		StringHolder(String& _str);
		String& get();
		bool empty() const;
		size_type length() const;
		size_type size() const;
		value_type& operator[](size_type _index);
		void assign(const value_type* _p);
		void assign(const value_type* _p, size_type _n);
		void append(const value_type* _p);
		void append(const value_type* _p, size_type _n);
	};
	//---------------------------------------------------------------------------
	template<>
	class StringHolder<sizeof(String::value_type)> : public StringHolderTypedefs<sizeof(String::value_type)>
	{
	public:
		StringHolder(String& _str) : mStrRef(_str) {}
		String& get() {return mStrRef;}
		bool empty() const {return mStrRef.empty();}
		size_type length() const {return mStrRef.length();}
		size_type size() const {return mStrRef.size();}
		value_type& operator[](size_type _index) {return *char_cast<value_type*>( &mStrRef[_index] );}
		void assign(const value_type* _p) {mStrRef.assign( char_cast<const String::value_type*>(_p));}
		void assign(const value_type* _p, size_type _n) {mStrRef.assign( char_cast<const String::value_type*>(_p), _n);}
		void append(const value_type* _p) {mStrRef.append( char_cast<const String::value_type*>(_p));}
		void append(const value_type* _p, size_type _n) {mStrRef.append( char_cast<const String::value_type*>(_p), _n);}

	private:
		String& mStrRef;
	};
}

#endif // STRING_HOLDER_H