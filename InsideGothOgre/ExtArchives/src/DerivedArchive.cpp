#include "Precompiled.h"
#include "DerivedArchive.h"

namespace GothOgre
{
	class ArchiveManagerHack : public ArchiveManager
	{
	public:
		Archive* createArchiveWithoutLoading(const String& _name, const String& _archType)
		{
			// Search factories
			ArchiveFactoryMap::iterator it = mArchFactories.find(_archType);
			if (it == mArchFactories.end())
				// Factory not found
				OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "Cannot find an archive factory "
					"to deal with archive of type " + _archType, "ArchiveManager::load");

			return it->second->createInstance(_name);
		}

		void destroyArchive(Archive* _archive)
		{
			if(_archive)
			{
				_archive->unload();
				
				// Find factory to destroy
				ArchiveFactoryMap::iterator fit = mArchFactories.find(_archive->getType());
				if (fit == mArchFactories.end())
				{
					// Factory not found
					OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "Cannot find an archive factory "
						"to deal with archive of type " + _archive->getType(), "ArchiveManager::~ArchiveManager");
				}
				fit->second->destroyInstance(_archive);
			}
		}
	};
	//------------------------------------------------------------------------------
	static Archive* createArchiveWithoutLoading(const String& _name, const String& _archType)
	{
		Archive* archive = nullptr;
		if(ArchiveManager::getSingletonPtr())
		{
			ArchiveManagerHack* hack = reinterpret_cast<ArchiveManagerHack*>(ArchiveManager::getSingletonPtr());
			archive = hack->createArchiveWithoutLoading(_name, _archType);
		}
		return archive;
	}
	//------------------------------------------------------------------------------
	static void destroyArchive(Archive* _archive)
	{
		if(ArchiveManager::getSingletonPtr())
		{
			ArchiveManagerHack* hack = reinterpret_cast<ArchiveManagerHack*>(ArchiveManager::getSingletonPtr());
			hack->destroyArchive(_archive);
		}
	}
	//------------------------------------------------------------------------------
	DerivedArchive::DerivedArchive(const String& _name, const String& _archType, const String& _baseArchType )
		: Archive(_name, _archType)
	{
		mBaseArchive = createArchiveWithoutLoading(_name, _baseArchType);
	}
	//------------------------------------------------------------------------------
	DerivedArchive::~DerivedArchive()
	{
		destroyArchive(mBaseArchive);
	}
	//------------------------------------------------------------------------------
	bool DerivedArchive::isCaseSensitive() const
	{
		return getBaseArchive()->isCaseSensitive();
	}
	//------------------------------------------------------------------------------
	void DerivedArchive::load()
	{
		getBaseArchive()->load();
	}
	//------------------------------------------------------------------------------
	void DerivedArchive::unload()
	{
		getBaseArchive()->unload();
	}
	//------------------------------------------------------------------------------
	bool DerivedArchive::isReadOnly() const
	{
		return getBaseArchive()->isReadOnly();
	}
	//------------------------------------------------------------------------------
	StringVectorPtr DerivedArchive::list(bool _recursive, bool _dirs)
	{
		return getBaseArchive()->list(_recursive, _dirs);
	}
	//------------------------------------------------------------------------------
	FileInfoListPtr DerivedArchive::listFileInfo(bool _recursive, bool _dirs)
	{
		return getBaseArchive()->listFileInfo(_recursive, _dirs);
	}
	//------------------------------------------------------------------------------
	StringVectorPtr DerivedArchive::find(const String& _pattern, bool _recursive, bool _dirs)
	{
		return getBaseArchive()->find(_pattern, _recursive, _dirs);
	}
	//------------------------------------------------------------------------------
	FileInfoListPtr DerivedArchive::findFileInfo(const String& _pattern, bool _recursive, bool _dirs) const
	{
		return getBaseArchive()->findFileInfo(_pattern, _recursive, _dirs);
	}
	//------------------------------------------------------------------------------
	bool DerivedArchive::exists(const String& _filename)
	{
		return getBaseArchive()->exists(_filename);
	}
	//------------------------------------------------------------------------------
	time_t DerivedArchive::getModifiedTime(const String& _filename)
	{
		return getBaseArchive()->getModifiedTime(_filename);
	}
	//------------------------------------------------------------------------------
	Ogre::DataStreamPtr DerivedArchive::open(const String& _filename, bool _readOnly) const
	{
		return getBaseArchive()->open(_filename, _readOnly);
	}
	//------------------------------------------------------------------------------
	Ogre::DataStreamPtr DerivedArchive::create(const String& _filename) const
	{
		return getBaseArchive()->create(_filename);
	}
	//------------------------------------------------------------------------------
	void DerivedArchive::remove(const String& _filename) const
	{
		return getBaseArchive()->remove(_filename);
	}

} // namespace GothOgre