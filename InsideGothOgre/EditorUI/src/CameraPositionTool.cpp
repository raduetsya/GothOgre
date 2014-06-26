#include "Precompiled.h"
#include "CameraPositionTool.h"

namespace GUISystem
{
	//------------------------------------------------------------------------------------
	void CameraPositionTool::initConstants()
	{
		ROOT_MENU_ID           = "Tools";
		ROOT_MENU_CAPTION      = "#{Tools}";
		POPUP_MENU_ID          = "CameraPosition";
		POPUP_MENU_CAPTION     = "#{CameraPosition}";
		MENU_SEPARATOR_ID[0]   = "CameraPositionSep";
		MENU_SEPARATOR_ID[1]   = "CameraPositionSep2";
		STORE_MENU_ITEM_DESC   = "#{StoreCameraPosition_Desc}";
		RESTORE_MENU_ITEM_DESC = "#{RestoreCameraPosition_Desc}";

		for(size_t i = 0; i != NUM_POSITIONS; ++i)
		{
			String ip1 = StrConv::toString((i + 1) % NUM_POSITIONS);
			POSITION_CONFIG_KEY_NAME[i]  = "CameraPosition" + ip1;
			ROTATION_CONFIG_KEY_NAME[i]  = "CameraRotation" + ip1;
			STORE_MENU_ITEM_ID[i]        = "StoreCameraPosition" + ip1;
			RESTORE_MENU_ITEM_ID[i]      = "RestoreCameraPosition" + ip1;
			STORE_MENU_ITEM_CAPTION[i]   = "#{StoreCameraPosition}";
			RESTORE_MENU_ITEM_CAPTION[i] = "#{RestoreCameraPosition}";
			DEFAULT_STORE_HOTKEY[i]      = KeyComb(KeyCode::LCONTROL, static_cast<KeyCode::Enum>(KeyCode::ONE + i));
			DEFAULT_RESTORE_HOTKEY[i]    = KeyCode(static_cast<KeyCode::Enum>(KeyCode::ONE + i));
			STORE_HOTKEY_CONFIG_KEY_NAME[i] = "StoreCameraPosition" + ip1;
			RESTORE_HOTKEY_CONFIG_KEY_NAME[i] = "RestoreCameraPosition" + ip1;
		}
	}
	//------------------------------------------------------------------------------------
	CameraPositionTool::CameraPositionTool()
	{
		initConstants();

		MenuBar* mb = MenuBarList::getSingleton().getMenuBar( "EditorMenuBar" );
		mb->insertItem( ROOT_MENU_ID, MenuItemType::Popup, ROOT_MENU_CAPTION );
		mb->insertItem( MENU_SEPARATOR_ID[0], MenuItemType::Separator, StringUtil::BLANK, ROOT_MENU_ID );
		mb->insertItem( POPUP_MENU_ID, MenuItemType::Popup, POPUP_MENU_CAPTION, ROOT_MENU_ID );

		size_t i;
		for(i = 0; i != NUM_POSITIONS; ++i)
		{
			mb->insertItem( STORE_MENU_ITEM_ID[i], MenuItemType::Normal, STORE_MENU_ITEM_CAPTION[i], POPUP_MENU_ID );
			mb->insertHotkey( STORE_MENU_ITEM_ID[i], STORE_HOTKEY_CONFIG_KEY_NAME[i], "Editor", DEFAULT_STORE_HOTKEY[i] );
			mb->setItemDescription( STORE_MENU_ITEM_ID[i], STORE_MENU_ITEM_DESC );

			ToolParam tp;
			tp.store = true;
			tp.index = i;
			mb->attachTool( STORE_MENU_ITEM_ID[i], this, (AnyLite) tp );
		}
		
		mb->insertItem( MENU_SEPARATOR_ID[1], MenuItemType::Separator, StringUtil::BLANK, POPUP_MENU_ID );

		for(i = 0; i != NUM_POSITIONS; ++i)
		{
			mb->insertItem( RESTORE_MENU_ITEM_ID[i], MenuItemType::Normal, RESTORE_MENU_ITEM_CAPTION[i], POPUP_MENU_ID );
			mb->insertHotkey( RESTORE_MENU_ITEM_ID[i], RESTORE_HOTKEY_CONFIG_KEY_NAME[i], "Editor", DEFAULT_RESTORE_HOTKEY[i]);
			mb->setItemDescription( RESTORE_MENU_ITEM_ID[i], RESTORE_MENU_ITEM_DESC );
			
			ToolParam tp;
			tp.store = false;
			tp.index = i;
			mb->attachTool(RESTORE_MENU_ITEM_ID[i], this, (AnyLite) tp);
		}

		updateMenuItems();
		registerWorldListener();
	}
	//------------------------------------------------------------------------------------
	CameraPositionTool::~CameraPositionTool()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");

		size_t i;
		for(i = 0; i != NUM_POSITIONS; ++i)
		{
			mb->removeItem( STORE_MENU_ITEM_ID[i] );
		}

		mb->removeItem( MENU_SEPARATOR_ID[0] );
		mb->removeItem( MENU_SEPARATOR_ID[1] );

		for(i = 0; i != NUM_POSITIONS; ++i)
		{
			mb->removeItem( RESTORE_MENU_ITEM_ID[i] );
		}

		mb->removeItem( POPUP_MENU_ID );
		mb->removeItem( ROOT_MENU_ID );
	}
	//------------------------------------------------------------------------------------
	void CameraPositionTool::updateMenuItems()
	{
		MenuBar* mb = MenuBarList::getSingleton().getMenuBar("EditorMenuBar");
		bool hasActiveWorld = (WorldManager::getSingleton().getActiveWorld() != nullptr);
		for(int i = 0; i != NUM_POSITIONS; ++i)
		{
			mb->setItemEnabled( STORE_MENU_ITEM_ID[i], hasActiveWorld );
			mb->setItemEnabled( RESTORE_MENU_ITEM_ID[i], hasActiveWorld && isPositionSaved(i) );
		}
	}
	//------------------------------------------------------------------------------------
	void CameraPositionTool::worldActiveChanged(const WorldEvent& _evt)
	{
		updateMenuItems();
	}
	//------------------------------------------------------------------------------------
	bool CameraPositionTool::isPositionSaved(int _index)
	{
		World* world = WorldManager::getSingleton().getActiveWorld();
		if(!world)
			return false;

		return WorldEditSettings::getSingleton().hasSetting(
			POSITION_CONFIG_KEY_NAME[_index], world->getName() );
	}
	//------------------------------------------------------------------------------------
	void CameraPositionTool::readPosition(int _index, Vector3& _position, Quaternion& _rotation)
	{
		World* world = WorldManager::getSingleton().getActiveWorld();
		if(!world)
			return;

		_position = WorldEditSettings::getSingleton().getSetting<Vector3>(
			POSITION_CONFIG_KEY_NAME[_index], world->getName() );
		
		_rotation = WorldEditSettings::getSingleton().getSetting<Quaternion>(
			ROTATION_CONFIG_KEY_NAME[_index], world->getName() );
	}
	//------------------------------------------------------------------------------------
	void CameraPositionTool::savePosition(int _index, const Vector3& _position, const Quaternion& _rotation)
	{
		World* world = WorldManager::getSingleton().getActiveWorld();
		if(!world)
			return;

		WorldEditSettings::getSingleton().setSetting<Vector3>(
			POSITION_CONFIG_KEY_NAME[_index], world->getName(), _position);
		
		WorldEditSettings::getSingleton().setSetting<Quaternion>(
			ROTATION_CONFIG_KEY_NAME[_index], world->getName(), _rotation);
	}
	//------------------------------------------------------------------------------------
	ToolOperation* CameraPositionTool::activate(const Any& _toolParameter)
	{
		const ToolParam& tp = any_cast<ToolParam>(_toolParameter);
		if(tp.store)
		{
			World* world = WorldManager::getSingleton().getActiveWorld();
			if(world)
			{
				Camera* camera = world->getCamera();
				savePosition( tp.index, camera->getPosition(), camera->getOrientation());
				updateMenuItems();
			}
		}
		else
		{
			Vector3 position;
			Quaternion rotation;
			readPosition(tp.index, position, rotation);
			World* world = WorldManager::getSingleton().getActiveWorld();
			Camera* camera = world->getCamera();
			camera->setPosition(position);
			camera->setOrientation(rotation);
		}
		return nullptr;
	}
	//------------------------------------------------------------------------------------
	CameraPositionToolFactory::CameraPositionToolFactory()
		: ToolFactory("CameraPositionTool", "EditorToolManager")
	{
	}
	//------------------------------------------------------------------------------------
	Tool* CameraPositionToolFactory::createTool()
	{
		return new CameraPositionTool;
	}
	
} // namespace GUISystem
