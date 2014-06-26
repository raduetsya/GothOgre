#ifndef ZIP_R_H
#define ZIP_R_H

#include "Zip.h"
#include "FastArchive.h"
#include "RecursiveArchive.h"

namespace GothOgre
{
	typedef RecursiveArchive<FastArchive<ZipArchive> > ZipRArchive;
	//-----------------------------------------------------------------------------
	class ZipRArchiveFactory : public ArchiveFactory
	{
	public:
		ZipRArchiveFactory();
        virtual ~ZipRArchiveFactory();

		/** Returns the factory type. */
		const String& getType() const;

		/** Creates a new archive. */
		Archive* createInstance( const String& _name );

		/** Destroys an archive which was created by this factory. */
		void destroyInstance( Archive* _arch);
	};

} // namespace GothOgre

#endif // ZIP_R_H
