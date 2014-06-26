#include "Precompiled.h"
#include "WorldViewportWindow.h"

namespace GUISystem
{
	WorldViewportWindow::WorldViewportWindow()
	{
		initialiseByAttributes(this);
	}
	//--------------------------------------------------------------------------
	void WorldViewportWindow::notifyWindowChangeCoord(MyGUI::Window* _sender)
	{
		MyGUI::Widget* clientWidget = getMainWidget()->getClientWidget();
		MyGUI::IntCoord coord = clientWidget->getAbsoluteCoord();

		int winWidth = InputSystem::getSingleton().getWidth();
		int winHeight = InputSystem::getSingleton().getHeight();
		Real dimLeft = Real(coord.left) / winWidth;
		Real dimTop = Real(coord.top) / winHeight;
		Real dimWidth = Real(coord.width) / winWidth;
		Real dimHeight = Real(coord.height) / winHeight;

		Real epsX = Real(2) / winWidth;
		Real epsY = Real(2) / winHeight;
		
		dimLeft   = Math::Clamp(dimLeft,   Real(    0 ), Real( 1 - epsX    ));
		dimTop    = Math::Clamp(dimTop,    Real(    0 ), Real( 1 - epsY    ));
		dimWidth  = Math::Clamp(dimWidth,  Real( epsX ), Real( 1 - dimLeft ));
		dimHeight = Math::Clamp(dimHeight, Real( epsY ), Real( 1 - dimTop  ));

		Viewport* viewport = World::getSingleton().getViewport();
		viewport->setDimensions(dimLeft, dimTop, dimWidth, dimHeight);
	}

} // namespace GUISystem
