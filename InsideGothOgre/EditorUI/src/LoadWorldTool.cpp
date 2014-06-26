#include "Precompiled.h"
#include "ZenFileDialog.h"
#include "LoadWorldTool.h"

namespace GUISystem
{
	LoadWorldTool::LoadWorldTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->insertItem("File", MenuItemType::Popup, "#{File}");

		mb->insertItem("LoadWorldSep", MenuItemType::Separator, "", "File");

		mb->insertItem("LoadWorld", MenuItemType::Normal, "#{LoadWorld}", "File");
		mb->setItemDescription("LoadWorld", "#{LoadWorld_Desc}");
		ToolParam tp1;
		tp1.merge = false;
		tp1.recentlyFileIndex = -1;
		mb->attachTool("LoadWorld", this, (AnyLite) tp1);

		mb->insertItem("MergeWorld", MenuItemType::Normal, "#{MergeWorld}", "File");
		mb->setItemDescription("MergeWorld", "#{MergeWorld_Desc}");
		ToolParam tp2;
		tp2.merge = true;
		tp2.recentlyFileIndex = -1;
		mb->attachTool("MergeWorld", this, (AnyLite) tp2);

		updateMenuItems();
		registerWorldListener();
	}
	//------------------------------------------------------------------------------------
	LoadWorldTool::~LoadWorldTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->removeItem("LoadWorld");
		mb->removeItem("MergeWorld");
		mb->removeItem("LoadWorldSep");
		mb->removeItem("File");
	}
	//------------------------------------------------------------------------------------
	ToolOperation* LoadWorldTool::activate(const Any& _toolParameter)
	{
		const ToolParam& tp = *any_cast<ToolParam>( &_toolParameter );
		return new LoadWorldToolOperation( tp );
	}
	//------------------------------------------------------------------------------------
	void LoadWorldTool::worldActiveChanged(const WorldEvent& _evt)
	{
		updateMenuItems();
	}
	//------------------------------------------------------------------------------------
	void LoadWorldTool::updateMenuItems()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->setItemEnabled("MergeWorld", WorldManager::getSingleton().getActiveWorld() != nullptr);
	}
	//------------------------------------------------------------------------------------
	LoadWorldToolOperation::LoadWorldToolOperation(const LoadWorldTool::ToolParam& _tp)
		: mDialog(nullptr)
	{
		if(_tp.recentlyFileIndex == -1)
		{
			mDialog = new ZenFileDialog;
			mDialog->setType(FileDialog::LOAD);
			mDialog->setCaption(_tp.merge ? "#{SelectZenToMerge}" : "#{SelectZenToLoad}");
			mDialog->setOkText(_tp.merge ? "#{Merge}" : "#{Load}");
			mDialog->eventEndDialog = MyGUI::newDelegate(this, &LoadWorldToolOperation::notifyEndDialog);
			mMerge = _tp.merge;
		}
		else
		{
			//processFileSelect( filename, _tp.merge );
			deactivateTool();
		}
	}
	//------------------------------------------------------------------------------------
	LoadWorldToolOperation::~LoadWorldToolOperation()
	{
		if(mDialog)
			delete mDialog;
	}
	//------------------------------------------------------------------------------------
	void LoadWorldToolOperation::notifyEndDialog(const String& _filename)
	{
		mDialog->setVisible(false);
		if(!_filename.empty())
		{
			processFileSelect( _filename, mMerge );
		}
		deactivateTool();
	}
	//------------------------------------------------------------------------------------
	void LoadWorldToolOperation::processFileSelect( const String& _filename, bool _merge )
	{
		if(_merge)
		{
			World* world = WorldManager::getSingleton().getActiveWorld();
			if(world)
			{
				WorldManager::getSingleton().mergeWorld(
					world, _filename, ResourceGroup::WORLDS );
			}
		}
		else
		{
			WorldManager::getSingleton().loadWorld(
				_filename, ResourceGroup::WORLDS );
		}
	}
	//------------------------------------------------------------------------------------
	LoadWorldToolFactory::LoadWorldToolFactory()
		: ToolFactory("LoadWorldTool", "EditorToolManager")
	{
	}
	//------------------------------------------------------------------------------------
	Tool* LoadWorldToolFactory::createTool()
	{
		return new LoadWorldTool;
	}
	
} // namespace GUISystem