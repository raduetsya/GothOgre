#include "Precompiled.h"
#include "AboutGothOgreDialog.h"

namespace GUISystem
{
	AboutGothOgreDialog::AboutGothOgreDialog()
	{
		initialiseByAttributes(this);
		setModal(true);
	}
	//--------------------------------------------------------------------------
	void AboutGothOgreDialog::notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name)
	{
		if (_name == "close")
			close();
	}
	//--------------------------------------------------------------------------
	void AboutGothOgreDialog::close()
	{
		eventEndDialog();
	}

} // namespace GUISystem
