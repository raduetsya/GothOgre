#ifndef GOTHOGRE_SCRIPT_LOADER_EX_H
#define GOTHOGRE_SCRIPT_LOADER_EX_H

#include "GothOgre_Priority.h"
#include "GothOgre_DataStreamEx.h"


namespace GothOgre
{
	/** Abstract base class for script loaders. */
	class GOTHOGRE_EXPORT ScriptLoaderEx : public ScriptLoader
	{
	public:
		ScriptLoaderEx(const String& _scriptPattern, Priority _priority = Priority::LOWEST);
		~ScriptLoaderEx();

		/** Gets the file patterns which should be used 
		to find scripts for this class. */
		const StringVector& getScriptPatterns() const;

		/** Parse a script file. */
		virtual void parseScript( DataStreamExPtr& _dataStream, const String& _resourceGroup ) = 0;

	private:
		/** Gets the relative loading order of scripts of this type. */
		Real getLoadingOrder() const;

		/** Parse a script file. */
		void parseScript( DataStreamPtr& _stream, const String& _resourceGroup);

	private:
		String        mName;
		Priority      mPriority;
		StringVector  mScriptPatterns;
	};


} // namespace GothOgre

#endif // GOTHOGRE_SCRIPT_LOADER_EX_H