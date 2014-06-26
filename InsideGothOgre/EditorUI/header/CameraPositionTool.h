#ifndef CAMERA_POSITION_TOOL_H
#define CAMERA_POSITION_TOOL_H


namespace GUISystem
{
	//-----------------------------------------------------------------------
	class CameraPositionTool : public Tool
	{
	public:
		CameraPositionTool();
		~CameraPositionTool();
		
		ToolOperation* activate(const Any& _toolParameter);

		struct ToolParam
		{
			bool    store;
			size_t  index;
		};

	private:
		GOTHOGRE_DECLARE_WORLD_ACTIVE_CHANGED( CameraPositionTool );
		void updateMenuItems();

	private:
		bool isPositionSaved(int _index);
		void savePosition(int _index, const Vector3& _position, const Quaternion& _rotation);
		void readPosition(int _index, Vector3& _position, Quaternion& _rotation);

	private:
		void initConstants();

		String ROOT_MENU_ID;
		String ROOT_MENU_CAPTION;
		String POPUP_MENU_ID;
		String POPUP_MENU_CAPTION;
		String MENU_SEPARATOR_ID[2];
		String STORE_MENU_ITEM_DESC;
		String RESTORE_MENU_ITEM_DESC;

		static const int NUM_POSITIONS = 10;
		String POSITION_CONFIG_KEY_NAME [ NUM_POSITIONS ];
		String ROTATION_CONFIG_KEY_NAME [ NUM_POSITIONS ];
		String STORE_MENU_ITEM_ID       [ NUM_POSITIONS ];
		String RESTORE_MENU_ITEM_ID     [ NUM_POSITIONS ];
		String STORE_MENU_ITEM_CAPTION  [ NUM_POSITIONS ];
		String RESTORE_MENU_ITEM_CAPTION[ NUM_POSITIONS ];
		KeyList DEFAULT_STORE_HOTKEY    [ NUM_POSITIONS ];
		KeyList DEFAULT_RESTORE_HOTKEY  [ NUM_POSITIONS ];
		String STORE_HOTKEY_CONFIG_KEY_NAME[ NUM_POSITIONS ];
		String RESTORE_HOTKEY_CONFIG_KEY_NAME[ NUM_POSITIONS ];
	};


	//-----------------------------------------------------------------------
	class CameraPositionToolFactory : public ToolFactory
	{
	public:
		CameraPositionToolFactory();
		Tool* createTool();
	};
	
} // namespace GUISystem
	
#endif // CAMERA_POSITION_TOOL_H