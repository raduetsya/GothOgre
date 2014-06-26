#ifndef SCRIPT_DAT_LOADER_H
#define SCRIPT_DAT_LOADER_H


namespace GothOgre
{
	/** Abstract base class for loaders of gothic compiled *.dat files. */
	class ScriptDatLoader : public ScriptLoaderEx
	{
	public:
		ScriptDatLoader(const String& _scriptPattern, Priority _priority = Priority::LOWEST);
	};
}

#endif // SCRIPT_DAT_LOADER_H