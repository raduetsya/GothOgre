#include "GothOgre_Precompiled.h"
#include "GothOgre_ResourceSaver.h"
#include "GothOgre_UnicodeUtil.h"

namespace GothOgre
{
	//----------------------------------------------------------------------
	// ResourceSaver
	//----------------------------------------------------------------------
	ResourceSaver::ResourceSaver(ResourceSaverEnumerator* _enumerator, const String& _name, Priority _priority)
	{
		mName = _name;
		mSaverEnumerator = _enumerator;
		mSaverEnumerator->addResourceSaver(this, _priority);
	}
	//----------------------------------------------------------------------
	ResourceSaver::~ResourceSaver()
	{
		mSaverEnumerator->removeResourceSaver(this);
	}
	//----------------------------------------------------------------------
	bool ResourceSaver::canSave(String& _filename, const String& _resourceGroup) const
	{
		if(getFileExt().empty())
			return false;

		size_t dotpos = _filename.rfind('.');
		String fileext = (dotpos != String::npos) ? _filename.substr(dotpos) : StringUtil::BLANK;
		if(!UnicodeUtil::equalsNoCase(fileext, getFileExt()))
			return false;

		return true;
	}
	//----------------------------------------------------------------------
	const String& ResourceSaver::getFileExt() const
	{
		return StringUtil::BLANK;
	}


	//----------------------------------------------------------------------
	// ResourceSaverEnumerator
	//----------------------------------------------------------------------
	ResourceSaverEnumerator::ResourceSaverEnumerator()
	{
	}
	//-----------------------------------------------------------------------
	ResourceSaverEnumerator::~ResourceSaverEnumerator()
	{
	}
	//-----------------------------------------------------------------------
	const StringVector& ResourceSaverEnumerator::getAllResourceSavers() const
	{
		return mAllSaverNames;
	}
	//-----------------------------------------------------------------------
	bool ResourceSaverEnumerator::canSave(const String& _filename, const String& _resourceGroup) const
	{
		String filename = _filename;
		ResourceSaver* saver = findSaverCanSave(filename, _resourceGroup);
		return saver != nullptr;
	}
	//-----------------------------------------------------------------------
	void ResourceSaverEnumerator::save(const Resource* _resource, const String& _filename, const String& _resourceGroup)
	{
		try
		{
			String filename = _filename;
			ResourceSaver* saver = findSaverCanSave(filename, _resourceGroup);
			if(!saver)
				GOTHOGRE_EXCEPT(_filename << " - Saver not found for this file extension.");

			DataStreamPtr dataStream = ResourceGroupManager::getSingleton().createResource(filename, _resourceGroup, true);
			saveResource(_resource, dataStream, saver);
		}
		catch(Exception& _exception)
		{
			GOTHOGRE_ERROR(_exception.getDescription());
		}
	}
	//-----------------------------------------------------------------------
	void ResourceSaverEnumerator::save(const Resource* _resource, const String& _filename, const String& _resourceGroup, const String& _resourceSaverName)
	{
		try
		{
			String filename = _filename;
			ResourceSaver* saver = getSaverByName(_resourceSaverName);

			DataStreamPtr dataStream = ResourceGroupManager::getSingleton().createResource(filename, _resourceGroup, true);
			saveResource(_resource, dataStream, saver);
		}
		catch(Exception& _exception)
		{
			GOTHOGRE_ERROR(_exception.getDescription());
		}
	}
	//-----------------------------------------------------------------------
	void ResourceSaverEnumerator::saveResource(const Resource* _resource, const DataStreamPtr& _dataStream, ResourceSaver* _saver)
	{
		ResourceSaveContext* context = createSaveContext(_saver);
		try
		{
			context->perceive(_resource);
			context->save(_dataStream);
		}
		catch(Exception& _exception)
		{
			GOTHOGRE_ERROR(_exception.getDescription());
		}
		freeSaveContext(context, _saver);
	}
	//-----------------------------------------------------------------------
	ResourceSaveContext* ResourceSaverEnumerator::createSaveContext(ResourceSaver* _saver)
	{
		OGRE_LOCK_AUTO_MUTEX
		ResourceSaveContext* context;
		FreeSaveContextsBySaver::iterator it;
		it = mFreeSaveContextsBySaver.find(_saver);
		if(it != mFreeSaveContextsBySaver.end())
		{
			context = it->second;
			mFreeSaveContextsBySaver.erase(it);
		}
		else
		{
			context = _saver->createSaveContext();
		}
		return context;
	}
	//-----------------------------------------------------------------------
	void ResourceSaverEnumerator::freeSaveContext(ResourceSaveContext* _context, ResourceSaver* _saver)
	{
		if(_saver && _context)
			mFreeSaveContextsBySaver.insert(std::make_pair(_saver, _context));
	}
	//-----------------------------------------------------------------------
	ResourceSaver* ResourceSaverEnumerator::findSaverCanSave(String& _filename, const String& _resourceGroup) const
	{
		String filename2;
		for(SaversByPriority::const_reverse_iterator it = mSaversByPriority.rbegin();
			it != mSaversByPriority.rend(); ++it)
		{
			ResourceSaver* saver = it->second;
			filename2 = _filename;
			if(saver->canSave(filename2, _resourceGroup))
			{
				_filename = filename2;
				return saver;
			}
		}
		return nullptr;
	}
	//-----------------------------------------------------------------------
	ResourceSaver* ResourceSaverEnumerator::getSaverByName(const String& _saverName) const
	{
		SaversByName::const_iterator it = mSaversByName.find(_saverName);
		if(it == mSaversByName.end())
			GOTHOGRE_EXCEPT(_saverName << " - Resource saver not found.");
		ResourceSaver* saver = it->second;
		return saver;
	}
	//-----------------------------------------------------------------------
	void ResourceSaverEnumerator::addResourceSaver(ResourceSaver* _resourceSaver, Priority _priority)
	{
		mSaversByPriority.insert(std::make_pair(_priority, _resourceSaver));
		mSaversByName.insert(std::make_pair(_resourceSaver->getName(), _resourceSaver));
		mAllSaverNames.push_back(_resourceSaver->getName());
	}
	//-----------------------------------------------------------------------
	void ResourceSaverEnumerator::removeResourceSaver(ResourceSaver* _resourceSaver)
	{
		// remove all references to the removing saver
		{
			for(SaversByPriority::iterator it = mSaversByPriority.begin();
				it != mSaversByPriority.end(); ++it)
			{
				if(it->second == _resourceSaver)
				{
					mSaversByPriority.erase(it);
					break;
				}
			}
			mSaversByName.erase(_resourceSaver->getName());
			mAllSaverNames.erase(std::find(mAllSaverNames.begin(), mAllSaverNames.end(), _resourceSaver->getName()));
		}

		// delete free save contexts which have been created by the saver
		{
			size_t numContexts = 0;
			FreeSaveContextsBySaver::iterator it = mFreeSaveContextsBySaver.find(_resourceSaver);
			if(it != mFreeSaveContextsBySaver.end())
			{
				ResourceSaveContext* context = it->second;
				delete context;
				mFreeSaveContextsBySaver.erase(it);
				numContexts = 1;
			}
			GOTHOGRE_INFO(_resourceSaver->getName() << ": numContexts = " << numContexts << ".");
		}
	}

} // namespace GothOgre