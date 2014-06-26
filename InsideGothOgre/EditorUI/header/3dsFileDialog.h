#ifndef _3DS_FILE_DIALOG_H
#define _3DS_FILE_DIALOG_H

namespace GUISystem
{
	ATTRIBUTE_CLASS_CONFIG_SECTION (_3dsFileDialog, "3dsFileDialog");

	class _3dsFileDialog : public BaseFileDialog
	{
	public:
		_3dsFileDialog();
	};
};

#endif // _3DS_FILE_DIALOG_H