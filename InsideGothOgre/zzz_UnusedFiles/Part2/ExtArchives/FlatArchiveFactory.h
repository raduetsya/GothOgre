#ifndef FLAT_ARCHIVE_FACTORY_H
#define FLAT_ARCHIVE_FACTORY_H

#include "OgreArchiveFactory.h"

namespace GothOgre
{
	/** Factory to create FlatArchive */
	class FlatArchiveFactory : public ArchiveFactory
    {
    public:
		/** Type of archive is transferred to the constructor */
		FlatArchiveFactory(const String& _archType, const String& _baseArchType);
        virtual ~FlatArchiveFactory();

		/** Returns the factory type. */
		const String& getType() const;

		/** Creates a new archive. */
		Archive* createInstance( const String& _name );

		/** Destroys an archive which was created by this factory. */
		void destroyInstance( Archive* _arch);

	private:
		String mArchType;
		String mBaseArchType;
	};

} // namespace GothOgre

#endif // FLAT_ARCHIVE_FACTORY_H
