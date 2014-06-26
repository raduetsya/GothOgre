#include "GUISystem_Precompiled.h"
#include "GUISystem_ProgressDialogManager.h"
#include "GUISystem_ProgressDialog.h"


namespace GUISystem
{
	//------------------------------------------------------------------------
	ProgressDialogManager::ProgressDialogManager()
	{
		mDialog = nullptr;
	}
	//------------------------------------------------------------------------
	ProgressDialogManager::~ProgressDialogManager()
	{
		destroyDialog();
	}
	//------------------------------------------------------------------------
	void ProgressDialogManager::initialise()
	{
		registerProgressListener();
	}		
	//------------------------------------------------------------------------
	void ProgressDialogManager::shutdown()
	{
		destroyDialog();
		unregisterProgressListener();
	}
	//------------------------------------------------------------------------
	void ProgressDialogManager::progressStarted()
	{
		if(!ResourceGroupManager::getSingleton().resourceGroupExists(
			ResourceGroup::GUIMEDIA))
			return;

		createDialog();;
	}
	//------------------------------------------------------------------------
	void ProgressDialogManager::notifyEndDialog()
	{
		destroyDialog();
	}
	//------------------------------------------------------------------------
	void ProgressDialogManager::createDialog()
	{
		destroyDialog();
		mDialog = new ProgressDialog;
		mDialog->eventEndDialog = MyGUI::newDelegate(this, &ProgressDialogManager::notifyEndDialog);
	}
	//------------------------------------------------------------------------
	void ProgressDialogManager::destroyDialog()
	{
		if(mDialog)
		{
			delete mDialog;
			mDialog = nullptr;
		}
	}
}
