#ifndef DEFAULT_INPUT_SYSTEM_H
#define DEFAULT_INPUT_SYSTEM_H

namespace GothOgre
{
	/** Base class to implement of the InputManager interface. 
	@remark
	The class implements work with listeners and priorities. */
	class DefaultInputSystem : public InputSystem,
	                           public Ogre::WindowEventListener, 
	                           public Ogre::FrameListener
	{
	public:
		DefaultInputSystem();
		virtual ~DefaultInputSystem();

		virtual void initialise(RenderWindow* _window);
		virtual void shutdown();

		RenderWindow* getRenderWindow() const;
		int getWidth() const;
		int getHeight() const;

		void clipCursorToViewport(Viewport* _viewport);
		
		virtual bool isMouseDown(const MouseButton& _mouseButton) const = 0;
		virtual bool isKeyDown(const KeyCode& _keyCode) const = 0;
		bool isKeyDown(const KeyComb& _keyComb) const;
		bool isKeyDown(const KeyList& _keyList) const;
		
		void sendExit();

	protected:
		void addKeyListener(BaseKeyListener* _keyListener, int _order);
		void removeKeyListener(BaseKeyListener* _keyListener);

		void addMouseListener(BaseMouseListener* _mouseListener, int _order);
		void removeMouseListener(BaseMouseListener* _mouseListener);
		
		void addWindowEventListener(BaseWindowEventListener* _windowEventListener);
		void removeWindowEventListener(BaseWindowEventListener* _windowEventListener);

		void addFrameListener(BaseFrameListener* _frameListener);
		void removeFrameListener(BaseFrameListener* _frameListener);

	protected:
		void fireMouseMoved( const MouseEvent& _evt );
		void fireMousePressed( const MouseEvent& _evt );
		void fireMouseReleased( const MouseEvent& _evt );
		void fireKeyPressed( const KeyEvent& _evt );
		void fireKeyReleased( const KeyEvent& _evt );
		void fireFileDropped( const String& _filename);
		
	protected:
		virtual void windowMoved(RenderWindow* rw);
		virtual void windowResized(RenderWindow* rw);
		virtual bool windowClosing(RenderWindow* rw);
		virtual void windowClosed(RenderWindow* rw);
		virtual void windowFocusChange(RenderWindow* rw);

		virtual bool frameStarted( const Ogre::FrameEvent& evt );
		virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
		virtual bool frameEnded(const Ogre::FrameEvent& evt);

	private:
		typedef SafeOrderedList<BaseKeyListener*>	KeyListeners;
		typedef SafeOrderedList<BaseMouseListener*>	MouseListeners;
		typedef SafeList<BaseWindowEventListener*>  WindowEventListeners;
		typedef SafeList<BaseFrameListener*>        FrameListeners;

		KeyListeners			mKeyListeners;
		MouseListeners			mMouseListeners;
		WindowEventListeners    mWindowEventListeners;
		FrameListeners          mFrameListeners;

		bool                    mOgreListener;
		RenderWindow*			mWindow;
		bool                    mExit;
	};

} // namespace GothOgre

#endif // DEFAULT_INPUT_SYSTEM_H
