#ifndef GOTHOGRE_APP_BLOCK_COMMAND_LINE_H
#define GOTHOGRE_APP_BLOCK_COMMAND_LINE_H

#include "GothOgre_AppBlock.h
#include "GothOgre_CommandLine.h"

namespace GothOgre
{

	class CommandLineAppBlock : public AppBlock
	{
	public:
		CommandLineAppBlock(const char* _cmdLine) 
		{
			mCommandLine = new CommandLine(_cmdLine);
		}

		CommandLineAppBlock(int _argc, char** _argv) 
		{
			mCommandLine = new CommandLine(_argc, _argv);
		}

		~CommandLineAppBlock()
		{
			if(mCommandLine)
				delete mCommandLine;
		}

		void initialise()
		{
			GOTHOGRE_LOG(Info, "CommandLine: appPath = '" << mCommandLine->getAppPath() << "'");
			GOTHOGRE_LOG(Info, "CommandLine: appFolder = '" << mCommandLine->getAppFolder() << "'");
		
			for(int i = 0; i != mCommandLine->getNumArguments(); ++i)
				GOTHOGRE_LOG(Info, "CommandLine: argument[" << i << "] = '" << mCommandLine->getArgument(i) << "'");
		}

	private:
		CommandLine*   mCommandLine;
	};

}

#endif // GOTHOGRE_APP_BLOCK_COMMAND_LINE_H