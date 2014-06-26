#include "GothOgre_StdHeaders.h"
#include "GothOgre_WindowEventUtilities.h"
#include "GothOgre_SafeList.h"

namespace GothOgre
{
	class WindowEventImpl : public WindowEventListener
	{
	public:
		WindowEventImpl();
		static WindowEventImpl& getSingleton();

		void addListener( RenderWindow* _window, WindowEventListener* _listener );
		void removeListener( RenderWindow* _window, WindowEventListener* _listener );
		size_t size();

	protected:
		virtual void windowMoved(RenderWindow* rw);
		virtual void windowResized(RenderWindow* rw);
		virtual bool windowClosing(RenderWindow* rw);
		virtual void windowClosed(RenderWindow* rw);
		virtual void windowFocusChange(RenderWindow* rw);

	private:
		void update();

	private:
		typedef SafeList<WindowEventListener*>						ListenersForOneWindow;
		typedef map<RenderWindow*, ListenersForOneWindow*>::type	WindowEventListeners;

		WindowEventListeners mWindowEventListeners;
		size_t mSize;
		bool   mNeedUpdate;
	};
	//----------------------------------------------------------------------------------
	WindowEventImpl::WindowEventImpl()
	{
		mNeedUpdate = false;
		mSize = 0;
	}
	//----------------------------------------------------------------------------------
	WindowEventImpl& WindowEventImpl::getSingleton()
	{
		static WindowEventImpl sWindowEventImpl;
		return sWindowEventImpl;
	}
	//----------------------------------------------------------------------------------
	void WindowEventImpl::addListener( RenderWindow* _window, WindowEventListener* _listener )
	{
		WindowEventListeners::iterator it = mWindowEventListeners.find(_window);
		if(it == mWindowEventListeners.end())
		{
			it = mWindowEventListeners.insert(std::make_pair(_window, new ListenersForOneWindow)).first;
			Ogre::WindowEventUtilities::addWindowEventListener(_window, this);
		}
		ListenersForOneWindow* lst = it->second;
		lst->add(_listener);
		mNeedUpdate = true;
	}
	//----------------------------------------------------------------------------------
	void WindowEventImpl::removeListener( RenderWindow* _window, WindowEventListener* _listener )
	{
		WindowEventListeners::iterator it = mWindowEventListeners.find(_window);
		if(it != mWindowEventListeners.end())
		{
			ListenersForOneWindow* lst = it->second;
			lst->remove(_listener);
		}
		mNeedUpdate = true;
	}
	//----------------------------------------------------------------------------------
	size_t WindowEventImpl::size()
	{
		update();
		return mSize;
	}
	//----------------------------------------------------------------------------------
	void WindowEventImpl::update()
	{
		if(!mNeedUpdate)
			return;

		mSize = 0;
		WindowEventListeners::iterator it = mWindowEventListeners.begin();
		while(it != mWindowEventListeners.end())
		{
			WindowEventListeners::iterator it2 = it;
			++it2;
			ListenersForOneWindow* lst = it->second;
			size_t sz = lst->size();
			if(sz)
				mSize += sz;
			else
			{
				mWindowEventListeners.erase(it);
				delete lst;
			}
			it = it2;
		}
		mNeedUpdate = false;
	}
	//----------------------------------------------------------------------------------
	void WindowEventImpl::windowMoved(RenderWindow* rw)
	{
		WindowEventListeners::iterator it = mWindowEventListeners.find(rw);
		if(it != mWindowEventListeners.end())
		{
			ListenersForOneWindow* lst = it->second;
			ListenersForOneWindow::Iterator it2 = lst->getIterator();
			while(it2.hasMoreElements())
				it2.getNext()->windowMoved(rw);
		}
	}
	//----------------------------------------------------------------------------------
	void WindowEventImpl::windowResized(RenderWindow* rw)
	{
		WindowEventListeners::iterator it = mWindowEventListeners.find(rw);
		if(it != mWindowEventListeners.end())
		{
			ListenersForOneWindow* lst = it->second;
			ListenersForOneWindow::Iterator it2 = lst->getIterator();
			while(it2.hasMoreElements())
				it2.getNext()->windowResized(rw);
		}
	}		
	//----------------------------------------------------------------------------------
	bool WindowEventImpl::windowClosing(RenderWindow* rw)
	{
		bool close = true;
		WindowEventListeners::iterator it = mWindowEventListeners.find(rw);
		if(it != mWindowEventListeners.end())
		{
			ListenersForOneWindow* lst = it->second;
			ListenersForOneWindow::Iterator it2 = lst->getIterator();
			while(it2.hasMoreElements())
				if(!(it2.getNext()->windowClosing(rw)))
					close = false;
		}
		return close;
	}
	//----------------------------------------------------------------------------------
	void WindowEventImpl::windowClosed(RenderWindow* rw)
	{
		WindowEventListeners::iterator it = mWindowEventListeners.find(rw);
		if(it != mWindowEventListeners.end())
		{
			ListenersForOneWindow* lst = it->second;
			ListenersForOneWindow::Iterator it2 = lst->getIterator();
			while(it2.hasMoreElements())
				it2.getNext()->windowClosed(rw);
		}
	}
	//----------------------------------------------------------------------------------
	void WindowEventImpl::windowFocusChange(RenderWindow* rw)
	{
		WindowEventListeners::iterator it = mWindowEventListeners.find(rw);
		if(it != mWindowEventListeners.end())
		{
			ListenersForOneWindow* lst = it->second;
			ListenersForOneWindow::Iterator it2 = lst->getIterator();
			while(it2.hasMoreElements())
				it2.getNext()->windowFocusChange(rw);
		}
	}
	//----------------------------------------------------------------------------------------------------------
	void WindowEventUtilities::addWindowEventListener( RenderWindow* _window, WindowEventListener* _listener )
	{
		WindowEventImpl::getSingleton().addListener( _window, _listener );
	}
	//----------------------------------------------------------------------------------------------------------
	void WindowEventUtilities::removeWindowEventListener( RenderWindow* _window, WindowEventListener* _listener )
	{
		WindowEventImpl::getSingleton().removeListener( _window, _listener );
	}
	//----------------------------------------------------------------------------------------------------------
	size_t WindowEventUtilities::getNumWindowEventListeners()
	{
		return WindowEventImpl::getSingleton().size();
	}
		
} // namespace GothOgre