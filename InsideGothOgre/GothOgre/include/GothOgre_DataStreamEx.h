#ifndef GOTHOGRE_DATA_STREAM_EX_H
#define GOTHOGRE_DATA_STREAM_EX_H

#include "GothOgre_EnumIO.h"
#include "GothOgre_CodepageConverter.h"


namespace GothOgre
{
	class EOL
	{
	public:
		enum Enum
		{
			CR,
			LF,
			CRLF,
			NULLCHAR
		};

		EOL(int _value = CRLF) : mValue(Enum(_value)) {}
		operator int() const {return mValue;}
		GOTHOGRE_DECLARE_ENUM_IO(EOL);
	private:
		Enum mValue;
	};


	//----------------------------------------------------------------------------------
	/** This class extends class DataStream. */
	class GOTHOGRE_EXPORT DataStreamEx : public DataStream
	{
	private:
		/// Never used constructor
		DataStreamEx(uint16 _accessMode = READ);
		
		/// Never used constructor
		DataStreamEx(const String& _name, uint16 _accessMode = READ);

		/// Never used destructor
		~DataStreamEx();

	public:
		/** Skips a defined number of bytes. 
		skipg(x) is the same as seekg(tellg() + x). */
		void skipg(long _count);

		/** Changes the current read position. */
		void seekg(size_t _pos);

		/** Returns the current read position. */
		size_t tellg() const;

		/** Changes the current write position. */
		void seekp(size_t _pos);

		/** Returns the current write position. */
		size_t tellp() const;

		/** Read binary data from data stream.
		The function throws exception if it's not possible. */
		void readData(void* _data, size_t _sizeInBytes);
		
		/** Read binary data from data stream.
		The function throws exception if it's not possible. */
		template<typename T>
		void readData(T& _data);

		/** Read the specified number of characters from data stream
		and return them as a string. */
 		String readString(size_t _sizeInBytes);
		void readString(String& _outString, size_t _sizeInBytes);

		/** Get a single line from the stream.
		@remarks
			The delimiter characters are not included in the data
			returned, and it is skipped over so the next read will occur
			after it.
        @note
            If you used this function, you must open the stream in binary mode,
            otherwise, it'll produce unexpected results.
		@param _dataStream Pointer to an opened data stream
		@param _delimiters Sequence of delimiter characters. Any of the characters can be used as delimiter.
		@param _trimSpaces If true, the line is trimmed for whitespace (as function 
		    String.trim(true,true) does). */
		String readLine(EOL _eol, bool _trimSpaces = true);
		String readLine(EOL::Enum _eol, bool _trimSpaces = true);
		String readLine(bool _trimSpaces = true);
		
		void readLine(String& _outString, EOL _eol, bool _trimSpaces = true);
		void readLine(String& _outString, EOL::Enum _eol, bool _trimSpaces = true);
		void readLine(String& _outString, bool _trimSpaces = true);

		/** Write binary data to data stream.
		The function throws exception if it's not possible. 
		@return number of written bytes (it always equals _sizeInBytes). */
		void writeData(const void* _data, size_t _sizeInBytes);
		
		/** Write binary data to data stream.
		The function throws exception if it's not possible.
		@return number of written bytes (it always equals sizeof(T). */
		template<typename T>
		void writeData(const T* _data);

		/** Write a string to the stream without a terminator.
		@param _value must be a string in UTF-8, but the string can be written to the stream
		in any code page (see DataStream::setCodePage).
		@return number of written bytes (not characters).	*/
		void writeString(const String& _value);
		
		/** Write a string to the stream with a terminator.
		@param _value must be a string in UTF-8, but the string can be written to the stream
		in any code page (see DataStream::setCodePage).
		@param _eol type of terminator which will be written to the stream.
		@return number of written bytes (not characters).	*/
		void writeLine(const String& _value, EOL _eol);
		void writeLine(const String& _value);

		typedef CodepageConverter::ByteBuffer ByteBuffer;
		void writeBuffer(const ByteBuffer& _buffer);
		
		void prepareString(ByteBuffer& _rBuffer, const String& _value);
		void prepareLine(ByteBuffer& _rBuffer, const String& _value, EOL _eol);
		void prepareLine(ByteBuffer& _rBuffer, const String& _value);

		/** Sets encoding which is used for writing strings and lines to the data stream. */
		void setEncoding(const String& _encoding);

		/** Returns the current encoding. */
		String getEncoding() const;

		/** Sets default end-of-line character.
		The default end-of-line character is used by the functions
		which read/write a line when these functions are called
		without spicification of end-of-line character in their arguments. */
		void setDefaultEOL(EOL _eol);

		/** Returns default end-of-line character. */
		EOL getDefaultEOL() const;
	};

	//----------------------------------------------------------------------------------
	template<typename T>
	void DataStreamEx::readData(T& _data)
	{
		readData( static_cast<void*>(&_data), sizeof(T));
	}
	//----------------------------------------------------------------------------------
	template<typename T>
	void DataStreamEx::writeData(const T* _data)
	{
		writeData( static_cast<const void*>(_data), sizeof(T));
	}




	//----------------------------------------------------------------------------------
	/** A shared pointer to an instance of the new class DataStreamEx. */
	class GOTHOGRE_EXPORT DataStreamExPtr : public DataStreamPtr
	{
	public:
		DataStreamExPtr() : DataStreamPtr() {}
		explicit DataStreamExPtr(DataStream* _rep) : DataStreamPtr(_rep) {}
		DataStreamExPtr(const DataStreamExPtr& _r) : DataStreamPtr(_r) {}
		DataStreamExPtr(const DataStreamPtr& _r) : DataStreamPtr(_r) {}

		/// Operator =
		DataStreamExPtr& operator =(const DataStreamExPtr& _r)
		{
			DataStreamPtr::operator =(_r);
			return *this;
		}

		DataStreamExPtr& operator =(const DataStreamPtr& _r)
		{
			DataStreamPtr::operator =(_r);
			return *this;
		}

		DataStreamEx& operator*() const 
		{ 
			return (DataStreamEx&) (DataStreamPtr::operator*()); 
		}

		DataStreamEx* operator->() const 
		{
			return (DataStreamEx*) (DataStreamPtr::operator->());
		}

		DataStreamEx* get() const 
		{ 
			return (DataStreamEx*) (DataStreamPtr::get());
		}

		DataStreamEx* getPointer() const
		{
			return (DataStreamEx*) DataStreamPtr::getPointer();
		}

		// Implicit conversion to bool
		// to make valid the following code:
		// if(p) {...} where p is a smart pointer
		static void unspecified_bool(DataStreamExPtr***)
		{
		}
		typedef void (*unspecified_bool_type)(DataStreamExPtr***);
		operator unspecified_bool_type() const
		{
			return isNull() ? 0: unspecified_bool;
		}
	};

} // namespace GothOgre

#endif // GOTHOGRE_DATA_STREAM_EX_H