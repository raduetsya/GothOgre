#ifndef ZEN_FILE_DIALOG_H
#define ZEN_FILE_DIALOG_H

namespace GUISystem
{
	ATTRIBUTE_CLASS_CONFIG_SECTION( ZenFileDialog, "ZenFileDialog");

	class ZenFileDialog : public BaseFileDialog
	{
	public:
		ZenFileDialog();
	};
};

#endif // ZEN_FILE_DIALOG_H