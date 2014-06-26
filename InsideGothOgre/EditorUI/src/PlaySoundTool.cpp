#include "Precompiled.h"
#include "SoundFileDialog.h"
#include "SoundPlayerDialog.h"
#include "PlaySoundTool.h"

namespace GUISystem
{
	PlaySoundTool::PlaySoundTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->insertItem("Sound", MenuItemType::Popup, "#{Sound}");

		mb->insertItem("PlaySound", MenuItemType::Normal, "#{PlaySound}", "Sound");
		mb->setItemDescription("PlaySound", "#{PlaySound_Desc}");
		ToolParam tp1;
		tp1.resourceGroup = ResourceGroup::SOUND;
		mb->attachTool("PlaySound", this, (AnyLite) tp1);

		mb->insertItem("PlayMusic", MenuItemType::Normal, "#{PlayMusic}", "Sound");
		mb->setItemDescription("PlayMusic", "#{PlayMusic_Desc}");
		ToolParam tp2;
		tp2.resourceGroup = ResourceGroup::MUSIC;
		mb->attachTool("PlayMusic", this, (AnyLite) tp2);
	}
	//------------------------------------------------------------------------------------
	PlaySoundTool::~PlaySoundTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->removeItem("PlaySound");
		mb->removeItem("PlayMusic");
		mb->removeItem("Sound");
	}
	//------------------------------------------------------------------------------------
	ToolOperation* PlaySoundTool::activate(const Any& _toolParameter)
	{
		const ToolParam& tp = *any_cast<ToolParam>(&_toolParameter);
		return new PlaySoundToolOperation(tp);
	}
	//------------------------------------------------------------------------------------
	PlaySoundToolOperation::PlaySoundToolOperation(const PlaySoundTool::ToolParam& _tp)
		: mFileDialog(nullptr), mPlayerDialog(nullptr)
	{
		if(_tp.resourceGroup == ResourceGroup::SOUND)
			mFileDialog = new SoundFileDialog;
		else
			mFileDialog = new MusicFileDialog;

		mFileDialog->eventEndDialog = MyGUI::newDelegate(this, &PlaySoundToolOperation::notifyEndFileDialog);
	}
	//------------------------------------------------------------------------------------
	PlaySoundToolOperation::~PlaySoundToolOperation()
	{
		if(mPlayerDialog)
			delete mPlayerDialog;
		if(mFileDialog)
			delete mFileDialog;
	}
	//------------------------------------------------------------------------------------
	void PlaySoundToolOperation::notifyEndFileDialog(const String& _filename)
	{
		if(!_filename.empty())
		{
			if(!mPlayerDialog)
			{
				mPlayerDialog = new SoundPlayerDialog;
				mPlayerDialog->eventEndDialog = MyGUI::newDelegate(this, &PlaySoundToolOperation::notifyEndPlayerDialog);
			}
			mPlayerDialog->setFileName(_filename, mFileDialog->getResourceGroup());
		}
		else
		{
			// Close button
			deactivateTool();
		}
	}
	//------------------------------------------------------------------------------------
	void PlaySoundToolOperation::notifyEndPlayerDialog()
	{
		if(mPlayerDialog)
		{
			delete mPlayerDialog;
			mPlayerDialog = nullptr;
		}
	}
	//------------------------------------------------------------------------------------
	PlaySoundToolFactory::PlaySoundToolFactory()
		: ToolFactory("PlaySoundTool", "EditorToolManager")
	{
	}
	//------------------------------------------------------------------------------------
	Tool* PlaySoundToolFactory::createTool()
	{
		return new PlaySoundTool;
	}
	
} // namespace GUISystem