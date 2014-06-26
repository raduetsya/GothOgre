#ifndef WIN_CODEPAGE_CONVERTER_H
#define WIN_CODEPAGE_CONVERTER_H

#include "StringHolder.h"
#include "UtfBuffers.h"

namespace GothOgre
{

	class WinCodepageConverter : public CodepageConverter
	{
	public:
		WinCodepageConverter(UINT _codepage);
		virtual ~WinCodepageConverter();

		/** Converts a string from Unicode to any code page. 
		This function doesn't clear destination buffer, 
		the function appends data to it. */
		virtual void unicodeToBuffer(ByteBuffer& _destBuffer, const String& _srcString);

		/** Converts a string from any code page to Unicode. 
		This function doesn't clear destination string, 
		the function appends data to it. */
		virtual void bufferToUnicode(String& _destString, const ByteBuffer& _srcBuffer);

		/** Converts a single character from any code page to Unicode. 
		This function doesn't clear destination string, 
		the function appends data to it. */
		virtual void singleCharToUnicode(String& _destString, const ByteBuffer& _srcBuffer, size_t& _srcOffset);

	private:
		template<int CharSize>
		void _unicodeToBuffer(ByteBuffer& _destBuffer, ConstStringHolder<CharSize> _srcString);

		template<>
		void _unicodeToBuffer<1>(ByteBuffer& _destBuffer, ConstStringHolder<1> _srcString);

		template<>
		void _unicodeToBuffer<2>(ByteBuffer& _destBuffer, ConstStringHolder<2> _srcString);


		template<int CharSize>
		void _bufferToUnicode(StringHolder<CharSize> _destString, const ByteBuffer& _srcBuffer);

		template<>
		void _bufferToUnicode<1>(StringHolder<1> _destString, const ByteBuffer& _srcBuffer);

		template<>
		void _bufferToUnicode<2>(StringHolder<2> _destString, const ByteBuffer& _srcBuffer);


		template<int CharSize>
		void _singleCharToUnicode(StringHolder<CharSize> _destString, const ByteBuffer& _srcBuffer, size_t& _srcOffset);

		template<>
		void _singleCharToUnicode<1>(StringHolder<1> _destString, const ByteBuffer& _srcBuffer, size_t& _srcOffset);

		template<>
		void _singleCharToUnicode<2>(StringHolder<2> _destString, const ByteBuffer& _srcBuffer, size_t& _srcOffset);

	private:
		UINT getCodepage() const {return mCodepage;}

	private:
		UINT    mCodepage;
		String  mName;
	};

}

#endif // WIN_CODEPAGE_CONVERTER_H