#ifndef R_FILE_SYSTEM_H
#define R_FILE_SYSTEM_H

#include "UnicodeFileSystem.h"
#include "FastArchive.h"
#include "RecursiveArchive.h"

namespace GothOgre
{
	typedef RecursiveArchive<FastArchive<UnicodeFileSystemArchive> > RFileSystemArchive;
	//-----------------------------------------------------------------------------
	class RFileSystemArchiveFactory : public ArchiveFactory
	{
	public:
		RFileSystemArchiveFactory();
        virtual ~RFileSystemArchiveFactory();

		/** Returns the factory type. */
		const String& getType() const;

		/** Creates a new archive. */
		Archive* createInstance( const String& _name );

		/** Destroys an archive which was created by this factory. */
		void destroyInstance( Archive* _arch);
	};

} // namespace GothOgre

#endif // R_FILE_SYSTEM_H
