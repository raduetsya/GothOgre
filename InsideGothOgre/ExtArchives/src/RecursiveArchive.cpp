#include "ExtArchives_Precompiled.h"
#include "ExtArchives_CommonInclude.h"
#include "RecursiveArchive.h"

namespace GothOgre
{
	RecursiveArchive::RecursiveArchive(const String& _name, const String& _archType, const String& _baseArchType )
		: DerivedArchive(_name, _archType, _baseArchType)
	{
	}
	//-----------------------------------------------------------------------------------
	RecursiveArchive::~RecursiveArchive()
	{
	}
	//-----------------------------------------------------------------------------------
	String RecursiveArchive::getPath(const String& _filename)
	{
		String filepath = _filename;
		if(!getBaseArchive()->exists(_filename))
		{
			StringVectorPtr vecptr = getBaseArchive()->find(_filename, true, false);
			if(!vecptr->empty())
			{
				filepath = (*vecptr)[0];
				if(vecptr->size() != 1)
				{
					GOTHOGRE_LOG(Warning, "RecursiveArchive: Found two files"
						" with the same name '" << _filename << "'"
						<< ": '" << (*vecptr)[0] << "', '" << (*vecptr)[1] << "'."
						<< "One of these files will be unavailable.");
				}
			}
		}
		return filepath;
	}
    //-----------------------------------------------------------------------
	bool RecursiveArchive::exists(const String& _filename)
	{
		return getBaseArchive()->exists(getPath(_filename));
	}
	//---------------------------------------------------------------------
	time_t RecursiveArchive::getModifiedTime(const String& _filename)
	{
		return mBaseArchive->getModifiedTime(getPath(_filename));
	}
    //-----------------------------------------------------------------------
	Ogre::DataStreamPtr RecursiveArchive::open(const String& _filename, bool _readOnly) const
    {
		return getBaseArchive->open(getPath(_filename), _readOnly);
    }
    //-----------------------------------------------------------------------
	Ogre::DataStreamPtr RecursiveArchive::create(const String& _filename) const
	{
		return getBaseArchive()->create(getPath(_filename));
	}
    //-----------------------------------------------------------------------
	void RecursiveArchive::remove(const String& _filename) const
    {
		return getBaseArchive()->remove(getPath(_filename));
	}

} // namespace GothOgre


/*	//-----------------------------------------------------------------------------------
	void RecursiveArchive::scanArchive()
	{
		resetScanData();

		// getting file information about all files in this archive
		// to get faster following searching files in the archive
		StringVectorPtr srcptr = getBaseArchive()->list(true, false);
		StringVector

		String basename;
		for(size_t i = 0; i != lst.size(); ++i)
		{
			String& path = lst[i];
			if(!isCaseSensitive())
			{
				StringUtil::toLowerCase(path);
			}

			retrieveBasename(path, basename);
			if(mPathByBasename.find(basename) == mPathByBasename.end())
			{
				mPathByBasename.insert(std::make_pair(basename, path));
				lst[i] = basename;
			}
			else
			{
				GOTHOGRE_LOG(Warning, "RecursiveArchive: Found two files with the same name '" << basename << "'. One of these files will be unavailable.");
			}
		}

		mBasenameListByPattern.push_back("*", lst);
	}
	//-----------------------------------------------------------------------------------
	void RecursiveArchive::resetScanData()
	{
		mBasenameListByPattern.clear();
		mPathByBasename.clear();
	}


	//---------------------------------------------------------------------
	void RecursiveArchive::retrieveBasename(const String& _filename, String& _outBasename)
	{
		// getting base name by removing directory path
		size_t i = _filename.find_last_of("\\/");
		if(i != String::npos)
			_outBasename = _filename.substr(i + 1);
		else
			_outBasename = _filename;
	}
	//-----------------------------------------------------------------------
    StringVectorPtr RecursiveArchive::list(bool _recursive, bool _dirs)
    {
		return find("*", _recursive, _dirs);
	}
    //-----------------------------------------------------------------------
	StringVectorPtr RecursiveArchive::find(const String& _pattern, bool _recursive, bool _dirs)
	{
		static const BLANK = StringVectorPtr(OGRE_NEW_T(StringVector, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);
		StringVectorPtr ret = BLANK;
		
		if(!_dirs)
		{
			BasenameListByPath::iterator i = mBasenameListByPath.find(_pattern);
			if(i != mBasenameListByPath.end())
			{
				ret = i->second;
			}
			else
			{
				i = mBasenameListByPath.find("*");
				if(i != mBasenameListByPath.end())
				{
					StringVectorPtr allBasenames = i->second;
					ret->reserve(allBasenames.size());
					for(size_t j = 0; j != allBasenames.size(); ++j)
					{
						const String& basename = allBasenames[j];
						if(StringUtil::match(filename, _pattern))
							ret->push_back(filename);
					}
					mBasenameListByPath.insert(std::pair(_pattern, ret));
				}
			}
		}
		return ret;
    }
	//---------------------------------------------------------------------
	String RecursiveArchive::getPath(const String& _filename) const
	{
		// getting base name by removing directory path
		size_t i = _filename.find_last_of("\\/");
		if(i != String::npos)
			return _filename;
		else
		{
			// convert name to lower case if we must do case insensitive search
			const String& basename = isCaseSensitive() ? _filename : StringUtil::toLowerCase_t(_filename);

			// search file by its base name
			PathByBasename::const_iterator it = mPathByBasename.find(basename);
			return (it != mPathByBasename.end()) ? (it->second) : basename;
		}
	}

    //-----------------------------------------------------------------------
	Ogre::DataStreamPtr RecursiveArchive::_create(const String& _filename)
    {

		Ogre::DataStreamPtr dataStream = getBaseArchive->create(_filename);
		if(!dataStream.isNull())
		{
			String basename;
			retrieveBasename(_filename, basename);
			PathByBasename::iterator i = mPathByBasename.find(basename);
			if(i == mPathByBasename.end())
			{
				mPathByBasename.insert(std::pair(basename, _filename));
				for(BasenameListByPattern::iterator j = mBasenameListByPattern.begin(); 
					j != mBasenameListByPattern.end(); ++j)
				{
					const String& pattern = j->first;
					StringVector* lst = *(j->second);
					if(StringUtil::match(basename, pattern))
					{
						StringVector::iterator k = std::upper_bound(lst.begin(), lst.end(), basename);
						lst.insert(k, basename);
					}
				}
			}
			else
			{
				i->second = _filename;
			}
		}
		return dataStream;
	}
    //-----------------------------------------------------------------------
	void RecursiveArchive::_remove(const String& _filename)
	{
		getBaseArchive->remove(_filename);
		
		String basename;
		retrieveBasename(_filename, basename);
		PathByBasename::iterator i = mPathByBasename.find(basename);
		if(i != mPathByBasename.end())
		{
			mPathByBasename.erase(i);
			for(BasenameListByPattern::iterator j = mBasenameListByPattern.begin(); 
				j != mBasenameListByPattern.end(); ++j)
			{
				const String& pattern = j->first;
				StringVector& lst = *(j->second);
				if(StringUtil::match(basename, pattern))
				{
					StringVector::iterator k = std::lower_bound(lst.begin(), lst.end(), basename);
					if(k != lst.end())
						lst.erase(k);
				}
			}
		}
	}
*/