#ifndef OIS_INPUT_MANAGER_H
#define OIS_INPUT_MANAGER_H


namespace GothOgre
{
	//-----------------------------------------------------------------------
	class OISInputSupport : public InputSupport,
		public OIS::KeyListener, public OIS::MouseListener,
		public Ogre::FrameListener, public Ogre::WindowEventListener
	{
	public:
		OISInputSupport();
		~OISInputSupport();

		const String& getName() const;

		void initialise(RenderWindow* _window, bool _useOISMouse);
		void initialise(RenderWindow* _window) {initialise(_window, true);}
		void shutdown();

		void clipCursor(int _left, int _top, int _width, int _height);
		void freeCursor();
		void setCursorPos(int _x, int _y);
		void getCursorPos(int* _px, int* _py) const;
		bool isKeyDown(const KeyCode& _keyCode) const;
		bool isMouseDown(const MouseButton& _mouseButton) const;

	protected:
		bool mouseMoved( const OIS::MouseEvent& _evt );
		bool mousePressed( const OIS::MouseEvent& _evt, OIS::MouseButtonID _id );
		bool mouseReleased( const OIS::MouseEvent& _evt, OIS::MouseButtonID _id );
		bool keyPressed( const OIS::KeyEvent& _evt );
		bool keyReleased( const OIS::KeyEvent& _evt );
		void windowResized( RenderWindow* _window );
		bool frameStarted( const Ogre::FrameEvent& _evt );

	protected:
		void createOISInputMgr();
		void destroyOISInputMgr();
		void createOISKeyboard();
		void destroyOISKeyboard();
		void createOISMouse();
		void destroyOISMouse();
		void clipCursorPos();

	protected:
		OIS::InputManager*  mOISInputMgr;
		OIS::Keyboard*		mOISKeyboard;
		OIS::Mouse*			mOISMouse;
		bool                mClipCursor;
		int					mClipLeft;
		int					mClipTop;
		int					mClipWidth;
		int					mClipHeight;
	};

} // namespace GothOgre

#endif // OIS_INPUT_MANAGER_H