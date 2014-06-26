#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include "DerivedArchive.h"

namespace GothOgre
{
	class FileSystemArchive : public DerivedArchive
	{
	public:
		FileSystemArchive(const String& _name, const String& _archType);
	};

} // namespace GothOgre

#endif // FILE_SYSTEM_H
