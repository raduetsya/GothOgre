#ifndef WIN_OIS_INPUT_MANAGER_H
#define WIN_OIS_INPUT_MANAGER_H

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "OISInputSupport.h"

namespace GothOgre
{
	//-----------------------------------------------------------------------
	class WinOISInputSupport : public OISInputSupport
	{
	public:
		WinOISInputSupport();
		~WinOISInputSupport();

		const String& getName() const;

		void initialise(RenderWindow* _window);
		void shutdown();

		void clipCursor(int _left, int _top, int _width, int _height);
		void freeCursor();
		void setCursorPos(int _x, int _y);
		void getCursorPos(int* _px, int* _py) const;
		bool isMouseDown(const MouseButton& _mouseButton) const;

	private:
		void hideWinCursor();
		void enableMouseTracking(bool _enable = true);
		void disableMouseTracking();
		void notifyMouseTrackingDisabled();
		LRESULT windowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK sWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		HWND		       mHwnd;
		HCURSOR            mHcur;
		bool               mMouseTrackingEnabled;
		WNDPROC		       mOldWindowProc;
		CodepageConverter* mUtf16Converter;
		int                mMouseAbsX;
		int                mMouseAbsY;
		int                mMouseAbsZ;
		bool               mMousePressed[3];

		typedef map<HWND, WinOISInputSupport*>::type PtrsByHwnd;
		static PtrsByHwnd ms_PtrsByHwnd;
	};


} // namespace GothOgre

#endif // OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#endif // WIN_OIS_INPUT_MANAGER_H