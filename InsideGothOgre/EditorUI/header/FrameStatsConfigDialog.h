#ifndef FRAME_STATS_CONFIG_DIALOG_H
#define FRAME_STATS_CONFIG_DIALOG_H

namespace GUISystem
{
	ATTRIBUTE_CLASS_LAYOUT        ( FrameStatsConfigDialog, "FrameStatsConfigDialog.layout");
	ATTRIBUTE_CLASS_CONFIG_SECTION( FrameStatsConfigDialog, "FrameStatsConfigDialog");

	class FrameStatsConfigDialog : public BaseLayout
	{
	public:
		FrameStatsConfigDialog();
		~FrameStatsConfigDialog();
		
		// event for pressed "ok" or "cancel" buttons
		typedef MyGUI::delegates::CDelegate0 EventHandle_Result;
		EventHandle_Result eventEndDialog;

	private:
		GUISYSTEM_DECLARE_UI_SETTINGS_CHANGED(FrameStatsConfigDialog);

		void notifyWindowChangeCoord(MyGUI::Window* _sender);
		void notifyCheckedVisible(MyGUI::Widget* _sender);
		void notifyCheckedMovable(MyGUI::Widget* _sender);
		void notifyChangedTextTemplate(MyGUI::EditBox* _sender);
		void notifyAcceptedKey(MyGUI::ListBox* _sender, size_t _index);
		void notifyPressedOK(MyGUI::Widget* _sender);
		void notifyPressedCancel(MyGUI::Widget* _sender);
		void notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name);

		void close();

	private:
		ATTRIBUTE_FIELD_WIDGET_NAME(FrameStatsConfigDialog, mButtonOK, "buttonOK");
		MyGUI::Button*      mButtonOK;

		ATTRIBUTE_FIELD_WIDGET_NAME(FrameStatsConfigDialog, mButtonCancel, "buttonCancel");
		MyGUI::Button*      mButtonCancel;

		ATTRIBUTE_FIELD_WIDGET_NAME(FrameStatsConfigDialog, mCheckVisible, "checkVisible");
		MyGUI::Button*      mCheckVisible;

		ATTRIBUTE_FIELD_WIDGET_NAME(FrameStatsConfigDialog, mCheckMovable, "checkMovable");
		MyGUI::Button*      mCheckMovable;

		ATTRIBUTE_FIELD_WIDGET_NAME(FrameStatsConfigDialog, mEditTextTemplate, "editTextTemplate");
		MyGUI::EditBox*        mEditTextTemplate;

		ATTRIBUTE_FIELD_WIDGET_NAME(FrameStatsConfigDialog, mListKeys, "listKeys");
		MyGUI::ListBox*        mListKeys;

		ATTRIBUTE_FIELD_WIDGET_NAME(FrameStatsConfigDialog, mPanelHContainer, "panelHContainer");
		MyGUI::Widget*      mPanelHContainer;

		ATTRIBUTE_FIELD_WIDGET_NAME(FrameStatsConfigDialog, mPanelHElement0, "panelHElement0");
		MyGUI::Widget*      mPanelHElement0;

		ATTRIBUTE_FIELD_WIDGET_NAME(FrameStatsConfigDialog, mPanelHElement1, "panelHElement1");
		MyGUI::Widget*      mPanelHElement1;

		ATTRIBUTE_FIELD_WIDGET_NAME(FrameStatsConfigDialog, mPanelVContainer, "panelVContainer");
		MyGUI::Widget*      mPanelVContainer;

		ATTRIBUTE_FIELD_WIDGET_NAME(FrameStatsConfigDialog, mPanelVElement0, "panelVElement0");
		MyGUI::Widget*      mPanelVElement0;

		ATTRIBUTE_FIELD_WIDGET_NAME(FrameStatsConfigDialog, mPanelVElement1, "panelVElement1");
		MyGUI::Widget*      mPanelVElement1;

		bool                mOrigVisible;
		bool                mOrigMovable;
		String              mOrigTextTemplate;
		float               mOrigLeft;
		float               mOrigTop;
		float               mOrigWidth;
		float               mOrigHeight;
	};

} // namespace GUISystem

#endif // FRAME_STATS_CONFIG_DIALOG_H