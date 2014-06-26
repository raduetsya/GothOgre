#include "GothOgre_Precompiled.h"
#include "GothOgre_SoundOgreManager.h"
#include "GothOgre_SoundManager.h"
#include "GothOgre_SoundPtr.h"
#include "GothOgre_Sound.h"
#include "GothOgre_ResourceGroup.h"

//-------------------------------------------------------------------------
GothOgre::SoundManager* 
	Ogre::Singleton<GothOgre::SoundManager>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------


namespace GothOgre
{

	SoundManager::SoundManager()
	{
	}
	//----------------------------------------------------------------------------
	SoundManager::~SoundManager()
	{
	}
	//----------------------------------------------------------------------------
	Ogre::ResourceManager* SoundManager::getOgreResourceManager() const
	{
		return SoundOgreManager::getSingletonPtr();
	}
	//----------------------------------------------------------------------------
	bool SoundManager::isCaseSensitive() const
	{
		return false;
	}
	//----------------------------------------------------------------------------
	const String& SoundManager::getDefaultResourceGroup() const
	{
		return ResourceGroup::SOUND;
	}
	//----------------------------------------------------------------------------
	SoundChannel2DPtr SoundManager::createChannel2D(SoundPtr _sound)
	{
		return SoundOgreManager::getSingleton().createChannel2D(_sound);
	}
	//----------------------------------------------------------------------------
	SoundChannel3DPtr SoundManager::createChannel3D(SoundPtr _sound)
	{
		return SoundOgreManager::getSingleton().createChannel3D(_sound);
	}
	//----------------------------------------------------------------------------
	void SoundManager::setChannelGroupVolume(const String& _channelGroup, Real _volumeFactor)
	{
		SoundOgreManager::getSingleton().setChannelGroupVolume(_channelGroup, _volumeFactor);
	}
	//----------------------------------------------------------------------------
	Real SoundManager::getChannelGroupVolume(const String& _channelGroup) const
	{
		return SoundOgreManager::getSingleton().getChannelGroupVolume(_channelGroup);
	}
	//----------------------------------------------------------------------------
	void SoundManager::pauseChannelGroup(const String& _channelGroup)
	{
		SoundOgreManager::getSingleton().pauseChannelGroup(_channelGroup);
	}
	//----------------------------------------------------------------------------
	void SoundManager::unpauseChannelGroup(const String& _channelGroup)
	{
		SoundOgreManager::getSingleton().unpauseChannelGroup(_channelGroup);
	}
	//----------------------------------------------------------------------------
	void SoundManager::stopChannelGroup(const String& _channelGroup)
	{
		SoundOgreManager::getSingleton().stopChannelGroup(_channelGroup);
	}
	//----------------------------------------------------------------------------
	void SoundManager::setListenerPosition(const Vector3& _position)
	{
		SoundOgreManager::getSingleton().setListenerPosition(_position);
	}
	//----------------------------------------------------------------------------
	const Vector3& SoundManager::getListenerPosition() const
	{
		return SoundOgreManager::getSingleton().getListenerPosition();
	}
	//----------------------------------------------------------------------------
	void SoundManager::setListenerDirection(const Vector3& _direction)
	{
		SoundOgreManager::getSingleton().setListenerDirection(_direction);
	}
	//----------------------------------------------------------------------------
	const Vector3& SoundManager::getListenerDirection() const
	{
		return SoundOgreManager::getSingleton().getListenerDirection();
	}
	//----------------------------------------------------------------------------
	void SoundManager::setListenerUp(const Vector3& _up)
	{
		SoundOgreManager::getSingleton().setListenerUp(_up);
	}
	//----------------------------------------------------------------------------
	const Vector3& SoundManager::getListenerUp() const
	{
		return SoundOgreManager::getSingleton().getListenerUp();
	}

} // namespace GothOgre