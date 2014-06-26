#include "GUISystem_Precompiled.h"
#include "GUISystem_MenuBar.h"
#include "GUISystem_MenuBarList.h"
#include "GUISystem_Tool.h"

namespace GUISystem
{
	//-------------------------------------------------------------------------
	// BaseMenuBar
	//-------------------------------------------------------------------------
	BaseMenuBar::BaseMenuBar()
	{
		mPrivateHotkeyManager = new BaseHotkeyManager;

		mUtf8Converter = nullptr;
		if(sizeof(String::value_type) == 2)
			mUtf8Converter = UnicodeUtil::openConverter("UTF-8");
	}
	//-------------------------------------------------------------------------
	void BaseMenuBar::initialise2(BaseMenuBar* _owner, MyGUI::Widget* _parent)
	{
		BaseLayout::initialiseByAttributes( _owner, _parent);

		mMenuCtrl = getMainWidget()->castType<MyGUI::MenuCtrl>();
		mMenuCtrl->setPopupAccept(true);
		mMenuCtrl->eventMenuCtrlAccept += MyGUI::newDelegate(this, &BaseMenuBar::notifyPopupMenuAccept);
		
		registerControlSettingsListener();
	}
	//-------------------------------------------------------------------------
	BaseMenuBar::~BaseMenuBar()
	{
		delete mPrivateHotkeyManager;
	}
	//-------------------------------------------------------------------------
	void BaseMenuBar::setHotkeysEnabled(bool _enable)
	{
		mPrivateHotkeyManager->setEnabled(_enable);
	}
	//-------------------------------------------------------------------------
	bool BaseMenuBar::areHotkeysEnabled() const
	{
		return mPrivateHotkeyManager->isEnabled();
	}
	//-------------------------------------------------------------------------
	MyGUI::MenuItem* BaseMenuBar::findItem(const String& _id) const
	{
		std::string id;
		if(sizeof(String::value_type) == 1)
		{
			id.assign( (const char*) _id.c_str(), _id.length() );
		}
		else
		{
			CodepageConverter::ByteBuffer buf;
			mUtf8Converter->unicodeToBuffer(buf, _id);
			id.assign( (const char*) buf.data(), buf.size() );
		}

		MyGUI::MenuItem* menuItem = mMenuCtrl->findItemById(id, true);
		return menuItem;
	}
	//-------------------------------------------------------------------------
	MyGUI::MenuItem* BaseMenuBar::getItem(const String& _id) const
	{
		MyGUI::MenuItem* menuItem = findItem(_id);
		if(!menuItem)
		{
			GOTHOGRE_EXCEPT("Menu item '" << _id << "' not found.");
		}
		return menuItem;
	}
	//-------------------------------------------------------------------------
	String BaseMenuBar::getItemId(MyGUI::MenuItem* _item) const
	{
		const std::string& id = _item->getItemId();
		String str;
		if(sizeof(String::value_type) == 1)
		{
			str.assign( (const String::value_type*) id.c_str(), id.length() );
		}
		else
		{
			CodepageConverter::ByteBuffer buf( (const uint8*) id.c_str(), id.length());
			mUtf8Converter->bufferToUnicode(str, buf);
		}
		return str;
	}
	//-------------------------------------------------------------------------
	BaseMenuBar::ItemData* BaseMenuBar::getItemData(MyGUI::MenuItem* _item) const
	{
		return _item->getItemData<BaseMenuBar::ItemData>();
	}
	//-------------------------------------------------------------------------
	void BaseMenuBar::updateCaption(MyGUI::MenuItem* _item)
	{
		ItemData* itemData = getItemData(_item);
		String id = getItemId(_item);

		bool hasHotkey = false;
		String hotkeyAsStr;
		if(mPrivateHotkeyManager->hasHotkey(id))
		{
			const KeyList& keyList = mPrivateHotkeyManager->getKeyList(id);
			if(!keyList.empty())
			{
				hasHotkey = true;
				hotkeyAsStr = StrConv::toString(keyList[0]);
			}
		}
		
		String templName;
		if(hasHotkey)
		{
			templName = "ItemWithHotkey";
		}
		else
		{
			templName = "ItemDefault";
		}
		String templ = mMenuCtrl->getUserString(templName);

		MyGUI::LanguageManager& lang = MyGUI::LanguageManager::getInstance();
		lang.addUserTag("1", itemData->origCaption);
		lang.addUserTag("2", hotkeyAsStr);
		MyGUI::UString finalCaption = lang.replaceTags(templ);
		_item->setCaption(finalCaption);
	}
	//-------------------------------------------------------------------------
	void BaseMenuBar::insertItem(const String& _id, MenuItemType _itemType, 
		const String& _caption, const String& _parentId,
		const String& _positionId)
	{
		MyGUI::MenuItem* item = nullptr;
		item = findItem(_id);
		if(item)
		{
			// menu item with specified id already exists 
			return;
		}
		
		// find parent item
		MyGUI::MenuCtrl* parentCtrl = mMenuCtrl;
		if(!_parentId.empty())
		{
			MyGUI::MenuItem* parentItem = getItem(_parentId);
			parentCtrl = parentItem->getItemChild();
			if(!parentCtrl)
				parentCtrl = parentItem->createItemChild();
		}

		// find position
		MyGUI::MenuItem* to = nullptr;
		if(!_positionId.empty())
			to = findItem(_positionId);

		// insert new item
		MyGUI::MenuItem* newItem;
		if(to)
			newItem = parentCtrl->insertItem(to, "", _itemType, _id);
		else
			newItem = parentCtrl->addItem("", _itemType, _id);

		// store additional info
		newItem->setItemData(ItemData());
		ItemData* itemData = getItemData(newItem);
		itemData->origCaption = _caption;
		itemData->tool = nullptr;

		// update real (displayed) caption
		updateCaption(newItem);
	}
	//-------------------------------------------------------------------------
	void BaseMenuBar::removeItem(const String& _id)
	{
		MyGUI::MenuItem* item = findItem(_id);
		if(item)
		{
			ItemData* itemData = getItemData(item);
			if(!item->getItemChild() || !item->getItemChild()->getItemCount())
				item->getMenuCtrlParent()->removeItem(item);
		}
	}
	//-------------------------------------------------------------------------
	bool BaseMenuBar::hasItem(const String& _id) const
	{
		return findItem(_id) != nullptr;
	}
	//-------------------------------------------------------------------------
	MenuItemType BaseMenuBar::getItemType(const String& _id) const
	{
		MyGUI::MenuItem* item = getItem(_id);
		return item->getItemType();
	}
	//-------------------------------------------------------------------------
	void BaseMenuBar::setItemCaption(const String& _id, const String& _caption)
	{
		MyGUI::MenuItem* item = getItem(_id);
		ItemData* itemData = getItemData(item);
		itemData->origCaption = _caption;
		updateCaption(item);
	}
	//-------------------------------------------------------------------------
	const String& BaseMenuBar::getItemCaption(const String& _id) const
	{
		MyGUI::MenuItem* item = getItem(_id);
		ItemData* itemData = getItemData(item);
		return itemData->origCaption;
	}
	//-------------------------------------------------------------------------
	void BaseMenuBar::setItemDescription(const String& _id, const String& _description)
	{
		MyGUI::MenuItem* item = getItem(_id);
		ItemData* itemData = getItemData(item);	
		itemData->description = _description;
	}
	//-------------------------------------------------------------------------
	const String& BaseMenuBar::getItemDescription(const String& _id) const
	{
		MyGUI::MenuItem* item = getItem(_id);
		ItemData* itemData = getItemData(item);	
		return itemData->description;
	}
	//-------------------------------------------------------------------------
	void BaseMenuBar::insertHotkey(const String& _id, const String& _keyInControlSettings, const String& _sectionInControlSettings, const KeyList& _defaultValue)
	{
		MyGUI::MenuItem* item = getItem(_id);
		ItemData* itemData = getItemData(item);

		mPrivateHotkeyManager->insertHotkey(_id, _keyInControlSettings, _sectionInControlSettings, _defaultValue);
		
		if(itemData->tool)
			mPrivateHotkeyManager->attachTool(_id, itemData->tool, itemData->toolParameter);

		updateCaption(item);
	}
	//-------------------------------------------------------------------------
	void BaseMenuBar::removeHotkey(const String& _id)
	{
		mPrivateHotkeyManager->removeHotkey(_id);
		MyGUI::MenuItem* item = getItem(_id);
		updateCaption(item);
	}
	//-------------------------------------------------------------------------
	void BaseMenuBar::attachTool(const String& _id, Tool* _tool, const Any& _toolParameter)
	{
		MyGUI::MenuItem* item = getItem(_id);
		ItemData* itemData = getItemData(item);
		itemData->tool = _tool;
		itemData->toolParameter = _toolParameter;

		if(mPrivateHotkeyManager->hasHotkey(_id))
			mPrivateHotkeyManager->attachTool(_id, _tool, _toolParameter);
	}
	//-------------------------------------------------------------------------
	void BaseMenuBar::detachTool(const String& _id)
	{
		MyGUI::MenuItem* item = getItem(_id);
		ItemData* itemData = getItemData(item);
		itemData->tool = nullptr;
		itemData->toolParameter = StringUtil::BLANK;

		if(mPrivateHotkeyManager->hasHotkey(_id))
			mPrivateHotkeyManager->detachTool(_id);
	}
	//-------------------------------------------------------------------------
	void BaseMenuBar::setItemEnabled(const String& _id, bool _enable)
	{
		MyGUI::MenuItem* item = getItem(_id);
		item->setEnabled(_enable);

		if(mPrivateHotkeyManager->hasHotkey(_id))
			mPrivateHotkeyManager->setHotkeyEnabled(_id, _enable);
	}
	//-------------------------------------------------------------------------
	bool BaseMenuBar::isItemEnabled(const String& _id) const
	{
		MyGUI::MenuItem* item = getItem(_id);
		return item->getEnabled();
	}
	//-------------------------------------------------------------------------
	String BaseMenuBar::getParent(const String& _id) const
	{
		MyGUI::MenuItem* item = getItem(_id);
		MyGUI::MenuCtrl* parentCtrl = item->getMenuCtrlParent();
		if(parentCtrl == mMenuCtrl)
			return StringUtil::BLANK;
		
		MyGUI::MenuItem* parentItem = parentCtrl->getMenuItemParent();
		return getItemId(parentItem);
	}
	//-------------------------------------------------------------------------
	vector<String>::type BaseMenuBar::getChildren(const String& _id) const
	{
		vector<String>::type ret;
		
		MyGUI::MenuCtrl* ctrl = mMenuCtrl;
		if(!_id.empty())
			ctrl = getItem(_id)->getItemChild();

		if(ctrl != nullptr)
		{
			size_t numItems = ctrl->getItemCount();
			for(size_t i = 0; i != numItems; ++i)
				ret.push_back(getItemId(ctrl->getItemAt(i)));
		}
		return ret;
	}
	//-------------------------------------------------------------------------
	void BaseMenuBar::notifyPopupMenuAccept(MyGUI::MenuCtrlPtr _sender, MyGUI::MenuItemPtr _item)
	{
		ItemData* itemData = getItemData(_item);
		if(itemData->tool != nullptr)
			itemData->tool->activateTool(itemData->toolParameter);
	}
	//-------------------------------------------------------------------------
	void BaseMenuBar::controlSettingsChanged(const ConfigSettingsEvent& _evt)
	{
		// The recursive function will update all captions.
		updateAllCaptions(mMenuCtrl);
	}
	//-------------------------------------------------------------------------
	void BaseMenuBar::updateAllCaptions(MyGUI::MenuCtrl* _ctrl)
	{
		size_t numItems = _ctrl->getItemCount();
		for(size_t i = 0; i != numItems; ++i)
		{
			MyGUI::MenuItem* item = _ctrl->getItemAt(i);
			updateCaption(item);

			MyGUI::MenuCtrl* childCtrl = item->getItemChild();
			updateAllCaptions(childCtrl);
		}
	}




	//-------------------------------------------------------------------------
	// MenuBar
	//-------------------------------------------------------------------------
	MenuBar::MenuBar(const String& _name)
	{
		// add to list of all menu bars
		mName = _name;
		MenuBarList::getSingleton().addMenuBar(this);	

		// initialise and show on the screen
		initialiseByAttributes(this);
	}

	MenuBar::~MenuBar()
	{
		MenuBarList::getSingleton().removeMenuBar(this);
	}

	const String& MenuBar::getName() const
	{
		return mName;
	}

} 