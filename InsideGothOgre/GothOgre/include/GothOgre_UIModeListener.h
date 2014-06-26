#ifndef GOTHOGRE_UI_MODE_LISTENER_H
#define GOTHOGRE_UI_MODE_LISTENER_H

#include "GothOgre_Priority.h"
#include "GothOgre_SafeOrderedList.h"


namespace GothOgre
{
	class UIModeEvent
	{
	public:
		UIModeEvent(const String& _oldUIMode, const String& _newUIMode) : mOldUIMode(_oldUIMode), mNewUIMode(_newUIMode) {}
		const String& getOldUIMode() const {return mOldUIMode;}
		const String& getNewUIMode() const {return mNewUIMode;}
		
	private:
		String mOldUIMode;
		String mNewUIMode;
	};


	//--------------------------------------------------------------------------
	/** Internal base class for listener. */
	class GOTHOGRE_EXPORT UIModeListener
	{
	public:
		/** Called at the end of switching to the specified UI mode
		from another UI mode. */
		virtual void uiModeEntered(const UIModeEvent& _evt) = 0;

		/** Called at the beginning of switching from the specified UI mode
		to another UI mode. */
		virtual void uiModeExited(const UIModeEvent& _evt) = 0;

	public:
		UIModeListener();
		~UIModeListener();

		/** Register this object as events' handler. */
		void registerListener(Priority _priority = Priority::LOWEST);

		/** Unregister this object as events' handler. */
		void unregisterListener();

	private:
		friend class UIModeManager;
		typedef SafeOrderedList<Priority, UIModeListener*>::ElementHandle ElementHandle;
		ElementHandle  mElementHandle;
	};


	// The macros is designed to be used as a base only. Should not be used directly.
#	define GOTHOGRE_DECLARE_UI_MODE_BASE(uiModeEnteredFunc, uiModeExitedFunc, registerFunc, unregisterFunc, classname) \
		void registerFunc(Priority _priority = Priority::LOWEST) \
		{ \
			mPrivateUIModeListener_##registerFunc.registerListener(this, _priority); \
		} \
		void unregisterFunc() \
		{ \
			mPrivateUIModeListener_##registerFunc.unregisterListener(); \
		} \
	\
	private: \
		void uiModeEntered_defaultImpl_##registerFunc(const UIModeEvent& _evt) \
		{ \
		} \
		void uiModeExited_defaultImpl_##registerFunc(const UIModeEvent& _evt) \
		{ \
		} \
		\
		class PrivateUIModeListener_##registerFunc : public UIModeListener \
		{ \
		public: \
			void uiModeEntered(const UIModeEvent& _evt) \
			{ \
				return mOwner->uiModeEnteredFunc(_evt); \
			} \
			\
			void uiModeExited(const UIModeEvent& _evt) \
			{ \
				return mOwner->uiModeExitedFunc(_evt); \
			} \
			\
			void registerListener(classname* _owner, Priority _priority) \
			{ \
				mOwner = _owner; \
				UIModeListener::registerListener(_priority); \
			} \
		\
		private: \
			classname* mOwner; \
		}; \
		friend class PrivateUIModeListener_##registerFunc; \
		PrivateUIModeListener_##registerFunc mPrivateUIModeListener_##registerFunc;


	//----------------------------------------------------------------------------
	// UIModeListener.
	// Macroses to inject uimode-listening functionality to any class.
	//----------------------------------------------------------------------------

#	define GOTHOGRE_DECLARE_UI_MODE_ENTERED_EX( \
		uiModeEnteredFunc, \
		registerFunc, unregisterFunc, classname) \
		void uiModeEnteredFunc(const UIModeEvent& _evt); \
		GOTHOGRE_DECLARE_UI_MODE_BASE( \
			uiModeEnteredFunc, \
			uiModeExited_defaultImpl_##registerFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_UI_MODE_EXITED_EX( \
		uiModeExitedFunc, \
		registerFunc, unregisterFunc, classname) \
		void uiModeExitedFunc(const UIModeEvent& _evt); \
		GOTHOGRE_DECLARE_UI_MODE_BASE( \
			uiModeEntered_defaultImpl_##registerFunc, \
			uiModeExitedFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_UI_MODE_ENTERED_EXITED_EX( \
		uiModeEnteredFunc, uiModeExitedFunc, \
		registerFunc, unregisterFunc, classname) \
		void uiModeEnteredFunc(const UIModeEvent& _evt); \
		void uiModeExitedFunc(const UIModeEvent& _evt); \
		GOTHOGRE_DECLARE_UI_MODE_BASE( \
			uiModeEnteredFunc, \
			uiModeExitedFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_UI_MODE_ENTERED( classname )\
		GOTHOGRE_DECLARE_UI_MODE_ENTERED_EX( \
		uiModeEntered, \
		registerUIModeListener, unregisterUIModeListener, classname)

#	define GOTHOGRE_DECLARE_UI_MODE_EXITED( classname )\
		GOTHOGRE_DECLARE_UI_MODE_EXITED_EX( \
		uiModeExited, \
		registerUIModeListener, unregisterUIModeListener, classname)

#	define GOTHOGRE_DECLARE_UI_MODE_ENTERED_EXITED( classname )\
		GOTHOGRE_DECLARE_UI_MODE_ENTERED_EXITED_EX( \
		uiModeEntered, uiModeExited, \
		registerUIModeListener, unregisterUIModeListener, classname)


} // namespace GothOgre

#endif // GOTHOGRE_UI_MODE_LISTENER_H