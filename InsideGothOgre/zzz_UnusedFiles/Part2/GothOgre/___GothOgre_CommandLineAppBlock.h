#ifndef GOTHOGRE_COMMAND_LINE_APP_BLOCK_H
#define GOTHOGRE_COMMAND_LINE_APP_BLOCK_H

#include "GothOgre_AppBlock.h"
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
			GOTHOGRE_INFO("CommandLine: appPath = '" << mCommandLine->getAppPath() << "'");
			GOTHOGRE_INFO("CommandLine: appFolder = '" << mCommandLine->getAppFolder() << "'");
		
			for(int i = 0; i != mCommandLine->getNumArguments(); ++i)
				GOTHOGRE_INFO("CommandLine: argument[" << i << "] = '" << mCommandLine->getArgument(i) << "'");
		}

	private:
		CommandLine*   mCommandLine;
	};

}

#endif // GOTHOGRE_COMMAND_LINE_APP_BLOCK_H