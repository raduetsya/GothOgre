#include "GothOgre_Precompiled.h"
#include "GothOgre_ResourceLoader.h"
#include "GothOgre_UnicodeUtil.h"

namespace GothOgre
{
	//-----------------------------------------------------------------------
	// ResourceLoader
	//----------------------------------------------------------------------
	ResourceLoader::ResourceLoader(ResourceLoaderEnumerator* _enumerator, const String& _name, Priority _priority)
	{
		mName = _name;
		mLoaderEnumerator = _enumerator;
		mLoaderEnumerator->addResourceLoader(this, _priority);
	}
	//----------------------------------------------------------------------
	ResourceLoader::~ResourceLoader()
	{
		mLoaderEnumerator->removeResourceLoader(this);
	}
	//----------------------------------------------------------------------
	bool ResourceLoader::canLoad(String& _filename, const String& _resourceGroup) const
	{
		if(getFileExt().empty())
			return false;

		size_t dotpos = _filename.rfind('.');
		String fileext = (dotpos != String::npos) ? _filename.substr(dotpos) : StringUtil::BLANK;
		if(!UnicodeUtil::equalsNoCase(fileext, getFileExt()))
			return false;

		if(!ResourceGroupManager::getSingleton().resourceExists(_resourceGroup, _filename))
			return false;

		return true;
	}
	//----------------------------------------------------------------------
	const String& ResourceLoader::getFileExt() const
	{
		return StringUtil::BLANK;
	}
	//----------------------------------------------------------------------
	size_t ResourceLoader::getMaxContexts() const
	{
		return 10;
	}
	
	
	
	
	//-----------------------------------------------------------------------
	// ResourceLoaderEnumerator
	//-----------------------------------------------------------------------
	ResourceLoaderEnumerator::ResourceLoaderEnumerator()
	{
	}
	//-----------------------------------------------------------------------
	ResourceLoaderEnumerator::~ResourceLoaderEnumerator()
	{
	}
	//-----------------------------------------------------------------------
	const StringVector& ResourceLoaderEnumerator::getAllResourceLoaders() const
	{
		return mAllLoaderNames;
	}
	//-----------------------------------------------------------------------
	bool ResourceLoaderEnumerator::canLoad(Resource* _destResource) const
	{
		return canLoad( _destResource->getName(), _destResource->getGroup());
	}
	//-----------------------------------------------------------------------
	bool ResourceLoaderEnumerator::canLoad(const String& _filename, const String& _resourceGroup) const
	{
		String filename = _filename;
		ResourceLoader* loader = findLoaderCanLoad(filename, _resourceGroup);
		return loader != nullptr;
	}
	//-----------------------------------------------------------------------
	void ResourceLoaderEnumerator::load(Resource* _resource)
	{
		Extra& extra = getExtra(_resource);
		extra.filename = _resource->getName();
		extra.group = _resource->getGroup();
		extra.loaderName.clear();
		extra.dataStream.setNull();
		_resource->load();
	}
	//-----------------------------------------------------------------------
	void ResourceLoaderEnumerator::load(Resource* _resource, const String& _resourceLoaderName)
	{
		Extra& extra = getExtra(_resource);
		extra.filename = _resource->getName();
		extra.group = _resource->getGroup();
		extra.loaderName = _resourceLoaderName;
		extra.dataStream.setNull();
		_resource->load();
	}
	//-----------------------------------------------------------------------
	void ResourceLoaderEnumerator::load(Resource* _resource, const String& _filename, const String& _resourceGroup)
	{
		Extra& extra = getExtra(_resource);
		extra.filename = _filename;
		extra.group = _resourceGroup;
		extra.loaderName.clear();
		extra.dataStream.setNull();
		_resource->load();
	}
	//-----------------------------------------------------------------------
	void ResourceLoaderEnumerator::load(Resource* _resource, const String& _filename, const String& _resourceGroup, const String& _resourceLoaderName)
	{
		Extra& extra = getExtra(_resource);
		extra.filename = _filename;
		extra.group = _resourceGroup;
		extra.loaderName = _resourceLoaderName;
		extra.dataStream.setNull();
		_resource->load();
	}
	//-----------------------------------------------------------------------
	void ResourceLoaderEnumerator::load(Resource* _resource, const DataStreamPtr& _dataStream, const String& _resourceLoaderName)
	{
		Extra& extra = getExtra(_resource);
		extra.filename = _dataStream->getName();
		extra.group.clear();
		extra.loaderName = _resourceLoaderName;
		extra.dataStream = _dataStream;
		_resource->load();
	}
	//-----------------------------------------------------------------------
	void ResourceLoaderEnumerator::skip(const DataStreamPtr& _dataStream, const String& _resourceLoaderName)
	{
		ResourceLoader* loader = getLoaderByName(_resourceLoaderName);
		ResourceLoadContext* context = createLoadContext(loader);
		context->skip(_dataStream);
		freeLoadContext(context, loader);
	}
	//-----------------------------------------------------------------------
	void ResourceLoaderEnumerator::prepareResource(Resource* _resource)
	{
		Extra& extra = getExtra(_resource);
		extra.loader = nullptr;
		extra.context = nullptr;
		extra.prepareBegan = true;

		bool partOfGreaterResource = !extra.dataStream.isNull();

		// Get filename
		if(extra.filename.empty())
		{
			if(!extra.dataStream.isNull())
				extra.filename = extra.dataStream->getName();
			else
				extra.filename = _resource->getName();
		}

		// Get resource group
		if(extra.group.empty())
			extra.group = _resource->getGroup();

		try
		{
			// Get loader
			if(!extra.loaderName.empty())
			{
				extra.loader = getLoaderByName( extra.loaderName );
			}
			else
			{
				extra.loader = findLoaderCanLoad( extra.filename, extra.group );
				if( !extra.loader )
				{
					if(ResourceGroupManager::getSingleton().resourceExists(extra.group, extra.filename))
					{
						GOTHOGRE_EXCEPT( extra.filename 
							<< " - Loader not found for this file extension.");
					}
					else
					{
						GOTHOGRE_EXCEPT( extra.filename 
							<< " - File not found.");
					}
				}
			}

			// Open data stream
			if(extra.dataStream.isNull())
			{
				extra.dataStream = ResourceGroupManager::getSingleton().
					openResource( extra.filename, extra.group, false );
			}

			// Get loading context.
			if(!extra.context)
				extra.context = createLoadContext( extra.loader );

			// Get resource into the context.
			extra.context->load( extra.dataStream );
			extra.dataStream.setNull();

			// Loaded successfully
			extra.prepareEnded = true;
		}
		catch(Exception& _exception)
		{
			extra.dataStream.setNull();

			if(partOfGreaterResource)
			{
				removeExtra( _resource );
				throw;
			}
			else
				GOTHOGRE_ERROR(_exception.getDescription());
		}
		catch(Ogre::Exception& _exception)
		{
			extra.dataStream.setNull();

			if(partOfGreaterResource)
			{
				removeExtra( _resource );
				throw;
			}
			else
				GOTHOGRE_ERROR(_exception.getDescription());
		}
	}
	//-----------------------------------------------------------------------
	void ResourceLoaderEnumerator::loadResource(Resource* _resource)
	{
		Extra& extra = getExtra(_resource);
		if(!extra.prepareBegan)
			prepareResource(_resource);

		if(extra.prepareEnded)
		{
			try
			{
				extra.context->apply(_resource);
			}
			catch(Exception& _exception)
			{
				GOTHOGRE_ERROR(_exception.getDescription());
			}
			catch(Ogre::Exception& _exception)
			{
				GOTHOGRE_ERROR(_exception.getDescription());
			}
		}

		removeExtra(_resource);
	}
	//-----------------------------------------------------------------------
	ResourceLoaderEnumerator::Extra& ResourceLoaderEnumerator::getExtra(Resource* _resource)
	{
		OGRE_LOCK_AUTO_MUTEX
		ExtraByResource::iterator it = mExtraByResource.find(_resource);
		if(it == mExtraByResource.end())
		{
			it = mExtraByResource.insert(std::make_pair(_resource, Extra())).first;
			Extra& extra = it->second;
			extra.context = nullptr;
			extra.loader  = nullptr;
			extra.prepareBegan = false;
			extra.prepareEnded = false;
		}
		Extra& extra = it->second;
		return extra;
	}
	//-----------------------------------------------------------------------
	void ResourceLoaderEnumerator::removeExtra(Resource* _resource)
	{
		OGRE_LOCK_AUTO_MUTEX
		ExtraByResource::iterator it = mExtraByResource.find(_resource);
		if(it != mExtraByResource.end())
		{
			Extra& extra = it->second;
			freeLoadContext(extra.context, extra.loader);
			mExtraByResource.erase(it);
		}
	}
	//-----------------------------------------------------------------------
	void ResourceLoaderEnumerator::freeLoadContext(ResourceLoadContext* _context, ResourceLoader* _loader)
	{
		if(_loader && _context)
		{
			size_t& numContexts = getNumContextsByLoader(_loader);
			if(numContexts > _loader->getMaxContexts())
			{
				delete _context;
				--numContexts;
			}
			else
			{
				mFreeLoadContextsByLoader.insert(std::make_pair(_loader, _context));
			}
		}
	}
	//-----------------------------------------------------------------------
	ResourceLoadContext* ResourceLoaderEnumerator::createLoadContext(ResourceLoader* _loader)
	{
		OGRE_LOCK_AUTO_MUTEX
		size_t& numContexts = getNumContextsByLoader(_loader);
		ResourceLoadContext* context;
		FreeLoadContextsByLoader::iterator it;
		it = mFreeLoadContextsByLoader.find(_loader);
		if(it != mFreeLoadContextsByLoader.end())
		{
			context = it->second;
			mFreeLoadContextsByLoader.erase(it);
		}
		else
		{
			++numContexts;
			context = _loader->createLoadContext();
		}
		return context;
	}
	//-----------------------------------------------------------------------
	size_t& ResourceLoaderEnumerator::getNumContextsByLoader(ResourceLoader* _loader)
	{
		OGRE_LOCK_AUTO_MUTEX
		NumContextsByLoader::iterator it = mNumContextsByLoader.find(_loader);
		if(it == mNumContextsByLoader.end())
		{
			it = mNumContextsByLoader.insert(std::make_pair(_loader, (size_t) 0)).first;
		}
		return it->second;
	}
	//-----------------------------------------------------------------------
	ResourceLoader* ResourceLoaderEnumerator::findLoaderCanLoad(String& _filename, const String& _resourceGroup) const
	{
		String filename2;
		for(LoadersByPriority::const_reverse_iterator it = mLoadersByPriority.rbegin();
			it != mLoadersByPriority.rend(); ++it)
		{
			ResourceLoader* loader = it->second;
			filename2 = _filename;
			if(loader->canLoad(filename2, _resourceGroup))
			{
				_filename = filename2;
				return loader;
			}
		}
		return nullptr;
	}
	//-----------------------------------------------------------------------
	ResourceLoader* ResourceLoaderEnumerator::getLoaderByName(const String& _loaderName) const
	{
		LoadersByName::const_iterator it = mLoadersByName.find(_loaderName);
		if(it == mLoadersByName.end())
			GOTHOGRE_EXCEPT(_loaderName << " - Resource loader not found.");
		ResourceLoader* loader = it->second;
		return loader;
	}
	//-----------------------------------------------------------------------
	void ResourceLoaderEnumerator::addResourceLoader(ResourceLoader* _resourceLoader, Priority _priority)
	{
		mLoadersByPriority.insert(std::make_pair(_priority, _resourceLoader));
		mLoadersByName.insert(std::make_pair(_resourceLoader->getName(), _resourceLoader));
		mAllLoaderNames.push_back(_resourceLoader->getName());
	}
	//-----------------------------------------------------------------------
	void ResourceLoaderEnumerator::removeResourceLoader(ResourceLoader* _resourceLoader)
	{
		// remove all references to the removing loader
		{
			for(LoadersByPriority::iterator it = mLoadersByPriority.begin();
				it != mLoadersByPriority.end(); ++it)
			{
				if(it->second == _resourceLoader)
				{
					mLoadersByPriority.erase(it);
					break;
				}
			}
			mLoadersByName.erase(_resourceLoader->getName());
			mAllLoaderNames.erase(std::find(mAllLoaderNames.begin(), mAllLoaderNames.end(), _resourceLoader->getName()));
		}

		// delete free load contexts which have been created by the loader
		{
			std::pair<FreeLoadContextsByLoader::iterator, FreeLoadContextsByLoader::iterator> range;
			range = mFreeLoadContextsByLoader.equal_range(_resourceLoader);
			for(FreeLoadContextsByLoader::iterator it = range.first;
				it != range.second; ++it)
			{
				ResourceLoadContext* context = it->second;
				delete context;
			}
			mFreeLoadContextsByLoader.erase(range.first, range.second);
		}
		
		// delete contexts in extra data which have been created by the loader
		{
			for(ExtraByResource::iterator it = mExtraByResource.begin();
				it != mExtraByResource.end(); ++it)
			{
				Extra& extra = it->second;
				if(extra.loader == _resourceLoader)
				{
					if(extra.context)
					{
						delete extra.context;
						extra.context = nullptr;
					}
					extra.loader = nullptr;
				}
			}
		}

		// delete num contexts of the loader
		{
			NumContextsByLoader::iterator it = mNumContextsByLoader.find(_resourceLoader);
			size_t numContexts = 0;
			if(it != mNumContextsByLoader.end())
			{
				numContexts = it->second;
				mNumContextsByLoader.erase(it);
			}
			GOTHOGRE_INFO(_resourceLoader->getName() << ": numContexts = " << numContexts);
		}
	}

} // namespace GothOgre