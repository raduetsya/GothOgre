#include "Precompiled.h"
#include "FrameStatsConfigDialog.h"

namespace GUISystem
{
	FrameStatsConfigDialog::FrameStatsConfigDialog()
	{
		initialiseByAttributes(this);

		mButtonOK->eventMouseButtonClick = MyGUI::newDelegate(this, &FrameStatsConfigDialog::notifyPressedOK);
		mButtonCancel->eventMouseButtonClick = MyGUI::newDelegate(this, &FrameStatsConfigDialog::notifyPressedCancel);
		mCheckVisible->eventMouseButtonClick = MyGUI::newDelegate(this, &FrameStatsConfigDialog::notifyCheckedVisible);
		mCheckMovable->eventMouseButtonClick = MyGUI::newDelegate(this, &FrameStatsConfigDialog::notifyCheckedMovable);
		mEditTextTemplate->eventEditTextChange = MyGUI::newDelegate(this, &FrameStatsConfigDialog::notifyChangedTextTemplate);

		mListKeys->eventListSelectAccept = MyGUI::newDelegate(this, &FrameStatsConfigDialog::notifyAcceptedKey);
		FrameStats::KeyIterator keyIt = FrameStats::getSingleton().getKeyIterator();
		while(keyIt.hasMoreElements())
		{
			String key = keyIt.getNext();
			mListKeys->addItem(key);
		}

		UISettings& uis = UISettings::getSingleton();
		mOrigVisible      = uis.getSetting<bool>("Visible", "FrameStatsPanel");
		mOrigMovable      = uis.getSetting<bool>("Movable", "FrameStatsPanel");
		mOrigTextTemplate = uis.getSetting<String>("TextTemplate", "FrameStatsPanel");
		mOrigLeft         = uis.getSetting<float>("Left", "FrameStatsPanel");
		mOrigTop          = uis.getSetting<float>("Top", "FrameStatsPanel");
		mOrigWidth        = uis.getSetting<float>("Width", "FrameStatsPanel");
		mOrigHeight       = uis.getSetting<float>("Height", "FrameStatsPanel");

		registerUISettingsListener();
	}
	//--------------------------------------------------------------------------
	FrameStatsConfigDialog::~FrameStatsConfigDialog()
	{
	}
	//--------------------------------------------------------------------------
	void FrameStatsConfigDialog::notifyCheckedVisible(MyGUI::Widget* _sender)
	{
		bool value = !mCheckVisible->getButtonPressed();
		UISettings::getSingleton().setSetting("Visible", "FrameStatsPanel", value);
	}
	//--------------------------------------------------------------------------
	void FrameStatsConfigDialog::notifyCheckedMovable(MyGUI::Widget* _sender)
	{
		bool value = !mCheckMovable->getButtonPressed();
		UISettings::getSingleton().setSetting("Movable", "FrameStatsPanel", value);
	}
	//-----------------------------------------------------------------------------
	void FrameStatsConfigDialog::notifyChangedTextTemplate(MyGUI::EditBox* _sender)
	{
		String value = mEditTextTemplate->getCaption();
		UISettings::getSingleton().setSetting("TextTemplate", "FrameStatsPanel", value);
	}
	//-----------------------------------------------------------------------------
	void FrameStatsConfigDialog::notifyAcceptedKey(MyGUI::ListBox* _sender, size_t _index)
	{
		if(_index < mListKeys->getItemCount())
		{
			String key = mListKeys->getItemNameAt(_index);
			String insertedText = key + ": <" + key + ">\n";
			size_t cursorPos = mEditTextTemplate->getTextCursor();
			mEditTextTemplate->insertText(insertedText, cursorPos);
			notifyChangedTextTemplate(mEditTextTemplate);
		}
	}
	//--------------------------------------------------------------------------
	void FrameStatsConfigDialog::notifyPressedCancel(MyGUI::Widget* _sender)
	{
		UISettings& uis = UISettings::getSingleton();
		uis.beginBatch();
		uis.setSetting("Visible", "FrameStatsPanel", mOrigVisible);
		uis.setSetting("Movable", "FrameStatsPanel", mOrigMovable);
		uis.setSetting("TextTemplate", "FrameStatsPanel", mOrigTextTemplate);
		uis.setSetting("Left", "FrameStatsPanel", mOrigLeft);
		uis.setSetting("Top", "FrameStatsPanel", mOrigTop);
		uis.setSetting("Width", "FrameStatsPanel", mOrigWidth);
		uis.setSetting("Height", "FrameStatsPanel", mOrigHeight);
		uis.endBatch();

		close();
	}
	//--------------------------------------------------------------------------
	void FrameStatsConfigDialog::notifyPressedOK(MyGUI::Widget* _sender)
	{
		close();
	}
	//-----------------------------------------------------------------------------
	void FrameStatsConfigDialog::notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name)
	{
		if (_name == "close")
			close();
	}
	//-----------------------------------------------------------------------------
	void FrameStatsConfigDialog::close()
	{
		eventEndDialog();
	}
	//-----------------------------------------------------------------------------
	void FrameStatsConfigDialog::notifyWindowChangeCoord(MyGUI::Window* _sender)
	{
		MyGUI::IntCoord containerCoord, elementCoord;

		containerCoord = mPanelHContainer->getCoord();
		elementCoord.set(0, 0, containerCoord.width / 2, containerCoord.height);
		mPanelHElement0->setCoord(elementCoord);
		elementCoord.left += elementCoord.width;
		mPanelHElement1->setCoord(elementCoord);

		containerCoord = mPanelVContainer->getCoord();
		elementCoord.set(0, 0, containerCoord.width, containerCoord.height / 2);
		mPanelVElement0->setCoord(elementCoord);
		elementCoord.top += elementCoord.height;
		mPanelVElement1->setCoord(elementCoord);
	}
	//-----------------------------------------------------------------------------
	void FrameStatsConfigDialog::uiSettingsChanged(const ConfigSettingsEvent& _evt)
	{
		UISettings& uis = UISettings::getSingleton();
		mCheckVisible->setButtonPressed(uis.getSetting<bool>("Visible", "FrameStatsPanel"));
		mCheckMovable->setButtonPressed(uis.getSetting<bool>("Movable", "FrameStatsPanel"));
		
		String textTemplate = uis.getSetting<String>("TextTemplate", "FrameStatsPanel");
		if(MyGUI::UString(textTemplate) != mEditTextTemplate->getCaption())
			mEditTextTemplate->setCaption(textTemplate);
	}
}