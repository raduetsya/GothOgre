#ifndef GOTHOGRE_DEFAULT_LOG_LISTENER_H
#define GOTHOGRE_DEFAULT_LOG_LISTENER_H

#include "GothOgre_LogListener.h"

namespace GothOgre
{

	class DefaultLogListener
	{
	public:
		DefaultLogListener(const String& _filename);
		~DefaultLogListener();

	private:
		GOTHOGRE_DECLARE_MESSAGE_LOGGED( DefaultLogListener );

	private:
		std::ofstream mfpLog;
	};
}

#endif // GOTHOGRE_DEFAULT_LOG_LISTENER_H