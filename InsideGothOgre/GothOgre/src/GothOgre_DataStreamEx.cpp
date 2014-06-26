#include "GothOgre_Precompiled.h"
#include "GothOgre_DataStreamEx.h"
#include "GothOgre_CodepageConverter.h"
#include "GothOgre_UnicodeUtil.h"
#include "GothOgre_AnyLite.h"


namespace GothOgre
{
	//----------------------------------------------------------------------------------------------
	// DataStreamAddon
	//----------------------------------------------------------------------------------------------
	struct DataStreamAddon
	{
		CodepageConverter*  codepageConverter;
		EOL                 defaultEOL;

		DataStreamAddon()
		{
			codepageConverter = nullptr;
			defaultEOL = EOL::CRLF;
		}
	};
	//----------------------------------------------------------------------------------------------
	const DataStreamAddon& getDataStreamAddon( const DataStreamEx* _dataStream )
	{
		const UserObjectBindings& uob = _dataStream->getUserObjectBindings();
		const DataStreamAddon* addon = uob.getUserAny().getPtr<DataStreamAddon>();
		if(!addon)
		{
			static const DataStreamAddon sEmpty;
			return sEmpty;
		}
		return *addon;
	}
	//----------------------------------------------------------------------------------------------
	DataStreamAddon& getDataStreamAddon( DataStreamEx* _dataStream )
	{
		UserObjectBindings& uob = _dataStream->getUserObjectBindings();
		DataStreamAddon* addon = uob.getUserAny().getPtr<DataStreamAddon>();
		if(!addon)
		{
			uob.setUserAny( AnyLite( DataStreamAddon() ) );
			addon = uob.getUserAny().getPtr<DataStreamAddon>();
		}
		return *addon;
	}




	//----------------------------------------------------------------------------------------------
	// DataStreamEx
	//----------------------------------------------------------------------------------------------
	DataStreamEx::DataStreamEx(uint16 _accessMode) 
		: DataStream(_accessMode) 
	{
		GOTHOGRE_ERROR("DataStreamEx::DataStreamEx never should be called!");
	}
	//----------------------------------------------------------------------------------------------
	DataStreamEx::DataStreamEx(const String& _name, uint16 _accessMode) 
		: DataStream(_name, _accessMode) 
	{
		GOTHOGRE_ERROR("DataStreamEx::DataStreamEx never should be called!");
	}
	//----------------------------------------------------------------------------------------------
	DataStreamEx::~DataStreamEx() 
	{
		GOTHOGRE_ERROR("DataStreamEx::~DataStreamEx never should be called!");
	}
	//----------------------------------------------------------------------------------------------
	void DataStreamEx::seekg(size_t _pos)
	{
		if(_pos > size())
		{
			GOTHOGRE_EXCEPT(getName() << " - Could not move file position to " << _pos);
		}
		seek(_pos);
	}
	//----------------------------------------------------------------------------------------------
	void DataStreamEx::readData(void* _data, size_t _dataSize)
	{
		if(_dataSize != 0)
		{
			if(read(_data, _dataSize) != _dataSize)
			{
				if(size() == 0)
				{
					GOTHOGRE_EXCEPT(getName() << " - File is empty.");
				}
				else
				{
					GOTHOGRE_EXCEPT(getName() << " - Unexpected end of file.");
				}
			}
		}
	}
	//----------------------------------------------------------------------------------------------
	void DataStreamEx::setEncoding(const String& _encoding)
	{
		getDataStreamAddon(this).codepageConverter 
			= UnicodeUtil::openConverter(_encoding);
	}
	//----------------------------------------------------------------------------------------------
	String DataStreamEx::getEncoding() const
	{
		CodepageConverter* codepageConverter = getDataStreamAddon(this).codepageConverter;
		return codepageConverter ? codepageConverter->getName() : StringUtil::BLANK;
	}
	//----------------------------------------------------------------------------------------------
	void DataStreamEx::setDefaultEOL(EOL _eol)
	{
		getDataStreamAddon(this).defaultEOL = _eol;
	}
	//----------------------------------------------------------------------------------------------
	EOL DataStreamEx::getDefaultEOL() const
	{
		return getDataStreamAddon(this).defaultEOL;
	}
	//----------------------------------------------------------------------------------------------
	void DataStreamEx::readString(String& _outString, size_t _sizeInBytes)
	{
		CodepageConverter* converter = getDataStreamAddon(this).codepageConverter;
		if(!converter)
			GOTHOGRE_EXCEPT(getName() << " - Encoding must be specified for a data stream before reading any string.");

		ByteBuffer tempBuffer(_sizeInBytes);
		readData(tempBuffer.data(), _sizeInBytes);

		_outString.clear();
		converter->bufferToUnicode(_outString, tempBuffer);
	}
	//----------------------------------------------------------------------------------------------
	String DataStreamEx::readString(size_t _sizeInBytes)
	{
		String str;
		readString(str, _sizeInBytes);
		return str;
	}
	//----------------------------------------------------------------------------------------------
	enum
	{
		// Size of one portion in bytes.
		// This size is used in DataStreamEx::readLine function 
		// and it must be DATASTREAMEX_PORTION_SIZE <= PREALLOC_BUFFER_SIZE
		DATASTREAMEX_PORTION_SIZE = 128
	};
	//----------------------------------------------------------------------------------------------
	void DataStreamEx::readLine(String& _outString, bool _trimSpaces)
	{
		readLine( _outString, getDefaultEOL(), _trimSpaces );
	}
	//----------------------------------------------------------------------------------------------
	void DataStreamEx::readLine(String& _outString, EOL::Enum _eol, bool _trimSpaces)
	{
		readLine(_outString, (EOL) _eol, _trimSpaces);
	}
	//----------------------------------------------------------------------------------------------
	void DataStreamEx::readLine(String& _outString, EOL _eol, bool _trimSpaces)
	{
		CodepageConverter* converter = getDataStreamAddon(this).codepageConverter;
		if(!converter)
			GOTHOGRE_EXCEPT(getName() << " - Encoding must be specified for a data stream before reading any string.");

		_outString.clear();
		ByteBuffer dataBuffer; // no read bytes yet
		size_t dataBufferOffset = 0;
		
		// No eol found yet
		bool found_EOL = false;

		bool found_CR = false; // used when _eol == CRLF

		// Keep looping while not hitting eol
		while(!found_EOL)
		{
			// Try to convert one character
			size_t oldDataBufferOffset = dataBufferOffset;
			converter->singleCharToUnicode(_outString, dataBuffer, dataBufferOffset);
			
			if(dataBufferOffset == oldDataBufferOffset)
			{
				// Read more characters from file if could not convert

				// Keep not converted bytes in buffer
				size_t remainSize = dataBuffer.size() - dataBufferOffset;
				if(remainSize)
				{
					memmove(dataBuffer.data(), dataBuffer.data() + dataBufferOffset, remainSize);
				}
				dataBufferOffset = 0;

				// Read next portion from the file
				dataBuffer.resize(DATASTREAMEX_PORTION_SIZE);
				size_t readCount = read(&dataBuffer[remainSize], DATASTREAMEX_PORTION_SIZE - remainSize);
				dataBuffer.resize(remainSize + readCount);

				// Try to convert one character again
				oldDataBufferOffset = dataBufferOffset;
				converter->singleCharToUnicode(_outString, dataBuffer, dataBufferOffset);
				
				// If it failed again (unexpected file end?) 
				// the best I can is only breaking this loop.
				if(dataBufferOffset == oldDataBufferOffset)
					break;
			}

			// Check read character whether it is a terminator
			String::value_type lastChar = _outString[_outString.length() - 1];
			switch(_eol)
			{
				case EOL::NULLCHAR:
				{
					if(lastChar == '\0') 
					{
						_outString.resize(_outString.size() - 1);
						found_EOL = true;
					}
					break;
				}
				case EOL::CR:
				{
					if(lastChar == '\r') 
					{
						_outString.resize(_outString.size() - 1);
						found_EOL = true;
					}
					break;
				}
				case EOL::LF:
				{
					if(lastChar == '\n') 
					{
						_outString.resize(_outString.size() - 1);
						found_EOL = true;
					}
					break;
				}
				case EOL::CRLF:
				{
					if(lastChar == '\r')
					{
						_outString.resize(_outString.size() - 1);
						if(found_CR)
						{
							// "\r\r" - double CR character hitted
							dataBufferOffset = oldDataBufferOffset;
							found_EOL = true;
						}
						else
						{
							found_CR = true;
						}
					}
					else if(lastChar == '\n')
					{
						_outString.resize(_outString.size() - 1);
						found_EOL = true;
					}
					break;
				}
				default:
					GOTHOGRE_EXCEPT("Unsupported EOL: " << (int) _eol);
			}
		}

		// Reposition backwards
		size_t remainSize = dataBuffer.size() - dataBufferOffset;
		if(remainSize != 0)
			skipg( -(long)remainSize );

		if(_trimSpaces)
			StringUtil::trim(_outString);
	}
	//----------------------------------------------------------------------------------------------
	String DataStreamEx::readLine(bool _trimSpaces)
	{
		return readLine( getDefaultEOL(), _trimSpaces );
	}
	//----------------------------------------------------------------------------------------------
	String DataStreamEx::readLine(EOL::Enum _eol, bool _trimSpaces)
	{
		return readLine( (EOL) _eol, _trimSpaces );
	}
	//----------------------------------------------------------------------------------------------
	String DataStreamEx::readLine(EOL _eol, bool _trimSpaces)
	{
		String line;
		readLine(line, _eol, _trimSpaces);
		return line;
	}
	//----------------------------------------------------------------------------------
	void DataStreamEx::skipg(long _count)
	{
		seekg(tellg() + _count);
	}
	//----------------------------------------------------------------------------------
	size_t DataStreamEx::tellg() const
	{
		return tell();
	}
	//----------------------------------------------------------------------------------------------
	void DataStreamEx::seekp(size_t _pos)
	{
		if(_pos > size())
		{
			GOTHOGRE_EXCEPT(getName() << " - Could not move file position to " << _pos);
		}
		seek(_pos);		
	}
	//----------------------------------------------------------------------------------------------
	size_t DataStreamEx::tellp() const
	{
		return tell();
	}
	//----------------------------------------------------------------------------------------------
	void DataStreamEx::writeData(const void* _data, size_t _dataSize)
	{
		if(write(_data, _dataSize) != _dataSize)
		{
			GOTHOGRE_EXCEPT(getName() << " - Could not write data to file.");
		}
	}
	//----------------------------------------------------------------------------------------------
	void DataStreamEx::prepareString(DataStreamEx::ByteBuffer& _rBuffer, const String& _value)
	{
		CodepageConverter* converter = getDataStreamAddon(this).codepageConverter;
		if(!converter)
			GOTHOGRE_EXCEPT(getName() << " - Encoding must be specified for a data stream before writing any string.");

		converter->unicodeToBuffer(_rBuffer, _value);
	}
	//----------------------------------------------------------------------------------------------
	void DataStreamEx::prepareLine(DataStreamEx::ByteBuffer& _rBuffer, const String& _value)
	{
		prepareLine(_rBuffer, _value, getDefaultEOL());
	}
	//----------------------------------------------------------------------------------------------
	void DataStreamEx::prepareLine(DataStreamEx::ByteBuffer& _rBuffer, const String& _value, EOL _eol)
	{
		CodepageConverter* converter = getDataStreamAddon(this).codepageConverter;
		if(!converter)
			GOTHOGRE_EXCEPT(getName() << " - Encoding must be specified for a data stream before writing any string.");
		
		converter->unicodeToBuffer(_rBuffer, _value);

		const String* pEOLStr;
		switch(_eol)
		{
			case EOL::CR:
			{
				static const String s("\r", 1); 
				pEOLStr = &s;
				break;
			}
			case EOL::LF:
			{
				static const String s("\n", 1); 
				pEOLStr = &s;
				break;
			}
			case EOL::CRLF:
			{
				static const String s("\r\n", 2); 
				pEOLStr = &s;
				break;
			}
			case EOL::NULLCHAR:
			{
				static const String s("\0", 1); 
				pEOLStr = &s;
				break;
			}
			default:
				GOTHOGRE_EXCEPT("Unsupported EOL: " << (int) _eol);
		}
		converter->unicodeToBuffer(_rBuffer, *pEOLStr);
	}
	//----------------------------------------------------------------------------------------------
	void DataStreamEx::writeString(const String& _value)
	{
		ByteBuffer buf;
		prepareString(buf, _value);
		writeBuffer(buf);
	}
	//----------------------------------------------------------------------------------------------
	void DataStreamEx::writeLine(const String& _value)
	{
		writeLine(_value, getDefaultEOL());
	}
	//----------------------------------------------------------------------------------------------
	void DataStreamEx::writeLine(const String& _value, EOL _eol)
	{
		ByteBuffer buf;
		prepareLine(buf, _value, _eol);
		writeBuffer(buf);
	}
	//----------------------------------------------------------------------------------------------
	void DataStreamEx::writeBuffer(const ByteBuffer& _buffer)
	{
		writeData(&_buffer[0], _buffer.size());
	}

} // namespace GothOgre