#ifndef GOTHOGRE_ASSERT_H
#define GOTHOGRE_ASSERT_H

#include "GothOgre_Log.h"

namespace GothOgre
{
	class GOTHOGRE_EXPORT AssertException
	{
	public:
		AssertException(const std::string& _description, const char* _file, long _line);

		const std::string& getDescription() const {return mDescription;}
		const std::string& getFile() const {return mFile;}
		long getLine() const {return mLine;}
		const std::string& getFullDescription() const;

	private:
		std::string			mDescription;
		std::string			mFile;
		long				mLine;
		mutable std::string mFullDesc;
	};

} // namespace GothOgre

#define GOTHOGRE_EXCEPT(description) throw GothOgre::AssertException((BaseLog::LogStream() << description).str(), __FILE__, __LINE__)

#define GOTHOGRE_ASSERT(expr, description) do { if( !(expr) ) GOTHOGRE_EXCEPT( description ); } while(false)

#endif // GOTHOGRE_ASSERT_H