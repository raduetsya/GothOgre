#ifndef GOTHOGRE_SOUND_MANAGER_H
#define GOTHOGRE_SOUND_MANAGER_H

#include "GothOgre_UpgradedResourceManager.h"
#include "GothOgre_SoundChannel.h"

namespace GothOgre
{
	// Менеджер для звуковых ресурсов
	class GOTHOGRE_EXPORT SoundManager : public Singleton<SoundManager>, 
		                                 public UpgradedResourceManager
	{
	public:
		SoundManager();
		~SoundManager();
		
		Ogre::ResourceManager* getOgreResourceManager() const;
		bool isCaseSensitive() const;
		const String& getDefaultResourceGroup() const;

		SoundChannel2DPtr createChannel2D(SoundPtr _sound);
		SoundChannel3DPtr createChannel3D(SoundPtr _sound);

		void setChannelGroupVolume(const String& _channelGroup, Real _volumeFactor);
		Real getChannelGroupVolume(const String& _channelGroup) const;

		void pauseChannelGroup(const String& _channelGroup);
		void unpauseChannelGroup(const String& _channelGroup);
		void stopChannelGroup(const String& _channelGroup);

		void setListenerPosition(const Vector3& _position);
		const Vector3& getListenerPosition() const;
		void setListenerDirection(const Vector3& _direction);
		const Vector3& getListenerDirection() const;
		void setListenerUp(const Vector3& _up);
		const Vector3& getListenerUp() const;
	};


} // namespace GothOgre

#endif // GOTHOGRE_SOUND_MANAGER_H