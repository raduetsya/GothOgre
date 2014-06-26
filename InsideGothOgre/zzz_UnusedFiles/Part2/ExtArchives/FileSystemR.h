#ifndef FILE_SYSTEM_R_H
#define FILE_SYSTEM_R_H

#include "FileSystemUtf8.h"
#include "FastArchive.h"
#include "RecursiveArchive.h"

namespace GothOgre
{
	typedef RecursiveArchive<FastArchive<FileSystemUtf8Archive> > FileSystemRArchive;
	//-----------------------------------------------------------------------------
	class FileSystemRArchiveFactory : public ArchiveFactory
	{
	public:
		FileSystemRArchiveFactory();
        virtual ~FileSystemRArchiveFactory();

		/** Returns the factory type. */
		const String& getType() const;

		/** Creates a new archive. */
		Archive* createInstance( const String& _name );

		/** Destroys an archive which was created by this factory. */
		void destroyInstance( Archive* _arch);
	};

} // namespace GothOgre

#endif // FILE_SYSTEM_R_H
