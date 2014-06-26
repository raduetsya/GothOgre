#ifndef GOTHOGRE_UI_MODE_MANAGER_H
#define GOTHOGRE_UI_MODE_MANAGER_H

#include "GothOgre_SafeOrderedList.h"
#include "GothOgre_Priority.h"


namespace GothOgre
{
	//-------------------------------------------------------------------------
	// Constants for UI modes
	struct GOTHOGRE_EXPORT UIMode
	{
		static const String INITIALISATION;
		static const String SHUTDOWN;
		static const String EDITOR;
		static const String GAME;
		static const String GAME_MENU;
	};


	//-------------------------------------------------------------------------
	/** Class is designed to keep common function to control user interface */
	class GOTHOGRE_EXPORT UIModeManager : public Singleton<UIModeManager>
	{
	public:
		UIModeManager(const String& _startUIMode = UIMode::INITIALISATION);
		~UIModeManager();

		/** Switches user interface to the specified mode. */
		void setUIMode(const String& _newUIMode);

		/** Returns the current user interface's mode. */
		const String& getUIMode() const {return mUIMode;}

	private:
		friend class UIModeListener;
		void _initListener(UIModeListener* _listener);
		void _addListener(UIModeListener* _listener, Priority _priority);
		void _removeListener(UIModeListener* _listener);

	private:
		typedef SafeOrderedList<Priority, UIModeListener*> Listeners;
		Listeners	 mListeners;
		String	     mUIMode;
	};

} // namespace GothOgre


#endif // GOTHOGRE_UI_MODE_MANAGER_H