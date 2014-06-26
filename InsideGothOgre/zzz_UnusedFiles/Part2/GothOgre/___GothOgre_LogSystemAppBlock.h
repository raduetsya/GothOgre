#ifndef GOTHOGRE_LOG_SYSTEM_APP_BLOCK_H
#define GOTHOGRE_LOG_SYSTEM_APP_BLOCK_H

#include "GothOgre_AppBlock.h"
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
			mDefaultLogListener = new DefaultLogListener(CommandLine::getSingleton().getAbsolutePath("GothOgre.log"));
			GOTHOGRE_INFO("****************************************************************************");
			GOTHOGRE_INFO("*                   App: Initializing                                      *");
			GOTHOGRE_INFO("****************************************************************************");
		}

		~LogSystemAppBlock()
		{
			GOTHOGRE_INFO("****************************************************************************");
			GOTHOGRE_INFO("*                   App: Successfully shutted down                         *");
			GOTHOGRE_INFO("****************************************************************************");

			delete mDefaultLogListener;
			mDefaultLogListener = nullptr;

			delete mLogSystem;
			mLogSystem = nullptr;			
		}

		void postinitialise()
		{
			GOTHOGRE_INFO("****************************************************************************");
			GOTHOGRE_INFO("*                   App: Successfully Initialised                          *");
			GOTHOGRE_INFO("****************************************************************************");
		}

		void preshutdown()
		{
			GOTHOGRE_INFO("****************************************************************************");
			GOTHOGRE_INFO("*                   App: Shutting down                                     *");
			GOTHOGRE_INFO("****************************************************************************");
		}

	private:
		LogSystem* 				      mLogSystem;
		DefaultLogListener*           mDefaultLogListener;
	};

}

#endif // GOTHOGRE_LOG_SYSTEM_APP_BLOCK_H