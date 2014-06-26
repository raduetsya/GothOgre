#include "GothOgre_StdHeaders.h"
#include "GothOgre_FrameEventUtilities.h"
#include "GothOgre_SafeList.h"

namespace GothOgre
{
	class FrameEventImpl : public FrameListener
	{
	public:
		FrameEventImpl();
		static FrameEventImpl& getSingleton();

		void addListener( FrameListener* _listener );
		void removeListener( FrameListener* _listener );
		size_t size();

	protected:
		virtual bool frameStarted(const FrameEvent& evt);
		virtual bool frameRenderingQueued(const FrameEvent& evt);
		virtual bool frameEnded(const FrameEvent& evt);

	private:
		typedef SafeList<FrameListener*> FrameListeners;
		FrameListeners mFrameListeners;
		bool           mInitialized;
	};
	//----------------------------------------------------------------------------------
	FrameEventImpl::FrameEventImpl()
	{
		mInitialized = false;
	}
	//----------------------------------------------------------------------------------
	FrameEventImpl& FrameEventImpl::getSingleton()
	{
		static FrameEventImpl sFrameEventImpl;
		return sFrameEventImpl;
	}
	//----------------------------------------------------------------------------------
	void FrameEventImpl::addListener( FrameListener* _listener )
	{
		if(!mInitialized)
		{
			if(Root::getSingletonPtr())
			{
				Root::getSingletonPtr()->addFrameListener(this);
				mInitialized = true;
			}
		}

		mFrameListeners.add(_listener);
	}
	//----------------------------------------------------------------------------------
	void FrameEventImpl::removeListener( FrameListener* _listener )
	{
		mFrameListeners.remove(_listener);
	}
	//----------------------------------------------------------------------------------
	size_t FrameEventImpl::size()
	{
		return mFrameListeners.size();
	}
	//----------------------------------------------------------------------------------
	bool FrameEventImpl::frameStarted(const FrameEvent& evt)
	{
		FrameListeners::Iterator it = mFrameListeners.getIterator();
		while(it.hasMoreElements())
			if(!(it.getNext()->frameStarted(evt)))
				return false;
		return true;
	}
	//----------------------------------------------------------------------------------
	bool FrameEventImpl::frameRenderingQueued(const FrameEvent& evt)
	{
		FrameListeners::Iterator it = mFrameListeners.getIterator();
		while(it.hasMoreElements())
			if(!(it.getNext()->frameRenderingQueued(evt)))
				return false;
		return true;
	}		
	//----------------------------------------------------------------------------------
	bool FrameEventImpl::frameEnded(const FrameEvent& evt)
	{
		bool ret = true;
		FrameListeners::Iterator it = mFrameListeners.getIterator();
		while(it.hasMoreElements())
			if(!(it.getNext()->frameEnded(evt)))
				ret = false;
		return ret;
	}
	//----------------------------------------------------------------------------------------------------------
	void FrameEventUtilities::addFrameListener( FrameListener* _listener )
	{
		FrameEventImpl::getSingleton().addListener( _listener );
	}
	//----------------------------------------------------------------------------------------------------------
	void FrameEventUtilities::removeFrameListener( FrameListener* _listener )
	{
		FrameEventImpl::getSingleton().removeListener( _listener );
	}
	//----------------------------------------------------------------------------------------------------------
	size_t FrameEventUtilities::getNumFrameListeners()
	{
		return FrameEventImpl::getSingleton().size();
	}
		
} // namespace GothOgre