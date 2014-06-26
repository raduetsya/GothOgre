#include "Precompiled.h"
#include "WinOISInputSupport.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <windows.h>
#include <shellapi.h>
#include "windowsX.h"

namespace GothOgre
{
	WinOISInputSupport::PtrsByHwnd WinOISInputSupport::ms_PtrsByHwnd;
	//------------------------------------------------------------------
	WinOISInputSupport::WinOISInputSupport()
		: mHwnd(NULL), mHcur(NULL), mMouseTrackingEnabled(false),
		  mOldWindowProc(nullptr), mMouseAbsX(0), mMouseAbsY(0), mMouseAbsZ(0),
		  mUtf16Converter(nullptr)
	{
	}
	//------------------------------------------------------------------
	WinOISInputSupport::~WinOISInputSupport()
	{
	}
	//------------------------------------------------------------------------------
	const String& WinOISInputSupport::getName() const
	{
		static const String s = "WinOISInputSupport";
		return s;
	}
	//------------------------------------------------------------------
	void WinOISInputSupport::initialise(RenderWindow* _window)
	{
		// call parent's initialisation
		OISInputSupport::initialise(_window, _window->isFullScreen());

		if(sizeof(String::value_type) != sizeof(WCHAR))
			mUtf16Converter = UnicodeUtil::openConverter("UTF16-LE");

		// shutdown mouse if the mode was changed
		if(!mOISMouse)
		{
			// replace standard Ogre's window procedure with WinOISInputSupport::sWindowProc
			// if it has not been replaced yet
			GOTHOGRE_INFO("WinOISInputSupport: Replacing window procedure");
			getRenderWindow()->getCustomAttribute("WINDOW", &mHwnd);
			ms_PtrsByHwnd.insert(std::make_pair(mHwnd, this));
			mOldWindowProc = (WNDPROC) ::GetWindowLong((HWND)mHwnd, GWL_WNDPROC);
			::SetWindowLong(mHwnd, GWL_WNDPROC, (LONG) sWindowProc);
			enableMouseTracking();
		}
	}
	//------------------------------------------------------------------
	void WinOISInputSupport::shutdown()
	{
		// restore old window procedure if it has been replaced
		if (mOldWindowProc) 
		{
			GOTHOGRE_INFO("WinOISInputSupport: Restoring window procedure...");
			disableMouseTracking();
			::SetWindowLong(mHwnd, GWL_WNDPROC, (long)mOldWindowProc);
			mOldWindowProc = NULL;
		}

		// free cursor if it has been clipped
		::ClipCursor(NULL);

		// destroy temporary Win32 cursor
		if(mHcur)
		{
			::DestroyCursor(mHcur);
			mHcur = NULL;
		}
		ms_PtrsByHwnd.erase(mHwnd);

		// call parent's shutdown
		OISInputSupport::shutdown();
	}
	//------------------------------------------------------------------
	void WinOISInputSupport::clipCursor(int _left, int _top, int _width, int _height)
	{
		if(mOISMouse)
		{
			// use OIS to clip the cursor
			OISInputSupport::clipCursor(_left, _top, _width, _height);
		}
		else
		{
			// use Win32 API to clip cursor
			POINT topLeft;
			topLeft.x = _left;
			topLeft.y = _top;
			::ClientToScreen(mHwnd, &topLeft);

			RECT rect;
			rect.left = topLeft.x;
			rect.top = topLeft.y;
			rect.right = topLeft.x + _width;
			rect.bottom = topLeft.y + _height;
			::ClipCursor(&rect);
		}
	}
	//------------------------------------------------------------------
	void WinOISInputSupport::freeCursor()
	{
		if(mOISMouse)
		{
			OISInputSupport::freeCursor();
		}
		else
		{
			::ClipCursor(NULL);
		}
	}
	//------------------------------------------------------------------
	void WinOISInputSupport::setCursorPos(int _x, int _y)
	{
		if(mOISMouse)
		{
			OISInputSupport::setCursorPos(_x, _y);
		}
		else
		{
			mMouseAbsX = _x;
			mMouseAbsY = _y;
			
			POINT point;
			point.x = mMouseAbsX;
			point.y = mMouseAbsY;
			::ClientToScreen(mHwnd, &point);
			::SetCursorPos(point.x, point.y);
		}
	}
	//------------------------------------------------------------------
	void WinOISInputSupport::getCursorPos(int* _px, int* _py) const
	{
		if(mOISMouse)
		{
			OISInputSupport::getCursorPos(_px, _py);
		}
		else
		{
			POINT point;
			::GetCursorPos(&point);
			::ScreenToClient(mHwnd, &point);
			*_px = point.x;
			*_py = point.y;
		}
	}
	//------------------------------------------------------------------
	bool WinOISInputSupport::isMouseDown(const MouseButton& _mouseButton) const
	{
		if(mOISMouse)
		{
			return OISInputSupport::isMouseDown(_mouseButton);
		}
		else
		{
			bool down;
			switch(_mouseButton)
			{
				case MouseButton::LEFT:   down = mMousePressed[0]; break;
				case MouseButton::RIGHT:  down = mMousePressed[1]; break;
				case MouseButton::MIDDLE: down = mMousePressed[2]; break;
				default:                  down = false;
			}
			return down;
		}
	}
	//------------------------------------------------------------------
	LRESULT CALLBACK WinOISInputSupport::sWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ms_PtrsByHwnd[hWnd]->windowProc(uMsg, wParam, lParam);
	}
	//------------------------------------------------------------------
	LRESULT WinOISInputSupport::windowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// The user has dropped files on the window
		switch(uMsg)
		{
			case WM_DROPFILES:
			{
				HDROP hDrop = (HDROP)wParam;
				WCHAR szFile[MAX_PATH] = { 0 };
				UINT fcount = DragQueryFileW(hDrop, 0xFFFFFFFF, NULL, 0);

				for(UINT i = 0; i < fcount; i++) 
				{
					DragQueryFileW(hDrop, i, szFile, MAX_PATH);

					String filename;
					if(sizeof(String::value_type) == sizeof(WCHAR))
					{
						filename.assign( (const String::value_type*) szFile);
					}
					else
					{
						CodepageConverter::ByteBuffer buf( (const uint8*) szFile, wcslen(szFile) * sizeof(WCHAR) );
						mUtf16Converter->bufferToUnicode(filename, buf);
					}

					InputSystem::getSingleton()._injectFileDrop( filename );
				}

				::DragFinish(hDrop);
				return 0; // the message was processed
			}
		
			// Hide WIN32 cursor when the cursor pos is within the window
			case WM_SETCURSOR:
			{
				int hitTest = LOWORD(lParam);
				if(hitTest == HTCLIENT)
				{
					hideWinCursor();
					return TRUE; // to halt further processing 
				}
				break;
			}

			case WM_MOUSELEAVE:
			{
				notifyMouseTrackingDisabled();
				MouseEvent evt2(mMouseAbsX, mMouseAbsY, mMouseAbsZ);
				InputSystem::getSingleton()._injectMouseLeave(evt2);
				break;
			}

			case WM_NCMOUSEMOVE:
			{
				MouseEvent evt2(mMouseAbsX, mMouseAbsY, mMouseAbsZ);
				InputSystem::getSingleton()._injectMouseLeave(evt2);
				break;
			}

			case WM_MOUSEMOVE:
			{
				enableMouseTracking(); // we need to determine when the cursor moves outside the window
				int newAbsX = GET_X_LPARAM(lParam);
				int newAbsY = GET_Y_LPARAM(lParam);
				if(mMouseAbsX != newAbsX || mMouseAbsY != newAbsY)
				{
					mMouseAbsX = GET_X_LPARAM(lParam);
					mMouseAbsY = GET_Y_LPARAM(lParam);
					MouseEvent evt2(mMouseAbsX, mMouseAbsY, mMouseAbsZ);
					InputSystem::getSingleton()._injectMouseMove(evt2);
				}
				break;
			}

			case WM_MOUSEWHEEL:
			{
				mMouseAbsZ += GET_WHEEL_DELTA_WPARAM(wParam);
				MouseEvent evt2(mMouseAbsX, mMouseAbsY, mMouseAbsZ);
				InputSystem::getSingleton()._injectMouseMove(evt2);
				break;
			}

			case WM_LBUTTONDOWN:
			case WM_LBUTTONDBLCLK: // Double-clicking the left mouse button actually generates a sequence of four messages: WM_LBUTTONDOWN, WM_LBUTTONUP, WM_LBUTTONDBLCLK, and WM_LBUTTONUP (MSDN).
			{
				mMousePressed[0] = true;
				mMouseAbsX = GET_X_LPARAM(lParam);
				mMouseAbsY = GET_Y_LPARAM(lParam);
				MouseEvent evt2(mMouseAbsX, mMouseAbsY, mMouseAbsZ, MouseButton::LEFT);
				InputSystem::getSingleton()._injectMousePress(evt2);
				break;
			}
			case WM_LBUTTONUP:
			{
				mMousePressed[0] = false;
				mMouseAbsX = GET_X_LPARAM(lParam);
				mMouseAbsY = GET_Y_LPARAM(lParam);
				MouseEvent evt2(mMouseAbsX, mMouseAbsY, mMouseAbsZ, MouseButton::LEFT);
				InputSystem::getSingleton()._injectMouseRelease(evt2);
				break;
			}

			case WM_RBUTTONDOWN:
			{
				mMousePressed[1] = true;
				mMouseAbsX = GET_X_LPARAM(lParam);
				mMouseAbsY = GET_Y_LPARAM(lParam);
				MouseEvent evt2(mMouseAbsX, mMouseAbsY, mMouseAbsZ, MouseButton::RIGHT);
				InputSystem::getSingleton()._injectMousePress(evt2);
				break;
			}
			case WM_RBUTTONUP:
			{
				mMousePressed[1] = false;
				mMouseAbsX = GET_X_LPARAM(lParam);
				mMouseAbsY = GET_Y_LPARAM(lParam);
				MouseEvent evt2(mMouseAbsX, mMouseAbsY, mMouseAbsZ, MouseButton::RIGHT);
				InputSystem::getSingleton()._injectMouseRelease(evt2);
				break;
			}

			case WM_MBUTTONDOWN:
			{
				mMousePressed[2] = true;
				mMouseAbsX = GET_X_LPARAM(lParam);
				mMouseAbsY = GET_Y_LPARAM(lParam);
				MouseEvent evt2(mMouseAbsX, mMouseAbsY, mMouseAbsZ, MouseButton::MIDDLE);
				InputSystem::getSingleton()._injectMousePress(evt2);
				break;
			}
			case WM_MBUTTONUP:
			{
				mMousePressed[2] = false;
				mMouseAbsX = GET_X_LPARAM(lParam);
				mMouseAbsY = GET_Y_LPARAM(lParam);
				MouseEvent evt2(mMouseAbsX, mMouseAbsY, mMouseAbsZ, MouseButton::MIDDLE);
				InputSystem::getSingleton()._injectMouseRelease(evt2);
				break;
			}
		}
	
		// call Ogre's window procedure
		return CallWindowProc((WNDPROC)mOldWindowProc, mHwnd, uMsg, wParam, lParam);
	}
	//------------------------------------------------------------------
	void WinOISInputSupport::hideWinCursor()
	{
		if(!mHcur)
		{
			int cxCursor = ::GetSystemMetrics(SM_CXCURSOR);
			int cyCursor = ::GetSystemMetrics(SM_CYCURSOR);
			
			int maskSize = (cxCursor * cyCursor + 7) / 8;
			BYTE* andMaskCursor = new BYTE[maskSize];
			BYTE* xorMaskCursor = new BYTE[maskSize];
			for(int j = 0; j < maskSize; ++j)
			{
				andMaskCursor[j] = 0xFF;
				xorMaskCursor[j] = 0x00;
			}
			
			mHcur = ::CreateCursor(GetModuleHandle(NULL), 0, 0, cxCursor, cyCursor, andMaskCursor, xorMaskCursor);
			
			delete[] andMaskCursor;
			delete[] xorMaskCursor;
		}
		::SetCursor(mHcur);
	}
	//------------------------------------------------------------------
	void WinOISInputSupport::enableMouseTracking(bool _enable)
	{
		if(_enable != mMouseTrackingEnabled)
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.hwndTrack = mHwnd;
			tme.dwFlags = TME_LEAVE;
			if(!_enable)
				tme.dwFlags |= TME_CANCEL;
			tme.dwHoverTime = 0;
			TrackMouseEvent(&tme);
			mMouseTrackingEnabled = _enable;
		}
	}
	//------------------------------------------------------------------
	void WinOISInputSupport::disableMouseTracking()
	{
		enableMouseTracking(false);
	}
	//------------------------------------------------------------------
	void WinOISInputSupport::notifyMouseTrackingDisabled()
	{
		mMouseTrackingEnabled = false;
	}	

} // namespace GothOgre

#endif // OGRE_PLATFORM == OGRE_PLATFORM_WIN32
