#include "Precompiled.h"
#include "Zip.h"

namespace GothOgre
{
	ZipArchive::ZipArchive(const String& _name, const String& _archType)
			: DerivedArchive(_name, _archType, "Zip") 
	{
	}	

} // namespace GothOgre