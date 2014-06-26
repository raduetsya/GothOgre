#ifndef GUISYSTEM_PROGRESS_DIALOG_H
#define GUISYSTEM_PROGRESS_DIALOG_H

#include "GUISystem_BaseLayout.h"

namespace GUISystem
{
	//-------------------------------------------------------------------------
	// BaseProgressDialog
	//-------------------------------------------------------------------------
	// Class attributes
	ATTRIBUTE_CLASS_LAYOUT(         BaseProgressDialog, "#{ProgressDialog_Layout}" );
	ATTRIBUTE_CLASS_CONFIG_SECTION( BaseProgressDialog, "#{ProgressDialog_ConfigSection}" );

	/** Base class for the "ProgressDialog" class. */
	class BaseProgressDialog : public BaseLayout
	{
	public:
		BaseProgressDialog();
		~BaseProgressDialog();

		/** Event which is raised when the user closed the dialog.
		Signature: void method(). */
		typedef MyGUI::delegates::CDelegate0 EventHandle_Result;
		EventHandle_Result eventEndDialog;

	protected:
		template<typename T>
		void initialiseByAttributes(T* _owner, MyGUI::Widget* _parent = nullptr);
		
	private:
		void initialise2(BaseProgressDialog* _owner, MyGUI::Widget* _parent);

		GOTHOGRE_DECLARE_FRAME_STARTED( BaseProgressDialog );
		GOTHOGRE_DECLARE_PROGRESS_ENDED( BaseProgressDialog );
		GOTHOGRE_DECLARE_MESSAGE_LOGGED( BaseProgressDialog );

		void notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name);
		void notifyButtonClose(MyGUI::Widget* _sender);
		void notifyButtonStopTimer(MyGUI::Widget* _sender);
		void updateButtons();
		void close();

	private:
		ATTRIBUTE_FIELD_WIDGET_NAME_OPTIONAL(BaseProgressDialog, mTextCaption, "textCaption");
		MyGUI::TextBox*     mTextCaption;

		ATTRIBUTE_FIELD_WIDGET_NAME_OPTIONAL(BaseProgressDialog, mTextMessage, "textMessage");
		MyGUI::TextBox*     mTextMessage;

		ATTRIBUTE_FIELD_WIDGET_NAME_OPTIONAL(BaseProgressDialog, mProgressBar, "progressBar");
		MyGUI::ProgressBar*       mProgressBar;

		ATTRIBUTE_FIELD_WIDGET_NAME_OPTIONAL(BaseProgressDialog, mListLog, "listLog");
		MyGUI::ListBox*           mListLog;

		ATTRIBUTE_FIELD_WIDGET_NAME_OPTIONAL(BaseProgressDialog, mButtonClose, "buttonClose");
		MyGUI::Button*         mButtonClose;

		ATTRIBUTE_FIELD_WIDGET_NAME_OPTIONAL(BaseProgressDialog, mButtonStopTimer, "buttonStopTimer");
		MyGUI::Button*         mButtonStopTimer;

	private:
		bool   mWaitProgressEnd;
		size_t mNumInfos;
		size_t mNumWarnings;
		size_t mNumErrors;
		Real   mTimeout;
	};
	//--------------------------------------------------------------------------
	template<typename T>
	void BaseProgressDialog::initialiseByAttributes(T* _owner, MyGUI::Widget* _parent)
	{
		applyClassAttributes(_owner);
		initialise2(_owner, _parent);		
	}



	//-------------------------------------------------------------------------
	// ProgressDialog
	//-------------------------------------------------------------------------
	/* Dialog which can display a progress bar. */
	class ProgressDialog : public BaseProgressDialog
	{
	public:
		ProgressDialog()
		{
			initialiseByAttributes(this);
		}
	};


} // namespace GUISystem

#endif // GUISYSTEM_PROGRESS_DIALOG_H
