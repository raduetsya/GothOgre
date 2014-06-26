#ifndef MUSIC_FILE_DIALOG_H
#define MUSIC_FILE_DIALOG_H

namespace GUISystem
{
	ATTRIBUTE_CLASS_CONFIG_SECTION( MusicFileDialog, "MusicFileDialog");

	class MusicFileDialog : public SoundFileDialog
	{
	public:
		SoundFileDialog(const String& _resourceGroup);
	};
};

#endif // MUSIC_FILE_DIALOG_H