#include "GothOgre_Precompiled.h"
#include "GothOgre_UnicodeUtil.h"
#include "GothOgre_SoundPlayer.h"


//-------------------------------------------------------------------------
GothOgre::SoundPlayerEnumerator* 
	Ogre::Singleton<GothOgre::SoundPlayerEnumerator>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------


namespace GothOgre
{
	//-----------------------------------------------------------------------
	// SoundPlayer
	//-----------------------------------------------------------------------
	SoundPlayer::SoundPlayer(const String& _name, Priority _priority)
	{
		mName = _name;
		SoundPlayerEnumerator::getSingleton().addSoundPlayer(this, _priority);	
	}
	//-----------------------------------------------------------------------
	SoundPlayer::~SoundPlayer()
	{
		SoundPlayerEnumerator::getSingleton().removeSoundPlayer(this);
	}
	//-----------------------------------------------------------------------
	bool SoundPlayer::canPlay(bool _3D, String& _filename, const String& _resourceGroup) const
	{
		if((_3D && !canPlay3D()) || (!_3D && !canPlay2D()))
			return false;

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
	const String& SoundPlayer::getFileExt() const
	{
		return StringUtil::BLANK;
	}
	//----------------------------------------------------------------------
	bool SoundPlayer::canPlay2D() const
	{
		return false;
	}
	//----------------------------------------------------------------------
	bool SoundPlayer::canPlay3D() const
	{
		return false;
	}




	//-----------------------------------------------------------------------
	// SoundPlayerEnumerator
	//-----------------------------------------------------------------------
	SoundPlayerEnumerator::SoundPlayerEnumerator()
	{
	}
	//-----------------------------------------------------------------------
	SoundPlayerEnumerator::~SoundPlayerEnumerator()
	{
	}
	//-----------------------------------------------------------------------
	const StringVector& SoundPlayerEnumerator::getAllSoundPlayers() const
	{
		return mAllPlayerNames;
	}
	//-----------------------------------------------------------------------
	bool SoundPlayerEnumerator::canPlay(bool _3D, const String& _filename, const String& _resourceGroup) const
	{
		String filename = _filename;
		SoundPlayer* player = findPlayerCanPlay(_3D, filename, _resourceGroup);
		return player != nullptr;
	}
	//-----------------------------------------------------------------------
	SoundPlayer* SoundPlayerEnumerator::findPlayerCanPlay(bool _3D, String& _filename, const String& _resourceGroup) const
	{
		String filename2;
		for(PlayersByPriority::const_reverse_iterator it = mPlayersByPriority.rbegin();
			it != mPlayersByPriority.rend(); ++it)
		{
			SoundPlayer* player = it->second;
			filename2 = _filename;
			if(player->canPlay(_3D, filename2, _resourceGroup))
			{
				_filename = filename2;
				return player;
			}
		}
		return nullptr;
	}
	//-----------------------------------------------------------------------
	SoundPlayer* SoundPlayerEnumerator::getPlayerByName(const String& _playerName) const
	{
		PlayersByName::const_iterator it = mPlayersByName.find(_playerName);
		if(it == mPlayersByName.end())
			GOTHOGRE_EXCEPT(_playerName << " - Sound player not found.");
		SoundPlayer* player = it->second;
		return player;
	}
	//-----------------------------------------------------------------------
	void SoundPlayerEnumerator::addSoundPlayer(SoundPlayer* _player, Priority _priority)
	{
		mPlayersByPriority.insert(std::make_pair(_priority, _player));
		mPlayersByName.insert(std::make_pair(_player->getName(), _player));
		mAllPlayerNames.push_back(_player->getName());
	
		// Player is not initialised yet.
		SoundPlayerInfo& info = mPlayerInfosByPlayer.insert(std::make_pair(_player, SoundPlayerInfo())).first->second;
		info.initialised = false;
	}
	//-----------------------------------------------------------------------
	void SoundPlayerEnumerator::removeSoundPlayer(SoundPlayer* _player)
	{
		// remove all references to the player
		{
			for(PlayersByPriority::iterator it = mPlayersByPriority.begin();
				it != mPlayersByPriority.end(); ++it)
			{
				if(it->second == _player)
				{
					mPlayersByPriority.erase(it);
					break;
				}
			}
			mPlayersByName.erase(_player->getName());
			mAllPlayerNames.erase(std::find(mAllPlayerNames.begin(), mAllPlayerNames.end(), _player->getName()));
		}

		// shut down the player
		{
			PlayerInfosByPlayer::iterator it = mPlayerInfosByPlayer.find(_player);
			if(it != mPlayerInfosByPlayer.end())
			{
				SoundPlayerInfo& info = it->second;
				if(info.initialised)
				{
					_player->shutdown();
				}
				mPlayerInfosByPlayer.erase(it);
			}
		}
	}
	//-----------------------------------------------------------------------
	SoundPlayContextPtr SoundPlayerEnumerator::play(bool _3D, const String& _filename, const String& _resourceGroup)
	{
		// Find an appropriate player.
		String filename2 = _filename;
		SoundPlayer* player = findPlayerCanPlay(_3D, filename2, _resourceGroup);
		if(!player)
		{
			if(ResourceGroupManager::getSingleton().resourceExists(_resourceGroup, _filename))
				GOTHOGRE_EXCEPT( _filename << " - Player not found for this file extension.");
			else
				GOTHOGRE_EXCEPT( _filename << " - File not found.");
			return SoundPlayContextPtr();
		}

		// Initialise the found player, if not yet.
		SoundPlayerInfo& info = mPlayerInfosByPlayer[player];
		if(!info.initialised)
		{
			player->initialise();
			info.initialised = true;
		}

		// Get a free play context.
		SoundPlayContextPtr context;
		if(!info.freeContexts.empty())
		{
			// We use a pool of free contexts.
			context = info.freeContexts.front();
			info.freeContexts.pop_front();
			info.activeContexts.push_front(context);
		}
		else
		{
			context = (SoundPlayContextPtr) player->createPlayContext();
			info.activeContexts.push_front(context);
		}

		// Play!
		context->play(_3D, filename2, _resourceGroup);

		// Return a shared pointer to the context.
		return context;
	}
	//-----------------------------------------------------------------------
	void SoundPlayerEnumerator::updateAll(const Vector3& _listenerPosition, const Quaternion& _listenerOrientation)
	{
		// For each initialised player...
		for(PlayerInfosByPlayer::iterator itI = mPlayerInfosByPlayer.begin();
			itI != mPlayerInfosByPlayer.end(); ++itI)
		{
			SoundPlayer* player = itI->first;
			SoundPlayerInfo& info = itI->second;
			if(info.initialised)
			{
				// Send the new listener's position to the player
				player->update(_listenerPosition, _listenerOrientation);
				
				// For each active (playing) context...
				Contexts::iterator itC = info.activeContexts.begin();
				while(itC != info.activeContexts.end())
				{
					Contexts::iterator curItC = itC++;
					const SoundPlayContextPtr& context = *curItC;
					if(context.useCount() == SOUND_PLAYER_ENUMERATOR_NUM_REFERENCE_COUNTS)
					{
						// If the context is not used now, then stop playing the sound
						if(context->isPlaying())
							context->stop();
						info.freeContexts.push_front(context);
						info.activeContexts.erase(curItC);
					}
					else
					{
						// If the context is playing now then update it.
						if(context->isPlaying())
							context->update();
					}
				}
			}
		}
	}

} // namespace GothOgre