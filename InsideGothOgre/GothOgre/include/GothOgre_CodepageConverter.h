#ifndef GOTHOGRE_CODEPAGE_CONVERTER_H
#define GOTHOGRE_CODEPAGE_CONVERTER_H

#include "GothOgre_Buffer.h"


namespace GothOgre
{
	class UnicodeSupport;


	//------------------------------------------------------------------------
	class GOTHOGRE_EXPORT CodepageConverter
	{
	public:
		typedef Buffer<uint8, 1024> ByteBuffer;

	public:
		CodepageConverter();
		virtual ~CodepageConverter();

		UnicodeSupport* getUnicodeSupport() const {return mUnicodeSupport;}
		const String& getName() const {return mName;}

		/** Converts a string from Unicode to any code page. 
		This function doesn't clear destination buffer, 
		the function appends data to it. */
		virtual void unicodeToBuffer(ByteBuffer& _destBuffer, const String& _srcString) = 0;

		/** Converts a string from any code page to Unicode. 
		This function doesn't clear destination string, 
		the function appends data to it. */
		virtual void bufferToUnicode(String& _destString, const ByteBuffer& _srcBuffer) = 0;

		/** Converts a single character from any code page to Unicode. 
		This function doesn't clear destination string, 
		the function appends data to it. */
		virtual void singleCharToUnicode(String& _destString, const ByteBuffer& _srcBuffer, size_t& _srcOffset) = 0;

	private:
		friend class UnicodeSupport;
		void _setUnicodeSupport(UnicodeSupport* _usupport);
		void _setName(const String& _name);

	private:
		UnicodeSupport*  mUnicodeSupport;
		String           mName;
	};

}

#endif // GOTHOGRE_CODEPAGE_CONVERTER_H