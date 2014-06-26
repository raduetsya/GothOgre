#include "GothOgre_Precompiled.h"
#include "GothOgre_UnicodeSupport.h"
#include "GothOgre_CodepageConverter.h"

//-------------------------------------------------------------------------
GothOgre::UnicodeSupportRegistration* 
	Ogre::Singleton<GothOgre::UnicodeSupportRegistration>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------



namespace GothOgre
{
	//-------------------------------------------------------------------------
	// UnicodeSupport
	//-------------------------------------------------------------------------
	UnicodeSupport::UnicodeSupport()
	{
		UnicodeSupportRegistration::getSingleton().registerSupport(this);
	}
	//-------------------------------------------------------------------------
	UnicodeSupport::~UnicodeSupport()
	{
		UnicodeSupportRegistration::getSingleton().unregisterSupport(this);
		deleteSharedConverters();
	}
	//-----------------------------------------------------------------------------------
	bool UnicodeSupport::lessEncoding(const String& _first, const String& _second)
	{
		String tempString1, tempString2;
		size_t len1 = _first.length();
		size_t len2 = _second.length();
		size_t offset1 = 0;
		size_t offset2 = 0;
		while(offset1 != len1 && offset2 != len2)
		{
			String::value_type c1 = _first[offset1++];
			String::value_type c2 = _second[offset2++];
			if(isdigit(c1) && isdigit(c2))
			{
				size_t offset1_start = offset1 - 1;
				while(offset1 != len1 && isdigit(_first[offset1]))
					++offset1;
				
				size_t offset2_start = offset2 - 1;
				while(offset2 != len2 && isdigit(_second[offset2]))
					++offset2;

				tempString1.assign(&_first[offset1_start], offset1 - offset1_start);
				tempString2.assign(&_second[offset2_start], offset2 - offset2_start);
				ulong number1 = atol(tempString1.c_str());
				ulong number2 = atol(tempString2.c_str());
				
				if(number1 != number2)
					return number1 < number2;
			}
			if(c1 != c2)
				return c1 < c2;
		}
		return (offset1 == len1) && (offset2 != len2);
	}
	//-------------------------------------------------------------------------
	const StringVector& UnicodeSupport::getAllEncodings()
	{
		{
			OGRE_LOCK_RW_MUTEX_READ(mAllEncodingsMutex);
			if(!mAllEncodings.empty())
				return mAllEncodings;
		}

		{
			OGRE_LOCK_RW_MUTEX_WRITE(mAllEncodingsMutex);
			if(!mAllEncodings.empty())
				return mAllEncodings;

			buildListEncodings();
			std::sort(mAllEncodings.begin(), mAllEncodings.end(), lessEncoding);
			return mAllEncodings;
		}
	}
	//-------------------------------------------------------------------------
	void UnicodeSupport::addEncoding(const String& _encoding)
	{
		mAllEncodings.push_back(_encoding);
	}
	//-------------------------------------------------------------------------
	CodepageConverter* UnicodeSupport::openConverter(const String& _encoding)
	{
		CodepageConverter* converter = nullptr;
		if(!_encoding.empty())
		{
			{
				// Search a converter using a map of shared converters (mUConverterByEncoding)
				OGRE_LOCK_RW_MUTEX_READ(mConverterByEncodingMutex);
				ConverterByEncoding::iterator it = mConverterByEncoding.find(_encoding);
				if(it != mConverterByEncoding.end())
					converter = it->second;
			}
			
			if(!converter)
			{
				// If a converter not found by name then we must create a new converter.
				OGRE_LOCK_RW_MUTEX_WRITE(mConverterByEncodingMutex);
				ConverterByEncoding::iterator it = mConverterByEncoding.find(_encoding);
				if(it != mConverterByEncoding.end())
					converter = it->second;
				else
				{
					// Create a new converter ...
					converter = openConverterImpl(_encoding);
					if(!converter)
					{
						_throwEncodingNotFound(_encoding);
					}
					else
					{
						converter->_setUnicodeSupport(this);
						converter->_setName(_encoding);
						// append the new converter to the map of shared converters.
						it = mConverterByEncoding.insert(std::make_pair(_encoding, converter)).first;
					}
				}
			}
		}
		return converter;
	}
	//----------------------------------------------------------------------------
	void UnicodeSupport::deleteSharedConverters()
	{
		for(ConverterByEncoding::iterator it = mConverterByEncoding.begin();
			it != mConverterByEncoding.end(); ++it)
		{
			CodepageConverter* converter = it->second;
			delete converter;
		}
	}
	//-------------------------------------------------------------------------
	void UnicodeSupport::_throwEncodingNotFound(const String& _encoding)
	{
		GOTHOGRE_EXCEPT("Encoding '" << _encoding << "' not found.");
	}
	//-------------------------------------------------------------------------
	void UnicodeSupport::_throwCouldNotConvert(const String& _encodingTo, const String& _encodingFrom, const void* _data, size_t _size)
	{
		StrStream ss;
		ss << "Could not convert text to encoding '"
			<< _encodingTo << "' from '" << _encodingFrom 
			<< "'. Buffer (size = " << _size << "): ";

		size_t size_to_print = std::min(_size, (size_t)16);
		uint8* data8 = (uint8*) _data;
		ss.flags(std::ios_base::hex | std::ios_base::uppercase);
		ss.width(2);
		ss.fill('0');
		for(size_t i = 0; i != size_to_print; ++i)
			ss << "\\x" << (unsigned int) data8[i];

		if(size_to_print < _size)
			ss << "...";

		GOTHOGRE_EXCEPT(ss.str());
	}





	//-----------------------------------------------------------------------
	// UnicodeSupportRegistration
	//-----------------------------------------------------------------------
	UnicodeSupportRegistration::UnicodeSupportRegistration()
	{
		mSupport = nullptr;
	}
	//-----------------------------------------------------------------------
	UnicodeSupportRegistration::~UnicodeSupportRegistration()
	{
		if(mSupport)
			GOTHOGRE_ERROR("Unicode support is not unregistered before program exit.");
	}
	//-----------------------------------------------------------------------
	void UnicodeSupportRegistration::registerSupport(UnicodeSupport* _support)
	{
		if(mSupport && mSupport != _support)
			GOTHOGRE_ERROR("Unicode support is already registered.");
		mSupport = _support;
	}
	//-----------------------------------------------------------------------
	void UnicodeSupportRegistration::unregisterSupport(UnicodeSupport* _support)
	{
		if(mSupport == _support)
			mSupport = nullptr;
	}
	//-----------------------------------------------------------------------
	UnicodeSupport* UnicodeSupportRegistration::getSupport() const
	{
		GOTHOGRE_ASSERT(mSupport, "Unicode support is not registered.");
		return mSupport;
	}

} // namespace GothOgre