#include "ExtArchives_Precompiled.h"
#include "ExtArchives_CommonInclude.h"
#include "FastArchive.h"

namespace GothOgre
{
	FastArchive::FastArchive(const String& _name, const String& _archType, const String& _baseArchType )
		: DerivedArchive(_name, _archType, _baseArchType)
	{
	}
	//---------------------------------------------------------------------------
	FastArchive::~FastArchive()
	{
		clearContents();
	}
	//---------------------------------------------------------------------------
	void FastArchive::load()
	{
		getBaseArchive()->load();
		scanContents();
	}
	//---------------------------------------------------------------------------
	void FastArchive::unload()
	{
		getBaseArchive()->unload();
		clearContents();
	}
	//---------------------------------------------------------------------------
	void FastArchive::scanContents()
	{

	}
	//---------------------------------------------------------------------------
	void FastArchive::clearContents()
	{

	}
	//---------------------------------------------------------------------------
	StringVectorPtr FastArchive::list(bool _recursive, bool _dirs)
	{
		return getBaseArchive()->list(_recursive, _dirs);
	}
	//---------------------------------------------------------------------------
	FileInfoListPtr FastArchive::listFileInfo(bool _recursive, bool _dirs)
	{
		return getBaseArchive()->listFileInfo(_recursive, _dirs);
	}
	//---------------------------------------------------------------------------
	StringVectorPtr FastArchive::find(const String& _pattern, bool _recursive, bool _dirs)
	{
		return getBaseArchive()->find(_pattern, _recursive, _dirs);
	}
	//---------------------------------------------------------------------------
	FileInfoListPtr FastArchive::findFileInfo(const String& _pattern, bool _recursive, bool _dirs)
	{
		return getBaseArchive()->findFileInfo(_pattern, _recursive, _dirs);
	}

} // namespace GothOgre