#ifndef GOTHOGRE_STR_STREAM_H
#define GOTHOGRE_STR_STREAM_H

#ifndef GOTHOGRE_USE_OWN_STRING_STREAM
#define GOTHOGRE_USE_OWN_STRING_STREAM 1
#endif


namespace GothOgre
{
#if GOTHOGRE_USE_OWN_STRING_STREAM == 1

	//-----------------------------------------------------------------------------------------------
	// StrStreamBuffer - helper class
	//-----------------------------------------------------------------------------------------------
	class GOTHOGRE_EXPORT StrStreamBuffer : public std::basic_stringbuf<String::value_type, String::traits_type, String::allocator_type>
	{
	public:
		typedef String::value_type  char_type;
		typedef String::traits_type traits_type;
		typedef String::allocator_type allocator_type;

		StrStreamBuffer()
			: mStringPtr(&mString) 
		{
		}

		StrStreamBuffer(const String& _str, bool _useCopy = true)
			: mStringPtr(&mString) 
		{
			str(_str, _useCopy);
		}

		~StrStreamBuffer()
		{
		}

		/** Gets the string content. */
		const String& str() const
		{
			return *mStringPtr;
		}

		/** Sets the string content. */
		void str(const String& _str, bool _useCopy = true);
		
		/** Attempts to write c to the current position of the put pointer pptr, 
		and advances that pointer one position forward. */
		int_type overflow(int_type _c = traits_type::eof());

		/** Put character back. */
		int_type pbackfail(int_type _c = traits_type::eof());

		/** Returns the character at the current get position, 
		if one is available, otherwise it returns EOF. */
		int_type underflow();

		/** Sets the relative new position _ptr for the position pointers specified by parameter which. */
		pos_type seekoff(off_type _off,	std::ios_base::seekdir _way,
			std::ios_base::openmode _which = std::ios_base::in | std::ios_base::out);

		/** Sets the absolute new position _ptr for the position pointers specified by parameter which. */
		pos_type seekpos(pos_type _ptr, 
			std::ios_base::openmode _which = std::ios_base::in | std::ios_base::out);

	private:
		String   mString;
		String*  mStringPtr;
	};



	//-----------------------------------------------------------------------------------------------
	// StrStream - the class is designed as replacement for slow the StringStream class.
	//-----------------------------------------------------------------------------------------------
	class GOTHOGRE_EXPORT StrStream : public std::basic_iostream<String::value_type, String::traits_type>
	{
	public:
		typedef String::value_type  char_type;
		typedef String::traits_type traits_type;
		typedef String::allocator_type allocator_type;
		typedef traits_type::int_type int_type;
		typedef traits_type::pos_type pos_type;
		typedef traits_type::off_type off_type;

		enum CopyFlag
		{
			DONT_COPY,
			COPY
		};

		/** Constructor. */
		StrStream()
			: std::basic_iostream<String::value_type, String::traits_type>(&mBuffer)
		{
		}

		/** Constructor with initial string. */
		StrStream(const String& _str, CopyFlag _copyFlag = COPY)
			: std::basic_iostream<String::value_type, String::traits_type>(&mBuffer),
   			  mBuffer(_str, _copyFlag == COPY)
		{
		}

		/** Destructor. */
		~StrStream()
		{
		}

		/** Sets the associated string object. */
		void str(const String& _str, CopyFlag _copyFlag = COPY)
		{
			mBuffer.str(_str, _copyFlag == COPY);
		}
		
		/** Gets the associated string object. */
		const String& str() const
		{
			return mBuffer.str();
		}

		/** Returns a pointer to the stringbuf object associated with the stream. */
		std::basic_stringbuf<char_type, traits_type, allocator_type>* rdbuf() const
		{
			return (std::basic_stringbuf<char_type, traits_type, allocator_type>*) &mBuffer;
		}

	private:
		StrStreamBuffer mBuffer;
	};

#else // GOTHOGRE_USE_OWN_STRING_STREAM

	// Simplified (and slow) implementation (via Ogre::StringStream)
	class StrStream : public StringStream
	{
	public:
		enum CopyFlag
		{
			DONT_COPY,
			COPY
		};

		StrStream() {}
		StrStream(const String& _str, CopyFlag _copyFlag = COPY) : StringStream(_str) {}

		String str() const {return StringStream::str();}
		void str(const String& _str, CopyFlag _copyFlag = COPY) {StringStream::str(_str);}
	};

#endif // GOTHOGRE_USE_OWN_STRING_STREAM

} // namespace 

#endif // GOTHOGRE_STR_STREAM_H