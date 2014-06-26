#include "GothOgre_Precompiled.h"
#include "GothOgre_UIModeManager.h"
#include "GothOgre_UIModeListener.h"

//-------------------------------------------------------------------------
GothOgre::UIModeManager* 
	Ogre::Singleton<GothOgre::UIModeManager>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------

namespace GothOgre
{
	//----------------------------------------------------------------------
	const String UIMode::INITIALISATION = "Initialisation";
	const String UIMode::SHUTDOWN = "Shutdown";
	const String UIMode::EDITOR = "Editor";
	const String UIMode::GAME = "Game";
	const String UIMode::GAME_MENU = "GameMenu";
	//----------------------------------------------------------------------
	UIModeManager::UIModeManager(const String& _startUIMode)
	{
		mUIMode = _startUIMode;
	}
	//----------------------------------------------------------------------
	UIModeManager::~UIModeManager()
	{
	}
	//----------------------------------------------------------------------
	void UIModeManager::_initListener(UIModeListener* _uiModeListener)
	{
		_uiModeListener->mElementHandle = mListeners.getNullHandle();
	}
	//----------------------------------------------------------------------
	void UIModeManager::_addListener(UIModeListener* _listener, Priority _priority)
	{
		_removeListener(_listener);
		_listener->mElementHandle = mListeners.add(_listener, _priority);
	}
	//----------------------------------------------------------------------
	void UIModeManager::_removeListener(UIModeListener* _listener)
	{
		if(_listener->mElementHandle != mListeners.getNullHandle())
		{
			mListeners.remove(_listener->mElementHandle);
			_listener->mElementHandle = mListeners.getNullHandle();
		}
	}
	//----------------------------------------------------------------------
	void UIModeManager::setUIMode(const String& _newUIMode)
	{
		if(mUIMode != _newUIMode)
		{
			// exiting
			{
				GOTHOGRE_INFO("Exiting from UI mode '" << mUIMode << "'");
				
				UIModeEvent evt(mUIMode, _newUIMode);
				
				Listeners::Iterator it = mListeners.getIterator();
				while(it.hasMoreElements())
				{
					UIModeListener* listener = it.getNext();
					listener->uiModeExited( evt );
				}
				GOTHOGRE_INFO("Exited from UI mode '" << mUIMode << "' successfully.");
			}

			// entering
			{
				GOTHOGRE_INFO("Entering to UI mode '" << _newUIMode << "'.");
				
				UIModeEvent evt(mUIMode, _newUIMode);
				mUIMode = _newUIMode;

				Listeners::ReverseIterator it = mListeners.getReverseIterator();
				while(it.hasMoreElements())
				{
					UIModeListener* listener = it.getNext();
					listener->uiModeEntered( evt );
				}
				GOTHOGRE_INFO("Entered to UI mode '" << _newUIMode << "' successfully.");
			}
		}
	}

} // namespace GothOgre

