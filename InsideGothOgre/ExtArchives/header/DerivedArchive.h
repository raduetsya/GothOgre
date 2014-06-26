#ifndef DERIVED_ARCHIVE_H
#define DERIVED_ARCHIVE_H

namespace GothOgre 
{
	/** Derived archive is an archive which is based on another archive. */
	class DerivedArchive : public Archive
    {
    public:
		/** Constructor - don't call direct, used by FlatArchiveFactory. */
		DerivedArchive(const String& _name, const String& _archType, const String& _baseArchType );

		/** Destructor. */
		virtual ~DerivedArchive();

		/** Returns the base archive for this archive. */
		Archive* getBaseArchive() const {return mBaseArchive;}

		/** Returns whether this archive is case sensitive 
		in the way it matches files. */
		virtual bool isCaseSensitive() const;

        /** Loads the archive.
        @remarks
            This initialises all the internal data of the class.
        @warning
            Do not call this function directly, it is meant to be used
            only by the FlatArchiveManager class. */
	    virtual void load();

        /** Unloads the archive.
        @warning
            Do not call this function directly, it is meant to be used
            only by the ArchiveManager class. */
		virtual void unload();

		/** Reports whether this Archive is read-only, or whether the contents 
		can be updated. */
		virtual bool isReadOnly() const;

        /** List all file names in the archive.
        @note
            This method only returns filenames, you can also retrieve other
            information using listFileInfo.
        @param recursive Ignored. The search is recursive in any case
        @param dirs If this parameter is true then the function returns an empty
		list.
        @returns A list of filenames matching the criteria, without pathes */
		virtual StringVectorPtr list(bool _recursive = true, bool _dirs = false);

        /** List all files in the archive with accompanying information.
        @param recursive Ignored. The search is recursive in any case
        @param dirs If this parameter is true then the function returns an empty
		list.
        @returns A list of structures detailing quite a lot of information about
            all the files in the archive. */
		virtual FileInfoListPtr listFileInfo(bool _recursive = true, bool _dirs = false);
        
        /** Find all file or directory names matching a given pattern
            in this archive.
        @note
            This method only returns filenames, you can also retrieve other
            information using findFileInfo.
        @param pattern The pattern to search for; wildcards (* and ?) are allowed
        @param recursive Ignored. The search is recursive in any case
        @param dirs If this parameter is true then the function returns an empty
		list.
        @returns A list of filenames matching the criteria, without pathes. */
		virtual StringVectorPtr find(const String& _pattern, bool _recursive = true, bool _dirs = false);
        
        /** Find all files or directories matching a given pattern in this
            archive and get some detailed information about them.
        @param pattern The pattern to search for; wildcards (* and ?) are allowed
        @param recursive Ignored. The search is recursive in any case
        @param dirs If this parameter is true then the function returns an empty
		list.
        @returns A list of file information structures for all files matching 
            the criteria. */
		virtual FileInfoListPtr findFileInfo(const String& _pattern, bool _recursive = true, bool _dirs = false) const;

		/** Find out if the named file exists (note: only base filename is used,
		path is ignored). */
        virtual bool exists(const String& _filename);

		/** Retrieve the modification time of a given file */
		virtual time_t getModifiedTime(const String& _filename);

		/** Open a stream on a given file. 
        @note
            There is no equivalent 'close' method; the returned stream
            controls the lifecycle of this file operation.
        @param filename name of the file (note: only base filename is used,
		path is ignored)
        @returns A shared pointer to a DataStream which can be used to 
            read / write the file. If the file is not present, returns a null
			shared pointer. */
		virtual Ogre::DataStreamPtr open(const String& _filename, bool _readOnly) const;

		/** Create a new file (or overwrite one already there). */
		virtual Ogre::DataStreamPtr create(const String& _filename) const;

		/** Delete a named file. */
		virtual void remove(const String &filename) const;

	private:
		Archive*			mBaseArchive;
    };


} // namespace GothOgre

#endif // DERIVED_ARCHIVE_H
