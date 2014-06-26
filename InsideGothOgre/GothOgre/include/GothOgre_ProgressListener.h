#ifndef GOTHOGRE_PROGRESS_LISTENER_H
#define GOTHOGRE_PROGRESS_LISTENER_H

#include "GothOgre_SafeList.h"


namespace GothOgre
{
	//--------------------------------------------------------------------------
	/** Internal base class for listener. */
	class GOTHOGRE_EXPORT ProgressListener
	{
	public:
		/** Called when the progress started. */
		virtual void progressStarted() = 0;

		/** Called when the progress ended. */
		virtual void progressEnded() = 0;

	public:
		ProgressListener();
		~ProgressListener();

		/** Register this object as events' handler. */
		void registerListener();

		/** Unregister this object as events' handler. */
		void unregisterListener();

	private:
		friend class Progress;
		typedef SafeList<ProgressListener*>::ElementHandle ElementHandle;
		ElementHandle  mElementHandle;
	};


	// The macros is designed to be used as a base only. Should not be used directly.
#	define GOTHOGRE_DECLARE_PROGRESS_BASE(progressStartedFunc, progressEndedFunc, registerFunc, unregisterFunc, classname) \
		void registerFunc() \
		{ \
			mPrivateProgressListener_##registerFunc.registerListener(this); \
		} \
		\
		void unregisterFunc() \
		{ \
			mPrivateProgressListener_##registerFunc.unregisterListener(); \
		} \
	\
	private: \
		void progressStarted_defaultImpl_##registerFunc() \
		{ \
		} \
		void progressEnded_defaultImpl_##registerFunc() \
		{ \
		} \
		\
		class PrivateProgressListener_##registerFunc : public ProgressListener \
		{ \
		public: \
			void progressStarted() \
			{ \
				return mOwner->progressStartedFunc(); \
			} \
			\
			void progressEnded() \
			{ \
				return mOwner->progressEndedFunc(); \
			} \
			\
			void registerListener(classname* _owner) \
			{ \
				mOwner = _owner; \
				ProgressListener::registerListener(); \
			} \
		\
		private: \
			classname* mOwner; \
		}; \
		friend class PrivateProgressListener_##registerFunc; \
		PrivateProgressListener_##registerFunc  mPrivateProgressListener_##registerFunc;


	//----------------------------------------------------------------------------
	// ProgressListener.
	// Macroses to inject progress-listening functionality to any class.
	//----------------------------------------------------------------------------

#	define GOTHOGRE_DECLARE_PROGRESS_STARTED_EX( \
		progressStartedFunc, \
		registerFunc, unregisterFunc, classname) \
		void progressStarted(); \
		GOTHOGRE_DECLARE_PROGRESS_BASE( \
			progressStartedFunc, \
			progressStarted_defaultImpl_##registerFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_PROGRESS_ENDED_EX( \
		progressEndedFunc, \
		registerFunc, unregisterFunc, classname) \
		void progressEnded(); \
		GOTHOGRE_DECLARE_PROGRESS_BASE( \
			progressStarted_defaultImpl_##registerFunc, \
			progressEndedFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_PROGRESS_STARTED_ENDED_EX( \
		progressStartedFunc, progressEndedFunc, \
		registerFunc, unregisterFunc, classname) \
		void progressStarted(); \
		void progressEnded(); \
		GOTHOGRE_DECLARE_PROGRESS_BASE( \
			progressStartedFunc, \
			progressEndedFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_PROGRESS_STARTED( classname ) \
		GOTHOGRE_DECLARE_PROGRESS_STARTED_EX( \
			progressStarted, \
			registerProgressListener, unregisterProgressListener, classname )

#	define GOTHOGRE_DECLARE_PROGRESS_ENDED( classname ) \
		GOTHOGRE_DECLARE_PROGRESS_ENDED_EX( \
			progressEnded, \
			registerProgressListener, unregisterProgressListener, classname )

#	define GOTHOGRE_DECLARE_PROGRESS_STARTED_ENDED( classname ) \
		GOTHOGRE_DECLARE_PROGRESS_STARTED_ENDED_EX( \
			progressStarted, progressEnded, \
			registerProgressListener, unregisterProgressListener, classname )

} // namespace GothOgre


#endif // GOTHOGRE_PROGRESS_LISTENER_H