#include "GothOgre_Precompiled.h"
#include "GothOgre_CommandLine.h"

//-------------------------------------------------------------------------
GothOgre::CommandLine* 
	Ogre::Singleton<GothOgre::CommandLine>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------

namespace GothOgre
{
	//-------------------------------------------------------------------------
	/** The function builds standard C-style argc/argv pair from command line string.
	@remark
	by Alter aka Alexander A. Telyatnikov
	(http://alter.org.ua/ru/docs/win/args/)
	@par
	The function returns a pointer to dynamically allocated memory.
	To free it, use the "free" function (from C runtime). */
	template<typename Char>
	Char** CommandLineToArgv(const Char* _cmdLine, int* _argc)
    {
        Char** argv;
        Char*  _argv;
        size_t   len;
        size_t   argc;
        Char   a;
        size_t   i, j;

        bool  in_QM;
        bool  in_TEXT;
        bool  in_SPACE;

        len = strlen(_cmdLine);
        i = ((len+2)/2)*sizeof(void*) + sizeof(void*);

		argv = (Char**) malloc(i + (len+2)*sizeof(Char));
        _argv = ((Char*)argv) + i;

        argc = 0;
        argv[argc] = _argv;
        in_QM = false;
        in_TEXT = false;
        in_SPACE = true;
        i = 0;
        j = 0;

        while( a = _cmdLine[i] ) 
		{
            if(in_QM) 
			{
                if(a == '\"') 
				{
                    in_QM = false;
                } 
				else 
				{
                    _argv[j] = a;
                    j++;
                }
            } 
			else 
			{
                switch(a) 
				{
                case '\"':
                    in_QM = true;
                    in_TEXT = true;
                    if(in_SPACE) 
					{
                        argv[argc] = _argv+j;
                        argc++;
                    }
                    in_SPACE = false;
                    break;
                case ' ':
                case '\t':
                case '\n':
                case '\r':
                    if(in_TEXT) 
					{
                        _argv[j] = '\0';
                        j++;
                    }
                    in_TEXT = false;
                    in_SPACE = true;
                    break;
                default:
                    in_TEXT = true;
                    if(in_SPACE) 
					{
                        argv[argc] = _argv+j;
                        argc++;
                    }
                    _argv[j] = a;
                    j++;
                    in_SPACE = false;
                    break;
                }
            }
            i++;
        }
        _argv[j] = '\0';
        argv[argc] = NULL;

        (*_argc) = argc;
        return argv;
    }
	//---------------------------------------------------------------------
	void CommandLine::init(int _argc, char** _argv)
	{
		mArguments.clear();
		for(int i = 0; i < _argc; ++i)
			mArguments.push_back(_argv[i]);

		mAppPath = mArguments[0];

		int i = mAppPath.find_last_of("\\/");
		mAppFolder = mAppPath.substr(0, i + 1);
	}
	//---------------------------------------------------------------------
	CommandLine::CommandLine(int _argc, char** _argv)
	{
		init(_argc, _argv);
	}
	//---------------------------------------------------------------------
	CommandLine::CommandLine(const char* _commandLine)
	{
		int argc;
		char** argv = CommandLineToArgv(_commandLine, &argc);
		init(argc, argv);
		free(argv);
	}
	//---------------------------------------------------------------------
	CommandLine::~CommandLine()
	{
	}
	//---------------------------------------------------------------------
	const String& CommandLine::getAppPath() const
	{
		return mAppPath;
	}
	//---------------------------------------------------------------------
	const String& CommandLine::getAppFolder() const
	{
		return mAppFolder;
	}
	//----------------------------------------------------------------------------------
	const String& CommandLine::getArgument(size_t i) const
	{
		return mArguments[i];
	}
	//---------------------------------------------------------------------
	size_t CommandLine::getNumArguments() const
	{
		return mArguments.size();
	}
	//---------------------------------------------------------------------
	String CommandLine::getAbsolutePath(const String& _relativePath) const
	{
		int i = mAppFolder.length();
		int j = 0;
		while(true)
		{
			if(_relativePath[j] == '\\' || _relativePath[j] == '/')
			{
				++j;
			}
			else if(_relativePath[j] == '.')
			{
				++j;
				if(_relativePath[j] == '.') // point and point mean two points ("..")
				{
					i = mAppFolder.find_last_of("\\/", i - 2) + 1;
					++j;
				}
			}
			else
				break;
		}
		return mAppFolder.substr(0, i) + _relativePath.substr(j);
	}

} // namespace GothOgre