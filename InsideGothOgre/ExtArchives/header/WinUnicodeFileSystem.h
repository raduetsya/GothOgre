#ifndef WIN_UNICODE_FILE_SYSTEM_H
#define WIN_UNICODE_FILE_SYSTEM_H

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
namespace GothOgre 
{
	typedef std::basic_string<WCHAR, std::char_traits<WCHAR>, String::allocator_type> WString;

	/** Specialisation of the Archive class to allow reading of files from 
        filesystem folders / directories.
		This class looks like FileSystemArchive,
		but it treats all strings as UTF-8.
    */
    class UnicodeFileSystemArchive : public Archive 
    {
    protected:
		WString mWName;

		OGRE_AUTO_MUTEX
    public:
        UnicodeFileSystemArchive(const String& _name, const String& _archType );
        ~UnicodeFileSystemArchive();

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
            bool _dirs = false) const;

        /// @copydoc Archive::exists
        bool exists(const String& _filename);

		/// @copydoc Archive::getModifiedTime
		time_t getModifiedTime(const String& _filename);

		/// Set whether filesystem enumeration will include hidden files or not.
		/// This should be called prior to declaring and/or initializing filesystem
		/// resource locations. The default is true (ignore hidden files).
		void setIgnoreHidden(bool _ignore)
		{
			mIgnoreHidden = _ignore;
		}

		/// Get whether hidden files are ignored during filesystem enumeration.
		bool getIgnoreHidden() const
		{
			return mIgnoreHidden;
		}

	private:
		WString toWString( const String& _string ) const;
		String toString( const WString& _wstring ) const;
		String toString( const wchar_t* _wstr ) const;
		bool isAbsolutePath(const WString& _wFilename) const;
		WString getFullPath(const String& _filename) const;
		WString getFullPath(const WString& _wFilename) const;

	private:
		bool                mIgnoreHidden;
		CodepageConverter*  mUtf16Converter;

	private:
		class FileFinder
		{
		public:
			FileFinder(UnicodeFileSystemArchive* _archive, const WString& _wArchiveName, 
				bool _ignoreHidden);

			void run(const WString& _wpattern, bool _recursive, bool _dirs,
				StringVector* _simpleList, FileInfoList* _detailList);

		private:
			void _run(const String& _dir, const WString& _wFullDir);
			bool isReservedDir(const wchar_t* _wstr) const;

		private:
			UnicodeFileSystemArchive* mArchive;
			bool		           mIgnoreHidden;
			bool		           mRecursive;
			bool		           mDirs;
			String	               mMask;
			StringVector*          mSimpleList;
			FileInfoList*          mDetailList;
		};
    };

    /** Specialisation of ArchiveFactory for FileSystem files. */
	class UnicodeFileSystemArchiveFactory : public ArchiveFactory
    {
    public:
		UnicodeFileSystemArchiveFactory();
        virtual ~UnicodeFileSystemArchiveFactory();

		/// @copydoc FactoryObj::getType
        const String& getType() const;

		/// @copydoc FactoryObj::createInstance
		Archive* createInstance( const String& _name );

		/// @copydoc FactoryObj::destroyInstance
		void destroyInstance( Archive* _arch);
    };
}

#endif // OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#endif // WIN_UNICODE_FILE_SYSTEM_H
