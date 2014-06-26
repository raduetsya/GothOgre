#ifndef GOTHOGRE_SOUND_OGRE_MANAGER_H
#define GOTHOGRE_SOUND_OGRE_MANAGER_H

#include "GothOgre_CachedResourceOgreManager.h"
#include "GothOgre_SafeList.h"
#include "GothOgre_SoundPtr.h"
#include "GothOgre_SoundChannel.h"

namespace GothOgre
{
	class SoundChannelGroupImpl;


	//---------------------------------------------------------------------
	// Менеджер для звуковых ресурсов (огровая реализация)
	class GOTHOGRE_EXPORT SoundOgreManager : public CachedResourceOgreManager, 
		                                     public Singleton<SoundOgreManager>

	{
	public:
		/** Default constructor. */
		SoundOgreManager();

		/** Default destructor. */
		virtual ~SoundOgreManager();

		SoundChannel2DPtr createChannel2D(const SoundPtr& _sound);
		SoundChannel3DPtr createChannel3D(const SoundPtr& _sound);

		// Factor volumes of the all channels from the channel group
		void setChannelGroupVolume(const String& _channelGroup, Real _volumeFactor);
		Real getChannelGroupVolume(const String& _channelGroup) const;

		// Pause or unpause all channels from the channel group
		void pauseChannelGroup(const String& _channelGroup);
		void unpauseChannelGroup(const String& _channelGroup);

		void stopChannelGroup(const String& _channelGroup);

		// Sets the position of the sound listener
		void setListenerPosition(const Vector3& _position);

		// Returns the position of the sound listener
		const Vector3& getListenerPosition() const {return mListenerPosition;}

		// Sets the direction vector of the sound listener
		void setListenerDirection(const Vector3& _direction);

		// Returns the direction vector of the sound listener
		const Vector3& getListenerDirection() const {return mListenerDirection;}

		// Sets the up vector of the sound listener
		void setListenerUp(const Vector3& _direction);

		// Returns the up vector of the sound listener
		const Vector3& getListenerUp() const {return mListenerUp;}

	private:
		friend class SoundChannel;
		SoundChannelGroupImpl* getChannelGroupImpl(const String& _channelGroup) const;

	private:
		friend class SoundChannel;
		void addPlayingChannel(SoundChannel* _channel);
		void removePlayingChannel(SoundChannel* _channel);

	protected:
		void frameRenderingQueued(const FrameEvent& _evt);

	private:
		virtual Resource* createImpl(const String& _name, ResourceHandle _handle, 
			const String& _group, bool _isManual, ManualResourceLoader* _loader, 
			const NameValuePairList* _createParams);

	private:
		typedef map<String, SoundChannelGroupImpl*>::type  ChannelGroupImpls;
		typedef vector<SoundChannel*>::type                ChannelVector;

		ChannelGroupImpls  mChannelGroupImpls;
		ChannelVector      mPlayingChannels;
		Vector3            mListenerPosition;
		Vector3            mListenerDirection;
		Vector3            mListenerUp;
	};

} // namespace GothOgre

#endif // GOTHOGRE_SOUND_OGRE_MANAGER_H