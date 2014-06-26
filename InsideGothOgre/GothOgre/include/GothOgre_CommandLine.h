#ifndef GOTHOGRE_COMMAND_LINE_H
#define GOTHOGRE_COMMAND_LINE_H

namespace GothOgre
{
	/** Class for keeping command line and parsing arguments from it */
	class GOTHOGRE_EXPORT CommandLine : public Singleton<CommandLine>
	{
	public:
		/** Constructor getting the "main" function's arguments */
		CommandLine(int _argc, char** _argv);

		/** Constructor getting the "WinMain" function's arguments */
		CommandLine(const char* _commandLine);

		/** Destructor */
		~CommandLine();

		/** Returns a fully qualified path to the executable */
		const String& getAppPath() const;

		/** Returns a path to a directory which contains the executable;
		ending with slash */
		const String& getAppFolder() const;

		/** Calculate a fully qualified path by a path relative
		to a directory which contains the executable */
		String getAbsolutePath(const String& _relativePath) const;

		/** Returns the command line argument */
		const String& getArgument(size_t _index) const;

		/** Returns the number of command line arguments. */
		size_t getNumArguments() const;

	private:
		void init(int _argc, char** _argv);

	private:
		vector<String>::type	mArguments;
		String					mAppPath;
		String					mAppFolder;
	};


} // namespace GothOgre

#endif // GOTHOGRE_COMMAND_LINE_H