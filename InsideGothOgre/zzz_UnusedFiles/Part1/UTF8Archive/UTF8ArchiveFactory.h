#ifndef UTF8_ARCHIVE_FACTORY_H
#define UTF8_ARCHIVE_FACTORY_H

#include "OgreArchiveFactory.h"

namespace GothOgre
{
	/** Factory to create Utf8Archive */
	class Utf8ArchiveFactory : public ArchiveFactory
    {
    public:
		/** Type of archive is transferred to the constructor */
		Utf8ArchiveFactory(const String& _archType, const String& _baseArchType);
        virtual ~Utf8ArchiveFactory();

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

#endif // UTF8_ARCHIVE_FACTORY_H
