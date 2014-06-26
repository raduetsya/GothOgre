#include "GothOgre_Precompiled.h"
#include "GothOgre_CommandLine.h"
#include "GothOgre_LogListener.h"
#include "GothOgre_SafeList.h"
#include "GothOgre_DefaultLogListener.h"
#include "GothOgre_LogSystem.h"


//-------------------------------------------------------------------------
GothOgre::LogSystem* 
	Ogre::Singleton<GothOgre::LogSystem>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------

namespace GothOgre
{
	//---------------------------------------------------------------------------------------------------
	GOTHOGRE_IMPLEMENT_ENUM_IO(LogLevel)
		GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT2(INFO,     "Info")
		GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT2(WARNING,  "Warning")
		GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT2(ERROR,    "Error")
		GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT2(CRITICAL, "Critical")
	GOTHOGRE_IMPLEMENT_ENUM_IO_END
	//---------------------------------------------------------------------------------------------------
	class LogSystemImpl
	{
	public:
		LogSystemImpl() {}
		~LogSystemImpl() {}

		void logMessage(const String& _module, LogLevel _lvl, const String& _message)
		{
			OGRE_LOCK_AUTO_MUTEX
			Listeners::Iterator it = mListeners.getIterator();
			while(it.hasMoreElements())
			{
				NewLogListener* listener = it.getNext();
				listener->messageLogged(_module, _lvl, _message);
			}
		}

		void _addListener(NewLogListener* _listener)
		{
			OGRE_LOCK_AUTO_MUTEX
			if(!_listener->mData)
			{
				_listener->mData = new ElementHandle( mListeners.add(_listener) );
			}
		}

		void _removeListener(NewLogListener* _listener)
		{
			OGRE_LOCK_AUTO_MUTEX
			if(_listener->mData)
			{
				ElementHandle* handleptr = (ElementHandle*) _listener->mData;
				mListeners.remove(*handleptr);
				delete handleptr;
				_listener->mData = nullptr;
			}
		}

	private:
		OGRE_AUTO_MUTEX
		typedef SafeList<NewLogListener*> Listeners;
		typedef Listeners::ElementHandle     ElementHandle;
		Listeners   mListeners;
	};
	//---------------------------------------------------------------------------------------------------
	LogSystem::LogSystem(const String& _logFileName)
	{
		mImpl = new LogSystemImpl;
		mDefaultLogListener = new DefaultLogListener(CommandLine::getSingleton().getAbsolutePath(_logFileName));
	}
	//---------------------------------------------------------------------------------------------------
	LogSystem::~LogSystem()
	{
		delete mDefaultLogListener;
		delete mImpl;
	}
	//---------------------------------------------------------------------------------------------------
	void LogSystem::logMessage(const String& _module, LogLevel _lvl, const String& _message)
	{
		mImpl->logMessage(_module, _lvl, _message);
	}



	//----------------------------------------------------------------------------
	// NewLogListener
	//---------------------------------------------------------------------------
	NewLogListener::NewLogListener()
	{
		mData = nullptr;
	}
	//---------------------------------------------------------------------------
	NewLogListener::~NewLogListener()
	{
		unregisterListener();
	}
	//---------------------------------------------------------------------------
	void NewLogListener::registerListener()
	{
		LogSystem::getSingleton().mImpl->_addListener(this);
	}
	//---------------------------------------------------------------------------
	void NewLogListener::unregisterListener()
	{
		LogSystem::getSingleton().mImpl->_removeListener(this);
	}




	//---------------------------------------------------------------------------
	// The root GothOgre module
	//---------------------------------------------------------------------------
	GOTHOGRE_SET_MODULE_NAMES("GothOgre", "GothOgre")

}