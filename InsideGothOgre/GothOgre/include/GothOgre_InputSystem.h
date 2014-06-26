#ifndef GOTHOGRE_INPUT_SYSTEM_H
#define GOTHOGRE_INPUT_SYSTEM_H

#include "GothOgre_SafeList.h"
#include "GothOgre_SafeOrderedList.h"
#include "GothOgre_Priority.h"


namespace GothOgre
{
	class KeyCode;
	class KeyComb;
	class KeyList;
	class KeyEvent;
	class MouseButton;
	class MouseEvent;
	class KeyListener;
	class MouseListener;
	class NewFrameListener;
	class NewWindowEventListener;
	class ViewportAbsCoord;


	//-----------------------------------------------------------------------
	/** Front-end for input operations */
	class GOTHOGRE_EXPORT InputSystem : public Singleton<InputSystem>,
		public Ogre::FrameListener, public Ogre::WindowEventListener
	{
	public:
		InputSystem();
		~InputSystem();

		void initialise(RenderWindow* _window);
		void shutdown();

		/** Returns RenderWindow specified when initializing */
		RenderWindow* getRenderWindow() const;

		/** Gets width (in pixels) of Ogre's render window */
		int getWidth() const;

		/** Gets height (in pixels) of Ogre's render window */
		int getHeight() const;

		/** Confines the cursor to a rectangular area on the Ogre's render window
		@param _coord pointer to MyGUI::IntCoord specified a rectangular area 
		relative to the top left corner of the Ogre's render window.
		If this parameter is nullptr, the cursor is free to move anywhere on the screen. */
		void clipCursor(int _left, int _top, int _width, int _height);
		void clipCursor(const ViewportAbsCoord& _vpAbsCoord);

		/** Frees cursor to move anywhere on the screen */
		void freeCursor();

		/** Moves the cursor to the specified coordinates relative to the top left corner
		of the Ogre's render window. */
		void setCursorPos(int _x, int _y, bool _sendMouseMovedEvent = true);

		/** Retrieves the cursor's position, in coordinates relative to the top left corner
		of the Ogre's render window. */
		void getCursorPos(int* _px, int* _py) const;

		/**	Returns true if key is down	*/
		bool isKeyDown(const KeyCode& _keyCode) const;
		
		/**	Returns true if mouse button is down */
		bool isMouseDown(const MouseButton& _mouseButton) const;

		/**	Returns true if any key (or mouse button) from a provided list is down */
		bool isKeyDown(const KeyComb& _keyComb) const;

		/**	Returns true if all keys (and mouse buttons) from a provided list is down */
		bool isKeyDown(const KeyList& _keyList) const;

		/** Try to close the application.
		This function raises windowClosing event,
		then (if windowClosing returns true) raises windowClosed event, 
		and after that it sets a special flag to stop rendering 
		at the next frame. */
		void setExitFlag();

	private:
		// Inherited from Ogre::NewWindowEventListener
		void windowMoved(RenderWindow* rw);
		void windowResized(RenderWindow* rw);
		bool windowClosing(RenderWindow* rw);
		void windowClosed(RenderWindow* rw);
		void windowFocusChange(RenderWindow* rw);

		// Inherited from Ogre::NewFrameListener
		bool frameStarted( const Ogre::FrameEvent& evt );
		bool frameRenderingQueued(const Ogre::FrameEvent& evt);
		bool frameEnded(const Ogre::FrameEvent& evt);

	public:
		// Privately used by implementations of classes derived from InputManager
		void _injectKeyPress(const KeyEvent& _evt);
		void _injectKeyRelease(const KeyEvent& _evt);
		void _injectMousePress(const MouseEvent& _evt);
		void _injectMouseRelease(const MouseEvent& _evt);
		void _injectMouseMove(const MouseEvent& _evt);
		void _injectMouseLeave(const MouseEvent& _evt);
		void _injectFileDrop(const String& _filename);

	public:
		// Privately used by implementations of KeyListener,
		// MouseListener, NewWindowEventListener, NewFrameListener
		void _initListener(KeyListener* _listener);
		void _addListener(KeyListener* _listener, Priority _priority);
		void _removeListener(KeyListener* _listener);

		void _initListener(MouseListener* _lListener);
		void _addListener(MouseListener* _listener, Priority _priority);
		void _removeListener(MouseListener* _listener);

		void _initListener(NewWindowEventListener* _listener);
		void _addListener(NewWindowEventListener* _listener);
		void _removeListener(NewWindowEventListener* _listener);

		void _initListener(NewFrameListener* _listener);
		void _addListener(NewFrameListener* _listener);
		void _removeListener(NewFrameListener* _listener);

	private:
		typedef SafeOrderedList<Priority, KeyListener*>	 KeyListeners;
		typedef SafeOrderedList<Priority, MouseListener*> MouseListeners;
		typedef SafeList<NewWindowEventListener*>   WindowEventListeners;
		typedef SafeList<NewFrameListener*>         FrameListeners;

		bool                    mClosed;
		KeyListeners			mKeyListeners;
		MouseListeners			mMouseListeners;
		WindowEventListeners    mWindowEventListeners;
		FrameListeners          mFrameListeners;
		int                     mMouseAbsX;
		int                     mMouseAbsY;
		int                     mMouseAbsZ;
		bool                    mInitialised;
	};

} // namespace GothOgre

#endif // GOTHOGRE_INPUT_SYSTEM_H
