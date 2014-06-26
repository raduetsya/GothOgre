#include "GothOgre_Precompiled.h"
#include "GothOgre_CommonInclude.h"
#include "GothOgre_Assert.h"

namespace GothOgre
{
	AssertException::AssertException(const std::string& _description, const char* _file, long _line)
	{
		mDescription = _description;
		mFile = _file;
		mLine = _line;
		GOTHOGRE_LOG(Critical, _description);
	}
	//-------------------------------------------------------------------------------
	const std::string& AssertException::getFullDescription() const
	{
		if (mFullDesc.empty())
		{
			std::stringstream ss;
			if( mLine > 0 )
			{
				ss << "GothOgre::AssertException : " << mDescription << " at " << mFile << " (line " << mLine << ")";
			}
			else
			{
				ss << "GothOgre::AssertException : " << mDescription;
			}
			mFullDesc = ss.str();
		}
		return mFullDesc;		
	}


} // namespace GothOgre