#ifndef GOTHOGRE_INPUT_SUPPORT_H
#define GOTHOGRE_INPUT_SUPPORT_H


namespace GothOgre
{
	class KeyCode;
	class MouseButton;

	//-----------------------------------------------------------------------
	// Auxiliary class
	// This class is designed to implement input support.
	// Don't use it directly until you create a new implementation of input support.
	class GOTHOGRE_EXPORT InputSupport
	{
	public:
		InputSupport();
		virtual ~InputSupport();

		/** Returns the name of this input support. */
		virtual const String& getName() const = 0;

		/** Returns RenderWindow specified when initializing */
		RenderWindow* getRenderWindow() const {return mWindow;}

		/** Gets width (in pixels) of Ogre's render window */
		int getWidth() const {return mWindow->getWidth();}

		/** Gets height (in pixels) of Ogre's render window */
		int getHeight() const {return mWindow->getHeight();}

		/** Confines the cursor to a rectangular area on the Ogre's render window
		@param _coord pointer to MyGUI::IntCoord specified a rectangular area 
		relative to the top left corner of the Ogre's render window.
		If this parameter is nullptr, the cursor is free to move anywhere on the screen. */
		virtual void clipCursor(int _left, int _top, int _width, int _height) = 0;

		/** Frees cursor to move anywhere on the screen */
		virtual void freeCursor() = 0;

		/** Moves the cursor to the specified coordinates relative to the top left corner
		of the Ogre's render window. */
		virtual void setCursorPos(int _x, int _y) = 0;

		/** Retrieves the cursor's position, in coordinates relative to the top left corner
		of the Ogre's render window. */
		virtual void getCursorPos(int* _px, int* _py) const = 0;

		/**	Returns true if key is down	*/
		virtual bool isKeyDown(const KeyCode& _keyCode) const = 0;
		
		/**	Returns true if mouse button is down */
		virtual bool isMouseDown(const MouseButton& _mouseButton) const = 0;

	public:
		/** Initialize the input support */
		virtual void initialise(RenderWindow* _window) = 0;

		/** Shut down the input support */
		virtual void shutdown() = 0;

	private:
		friend class InputSystem;
		void _setRenderWindow(RenderWindow* _window);

	private:
		RenderWindow*         mWindow;
	};


	//-----------------------------------------------------------------------
	// Auxiliary class.
	// This class is designed to implement input support.
	// Don't use it directly until you create a new implementation of input support.
	class GOTHOGRE_EXPORT InputSupportRegistration : public Singleton<InputSupportRegistration>
	{
	public:
		InputSupportRegistration();
		~InputSupportRegistration();

		// Returns a pointer to the registered implementation of unicode support
		InputSupport* getSupport() const;

	private:
		friend class InputSupport;
		void registerSupport(InputSupport* _support);
		void unregisterSupport(InputSupport* _support);

	private:
		InputSupport* mSupport;
	};

} // namespace GothOgre

#endif // GOTHOGRE_INPUT_SUPPORT_H
