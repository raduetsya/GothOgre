#include "GothOgre_Precompiled.h"
#include "GothOgre_Exception.h"

namespace GothOgre
{
	Exception::Exception()
	{
		mLine = 0;
	}
	//-------------------------------------------------------------------------------
	Exception::Exception(const String& _description, const String& _module, const char* _file, long _line)
	{
		mDescription = _description;
		mModule = _module;
		mFile = _file;
		mLine = _line;
	}
	//-------------------------------------------------------------------------------
	Exception::Exception(const Exception& _src)
	{
		*this = _src;
	}
	//-------------------------------------------------------------------------------
	Exception& Exception::operator =(const Exception& _src)
	{
		mDescription = _src.mDescription;
		mLine = _src.mLine;
		mFile = _src.mFile;
		mModule = _src.mModule;
		mFullDesc = _src.mFullDesc;
		return *this;
	}
	//-------------------------------------------------------------------------------
	const String& Exception::getFullDescription() const
	{
		if(mFullDesc.empty())
		{
			StrStream ss;
			ss << "GothOgre EXCEPTION : " << mDescription << std::endl << "In " << mModule;
			if ( mLine > 0 )
			{
				 ss << " at " << mFile << " (line " << mLine << ")";
			}
			const_cast<Exception*>(this)->mFullDesc = ss.str();
		}
		return mFullDesc;
	}
	//-------------------------------------------------------------------------------
	const char* Exception::what() const
	{
		return getFullDescription().c_str();
	}

} // namespace GothOgre