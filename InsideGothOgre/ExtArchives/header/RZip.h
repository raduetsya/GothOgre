#ifndef R_ZIP_H
#define R_ZIP_H

#include "Zip.h"
#include "FastArchive.h"
#include "RecursiveArchive.h"

namespace GothOgre
{
	typedef RecursiveArchive<FastArchive<ZipArchive> > RZipArchive;
	//-----------------------------------------------------------------------------
	class RZipArchiveFactory : public ArchiveFactory
	{
	public:
		RZipArchiveFactory();
        virtual ~RZipArchiveFactory();

		/** Returns the factory type. */
		const String& getType() const;

		/** Creates a new archive. */
		Archive* createInstance( const String& _name );

		/** Destroys an archive which was created by this factory. */
		void destroyInstance( Archive* _arch);
	};

} // namespace GothOgre

#endif // R_ZIP_H
