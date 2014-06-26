#ifndef GOTHOGRE_LOG_SYSTEM_H
#define GOTHOGRE_LOG_SYSTEM_H

#include "GothOgre_EnumIO.h"

// Remove the "ERROR" macro (could be defined in WinGDI.h)
#ifdef ERROR
#undef ERROR
#endif 

namespace GothOgre
{
	class GOTHOGRE_EXPORT LogLevel
	{
	public:
		enum Enum
		{
			INFO,
			WARNING,
			ERROR,
			CRITICAL
		};

		LogLevel(int _value = INFO) : mValue((Enum)_value) {}
		operator int() const {return mValue;}
		GOTHOGRE_DECLARE_ENUM_IO(LogLevel);
	private:
		Enum mValue;
	};


	//-----------------------------------------------------------------------
	class LogSystemImpl;
	class DefaultLogListener;

	class GOTHOGRE_EXPORT LogSystem : public Singleton<LogSystem>
	{
	public:
		LogSystem(const String& _logFileName);
		~LogSystem();

		void logMessage(const String& _module, LogLevel _lvl, const String& _message);

	private:
		friend class NewLogListener;
		LogSystemImpl*       mImpl;
		DefaultLogListener*  mDefaultLogListener;
	};

}

#endif // GOTHOGRE_LOG_SYSTEM_H