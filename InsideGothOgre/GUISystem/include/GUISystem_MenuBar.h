#ifndef GUISYSTEM_MENU_BAR_H
#define GUISYSTEM_MENU_BAR_H

#include "GUISystem_BaseLayout.h"
#include "GUISystem_HotkeyManager.h"


namespace GUISystem
{
	class Tool;

	//--------------------------------------------------------------------------
	// BaseMenuBar
	//--------------------------------------------------------------------------
	// Class attributes
	ATTRIBUTE_CLASS_LAYOUT( BaseMenuBar, "#{MenuBar_Layout}" );

	/** Menu item's type */
	typedef MyGUI::MenuItemType MenuItemType;

	/** Base class for the "MenuBar" class. */
	class GUISYSTEM_EXPORT BaseMenuBar : public BaseLayout
	{
	public:
		BaseMenuBar();
		virtual ~BaseMenuBar();

		/** Enables or disables private hotkey manager 
		which is used by this menu bar. */
		void setHotkeysEnabled(bool _enable);

		void enableHotkeys() {setHotkeysEnabled(true);}
		void disableHotkeys() {setHotkeysEnabled(false);}

		/** Returns true if the private hotkey manager is enabled. */
		bool areHotkeysEnabled() const;

		/** Insert a new item to the menu 
		@param _id unique identifier of the inserted item
		This function can be called with the same id again
		however the repeated calls don't insert new items.
		@param _itemType one of constants MenuItemType::Enum
		@param _name name (caption) of the inserted item.
		May be empty string
		@param _parentId identifier of a parent item.
		This parameter makes possible to create submenus.
		If the parameter is an empty string the item is inserted
		to the menu bar directly.
		@param _positionId identifier of an item after the new item.
		If the parameter is an empty string then the new item is added
		to the end of menu.
		*/
		void insertItem(const String& _id, MenuItemType _itemType, 
			const String& _caption = "", const String& _parentId = "",
			const String& _positionId = "");

		/** Remove an item from menu.
		This function does not remove an item if it has any children. */
		void removeItem(const String& _id);

		/** Whether if an item exists in this menu. */
		bool hasItem(const String& _id) const;

		/** Returns an items' type */
		MenuItemType getItemType(const String& _id) const;
		
		/** Returns an items' caption */
		const String& getItemCaption(const String& _id) const;

		/** Changes an items' caption */
		void setItemCaption(const String& _id, const String& _caption);

		/** Sets an item's description (may be used in status bar an so on). */
		void setItemDescription(const String& _id, const String& _description);

		/** Returns an item's description. */
		const String& getItemDescription(const String& _id) const;

		/** Insert a new hot key
		@param _id Item identifier
		@param _keyInControlSettings The name of the setting in ControlSettings
		which is used to read actual KeyList
		@param _sectionInControlSettings The name of the section it must be in
		*/
		void insertHotkey(const String& _id, const String& _keyInControlSettings, const String& _sectionInControlSettings, const KeyList& _defaultValue);

		/** Remove a hot key */
		void removeHotkey(const String& _id);

		/** Attach a tool which must be activated when the item is selected */
		void attachTool(const String& _id, Tool* _tool, const Any& _toolParameter = Any());

		/** Detach a tool */
		inline void detachTool(const String& _id);

		/** Enables or disables an item */
		void setItemEnabled(const String& _id, bool _enable);

		/** Enable an item */
		inline void enableItem(const String& _id) {setItemEnabled(_id, true);}
		
		/** Disable an item */
		inline void disableItem(const String& _id) {setItemEnabled(_id, false);}

		/** Whether an item is enabled */
		bool isItemEnabled(const String& _id) const;

		/** Returns identifier of a parent item of a specified item
		The function returns an empty string if the specified item has not a parent */
		String getParent(const String& _id) const;

		/** Returns list of identifiers of children items of a specified item. */
		vector<String>::type getChildren(const String& _id) const;

	protected:
		template<typename T>
		void initialiseByAttributes(T* _owner, MyGUI::Widget* _parent = nullptr);

	private:
		struct ItemData
		{
			String  origCaption;
			String  description;
			Tool*   tool;
			Any     toolParameter;
		};

		/** Internal initialisation. */
		void initialise2(BaseMenuBar* _owner, MyGUI::Widget* _parent);

		/** Finds a menu item by identifier, return nullptr if not found. */
		MyGUI::MenuItem* findItem(const String& _id) const;

		/** Finds a menu item by identifier, throws an exception if not found. */
		MyGUI::MenuItem* getItem(const String& _id) const;

		/** Returns identifier of a specified item. */
		String getItemId(MyGUI::MenuItem* _item) const;

		/** Returns additional data for an item. */
		ItemData* getItemData(MyGUI::MenuItem* _item) const;

		/** Updates real caption (a string which is displayed for user).  */
		void updateCaption(MyGUI::MenuItem* _item);

		/** Updates all real captions.
		This function needs to be called when hotkeys are changed. */
		void updateAllCaptions(MyGUI::MenuCtrl* _ctrl);

		/** Called when a menu is selected by user. */
		void notifyPopupMenuAccept(MyGUI::MenuCtrl* _sender, MyGUI::MenuItem* _item);

		GUISYSTEM_DECLARE_CONTROL_SETTINGS_CHANGED( BaseMenuBar );

	private:
		MyGUI::MenuCtrl*    mMenuCtrl;
		BaseHotkeyManager*  mPrivateHotkeyManager;
		CodepageConverter*  mUtf8Converter;
	};
	
	//--------------------------------------------------------------------------
	template<typename T>
	void BaseMenuBar::initialiseByAttributes(T* _owner, MyGUI::Widget* _parent)
	{
		applyClassAttributes(_owner);
		initialise2(_owner, _parent);		
	}


	

	//--------------------------------------------------------------------------
	// MenuBar
	//--------------------------------------------------------------------------
	/** Manages a menu bar with submenus and items.
	@remark
	Items and submenus are accessible by string-based id.
	*/
	class GUISYSTEM_EXPORT MenuBar : public BaseMenuBar
	{
	public:
		MenuBar(const String& _name);
		~MenuBar();

		/** Returns the name of the menu bar. */
		const String& getName() const;

	private:
		String             mName;
	};


} // namespace GUISystem

#endif // GUISYSTEM_MENU_BAR_H
