#include "Precompiled.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "WinUnicodeFileSystem.h"

#include <sys/types.h>
#include <sys/stat.h>
#define WIN32_LEAN_AND_MEAN
#if !defined(NOMINMAX) && defined(_MSC_VER)
#	define NOMINMAX // required to stop windows.h messing up std::min
#endif
#include <windows.h>
#include <direct.h>
#include <io.h>

namespace GothOgre 
{
    UnicodeFileSystemArchive::UnicodeFileSystemArchive(const String& _name, const String& _archType )
        : Archive(_name, _archType)
    {
		mIgnoreHidden = true;
		mUtf16Converter = UnicodeUtil::openConverter("UTF-16LE");
    }
    //-----------------------------------------------------------------------
	UnicodeFileSystemArchive::~UnicodeFileSystemArchive()
	{
		unload();
	}    
	//-----------------------------------------------------------------------
	WString UnicodeFileSystemArchive::toWString( const String& _string ) const 
	{
		if(sizeof(String::value_type) == sizeof(WString::value_type))
		{
			return reinterpret_cast<const WString&>(_string);
		}
		else
		{
			CodepageConverter::ByteBuffer buf;
			mUtf16Converter->unicodeToBuffer(buf, _string);
			WString wstring( (LPCWSTR) buf.data(), buf.size() / 2);
			return wstring;
		}
	}
	//-----------------------------------------------------------------------
	String UnicodeFileSystemArchive::toString( const WString& _wstring ) const
	{
		if(sizeof(String::value_type) == sizeof(WString::value_type))
		{
			return reinterpret_cast<const String&>(_wstring);
		}
		else
		{
			CodepageConverter::ByteBuffer buf( (const uint8*) _wstring.c_str(), _wstring.size() * 2);
			String string;
			mUtf16Converter->bufferToUnicode(string, buf);
			return string;
		}
	}
	//-----------------------------------------------------------------------
	String UnicodeFileSystemArchive::toString( const wchar_t* _wstr ) const
	{
		if(sizeof(String::value_type) == sizeof(WString::value_type))
		{
			return String( (const String::value_type*) _wstr);
		}
		else
		{
			CodepageConverter::ByteBuffer buf( (const uint8*) _wstr, wcslen(_wstr) * 2);
			String string;
			mUtf16Converter->bufferToUnicode(string, buf);
			return string;
		}
	}
	//-----------------------------------------------------------------------
	bool UnicodeFileSystemArchive::isCaseSensitive() const
	{
		return false;
	}
	//-----------------------------------------------------------------------
	bool UnicodeFileSystemArchive::isAbsolutePath(const WString& _wFilename) const
	{
		const wchar_t* p = _wFilename.c_str();
		if (iswalpha(p[0]) && p[1] == ':')
			return true;

		return p[0] == '/' || p[0] == '\\';
	}
	//---------------------------------------------------------------------
	WString UnicodeFileSystemArchive::getFullPath(const WString& _wFilename) const
	{
		WString result;
		if(mWName.empty() || isAbsolutePath(_wFilename))
		{
			result = _wFilename;
		}
		else
		{
			result = mWName;
			result += L'/';
			result += _wFilename;
		}
		return result;
	}
	//---------------------------------------------------------------------
	WString UnicodeFileSystemArchive::getFullPath(const String& _filename) const
	{
		return getFullPath(toWString(_filename));
	}
	//-----------------------------------------------------------------------
	UnicodeFileSystemArchive::FileFinder::FileFinder(UnicodeFileSystemArchive* _archive, const WString& _wArchiveName, 
		bool _ignoreHidden)
	{
		mIgnoreHidden = _ignoreHidden;
		mArchive = _archive;
	}
	//-----------------------------------------------------------------------
	void UnicodeFileSystemArchive::FileFinder::run(const WString& _wpattern, bool _recursive, bool _dirs,
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

		WString wdir, wmask;
		if(pos1 == String::npos)
		{
			wmask = _wpattern;
		}
		else
		{
			wdir = _wpattern.substr(0, pos1 + 1);
			wmask = _wpattern.substr(pos1 + 1);
		}

		if(wmask == L"*")
			wmask.clear();

		String mask = mArchive->toString(wmask);
		String dir = mArchive->toString(wdir);
		WString wFullDir = mArchive->getFullPath(wdir);

		_run(dir, wFullDir);
	}
	//-----------------------------------------------------------------------
	bool UnicodeFileSystemArchive::FileFinder::isReservedDir(const wchar_t* _name) const 
	{
		return (_name[0] == '.' && (_name[1] == 0 || (_name[1] == '.' && _name[2] == 0)));
	}
	//-----------------------------------------------------------------------
	void UnicodeFileSystemArchive::FileFinder::_run(const String& _dir, const WString& _wFullDir)
	{
		wchar_t wFindPattern[MAX_PATH];
		wcscpy(wFindPattern, _wFullDir.c_str());
		wcscpy(&wFindPattern[_wFullDir.length()], L"*");
       
        long lHandle;
		struct _wfinddata_t wTagData;
        lHandle = _wfindfirst(wFindPattern, &wTagData);

		String  tagDataName;
		String  dir2;
		WString wFullDir2;

		while(lHandle != -1)
		{
			bool isSubDir = ((wTagData.attrib & _A_SUBDIR) != 0);
			bool isHidden = ((wTagData.attrib & _A_HIDDEN) != 0);
			if((!mIgnoreHidden || !isHidden)
				&& !isReservedDir(wTagData.name)
				&& ((isSubDir == mDirs) || (isSubDir && mRecursive)))
			{
				tagDataName = mArchive->toString(wTagData.name);

				if(isSubDir == mDirs
					&& (mMask.empty() || StrUtil::match(tagDataName, mMask)))
				{
					if (mSimpleList)
					{
						mSimpleList->push_back(String());
						String& back = mSimpleList->back();
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
					wFullDir2 += wTagData.name;
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
    void UnicodeFileSystemArchive::load()
    {
		OGRE_LOCK_AUTO_MUTEX

        // test to see if this folder is writeable
		mWName = toWString(mName);
		WString wTestPath = getFullPath("__testwrite.ogre");

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
    void UnicodeFileSystemArchive::unload()
    {
        // nothing to see here, move along
    }
    //-----------------------------------------------------------------------
	Ogre::DataStreamPtr UnicodeFileSystemArchive::open(const String& _filename, bool _readOnly) const
    {
		WString wfullpath = getFullPath(_filename);

		// Use filesystem to determine size 
		// (quicker than streaming to the end and back)
		struct _stat tagStat;
		int ret = _wstat(wfullpath.c_str(), &tagStat);
		if(ret != 0)
		{
			GOTHOGRE_EXCEPT(_filename << " - Problem getting file size"
				<< " (archive " << getName() << ").");
		}

		// Always open in binary mode
		// Also, always include reading
		std::ios::openmode mode = std::ios::in | std::ios::binary;
		std::istream* baseStream = 0;
		std::ifstream* roStream = 0;
		std::fstream* rwStream = 0;

		if (_readOnly)
		{
			roStream = OGRE_NEW_T(std::ifstream, MEMCATEGORY_GENERAL)();
			roStream->open(wfullpath.c_str(), mode);
			baseStream = roStream;
		}
		else
		{
			if (isReadOnly())
			{
				GOTHOGRE_EXCEPT(_filename << " - Cannot open a file for writing in"
					<< " read-only archive " << getName() << ".");
			}
			mode |= std::ios::out;
			rwStream = OGRE_NEW_T(std::fstream, MEMCATEGORY_GENERAL)();
			rwStream->open(wfullpath.c_str(), mode);
			baseStream = rwStream;
		}

		// Should check ensure open succeeded, in case fail for some reason.
		if (baseStream->fail())
		{
			OGRE_DELETE_T(roStream, basic_ifstream, MEMCATEGORY_GENERAL);
			OGRE_DELETE_T(rwStream, basic_fstream, MEMCATEGORY_GENERAL);
			GOTHOGRE_EXCEPT(_filename << " - Cannot open file in"
				<< " archive " << getName() << ".");
		}

		GOTHOGRE_INFO(_filename << " - " << (_readOnly ? "Loading from" : "Saving to")
			<< " archive " << getName() << ".");

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
	Ogre::DataStreamPtr UnicodeFileSystemArchive::create(const String& _filename) const
	{
		if (isReadOnly())
		{
			GOTHOGRE_EXCEPT(_filename << " - Cannot create a file in"
				<< " read-only archive " << getName() << ".");
		}

		WString wfullpath = getFullPath(_filename);

		// Always open in binary mode
		// Also, always include reading
		std::ios::openmode mode = std::ios::out | std::ios::binary;
		std::fstream* rwStream = OGRE_NEW_T(std::fstream, MEMCATEGORY_GENERAL)();
		rwStream->open(wfullpath.c_str(), mode);

		// Should check ensure open succeeded, in case fail for some reason.
		if (rwStream->fail())
		{
			OGRE_DELETE_T(rwStream, basic_fstream, MEMCATEGORY_GENERAL);
			GOTHOGRE_EXCEPT(_filename << " - Cannot create file in"
				<< " archive " << getName() << ".");
		}

		GOTHOGRE_INFO(_filename << " - " << "Saving to"
			<< " archive " << getName() << ".");

		/// Construct return stream, tell it to delete on destroy
		FileStreamDataStream* stream = OGRE_NEW FileStreamDataStream(_filename,
				rwStream, 0, true);

		return DataStreamPtr(stream);
	}
	//---------------------------------------------------------------------
	void UnicodeFileSystemArchive::remove(const String& _filename) const
	{
		if (isReadOnly())
		{
			GOTHOGRE_EXCEPT(_filename << " - Cannot remove a file from"
				<< " read-only archive " << getName() << ".");
		}
		
		WString wfullpath = getFullPath(_filename);
		int ret = ::_wremove(wfullpath.c_str());

		if(ret == 0)
			GOTHOGRE_INFO(_filename << " - Removed from"
				<< " archive " << getName() << ".");
	}
    //-----------------------------------------------------------------------
    StringVectorPtr UnicodeFileSystemArchive::list(bool _recursive, bool _dirs)
    {
		// directory change requires locking due to saved returns
		// Note that we have to tell the SharedPtr to use OGRE_DELETE_T not OGRE_DELETE by passing category
		StringVectorPtr lst(OGRE_NEW_T(StringVector, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);
		
		FileFinder fileFinder(this, mWName, getIgnoreHidden());
		fileFinder.run(L"*", _recursive, _dirs, lst.getPointer(), nullptr);
        
		return lst;
    }
    //-----------------------------------------------------------------------
    FileInfoListPtr UnicodeFileSystemArchive::listFileInfo(bool _recursive, bool _dirs)
    {
		// Note that we have to tell the SharedPtr to use OGRE_DELETE_T not OGRE_DELETE by passing category
		FileInfoListPtr lst(OGRE_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);
		
		FileFinder fileFinder(this, mWName, getIgnoreHidden());
		fileFinder.run(L"*", _recursive, _dirs, nullptr, lst.getPointer());
        
		return lst;
    }
    //-----------------------------------------------------------------------
    StringVectorPtr UnicodeFileSystemArchive::find(const String& _pattern,
                                            bool _recursive, bool _dirs)
    {
		// Note that we have to tell the SharedPtr to use OGRE_DELETE_T not OGRE_DELETE by passing category
		StringVectorPtr lst(OGRE_NEW_T(StringVector, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);

		WString wpattern = toWString(_pattern);
		FileFinder fileFinder(this, mWName, getIgnoreHidden());
		fileFinder.run(wpattern, _recursive, _dirs, lst.getPointer(), nullptr);

        return lst;

    }
    //-----------------------------------------------------------------------
    FileInfoListPtr UnicodeFileSystemArchive::findFileInfo(const String& _pattern, 
        bool _recursive, bool _dirs) const
    {
		// Note that we have to tell the SharedPtr to use OGRE_DELETE_T not OGRE_DELETE by passing category
		FileInfoListPtr lst(OGRE_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);

		WString wpattern = toWString(_pattern);
		FileFinder fileFinder(const_cast<UnicodeFileSystemArchive*>(this), mWName, getIgnoreHidden());
		fileFinder.run(wpattern, _recursive, _dirs, nullptr, lst.getPointer());

        return lst;
    }
    //-----------------------------------------------------------------------
	bool UnicodeFileSystemArchive::exists(const String& _filename)
	{
		WString wfullpath = getFullPath(_filename);

        struct _stat tagStat;
        bool ret = (_wstat(wfullpath.c_str(), &tagStat) == 0);
		return ret;
	}
	//---------------------------------------------------------------------
	time_t UnicodeFileSystemArchive::getModifiedTime(const String& _filename)
	{
		WString wfullpath = getFullPath(_filename);

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
	UnicodeFileSystemArchiveFactory::UnicodeFileSystemArchiveFactory()
	{
	}
	//-----------------------------------------------------------------------------
	UnicodeFileSystemArchiveFactory::~UnicodeFileSystemArchiveFactory()
	{
	}
	//-----------------------------------------------------------------------------
	const String& UnicodeFileSystemArchiveFactory::getType() const
	{
		static const String sType = "UnicodeFileSystem";
		return sType;
	}
	//-----------------------------------------------------------------------------
	Archive* UnicodeFileSystemArchiveFactory::createInstance( const String& _name )
	{
		return OGRE_NEW UnicodeFileSystemArchive(_name, getType());		
	}
	//-----------------------------------------------------------------------------
	void UnicodeFileSystemArchiveFactory::destroyInstance( Archive* _arch)
	{
		delete _arch;
	}
}

#endif // #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32

