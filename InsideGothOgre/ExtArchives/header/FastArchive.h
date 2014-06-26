#ifndef FAST_ARCHIVE_H
#define FAST_ARCHIVE_H

namespace GothOgre 
{
	template<typename T>
	class FastArchive : public T
	{
	public:
		/** Constructor - don't call direct, used by FlatArchiveFactory. */
		FastArchive(const String& _name, const String& _archType);

		/** Destructor. */
		virtual ~FastArchive();

	    /** Loads the archive. */
	    virtual void load();

        /** Unloads the archive. */
		virtual void unload();

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

		// I don't know why Assaf Raman (one of the Ogre team) 
		// adds the constant specifier to the "Archive::findFileInfo" function.
		// So I need to make some workaround with the following function.
		FileInfoListPtr findFileInfo_nonconst(const String& _pattern, bool _recursive, bool _dirs);

		/** Create a new file (or overwrite one already there). 
		This function can also create cache instead of its normal functionality.
		To do it, specify "<cache>" as the function's parameter. */
		virtual Ogre::DataStreamPtr create(const String& _filename) const;
		
	protected:
		/** Ensure cache is destroyed. */
		void clearCache();

		/** Ensure cache is created. The cache is used to speed up file search operations. */
		void createCache();

	private:
		void createCache_scanFilesAndDirs(const String& _path);
		void createCache_addStringVectors();
		void createCache_addSynonymPatterns();
		static void makeSynonymPatterns(const String& _pattern, StringVector& _outSynonyms);

	private:
		typedef map<String, FileInfoListPtr>::type FileInfoListByPattern;
		typedef map<String, StringVectorPtr>::type StringVectorByPattern;
		FileInfoListByPattern           mFileInfoListByPattern[2][2];
		StringVectorByPattern           mStringVectorByPattern[2][2];
		FileInfoListByPattern::iterator mFIL_LastIterator[2][2];
		StringVectorByPattern::iterator mSV_LastIterator[2][2];
	};
	//---------------------------------------------------------------------------
	template<typename T>
	FastArchive<T>::FastArchive(const String& _name, const String& _archType )
		: T(_name, _archType)
	{
		for(size_t i = 0; i != 2; ++i)
			for(size_t j = 0; j != 2; ++j)
			{
				mFIL_LastIterator[i][j] = mFileInfoListByPattern[i][j].end();
				mSV_LastIterator[i][j] = mStringVectorByPattern[i][j].end();
			}		
	}
	//---------------------------------------------------------------------------
	template<typename T>
	FastArchive<T>::~FastArchive()
	{
		clearCache();
	}
	//---------------------------------------------------------------------------
	template<typename T>
	void FastArchive<T>::load()
	{
		T::load();
		createCache();
	}
	//---------------------------------------------------------------------------
	template<typename T>
	void FastArchive<T>::unload()
	{
		T::unload();
		clearCache();
	}
	//---------------------------------------------------------------------------
	template<typename T>
	void FastArchive<T>::clearCache()
	{
		for(size_t i = 0; i != 2; ++i)
			for(size_t j = 0; j != 2; ++j)
			{
				mFileInfoListByPattern[i][j].clear();
				mStringVectorByPattern[i][j].clear();
				mFIL_LastIterator[i][j] = mFileInfoListByPattern[i][j].end();
				mSV_LastIterator[i][j] = mStringVectorByPattern[i][j].end();
			}
	}
	//---------------------------------------------------------------------------
	template<typename T>
	void FastArchive<T>::createCache()
	{
		clearCache();
		createCache_scanFilesAndDirs("");
		createCache_addStringVectors();
		createCache_addSynonymPatterns();
	}
	//---------------------------------------------------------------------------
	template<typename T>
	void FastArchive<T>::createCache_scanFilesAndDirs(const String& _path)
	{
		// Find files in the directory specified (not-recursively).
		String pattern = _path + '*';
		FileInfoListPtr filesptr = T::findFileInfo(pattern, false, false);

		// Store files found in the directory specified.
		// Pattern: "*".
		// Recursive: false.
		// Dirs: false.
		{
			if(!isCaseSensitive())
				StrUtil::toLowerCase(pattern);
			FileInfoListByPattern& filmap = mFileInfoListByPattern[false][false];
			filmap.insert(std::make_pair(pattern, filesptr));
		}

		// Store files found in the directory specified.
		// Pattern: "*.ext".
		// Recursive: false.
		// Dirs: false.
		String fileext;
		size_t i;
		for(i = 0; i != filesptr->size(); ++i)
		{
			const FileInfo& fi = (*filesptr)[i];
			
			// get file extension
			const String& basename = fi.basename;
			size_t dotpos = basename.rfind('.');
			if(dotpos != String::npos)
			{
				fileext.assign(basename.c_str() + dotpos);
				pattern = _path + '*' + fileext;
				if(!isCaseSensitive())
					StrUtil::toLowerCase(pattern);
				FileInfoListByPattern& filmap = mFileInfoListByPattern[false][false];
				FileInfoListByPattern::iterator it = filmap.find(pattern);
				if(it == filmap.end())
				{
					FileInfoListPtr newfilptr(OGRE_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);
					it = filmap.insert(std::make_pair(pattern, newfilptr)).first;
				}
				FileInfoListPtr filptr = it->second;
				filptr->push_back(fi);
			}
		}

		// Store files found in the directory specified.
		// Pattern: "*".
		// Recursive: true.
		// Dirs: false.
		for(i = 0; i != filesptr->size(); ++i)
		{
			const FileInfo& fi = (*filesptr)[i];
			
			pattern = '*';
			FileInfoListByPattern& filmap = mFileInfoListByPattern[true][false];
			FileInfoListByPattern::iterator it = filmap.find(pattern);
			if(it == filmap.end())
			{
				FileInfoListPtr newfilptr(OGRE_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);
				it = filmap.insert(std::make_pair(pattern, newfilptr)).first;
			}

			FileInfoListPtr filptr = it->second;
			filptr->push_back(fi);
		}

		// Store files found in the directory specified.
		// Pattern: "*.ext".
		// Recursive: true.
		// Dirs: false.
		for(i = 0; i != filesptr->size(); ++i)
		{
			const FileInfo& fi = (*filesptr)[i];

			// get file extension
			const String& basename = fi.basename;
			size_t dotpos = basename.rfind('.');
			if(dotpos != String::npos)
			{
				fileext.assign(basename.c_str() + dotpos);
				pattern = '*' + fileext;
				if(!isCaseSensitive())
					StrUtil::toLowerCase(pattern);

				FileInfoListByPattern& filmap = mFileInfoListByPattern[true][false];
				FileInfoListByPattern::iterator it = filmap.find(pattern);
				if(it == filmap.end())
				{
					FileInfoListPtr newfilptr(OGRE_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);
					it = filmap.insert(std::make_pair(pattern, newfilptr)).first;
				}
				FileInfoListPtr filptr = it->second;
				filptr->push_back(fi);
			}
		}

		// Store files found in the directory specified.
		// Pattern: "basename" (filename without path).
		// Recursive: true.
		// Dirs: false.
		for(i = 0; i != filesptr->size(); ++i)
		{
			FileInfo& fi = (*filesptr)[i];
			pattern = fi.basename;
			if(!isCaseSensitive())
				StrUtil::toLowerCase(pattern);

			FileInfoListByPattern& filmap = mFileInfoListByPattern[true][false];
			FileInfoListByPattern::iterator it = filmap.find(pattern);
			if(it == filmap.end())
			{
				FileInfoListPtr newfilptr(OGRE_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);
				it = filmap.insert(std::make_pair(pattern, newfilptr)).first;
			}
			FileInfoListPtr filptr = it->second;
			filptr->push_back(fi);
		}

		// Store files found in the directory specified.
		// Pattern: "filename" (full qualified file name).
		// Recursive: false, true.
		// Dirs: false.
		for(i = 0; i != filesptr->size(); ++i)
		{
			FileInfo& fi = (*filesptr)[i];
			pattern = fi.filename;
			if(!isCaseSensitive())
				StrUtil::toLowerCase(pattern);

			for(uchar recursiveValue = 0; recursiveValue != 2; ++recursiveValue)
			{
				bool recursive = (recursiveValue == 1);
				FileInfoListByPattern& filmap = mFileInfoListByPattern[recursive][false];
				FileInfoListPtr newfilptr(OGRE_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);
				FileInfoListByPattern::iterator it;
				it = filmap.insert(std::make_pair(pattern, newfilptr)).first;
				FileInfoListPtr filptr = it->second;
				filptr->push_back(fi);
			}
		}

		// Find subdirectories in the directory specified (not-recursively).
		pattern = _path + '*';
		FileInfoListPtr dirsptr = T::findFileInfo(pattern, false, true);

		// Store subdirectories found in the directory specified.
		// Pattern: "*".
		// Recursive: false.
		// Dirs: true.
		{
			if(!isCaseSensitive())
				StrUtil::toLowerCase(pattern);
			FileInfoListByPattern& filmap = mFileInfoListByPattern[false][true];
			filmap.insert(std::make_pair(pattern, dirsptr));
		}
		
		// Store subdirectories found in the directory specified.
		// Pattern: "*".
		// Recursive: true.
		// Dirs: true.
		for(i = 0; i != dirsptr->size(); ++i)
		{
			const FileInfo& fi = (*dirsptr)[i];

			pattern = '*';
			FileInfoListByPattern& filmap = mFileInfoListByPattern[true][true];
			FileInfoListByPattern::iterator it = filmap.find(pattern);
			if(it == filmap.end())
			{
				FileInfoListPtr newfilptr(OGRE_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);
				it = filmap.insert(std::make_pair(pattern, newfilptr)).first;
			}

			FileInfoListPtr filptr = it->second;
			filptr->push_back(fi);
		}

		// Recursively call itself for each found subdirectory.
		String subdirpath;
		for(i = 0; i != dirsptr->size(); ++i)
		{
			subdirpath = _path + (*dirsptr)[i].basename + '/';
			createCache_scanFilesAndDirs(subdirpath);
		}
	}
	//---------------------------------------------------------------------------
	template<typename T>
	void FastArchive<T>::createCache_addStringVectors()
	{
		// Add one string vector per each fileinfolist.
		for(size_t i = 0; i != 2; ++i)
			for(size_t j = 0; j != 2; ++j)
			{
				FileInfoListByPattern& filmap = mFileInfoListByPattern[i][j];
				StringVectorByPattern& svmap = mStringVectorByPattern[i][j];
				for(FileInfoListByPattern::iterator it = filmap.begin(); it != filmap.end(); ++it)
				{
					const String& pattern = it->first;
					const FileInfoListPtr& filptr = it->second;

					StringVectorByPattern::iterator it2 = svmap.find(pattern);
					if(it2 == svmap.end())
					{
						StringVectorPtr newsvptr(OGRE_NEW_T(StringVector, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);
						it2 = svmap.insert(std::make_pair(pattern, newsvptr)).first;
					}
					StringVectorPtr svptr = it2->second;

					svptr->resize(filptr->size());
					for(size_t k = 0; k != filptr->size(); ++k)
						(*svptr)[k] = (*filptr)[k].filename;
				}
			}	
	}
	//---------------------------------------------------------------------------
	template<typename T>
	void FastArchive<T>::createCache_addSynonymPatterns()
	{
		// Add additional patterns: per each "*" pattern it will add pattern "*.*".
		StringVector synonymPatterns;
		for(size_t i = 0; i != 2; ++i)
			for(size_t j = 0; j != 2; ++j)
			{
				FileInfoListByPattern& filmap = mFileInfoListByPattern[i][j];
				for(FileInfoListByPattern::iterator it = filmap.begin(); it != filmap.end(); ++it)
				{
					const String& pattern = it->first;
					const FileInfoListPtr& filptr = it->second;
					makeSynonymPatterns(pattern, synonymPatterns);
					for(size_t k = 0; k != synonymPatterns.size(); ++k)
					{
						const String& synonymPattern = synonymPatterns[k];
						filmap.insert(std::make_pair(synonymPattern, filptr));
					}
				}

				StringVectorByPattern& svmap = mStringVectorByPattern[i][j];
				for(StringVectorByPattern::iterator it = svmap.begin(); it != svmap.end(); ++it)
				{
					const String& pattern = it->first;
					const StringVectorPtr& svptr = it->second;
					makeSynonymPatterns(pattern, synonymPatterns);
					for(size_t k = 0; k != synonymPatterns.size(); ++k)
					{
						const String& synonymPattern = synonymPatterns[k];
						svmap.insert(std::make_pair(synonymPattern, svptr));
					}
				}
			}		
	}
	//---------------------------------------------------------------------------
	template<typename T>
	void FastArchive<T>::makeSynonymPatterns(const String& _pattern, StringVector& _outSynonyms)
	{
		size_t notasteriskpos = _pattern.find_last_not_of('*');
		if(notasteriskpos == String::npos)
		{
			_outSynonyms.resize(1);
			_outSynonyms[0] = "*.*";
		}
		else if(_pattern[notasteriskpos] == '/')
		{
			_outSynonyms.resize(1);
			_outSynonyms[0] = _pattern.substr(0, notasteriskpos + 1) + "*.*";
		}
		else
		{
			_outSynonyms.clear();
		}
	}
	//---------------------------------------------------------------------------
	template<typename T>
	StringVectorPtr FastArchive<T>::list(bool _recursive, bool _dirs)
	{
		return find("*", _recursive, _dirs);
	}
	//---------------------------------------------------------------------------
	template<typename T>
	FileInfoListPtr FastArchive<T>::listFileInfo(bool _recursive, bool _dirs)
	{
		return findFileInfo("*", _recursive, _dirs);
	}
	//---------------------------------------------------------------------------
	template<typename T>
	StringVectorPtr FastArchive<T>::find(const String& _pattern, bool _recursive, bool _dirs)
	{
		String pattern = _pattern;
		if(!isCaseSensitive())
			StrUtil::toLowerCase(pattern);

		StringVectorByPattern& svmap = mStringVectorByPattern[_recursive][_dirs];
		StringVectorByPattern::iterator& it = mSV_LastIterator[_recursive][_dirs];

		// Check last found iterator
		if(it != svmap.end() && it->first == pattern)
			return it->second;

		// General procedure - find in map.
		it = svmap.find(pattern);
		if(it != svmap.end())
			return it->second;
		else
		{
			// Return an empty vector if not found.
			StringVectorPtr emptysvptr(OGRE_NEW_T(StringVector, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);
			return emptysvptr;
		}
	}
	//---------------------------------------------------------------------------
	template<typename T>
	FileInfoListPtr FastArchive<T>::findFileInfo_nonconst(const String& _pattern, bool _recursive, bool _dirs)
	{
		String pattern = _pattern;
		if(!isCaseSensitive())
			StrUtil::toLowerCase(pattern);

		FileInfoListByPattern& filmap = mFileInfoListByPattern[_recursive][_dirs];
		FileInfoListByPattern::iterator& it = mFIL_LastIterator[_recursive][_dirs];

		// Check last found iterator
		if(it != filmap.end() && it->first == pattern)
			return it->second;

		// General procedure - find in map.
		it = filmap.find(pattern);
		if(it != filmap.end())
			return it->second;
		else
		{
			// Return an empty vector if not found.
			FileInfoListPtr emptyfilptr(OGRE_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);
			return emptyfilptr;
		}
	}
	//---------------------------------------------------------------------------
	template<typename T>
	FileInfoListPtr FastArchive<T>::findFileInfo(const String& _pattern, bool _recursive, bool _dirs) const
	{
		return const_cast<FastArchive<T>*>(this)->findFileInfo_nonconst(_pattern, _recursive, _dirs);
	}
	//---------------------------------------------------------------------------
	template<typename T>
	Ogre::DataStreamPtr FastArchive<T>::create(const String& _filename) const
	{
		if(_filename == "<cache>")
		{
			const_cast<FastArchive<T>*>(this)->createCache();
			return Ogre::DataStreamPtr();
		}
		return T::create(_filename);
	}

} // namespace GothOgre

#endif // FAST_ARCHIVE_H