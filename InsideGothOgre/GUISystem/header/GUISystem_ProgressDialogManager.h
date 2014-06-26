#ifndef GUISYSTEM_PROGRESS_DIALOG_MANAGER_H
#define GUISYSTEM_PROGRESS_DIALOG_MANAGER_H


namespace GUISystem
{
	class ProgressDialog;

	/** This manager initializes progress dialog and shows it on the screen
	when a long-time operation started. */
	class ProgressDialogManager
	{
	public:
		ProgressDialogManager();
		~ProgressDialogManager();

		void initialise();
		void shutdown();

	private:
		GOTHOGRE_DECLARE_PROGRESS_STARTED( ProgressDialogManager );
		void notifyEndDialog();

		void createDialog();
		void destroyDialog();

	private:
		ProgressDialog* mDialog;
	};

} // namespace GUISystem

#endif // GUISYSTEM_PROGRESS_DIALOG_MANAGER_H
