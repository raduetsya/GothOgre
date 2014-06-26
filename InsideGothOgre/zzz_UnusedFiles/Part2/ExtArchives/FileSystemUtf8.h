#ifndef FILE_SYSTEM_UTF8_H
#define FILE_SYSTEM_UTF8_H

#include "OgreArchiveFactory.h"

namespace GothOgre 
{

	/** Specialisation of the Archive class to allow reading of files from 
        filesystem folders / directories.
		This class looks like FileSystemArchive,
		but it treats all strings as UTF-8.
    */
    class FileSystemUtf8Archive : public Archive 
    {
    protected:
		std::wstring mWName;

		OGRE_AUTO_MUTEX
    public:
        FileSystemUtf8Archive(const String& _name, const String& _archType );
        ~FileSystemUtf8Archive();

        /// @copydoc Archive::isCaseSensitive
        bool isCaseSensitive() const;

        /// @copydoc Archive::load
        void load();

        /// @copydoc Archive::unload
        void unload();

        /// @copydoc Archive::open
		Ogre::DataStreamPtr open(const String& _filename, bool _readOnly = true) const;

		/// @copydoc Archive::create
		Ogre::DataStreamPtr create(const String& _filename) const;

		/// @copydoc Archive::delete
		void remove(const String& _filename) const;

		/// @copydoc Archive::list
        StringVectorPtr list(bool _recursive = true, bool _dirs = false);

        /// @copydoc Archive::listFileInfo
        FileInfoListPtr listFileInfo(bool _recursive = true, bool _dirs = false);

        /// @copydoc Archive::find
        StringVectorPtr find(const String& _pattern, bool _recursive = true,
            bool _dirs = false);

        /// @copydoc Archive::findFileInfo
        FileInfoListPtr findFileInfo(const String& _pattern, bool _recursive = true,
            bool _dirs = false);

        /// @copydoc Archive::exists
        bool exists(const String& _filename);

		/// @copydoc Archive::getModifiedTime
		time_t getModifiedTime(const String& _filename);

		/// Set whether filesystem enumeration will include hidden files or not.
		/// This should be called prior to declaring and/or initializing filesystem
		/// resource locations. The default is true (ignore hidden files).
		static void setIgnoreHidden(bool _ignore)
		{
			ms_IgnoreHidden = _ignore;
		}

		/// Get whether hidden files are ignored during filesystem enumeration.
		static bool getIgnoreHidden()
		{
			return ms_IgnoreHidden;
		}

		static bool ms_IgnoreHidden;
    };

    /** Specialisation of ArchiveFactory for FileSystem files. */
    //class _OgrePrivate FileSystemArchiveFactory : public ArchiveFactory
	class FileSystemUtf8ArchiveFactory : public ArchiveFactory
    {
    public:
        virtual ~FileSystemUtf8ArchiveFactory() {}

		/// @copydoc FactoryObj::getType
        const String& getType() const;

		/// @copydoc FactoryObj::createInstance
		Archive *createInstance( const String& _name ) 
        {
            return OGRE_NEW FileSystemUtf8Archive(_name, "FileSystemUtf8");
        }

		/// @copydoc FactoryObj::destroyInstance
		void destroyInstance( Archive* _arch) { delete _arch; }
    };

	/** @} */
	/** @} */

}

#endif // FILE_SYSTEM_UTF8_H
