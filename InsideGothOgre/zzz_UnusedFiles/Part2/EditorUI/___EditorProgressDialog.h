#ifndef EDITOR_PROGRESS_DIALOG_H
#define EDITOR_PROGRESS_DIALOG_H

namespace GUISystem
{
	ATTRIBUTE_CLASS_LAYOUT        ( EditorProgressDialog, "EditorProgressDialog.layout");

	class EditorProgressDialog : public ProgressDialog
	{
	public:
		EditorProgressDialog();
	};

} // namespace GUISystem

#endif // EDITOR_PROGRESS_DIALOG_H