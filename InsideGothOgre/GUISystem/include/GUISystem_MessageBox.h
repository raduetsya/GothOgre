#ifndef GUISYSTEM_MESSAGE_BOX_H
#define GUISYSTEM_MESSAGE_BOX_H

#include "GUISystem_BaseLayout.h"


// Remove the "IGNORE" macro (could be defined in WinBase.h)
#ifdef IGNORE
#undef IGNORE
#endif

namespace GUISystem
{
	//------------------------------------------------------------------------
	/** Describes an icon in a message box. */
	class MessageBoxIcon
	{
	public:
		enum Enum
		{
			INFO,
			QUEST,
			WARNING,
			ERROR
		};
		
		MessageBoxIcon(int _value = INFO) : mValue(Enum(_value)) {}
		operator int() const {return mValue;}
	private:
		Enum mValue;
	};


	//------------------------------------------------------------------------
	/** Describes a button in a message box. */
	class MessageBoxButton
	{
	public:
		enum Enum
		{
			OK,
			YES,
			NO,
			ABORT,
			RETRY,
			IGNORE,
			CANCEL,
			TRY,
			CONTINUE,
			BUTTON1,
			BUTTON2,
			BUTTON3,
			BUTTON4
		};

		MessageBoxButton(int _value = OK) : mValue(Enum(_value)) {}
		operator int() const {return mValue;}
	private:
		Enum mValue;
	};


	//-------------------------------------------------------------------------
	// BaseMessageBox
	//-------------------------------------------------------------------------
	// Attribute
	ATTRIBUTE_CLASS_LAYOUT( BaseMessageBox, "#{MessageBox_Layout}" );

	/** Base class for the "MessageBox" class. */
	class GUISYSTEM_EXPORT BaseMessageBox : public BaseLayout
	{
	public:
		BaseMessageBox();
		~BaseMessageBox();

		/** Sets the window's caption. */
		void setCaption(const String& _caption);
		
		/** Sets the text of the message. */
		void setMessage(const String& _message);
		void setMessage(const String& _message, const String& _tag1, const String& _replace1);
		void setMessage(const String& _message, const String& _tag1, const String& _replace1, const String& _tag2, const String& _replace2);
		void setMessage(const String& _message, const String& _tag1, const String& _replace1, const String& _tag2, const String& _replace2, const String& _tag3, const String& _replace3);
		
		/** Sets the icon. 
		The default icon is INFO. */
		void setIcon(MessageBoxIcon _icon);
		void setIcon(const String& _iconName);
		void setIcon(const String& _iconName, const String& _imageGroup, const String& _imageResource);

		/** Adds a button to the message box.
		You can override the button text if you wish. */
		void addButton(MessageBoxButton _button, const String& _overrideName = StringUtil::BLANK);

		/** Removes all the buttons from the message box. */
		void clearButtons();

		/** Sets if the dialog (and instance of the class)
		must be destroyed automatically after the user clicks a button.
		The default value: true. */
		void setDestroyOnChoice(bool _destroyOnChoice);

		/** Event which is raised when an user clicks a button.
		Signature: void method(MessageBoxButton). */
		typedef MyGUI::delegates::CDelegate1<MessageBoxButton> EventHandle_Choice;
		EventHandle_Choice eventChoice;

	protected:
		template<typename T>
		void initialiseByAttributes(T* _owner, MyGUI::Widget* _parent = nullptr);
		
	private:
		void initialise2(BaseMessageBox* _owner, MyGUI::Widget* _parent);
		void gatherDialogInfo();
		void _setCaption(const String& _caption);
		String guessCaptionByIcon(MessageBoxIcon _icon) const;
		String guessIconNameById(MessageBoxIcon _icon) const;
		void updateSize();
		void notifyButtonPressed(MyGUI::Widget* _sender);

	private:
		ATTRIBUTE_FIELD_WIDGET_NAME(BaseMessageBox, mTextBox, "text");
		MyGUI::TextBox*        mTextBox;

		ATTRIBUTE_FIELD_WIDGET_NAME(BaseMessageBox, mImageBox, "icon");
		MyGUI::ImageBox*       mImageBox;

		ATTRIBUTE_FIELD_WIDGET_NAME(BaseMessageBox, mButtonPlace, "buttonPlace");
		MyGUI::Widget*         mButtonPlace;

		ATTRIBUTE_FIELD_WIDGET_NAME(BaseMessageBox, mButton1, "button1");
		MyGUI::Button*         mButton1;

		ATTRIBUTE_FIELD_WIDGET_NAME_OPTIONAL(BaseMessageBox, mButton2, "button2");
		MyGUI::Button*         mButton2;

		bool                   mDestroyOnChoice;

		vector<MyGUI::Button*>::type mButtons;
		vector<MyGUI::Button*>::type mPoolButtons;

		String                 mCaption;
		int                    mButtonOffset;
		MyGUI::IntSize         mButtonSize;
		String                 mButtonSkin;
		MyGUI::IntSize         mInitialTextSize;
		MyGUI::IntSize         mInitialButtonPlaceSize;
		MyGUI::IntSize         mInitialWindowSize;
	};
	//--------------------------------------------------------------------------
	template<typename T>
	void BaseMessageBox::initialiseByAttributes(T* _owner, MyGUI::Widget* _parent)
	{
		applyClassAttributes(_owner);
		initialise2(_owner, _parent);		
	}




	//-------------------------------------------------------------------------
	// MessageBox
	//-------------------------------------------------------------------------
	/** A small window to show message to the user. */
	class GUISYSTEM_EXPORT MessageBox : public BaseMessageBox
	{
	public:
		MessageBox()
		{
			initialiseByAttributes(this);
		}
	};

} // namespace GUISystem

#endif // GUISYSTEM_MESSAGE_BOX_H
