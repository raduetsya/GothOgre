#include "ExtArchives_Precompiled.h"
#include "ExtArchives_CommonInclude.h"
#include "FileSystemUtf8.h"

#include <sys/types.h>
#include <sys/stat.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX || OGRE_PLATFORM == OGRE_PLATFORM_APPLE || \
    OGRE_PLATFORM == OGRE_PLATFORM_SYMBIAN || OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
#   include "OgreSearchOps.h"
#   include <sys/param.h>
#   define MAX_PATH MAXPATHLEN
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#  define WIN32_LEAN_AND_MEAN
#  if !defined(NOMINMAX) && defined(_MSC_VER)
#	define NOMINMAX // required to stop windows.h messing up std::min
#  endif
#  include <windows.h>
#  include <direct.h>
#  include <io.h>
#endif

namespace GothOgre 
{
	bool FileSystemUtf8Archive::ms_IgnoreHidden = true;
    //-----------------------------------------------------------------------
    FileSystemUtf8Archive::FileSystemUtf8Archive(const String& name, const String& archType )
        : Archive(name, archType)
    {
    }
    //-----------------------------------------------------------------------
    bool FileSystemUtf8Archive::isCaseSensitive() const
    {
        #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            return false;
        #else
            return true;
        #endif
    }
    //-----------------------------------------------------------------------
    static bool is_reserved_dir (const wchar_t* _name)
    {
        return (_name[0] == '.' && (_name[1] == 0 || (_name[1] == '.' && _name[2] == 0)));
    }
    //-----------------------------------------------------------------------
    static bool is_absolute_path(const wchar_t* _path)
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        if (iswalpha(uchar(_path[0])) && _path[1] == ':')
            return true;
#endif
        return _path[0] == '/' || _path[0] == '\\';
    }
    //-----------------------------------------------------------------------
	static void concatenate_path(const std::wstring& _wbase, const std::wstring& _wname, std::wstring* _result)
    {
        if (_wbase.empty() || is_absolute_path(_wname.c_str()))
		{
            *_result = _wname;
		}
        else
		{
            *_result = _wbase;
			*_result += L'/';
			*_result += _wname;
		}
    }
    //-----------------------------------------------------------------------
	static void concatenate_path(const std::wstring& _wbase, const std::string& _name, std::wstring* _result)
    {
		std::wstring wname;
		StringUtil::utf8_to_utf16(_name, &wname);
		concatenate_path(_wbase, wname, _result);
    }
	//-----------------------------------------------------------------------
	class FileFinder
	{
	public:
		FileFinder(Archive* _archive, const std::wstring& _wArchiveName, 
			bool _ignoreHidden);

		void run(const std::wstring& _wpattern, bool _recursive, bool _dirs,
			StringVector* _simpleList, FileInfoList* _detailList);

	private:
		void _run(const std::string& _dir, const std::wstring& _wFullDir);

	private:
		std::wstring    mWArchiveName;
		Archive*		mArchive;
		bool			mIgnoreHidden;
		bool			mRecursive;
		bool			mDirs;
		std::string		mMask;
		StringVector*	mSimpleList;
		FileInfoList*	mDetailList;
	};
	//-----------------------------------------------------------------------
	FileFinder::FileFinder(Archive* _archive, const std::wstring& _wArchiveName, 
		bool _ignoreHidden)
	{
		mWArchiveName = _wArchiveName;
		mIgnoreHidden = _ignoreHidden;
		mArchive = _archive;
	}
	//-----------------------------------------------------------------------
	void FileFinder::run(const std::wstring& _wpattern, bool _recursive, bool _dirs,
			StringVector* _simpleList, FileInfoList* _detailList)
	{
		mRecursive = _recursive;
		mDirs = _dirs;
		mSimpleList = _simpleList;
		mDetailList = _detailList;

		// pattern can contain a directory name, separate it from mask
        size_t pos1 = _wpattern.rfind ('/');
        size_t pos2 = _wpattern.rfind ('\\');
        if (pos1 == _wpattern.npos || ((pos2 != _wpattern.npos) && (pos1 < pos2)))
            pos1 = pos2;

		std::wstring wdir, wmask;
		if(pos1 == String::npos)
		{
			wmask = _wpattern;
		}
		else
		{
			wdir = _wpattern.substr(0, pos1 + 1);
			wmask = _wpattern.substr(pos1 + 1);
		}

		wcslwr(&wdir[0]);
		wcslwr(&wmask[0]);

		if(wmask == L"*")
			wmask.clear();

		std::string mask;
		StringUtil::utf16_to_utf8(wmask, &mMask);

		std::string dir;
		StringUtil::utf16_to_utf8(wdir, &dir);

		std::wstring wFullDir;
		concatenate_path(mWArchiveName, wdir, &wFullDir);

		_run(dir, wFullDir);
	}
	//-----------------------------------------------------------------------
	void FileFinder::_run(const std::string& _dir, const std::wstring& _wFullDir)
	{
		wchar_t wFindPattern[MAX_PATH];
		wcscpy(wFindPattern, _wFullDir.c_str());
		wcscpy(&wFindPattern[_wFullDir.length()], L"*");
       
        long lHandle;
		struct _wfinddata_t wTagData;
        lHandle = _wfindfirst(wFindPattern, &wTagData);

		std::string  tagDataName;
		std::wstring wTagDataName;
		std::string  dir2;
		std::wstring wFullDir2;

		while(lHandle != -1)
		{
			bool isSubDir = ((wTagData.attrib & _A_SUBDIR) != 0);
			bool isHidden = ((wTagData.attrib & _A_HIDDEN) != 0);
			if((!mIgnoreHidden || !isHidden)
				&& !is_reserved_dir(wTagData.name)
				&& ((isSubDir == mDirs) || (isSubDir && mRecursive)))
			{
				wcslwr(wTagData.name);
				wTagDataName.assign(wTagData.name);
				StringUtil::utf16_to_utf8(wTagDataName, &tagDataName);

				if(isSubDir == mDirs
					&& (mMask.empty() || StringUtil::match(tagDataName, mMask)))
				{
					if (mSimpleList)
					{
						mSimpleList->push_back(std::string());
						std::string& back = mSimpleList->back();
						back = _dir;
						back += tagDataName;
					}
					else if (mDetailList)
					{
						FileInfo fi;
						fi.archive = mArchive;
						fi.filename = _dir;
						fi.filename += tagDataName;
						fi.basename = tagDataName;
						fi.path = _dir;
						fi.compressedSize = wTagData.size;
						fi.uncompressedSize = wTagData.size;
						mDetailList->push_back(fi);
					}
				}

				if(isSubDir && mRecursive)
				{
					dir2 = _dir;
					dir2 += tagDataName;
					dir2 += '/';
					
					wFullDir2 = _wFullDir;
					wFullDir2 += wTagDataName;
					wFullDir2 += '/';
					
					_run(dir2, wFullDir2);
				}
			}
			
			if(_wfindnext( lHandle, &wTagData ) == -1)
			{
				_findclose(lHandle);
				lHandle = -1;
			}
        }
    }
	//-----------------------------------------------------------------------
    FileSystemUtf8Archive::~FileSystemUtf8Archive()
    {
        unload();
    }
    //-----------------------------------------------------------------------
    void FileSystemUtf8Archive::load()
    {
		OGRE_LOCK_AUTO_MUTEX

        // test to see if this folder is writeable
		StringUtil::utf8_to_utf16(mName, &mWName);

		std::wstring wTestPath;
		concatenate_path(mWName, "__testwrite.ogre", &wTestPath);

		std::ofstream writeStream;
		writeStream.open(wTestPath.c_str());
		if (writeStream.fail())
			mReadOnly = true;
		else
		{
			mReadOnly = false;
			writeStream.close();
			::_wremove(wTestPath.c_str());
		}
    }
    //-----------------------------------------------------------------------
    void FileSystemUtf8Archive::unload()
    {
        // nothing to see here, move along
    }
    //-----------------------------------------------------------------------
	Ogre::DataStreamPtr FileSystemUtf8Archive::open(const String& _filename, bool _readOnly) const
    {
		std::wstring wfullpath;
		concatenate_path(mWName, _filename, &wfullpath);

		// Use filesystem to determine size 
		// (quicker than streaming to the end and back)
		struct _stat tagStat;
		int ret = _wstat(wfullpath.c_str(), &tagStat);
		assert(ret == 0 && "Problem getting file size" );
        (void)ret;  // Silence warning

		// Always open in binary mode
		// Also, always include reading
		std::ios::openmode mode = std::ios::in | std::ios::binary;
		std::istream* baseStream = 0;
		std::ifstream* roStream = 0;
		std::fstream* rwStream = 0;

		if (!_readOnly && isReadOnly())
		{
			mode |= std::ios::out;
			rwStream = OGRE_NEW_T(std::fstream, MEMCATEGORY_GENERAL)();
			rwStream->open(wfullpath.c_str(), mode);
			baseStream = rwStream;
		}
		else
		{
			roStream = OGRE_NEW_T(std::ifstream, MEMCATEGORY_GENERAL)();
			roStream->open(wfullpath.c_str(), mode);
			baseStream = roStream;
		}


		// Should check ensure open succeeded, in case fail for some reason.
		if (baseStream->fail())
		{
			OGRE_DELETE_T(roStream, basic_ifstream, MEMCATEGORY_GENERAL);
			OGRE_DELETE_T(rwStream, basic_fstream, MEMCATEGORY_GENERAL);
			OGRE_EXCEPT(Ogre::Exception::ERR_FILE_NOT_FOUND,
				"Cannot open file: " + _filename,
				"FileSystemUtf8Archive::open");
		}

		/// Construct return stream, tell it to delete on destroy
		FileStreamDataStream* stream = 0;
		if (rwStream)
		{
			// use the writeable stream 
			stream = OGRE_NEW FileStreamDataStream(_filename,
				rwStream, tagStat.st_size, true);
		}
		else
		{
			// read-only stream
			stream = OGRE_NEW FileStreamDataStream(_filename,
				roStream, tagStat.st_size, true);
		}
		return DataStreamPtr(stream);
    }
	//---------------------------------------------------------------------
	Ogre::DataStreamPtr FileSystemUtf8Archive::create(const String& _filename) const
	{
		if (isReadOnly())
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, 
				"Cannot create a file in a read-only archive", 
				"FileSystemUtf8Archive::remove");
		}

		std::wstring wfullpath;
		concatenate_path(mWName, _filename, &wfullpath);

		// Always open in binary mode
		// Also, always include reading
		std::ios::openmode mode = std::ios::out | std::ios::binary;
		std::fstream* rwStream = OGRE_NEW_T(std::fstream, MEMCATEGORY_GENERAL)();
		rwStream->open(wfullpath.c_str(), mode);

		// Should check ensure open succeeded, in case fail for some reason.
		if (rwStream->fail())
		{
			OGRE_DELETE_T(rwStream, basic_fstream, MEMCATEGORY_GENERAL);
			OGRE_EXCEPT(Ogre::Exception::ERR_FILE_NOT_FOUND,
				"Cannot open file: " + _filename,
				"FileSystemUtf8Archive::create");
		}

		/// Construct return stream, tell it to delete on destroy
		FileStreamDataStream* stream = OGRE_NEW FileStreamDataStream(_filename,
				rwStream, 0, true);

		return DataStreamPtr(stream);
	}
	//---------------------------------------------------------------------
	void FileSystemUtf8Archive::remove(const String& _filename) const
	{
		if (isReadOnly())
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, 
				"Cannot remove a file from a read-only archive", 
				"FileSystemUtf8Archive::remove");
		}
		std::wstring wfullpath;
		concatenate_path(mWName, _filename, &wfullpath);
		::_wremove(wfullpath.c_str());
	}
    //-----------------------------------------------------------------------
    StringVectorPtr FileSystemUtf8Archive::list(bool _recursive, bool _dirs)
    {
		// directory change requires locking due to saved returns
		// Note that we have to tell the SharedPtr to use OGRE_DELETE_T not OGRE_DELETE by passing category
		StringVectorPtr lst(OGRE_NEW_T(StringVector, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);
		
		FileFinder fileFinder(this, mWName, ms_IgnoreHidden);
		fileFinder.run(L"*", _recursive, _dirs, lst.getPointer(), nullptr);
        
		return lst;
    }
    //-----------------------------------------------------------------------
    FileInfoListPtr FileSystemUtf8Archive::listFileInfo(bool _recursive, bool _dirs)
    {
		// Note that we have to tell the SharedPtr to use OGRE_DELETE_T not OGRE_DELETE by passing category
		FileInfoListPtr lst(OGRE_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);
		
		FileFinder fileFinder(this, mWName, ms_IgnoreHidden);
		fileFinder.run(L"*", _recursive, _dirs, nullptr, lst.getPointer());
        
		return lst;
    }
    //-----------------------------------------------------------------------
    StringVectorPtr FileSystemUtf8Archive::find(const String& _pattern,
                                            bool _recursive, bool _dirs)
    {
		// Note that we have to tell the SharedPtr to use OGRE_DELETE_T not OGRE_DELETE by passing category
		StringVectorPtr lst(OGRE_NEW_T(StringVector, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);

		std::wstring wpattern;
		StringUtil::utf8_to_utf16(_pattern, &wpattern);
		
		FileFinder fileFinder(this, mWName, ms_IgnoreHidden);
		fileFinder.run(wpattern, _recursive, _dirs, lst.getPointer(), nullptr);

        return lst;

    }
    //-----------------------------------------------------------------------
    FileInfoListPtr FileSystemUtf8Archive::findFileInfo(const String& _pattern, 
        bool _recursive, bool _dirs)
    {
		// Note that we have to tell the SharedPtr to use OGRE_DELETE_T not OGRE_DELETE by passing category
		FileInfoListPtr lst(OGRE_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);

		std::wstring wpattern;
		StringUtil::utf8_to_utf16(_pattern, &wpattern);
		FileFinder fileFinder(this, mWName, ms_IgnoreHidden);
		fileFinder.run(wpattern, _recursive, _dirs, nullptr, lst.getPointer());

        return lst;
    }
    //-----------------------------------------------------------------------
	bool FileSystemUtf8Archive::exists(const String& _filename)
	{
		std::wstring wfullpath;
		concatenate_path(mWName, _filename, &wfullpath);

        struct _stat tagStat;
        bool ret = (_wstat(wfullpath.c_str(), &tagStat) == 0);

//		// stat will return true if the filename is absolute, but we need to check
//		// the file is actually in this archive
//        if (ret && is_absolute_path(filename.c_str()))
//		{
//			// only valid if full path starts with our base
//#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
//			// case insensitive on windows
//			std::string lowerCaseName = mName;
//			StringUtil::toLowerCase(lowerCaseName);
//			ret = StringUtil::startsWith(full_path, lowerCaseName, true);
//#else
//			// case sensitive
//			ret = StringUtil::startsWith(full_path, mName, false);
//#endif
//		}

		return ret;
	}
	//---------------------------------------------------------------------
	time_t FileSystemUtf8Archive::getModifiedTime(const String& _filename)
	{
		std::wstring wfullpath;
		concatenate_path(mWName, _filename, &wfullpath);

		struct _stat tagStat;
		bool ret = (_wstat(wfullpath.c_str(), &tagStat) == 0);

		if (ret)
		{
			return tagStat.st_mtime;
		}
		else
		{
			return 0;
		}
	}
    //-----------------------------------------------------------------------
    const String& FileSystemUtf8ArchiveFactory::getType() const
    {
        static String name = "FileSystemUtf8";
        return name;
    }

}
