#ifndef SOUND_PLAYER_DIALOG_H
#define SOUND_PLAYER_DIALOG_H


namespace GUISystem
{
	class SoundPlayerWorker;

	//-----------------------------------------------------------------------------
	// SoundPlayerDialog
	ATTRIBUTE_CLASS_LAYOUT  ( SoundPlayerDialog, "SoundPlayerDialog.layout");
	ATTRIBUTE_CLASS_CONFIG_SECTION( SoundPlayerDialog, "SoundPlayerDialog");

	class SoundPlayerDialog : public BaseLayout
	{
	public:
		SoundPlayerDialog();
		~SoundPlayerDialog();

		void setFileName(const String& _filename, const String& _resourceGroup);

		// event for pressed "close" button
		typedef MyGUI::delegates::CDelegate0 EventHandle_Result;
		EventHandle_Result eventEndDialog;

	private:
		void notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name);
		void notifyButtonPlay(MyGUI::Widget* _sender);
		void notifyButtonStop(MyGUI::Widget* _sender);
		void notifyCheckLooping(MyGUI::Widget* _sender);
		void notifyVolumeChanged(MyGUI::ScrollBar* _sender, size_t _position);
		
		GOTHOGRE_DECLARE_FRAME_STARTED(SoundPlayerDialog);

		static String secondsToString(Real _time, bool _microseconds);

	private:
		ATTRIBUTE_FIELD_WIDGET_NAME(SoundPlayerDialog, mSliderPosition, "sliderPosition");
		MyGUI::ScrollBar*    mSliderPosition;

		ATTRIBUTE_FIELD_WIDGET_NAME(SoundPlayerDialog, mSliderVolume, "sliderVolume");
		MyGUI::ScrollBar*    mSliderVolume;

		ATTRIBUTE_FIELD_WIDGET_NAME(SoundPlayerDialog, mEditPosition, "editPosition");
		MyGUI::EditBox*      mEditPosition;

		ATTRIBUTE_FIELD_WIDGET_NAME(SoundPlayerDialog, mEditLength, "editLength");
		MyGUI::EditBox*      mEditLength;

		ATTRIBUTE_FIELD_WIDGET_NAME(SoundPlayerDialog, mEditFileName, "editFileName");
		MyGUI::EditBox*      mEditFileName;

		ATTRIBUTE_FIELD_WIDGET_NAME(SoundPlayerDialog, mCheckLooping, "checkLooping");
		MyGUI::Button*       mCheckLooping;

		ATTRIBUTE_FIELD_WIDGET_NAME(SoundPlayerDialog, mButtonPlay, "buttonPlay");
		MyGUI::Button*       mButtonPlay;

		ATTRIBUTE_FIELD_WIDGET_NAME(SoundPlayerDialog, mButtonStop, "buttonStop");
		MyGUI::Button*       mButtonStop;

		SoundPlayerWorker*   mWorker;
	};
}

#endif // SOUND_PLAYER_DIALOG_H