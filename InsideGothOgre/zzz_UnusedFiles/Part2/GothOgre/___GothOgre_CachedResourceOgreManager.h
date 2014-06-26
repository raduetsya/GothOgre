#ifndef GOTHOGRE_CACHED_RESOURCE_OGRE_MANAGER_H
#define GOTHOGRE_CACHED_RESOURCE_OGRE_MANAGER_H

#include "GothOgre_FrameListener.h"


namespace GothOgre
{

	// Кэшированный менеджер ресурсов - удаляет ресурсы,
	// если они не используются в течении указанного числа кадров
	// (по умолчанию - 1000).
	class GOTHOGRE_EXPORT CachedResourceOgreManager : public Ogre::ResourceManager,
													  public AutoFrameListener
	{
	public:
		CachedResourceOgreManager();
		~CachedResourceOgreManager();

		void setNumFramesBeforeUnload(size_t _numFrames);
		size_t getNumFramesBeforeUnload() const;

	protected:
		void frameRenderingQueued(const FrameEvent& _evt);
		void _notifyResourceTouched(Resource* _res);
		void _notifyResourceLoaded(Resource* _res);
		void _notifyResourceUnloaded(Resource* _res);

	private:
		typedef map<ResourceHandle, size_t>::type NumFramesNotUsedMap;
		NumFramesNotUsedMap   mNumFramesNotUsedMap;
		size_t                mNumFramesBeforeUnload;
	};

} // namespace GothOgre

#endif // GOTHOGRE_CACHED_RESOURCE_OGRE_MANAGER_H