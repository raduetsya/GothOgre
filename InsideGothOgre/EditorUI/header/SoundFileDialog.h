#ifndef SOUND_FILE_DIALOG_H
#define SOUND_FILE_DIALOG_H


namespace GUISystem
{
	//-----------------------------------------------------------------------
	class BaseSoundFileDialog : public BaseFileDialog
	{
	public:
		BaseSoundFileDialog();
	};


	//-----------------------------------------------------------------------
	ATTRIBUTE_CLASS_CONFIG_SECTION( SoundFileDialog, "SoundFileDialog");
	class SoundFileDialog : public BaseSoundFileDialog
	{
	public:
		SoundFileDialog();
	};


	//-----------------------------------------------------------------------
	ATTRIBUTE_CLASS_CONFIG_SECTION( MusicFileDialog, "MusicFileDialog");
	class MusicFileDialog : public BaseSoundFileDialog
	{
	public:
		MusicFileDialog();
	};

};

#endif // SOUND_FILE_DIALOG_H