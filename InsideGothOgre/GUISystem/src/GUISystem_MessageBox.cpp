#include "GUISystem_Precompiled.h"
#include "GUISystem_MessageBox.h"


namespace GUISystem
{
	//---------------------------------------------------------------------------
	BaseMessageBox::BaseMessageBox()
	{
	}
	//---------------------------------------------------------------------------
	BaseMessageBox::~BaseMessageBox()
	{
	}
	//---------------------------------------------------------------------------
	void BaseMessageBox::initialise2(BaseMessageBox* _owner, MyGUI::Widget* _parent)
	{
		BaseLayout::initialiseByAttributes( _owner, _parent );
		gatherDialogInfo();
		clearButtons();
		setModal(true);
		mDestroyOnChoice = true;
		setIcon(MessageBoxIcon::INFO);
	}
	//---------------------------------------------------------------------------
	void BaseMessageBox::gatherDialogInfo()
	{
		mButtonSize = mButton1->getSize();
		mButtonSkin = mButton1->getUserString("Skin");
		mButtons.push_back(mButton1);
		
		if(mButton2)
		{
			mButtonOffset = mButton2->getLeft() - mButton1->getRight();
			mButtons.push_back(mButton2);
		}
		else
			mButtonOffset = 10;

		mInitialButtonPlaceSize = mButtonPlace->getSize();
		mInitialTextSize = mTextBox->getSize();
		mInitialWindowSize = getWindow()->getSize();
	}
	//---------------------------------------------------------------------------
	void BaseMessageBox::clearButtons()
	{
		if(!mButtons.empty())
		{
			for(size_t i = 0; i != mButtons.size(); ++i)
				mButtons[i]->setVisible(false);

			mPoolButtons.insert(mPoolButtons.end(), mButtons.begin(), mButtons.end());
			mButtons.clear();
			updateSize();
		}
	}
	//---------------------------------------------------------------------------
	void BaseMessageBox::setCaption(const String& _caption)
	{
		mCaption = _caption;
		_setCaption(_caption);
	}
	//---------------------------------------------------------------------------
	void BaseMessageBox::_setCaption(const String& _caption)
	{
		MyGUI::UString caption2 = MyGUI::LanguageManager::getInstance().replaceTags(_caption);
		getWindow()->setCaption(caption2);
	}
	//---------------------------------------------------------------------------
	void BaseMessageBox::setMessage(const String& _message)
	{
		mTextBox->setCaptionWithReplacing(_message);
		updateSize();
	}
	//---------------------------------------------------------------------------
	void BaseMessageBox::setMessage(const String& _message, const String& _tag1, const String& _replace1)
	{
		MyGUI::LanguageManager::getInstance().addUserTag(_tag1, _replace1);
		setMessage(_message);
	}
	//---------------------------------------------------------------------------
	void BaseMessageBox::setMessage(const String& _message, const String& _tag1, const String& _replace1, const String& _tag2, const String& _replace2)
	{
		MyGUI::LanguageManager::getInstance().addUserTag(_tag1, _replace1);
		MyGUI::LanguageManager::getInstance().addUserTag(_tag2, _replace2);
		setMessage(_message);
	}
	//---------------------------------------------------------------------------
	void BaseMessageBox::setMessage(const String& _message, const String& _tag1, const String& _replace1, const String& _tag2, const String& _replace2, const String& _tag3, const String& _replace3)
	{
		MyGUI::LanguageManager::getInstance().addUserTag(_tag1, _replace1);
		MyGUI::LanguageManager::getInstance().addUserTag(_tag2, _replace2);
		MyGUI::LanguageManager::getInstance().addUserTag(_tag3, _replace3);
		setMessage(_message);
	}
	//---------------------------------------------------------------------------
	void BaseMessageBox::setIcon(MessageBoxIcon _icon)
	{
		if(mCaption.empty())
			_setCaption( guessCaptionByIcon(_icon) );
		
		String iconName = guessIconNameById(_icon);
		setIcon(iconName);
	}
	//---------------------------------------------------------------------------
	void BaseMessageBox::setIcon(const String& _iconName)
	{
		mImageBox->setItemName(_iconName);
	}
	//---------------------------------------------------------------------------
	void BaseMessageBox::setIcon(const String& _iconName, const String& _imageGroup, const String& _imageResource)
	{
		mImageBox->setItemResource(_imageResource);
		mImageBox->setItemGroup(_imageGroup);
		setIcon(_iconName);
	}
	//---------------------------------------------------------------------------
	String BaseMessageBox::guessCaptionByIcon(MessageBoxIcon _icon) const
	{
		static const String stdNames[] =
			{"#{Information}", "#{Question}", "#{Warning}", "#{Error}"};
		if(0 <= _icon && _icon < sizeof(stdNames) / sizeof(stdNames[0]))
			return stdNames[_icon];
		return StringUtil::BLANK;
	}
	//---------------------------------------------------------------------------
	String BaseMessageBox::guessIconNameById(MessageBoxIcon _icon) const
	{
		static const String stdNames[] =
			{"Information", "Question", "Warning", "Error"};
		if(0 <= _icon && _icon < sizeof(stdNames) / sizeof(stdNames[0]))
			return stdNames[_icon];
		return StringUtil::BLANK;
	}
	//---------------------------------------------------------------------------
	void BaseMessageBox::addButton(MessageBoxButton _button, const String& _textOverride)
	{
		// Use standard name for button if not specified
		String text = _textOverride;
		if(text.empty())
		{
			static const String stdNames[] =
				{"#{Ok}", "#{Yes}", "#{No}", "#{Abort}", "#{Retry}", 
				 "#{Ignore}", "#{Cancel}", "#{Try}", "#{Continue}"};
			if(0 <= _button && _button < sizeof(stdNames) / sizeof(stdNames[0]))
				text = stdNames[_button];
		}

		// You should specify name for non-standard button
		if(text.empty())
			return;

		// Get a button from pool
		MyGUI::Button* newButton = nullptr;
		if(!mPoolButtons.empty())
		{
			newButton = mPoolButtons.back();
			mPoolButtons.pop_back();
		}

		// Create a new widget if the pool is empty
		if(!newButton)
			newButton = mButtonPlace->createWidget<MyGUI::Button>(
				mButtonSkin, MyGUI::IntCoord(), MyGUI::Align());

		// Setup properties of the created button
		newButton->eventMouseButtonClick += newDelegate(this, &BaseMessageBox::notifyButtonPressed);
		newButton->setCaptionWithReplacing(text);
		newButton->setUserData(_button);
		newButton->setVisible(true);
		mButtons.push_back(newButton);

		updateSize();
	}
	//---------------------------------------------------------------------------
	void BaseMessageBox::updateSize()
	{
		// Get the minimal size of the text box to show the entire message
		MyGUI::IntSize textSize = mTextBox->getTextSize();
		
		// Calculate the minimum size of the button place
		MyGUI::IntCoord allButtons;
		size_t numButtons = mButtons.size();
		allButtons.width = numButtons * mButtonSize.width + (numButtons - 1) * mButtonOffset;
		allButtons.height = mButtonSize.height;

		// Resizing the window to contain the entire message text and all the buttons
		MyGUI::IntCoord windowCoord(MyGUI::IntPoint(), mInitialWindowSize);
		int deltaXt = textSize.width - mInitialTextSize.width;
		int deltaYt = textSize.height - mInitialTextSize.height;
		int deltaXb = allButtons.width - mInitialButtonPlaceSize.width;
		int deltaX = std::max(deltaXt, deltaXb);
		int deltaY = std::max(deltaY, 0);
		windowCoord.width += deltaX;
		windowCoord.height += deltaY;

		// Center the window in the viewport
		MyGUI::IntSize viewSize = MyGUI::RenderManager::getInstance().getViewSize();
		windowCoord.left = (viewSize.width - windowCoord.width) / 2;
		windowCoord.top = (viewSize.height - windowCoord.height) / 2;
		getWindow()->setCoord(windowCoord);

		// Move buttons to correct places
		allButtons.left = (mButtonPlace->getSize().width - allButtons.width) / 2;
		allButtons.top = 0;
		
		MyGUI::IntCoord buttonCoord;
		buttonCoord.left = allButtons.left;
		buttonCoord.top = 0;
		buttonCoord.width = mButtonSize.width;
		buttonCoord.height = mButtonSize.height;

		for(size_t i = 0; i != numButtons; ++i)
		{
			mButtons[i]->setCoord(buttonCoord);
			buttonCoord.left += mButtonSize.width + mButtonOffset;
		}
	}
	//---------------------------------------------------------------------------
	void BaseMessageBox::setDestroyOnChoice(bool _destroyOnChoice)
	{
		mDestroyOnChoice = _destroyOnChoice;
	}
	//---------------------------------------------------------------------------
	void BaseMessageBox::notifyButtonPressed(MyGUI::Widget* _sender)
	{
		MessageBoxButton button = *_sender->getUserData<MessageBoxButton>();
		eventChoice(button);

		if(mDestroyOnChoice)
			delete this; // destroy the dialog
	}

} // namespace GUISystem