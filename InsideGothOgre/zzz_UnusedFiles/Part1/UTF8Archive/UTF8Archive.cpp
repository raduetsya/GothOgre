#include "FlatArchives_StdHeaders.h"
#include "SynonymPath.h"
#include "Utf8Archive.h"

namespace GothOgre
{

	Utf8Archive::Utf8Archive(const String& _name, const String& _archType, const String& _baseArchType )
		: Archive(_name, _archType)
	{
		mBaseArchiveName = SynonymPath::calculate(_name);
		StringUtil::utf8ToAnsi(mBaseArchiveName);
		mBaseArchive = ArchiveManager::getSingleton().load(mBaseArchiveName, _baseArchType);
		GOTHOGRE_ASSERT(mBaseArchive != nullptr, "ArchiveFactory '" << _baseArchType << "' not found.");
	}
	//-----------------------------------------------------------------------------------
	Utf8Archive::~Utf8Archive()
	{
		if(isBaseArchiveAlive())
		{
			ArchiveManager::getSingleton().unload(mBaseArchive);
		}
	}
	//-----------------------------------------------------------------------------------
	bool Utf8Archive::isBaseArchiveAlive() const
	{
		Ogre::ArchiveManager::ArchiveMapIterator it = ArchiveManager::getSingleton().getArchiveIterator();
		while(it.hasMoreElements())
		{
			const String& key = it.peekNextKey();
			Ogre::Archive* arch = it.getNext();
			if(key == mBaseArchiveName)
			{
				return true;
			}
		}
		return false;
	}
	//-----------------------------------------------------------------------------------
	bool Utf8Archive::isCaseSensitive() const
	{
		if(mBaseArchive)
			return mBaseArchive->isCaseSensitive();
		else
			return true;
	}
	//-----------------------------------------------------------------------------------
	void Utf8Archive::load()
	{
		mBaseArchive->load();
	}
	//-----------------------------------------------------------------------------------
	void Utf8Archive::unload()
	{
		if(isBaseArchiveAlive())
			mBaseArchive->unload();
	}
    //-----------------------------------------------------------------------
	
	// Helper class to set name of data stream 
	// (mName is a protected member of class Ogre::DataStream)
	class DataStream_NameHelper : public Ogre::DataStream
	{
	public:
		void setName(const String& _name) {mName = _name;}
	};

	Ogre::DataStreamPtr Utf8Archive::open(const String& _filename, bool _readOnly) const
    {
		String filename2 = _filename;
		StringUtil::utf8ToAnsi(filename2);
		Ogre::DataStreamPtr stream = mBaseArchive->open(filename2, _readOnly);
		if(!stream.isNull())
			static_cast<DataStream_NameHelper*>(stream.get())->setName(_filename);
		return stream;
    }
	//-----------------------------------------------------------------------
	Ogre::DataStreamPtr Utf8Archive::create(const String& _filename) const
	{
		String filename2 = _filename;
		StringUtil::utf8ToAnsi(filename2);
		return mBaseArchive->create(filename2);
	}
	//-----------------------------------------------------------------------
	void Utf8Archive::remove(const String& _filename) const
	{
		String filename2 = _filename;
		StringUtil::utf8ToAnsi(filename2);
		mBaseArchive->remove(filename2);
	}
	//-----------------------------------------------------------------------
	StringVectorPtr Utf8Archive::list(bool _recursive, bool _dirs)
	{
		StringVectorPtr ret = mBaseArchive->list(_recursive, _dirs);
		for(size_t i = 0; i != ret->size(); ++i)
		{
			String& elem = (*ret)[i];
			StringUtil::ansiToUtf8(elem);
		}
		return ret;
	}
	//-----------------------------------------------------------------------
	FileInfoListPtr Utf8Archive::listFileInfo(bool _recursive, bool _dirs)
	{
		FileInfoListPtr ret = mBaseArchive->listFileInfo(_recursive, _dirs);
		for(size_t i = 0; i != ret->size(); ++i)
		{
			FileInfo& fi = (*ret)[i];
			StringUtil::ansiToUtf8(fi.basename);
			StringUtil::ansiToUtf8(fi.filename);
			StringUtil::ansiToUtf8(fi.path);
		}
		return ret;
	}
	//-----------------------------------------------------------------------
	StringVectorPtr Utf8Archive::find(const String& _pattern, bool _recursive, bool _dirs)
	{
		String pattern2 = _pattern;
		StringUtil::utf8ToAnsi(pattern2);
		StringVectorPtr ret = mBaseArchive->find(pattern2, _recursive, _dirs);
		for(size_t i = 0; i != ret->size(); ++i)
		{
			String& elem = (*ret)[i];
			StringUtil::ansiToUtf8(elem);
		}
		return ret;		
	}
	//-----------------------------------------------------------------------
	FileInfoListPtr Utf8Archive::findFileInfo(const String& _pattern, bool _recursive, bool _dirs)
	{
		String pattern2 = _pattern;
		StringUtil::utf8ToAnsi(pattern2);
		FileInfoListPtr ret = mBaseArchive->findFileInfo(pattern2, _recursive, _dirs);
		for(size_t i = 0; i != ret->size(); ++i)
		{
			FileInfo& fi = (*ret)[i];
			StringUtil::ansiToUtf8(fi.basename);
			StringUtil::ansiToUtf8(fi.filename);
			StringUtil::ansiToUtf8(fi.path);
		}
		return ret;		
	}
	//-----------------------------------------------------------------------
	bool Utf8Archive::exists(const String& _filename)
	{
		String filename2 = _filename;
		StringUtil::utf8ToAnsi(filename2);
		return mBaseArchive->exists(filename2);
	}
	//-----------------------------------------------------------------------
	time_t Utf8Archive::getModifiedTime(const String& _filename)
	{
		String filename2 = _filename;
		StringUtil::utf8ToAnsi(filename2);
		return mBaseArchive->getModifiedTime(filename2);
	}

} // namespace GothOgre