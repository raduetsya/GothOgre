#include "GothOgre_Precompiled.h"
#include "GothOgre_CachedResourceOgreManager.h"


namespace GothOgre
{
	//-----------------------------------------------------------------------
	CachedResourceOgreManager::CachedResourceOgreManager()
	{
		mNumFramesBeforeUnload = 1000;
	}
	//-----------------------------------------------------------------------
	CachedResourceOgreManager::~CachedResourceOgreManager()
	{
	}
	//-----------------------------------------------------------------------
	void CachedResourceOgreManager::setNumFramesBeforeUnload(size_t _numFrames) 
	{
		mNumFramesBeforeUnload = _numFrames;
	}
	//-----------------------------------------------------------------------
	size_t CachedResourceOgreManager::getNumFramesBeforeUnload() const 
	{
		return mNumFramesBeforeUnload;
	}
	//-----------------------------------------------------------------------
	void CachedResourceOgreManager::frameRenderingQueued(const FrameEvent& _evt)
	{
		NumFramesNotUsedMap::iterator it = mNumFramesNotUsedMap.begin();
		while(it != mNumFramesNotUsedMap.end())
		{
			ResourceHandle handle = it->first;
			size_t& numFramesNotUsed = (it++)->second;
			if(numFramesNotUsed++ > mNumFramesBeforeUnload)
			{
				ResourcePtr resource = getByHandle(handle);
				resource->unload();
			}
		}
	}
	//-----------------------------------------------------------------------
	void CachedResourceOgreManager::_notifyResourceTouched(Resource* _res)
	{
		NumFramesNotUsedMap::iterator it = mNumFramesNotUsedMap.find(_res->getHandle());
		if(it != mNumFramesNotUsedMap.end())
		{
			size_t& numFramesNotUsed = it->second;
			numFramesNotUsed = 0;
		}
	}
	//-----------------------------------------------------------------------
	void CachedResourceOgreManager::_notifyResourceLoaded(Resource* _res)
	{
		mNumFramesNotUsedMap.insert( std::make_pair(_res->getHandle(), (size_t) 0) );
	}
	//-----------------------------------------------------------------------
	void CachedResourceOgreManager::_notifyResourceUnloaded(Resource* _res)
	{
		NumFramesNotUsedMap::iterator it = mNumFramesNotUsedMap.find(_res->getHandle());
		if(it != mNumFramesNotUsedMap.end())
			mNumFramesNotUsedMap.erase(it);
	}

} // namespace GothOgre
