#include "GothOgre_Precompiled.h"
#include "GothOgre_ScriptLoaderEx.h"


namespace GothOgre
{
	//-----------------------------------------------------------------------------
	ScriptLoaderEx::ScriptLoaderEx(const String& _scriptPattern, Priority _priority)
	{
		mScriptPatterns.push_back(_scriptPattern);
		mPriority = _priority;
		ResourceGroupManager::getSingleton()._registerScriptLoader(this);
	}
	//-----------------------------------------------------------------------------
	ScriptLoaderEx::~ScriptLoaderEx()
	{
		ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);
	}
	//-----------------------------------------------------------------------------
	const StringVector& ScriptLoaderEx::getScriptPatterns() const
	{
		return mScriptPatterns;
	}
	//-----------------------------------------------------------------------------
	Real ScriptLoaderEx::getLoadingOrder() const
	{
		return 10000.f - mPriority;
	}
	//-----------------------------------------------------------------------------
	void ScriptLoaderEx::parseScript( DataStreamPtr& _dataStream, const String& _resourceGroup)
	{
		DataStreamExPtr dataStreamEx = _dataStream;
		parseScript( dataStreamEx, _resourceGroup );
	}

} // namespace GothOgre