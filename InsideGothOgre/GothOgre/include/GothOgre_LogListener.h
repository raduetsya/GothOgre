#ifndef GOTHOGRE_LOG_LISTENER_H
#define GOTHOGRE_LOG_LISTENER_H


namespace GothOgre
{
	//------------------------------------------------------------------------
	// Internal listener class.
	class GOTHOGRE_EXPORT NewLogListener
	{
	public:
		virtual void messageLogged(const String& _module, LogLevel _lvl, const String& _message) = 0;

	public:
		NewLogListener();
		virtual ~NewLogListener();

		void registerListener();
		void unregisterListener();

	private:
		friend class LogSystemImpl;
		void* mData;
	};


	// The macros is designed to be used as a base only. Should not be used directly.
#	define GOTHOGRE_DECLARE_LOG_BASE(messageLoggedFunc, registerFunc, unregisterFunc, classname) \
		void registerFunc() \
		{ \
			mPrivateLogListener_##registerFunc.registerListener(this); \
		} \
		void unregisterFunc() \
		{ \
			mPrivateLogListener_##registerFunc.unregisterListener(); \
		} \
	\
	private: \
		void messageLogged_defaultImpl_##registerFunc(const String& _module, LogLevel _lvl, const String& _message) \
		{ \
		} \
		\
		class PrivateLogListener_##registerFunc : public NewLogListener \
		{ \
		public: \
			void messageLogged(const String& _module, LogLevel _lvl, const String& _message) \
			{ \
				return mOwner->messageLoggedFunc(_module, _lvl, _message); \
			} \
			\
			void registerListener(classname* _owner) \
			{ \
				mOwner = _owner; \
				NewLogListener::registerListener(); \
			} \
		\
		private: \
			classname* mOwner; \
		}; \
		friend class PrivateLogListener_##registerFunc; \
		PrivateLogListener_##registerFunc mPrivateLogListener_##registerFunc;




	//----------------------------------------------------------------------------
	// LogListener.
	// Macroses to inject log-listening functionality to any class.
	//----------------------------------------------------------------------------
#	define GOTHOGRE_DECLARE_MESSAGE_LOGGED_EX( \
		messageLoggedFunc, \
		registerFunc, unregisterFunc, classname) \
		void messageLoggedFunc(const String& _module, LogLevel _lvl, const String& _message); \
		GOTHOGRE_DECLARE_LOG_BASE( \
			messageLoggedFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_MESSAGE_LOGGED( classname ) \
		GOTHOGRE_DECLARE_MESSAGE_LOGGED_EX( \
			messageLogged, \
			registerLogListener, unregisterLogListener, classname)

} // namespace GothOgre
	
#endif // GOTHOGRE_LOG_LISTENER_H