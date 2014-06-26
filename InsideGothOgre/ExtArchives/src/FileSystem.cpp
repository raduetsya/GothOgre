#include "Precompiled.h"
#include "FileSystem.h"

namespace GothOgre
{
	FileSystemArchive::FileSystemArchive(const String& _name, const String& _archType)
			: DerivedArchive(_name, _archType, "FileSystem") 
	{
	}	

} // namespace GothOgre