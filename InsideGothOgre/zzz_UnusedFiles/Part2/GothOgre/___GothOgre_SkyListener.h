#ifndef GOTHOGRE_SKY_LISTENER_H
#define GOTHOGRE_SKY_LISTENER_H

#include "GothOgre_SafeList.h"


namespace GothOgre
{
	//--------------------------------------------------------------------------
	/** Base class for sky listeners. Should not be used directly. 
	Use macroses GOTHOGRE_DECLARE_SKY_* instead. */
	class GOTHOGRE_EXPORT SkyListener
	{
	protected:
		/** Called by a scene manager to queue skies for rendering 
		(when is the time for it). */
		virtual void skiesQueuedForRendering(Camera* _camera) = 0;

		/** A sky was shown to the observer. */
		virtual void skyShown(const SkyPtr& _sky, Camera* _camera) = 0;

		/** A sky was hidden from the observer. */
		virtual void skyHidden(const SkyPtr& _sky, Camera* _camera) = 0;

	public:
		SkyListener();
		~SkyListener();

		/** Register this object as events' handler. */
		void registerListener(Priority _priority = Priority::LOWEST);

		/** Unregister this object as events' handler. */
		void unregisterListener();

	private:
		friend class SkySystem;
		typedef SafeList<SkyListener*>::ElementHandle ElementHandle;
		ElementHandle  mElementHandle;	
	};


	// The macros is designed to be used as a base only. Should not be used directly.
#	define GOTHOGRE_DECLARE_SKY_BASE( \
		skiesQueuedForRenderingFunc, skyShownFunc, skyHiddenFunc, \
		registerFunc, unregisterFunc, classname) \
		\
		void registerFunc() \
		{ \
			mPrivateSkyListener_##registerFunc.registerListener(this); \
		} \
		\
		void unregisterFunc() \
		{ \
			mPrivateSkyListener_##registerFunc.unregisterListener(); \
		} \
	private: \
		void skiesQueuedForRendering_defaultImpl_##registerFunc(Camera* _camera) \
		{ \
		} \
		void skyShown_defaultImpl_##registerFunc(const SkyPtr& _sky, Camera* _camera) \
		{ \
		} \
		void skyHidden_defaultImpl_##registerFunc(const SkyPtr& _sky, Camera* _camera) \
		{ \
		} \
		\
		class PrivateSkyListener_##registerFunc : public KeyListener \
		{ \
		public: \
			void skiesQueuedForRendering(Camera* _camera) \
			{ \
				return mOwner->skiesQueuedForRenderingFunc(_camera); \
			} \
			\
			void skyShown(const SkyPtr& _sky, Camera* _camera) \
			{ \
				return mOwner->skyShownFunc(_sky, _camera); \
			} \
			\
			void skyHidden(const SkyPtr& _sky, Camera* _camera) \
			{ \
				return mOwner->skyHiddenFunc(_sky, _camera); \
			} \
			\
			void registerListener(classname* _owner) \
			{ \
				mOwner = _owner; \
				SkyListener::registerListener(); \
			} \
		\
		private: \
			classname* mOwner; \
		}; \
		friend class PrivateSkyListener_##registerFunc; \
		PrivateSkyListener_##registerFunc mPrivateSkyListener_##registerFunc;


	//----------------------------------------------------------------------------
	// SkyListener.
	// Macroses to inject sky-listening functionality to any class.
	//----------------------------------------------------------------------------
#	define GOTHOGRE_DECLARE_SKIES_QUEUED_FOR_RENDERING_EX( \
		skiesQueuedForRenderingFunc, \
		registerFunc, unregisterFunc, classname) \
		void skiesRenderingQueuedFunc(Camera* _camera); \
		GOTHOGRE_DECLARE_SKY_BASE( \
			skiesQueuedForRenderingFunc, \
			skyShown_defaultImpl_##registerFunc, \
			skyHidden_defaultImpl_##registerFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_SKY_SHOWN_EX( \
		skyShownFunc, \
		registerFunc, unregisterFunc, classname) \
		void skyShown(const SkyPtr& _sky, Camera* _camera); \
		GOTHOGRE_DECLARE_SKY_BASE( \
			skiesQueuedForRendering_defaultImpl_##registerFunc, \
			skyShownFunc, \
			skyHidden_defaultImpl_##registerFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_SKY_SHOWN_HIDDEN_EX( \
		skyShownFunc, skyHiddenFunc, \
		registerFunc, unregisterFunc, classname) \
		void skyShown(const SkyPtr& _sky, Camera* _camera); \
		GOTHOGRE_DECLARE_SKY_BASE( \
			skiesQueuedForRendering_defaultImpl_##registerFunc, \
			skyShownFunc, \
			skyHiddenFunc, \
			registerFunc, unregisterFunc, classname)

#	define GOTHOGRE_DECLARE_SKIES_QUEUED_FOR_RENDERING( classname ) \
		GOTHOGRE_DECLARE_SKIES_QUEUED_FOR_RENDERING_EX( \
			skiesQueuedForRendering, \
			registerKeyListener, unregisterKeyListener, classname)

#	define GOTHOGRE_DECLARE_SKY_SHOWN( classname ) \
		GOTHOGRE_DECLARE_SKY_SHOWN_EX( \
			skyShown, \
			registerKeyListener, unregisterKeyListener, classname)

#	define GOTHOGRE_DECLARE_SKY_SHOWN_HIDDEN( classname ) \
		GOTHOGRE_DECLARE_SKY_SHOWN_HIDDEN_EX( \
			skyShown, skyHidden, \
			registerKeyListener, unregisterKeyListener, classname)

} // namespace GothOgre

#endif // GOTHOGRE_SKY_LISTENER_H