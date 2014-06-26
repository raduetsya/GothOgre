#ifndef ZIP_H
#define ZIP_H

#include "DerivedArchive.h"

namespace GothOgre
{
	class ZipArchive : public DerivedArchive
	{
	public:
		ZipArchive(const String& _name, const String& _archType);
	};

} // namespace GothOgre

#endif // ZIP_H
