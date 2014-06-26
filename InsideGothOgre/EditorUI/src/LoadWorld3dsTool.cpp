#include "Precompiled.h"
#include "3dsFileDialog.h"
#include "LoadWorld3dsTool.h"

namespace GUISystem
{
	//------------------------------------------------------------------------------------
	LoadWorld3dsTool::LoadWorld3dsTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->insertItem("File", MenuItemType::Popup, "#{File}");

		mb->insertItem("LoadWorld3dsSep", MenuItemType::Separator, "", "File");

		mb->insertItem("LoadWorld3ds", MenuItemType::Normal, "#{LoadWorld3ds}", "File");
		mb->setItemDescription("LoadWorld3ds", "#{LoadWorld3ds_Desc}");
		ToolParam tp1;
		tp1.merge = false;
		tp1.recentlyFileIndex = -1;
		mb->attachTool("LoadWorld3ds", this, (AnyLite) tp1);

		mb->insertItem("MergeWorld3ds", MenuItemType::Normal, "#{MergeWorld3ds}", "File");
		mb->setItemDescription("MergeWorld3ds", "#{MergeWorld3ds_Desc}");
		ToolParam tp2;
		tp2.merge = true;
		tp2.recentlyFileIndex = -1;
		mb->attachTool("MergeWorld3ds", this, (AnyLite) tp2);

		updateMenuItems();
		registerWorldListener();
	}
	//------------------------------------------------------------------------------------
	LoadWorld3dsTool::~LoadWorld3dsTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->removeItem("MergeWorld3ds");
		mb->removeItem("LoadWorld3ds");
		mb->removeItem("LoadWorld3dsSep");
		mb->removeItem("File");
	}
	//------------------------------------------------------------------------------------
	ToolOperation* LoadWorld3dsTool::activate(const Any& _toolParameter)
	{
		const ToolParam& tp = *any_cast<ToolParam>( &_toolParameter );
		return new LoadWorld3dsToolOperation( tp );
	}
	//------------------------------------------------------------------------------------
	void LoadWorld3dsTool::worldActiveChanged(const WorldEvent& _evt)
	{
		updateMenuItems();
	}
	//------------------------------------------------------------------------------------
	void LoadWorld3dsTool::updateMenuItems()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		mb->setItemEnabled("MergeWorld3ds", WorldManager::getSingleton().getActiveWorld() != nullptr);
	}
	//------------------------------------------------------------------------------------
	LoadWorld3dsToolOperation::LoadWorld3dsToolOperation(const LoadWorld3dsTool::ToolParam& _tp)
		: mDialog(nullptr)
	{
		if(_tp.recentlyFileIndex == -1)
		{
			mDialog = new _3dsFileDialog;
			mDialog->setType(FileDialog::LOAD);
			mDialog->setCaption(_tp.merge ? "#{Select3dsToMerge}" : "#{Select3dsToLoad}");
			mDialog->setOkText(_tp.merge ? "#{Merge}" : "#{Load}");
			mDialog->eventEndDialog = MyGUI::newDelegate(this, &LoadWorld3dsToolOperation::notifyEndDialog);
			mMerge = _tp.merge;
		}
		else
		{
			//processFileSelect( filename, _tp.merge );
			deactivateTool();
		}
	}
	//------------------------------------------------------------------------------------
	LoadWorld3dsToolOperation::~LoadWorld3dsToolOperation()
	{
		if(mDialog)
			delete mDialog;
	}
	//------------------------------------------------------------------------------------
	void LoadWorld3dsToolOperation::notifyEndDialog(const String& _filename)
	{
		mDialog->setVisible(false);
		if(!_filename.empty())
		{
			processFileSelect(_filename, mMerge);
		}
		deactivateTool();
	}
	//------------------------------------------------------------------------------------
	void LoadWorld3dsToolOperation::processFileSelect(const String& _filename, bool _merge)
	{
		Progress& progress = Progress::getSingleton();
		progress.begin();
		progress.setCaption(_merge ? "#{Merging}" : "#{Loading}");
		progress.setMessage(_filename);

		ResourceLocationScanner::getSingleton().scan(ResourceGroup::TEXTURES);
		ResourceLocationScanner::getSingleton().scan(ResourceGroup::MESHES);

		if(!_merge)
		{
			String worldName = _filename;
			size_t i = worldName.rfind('.');
			if(i != String::npos)
				worldName.resize(i);
			worldName += ".zen";

			World* world = WorldManager::getSingleton().createWorld(worldName);
			WorldManager::getSingleton().setActiveWorld(world);
		}

		World* world = WorldManager::getSingleton().getActiveWorld();
		if(world)
		{
			WorldObjectPtr vobLevelCompo = ZenManager::getSingleton().createZenObject("VobLevelCompo");
			world->getRootWorldObject()->addChild(vobLevelCompo);
			vobLevelCompo->setName(_filename);
			vobLevelCompo->setParameter("ShowVisual", true);
			vobLevelCompo->setParameter("Visual", _filename);
		}

		progress.end();
	}
	//------------------------------------------------------------------------------------
	LoadWorld3dsToolFactory::LoadWorld3dsToolFactory()
		: ToolFactory("LoadWorld3dsTool", "EditorToolManager")
	{
	}
	//------------------------------------------------------------------------------------
	Tool* LoadWorld3dsToolFactory::createTool()
	{
		return new LoadWorld3dsTool;
	}
	
} // namespace GUISystem
