#ifndef GOTHOGRE_APP_BLOCK_LOG_SYSTEM_H
#define GOTHOGRE_APP_BLOCK_LOG_SYSTEM_H

#include "GothOgre_AppBlock.h
#include "GothOgre_LogSystem.h"
#include "GothOgre_DefaultLogListener.h"


namespace GothOgre
{

	class LogSystemAppBlock : public AppBlock
	{
	public:
		LogSystemAppBlock()
			: mLogSystem(nullptr), mDefaultLogListener(nullptr)
		{
			mLogSystem = new LogSystem;
			mDefaultLogListener = new DefaultLogListener(mCommandLine->getAbsolutePath("GothOgre.log"));

			GOTHOGRE_LOG(Info, "****************************************************************************");
			GOTHOGRE_LOG(Info, "*                   App: Initializing                                      *");
			GOTHOGRE_LOG(Info, "****************************************************************************");
		}

		~LogSystemAppBlock()
		{
			GOTHOGRE_LOG(Info, "****************************************************************************");
			GOTHOGRE_LOG(Info, "*                   App: Successfully shutted down                         *");
			GOTHOGRE_LOG(Info, "****************************************************************************");

			delete mDefaultLogListener;
			mDefaultLogListener = nullptr;

			delete mLogSystem;
			mLogSystem = nullptr;			
		}

		void postinitialise()
		{
			GOTHOGRE_LOG(Info, "****************************************************************************");
			GOTHOGRE_LOG(Info, "*                   App: Successfully Initialised                          *");
			GOTHOGRE_LOG(Info, "****************************************************************************");
		}

		void preshutdown()
		{
			GOTHOGRE_LOG(Info, "****************************************************************************");
			GOTHOGRE_LOG(Info, "*                   App: Shutting down                                     *");
			GOTHOGRE_LOG(Info, "****************************************************************************");
		}

	private:
		LogSystem* 				      mLogSystem;
		DefaultLogListener*           mDefaultLogListener;
	};

}

#endif // GOTHOGRE_APP_BLOCK_LOG_SYSTEM_H