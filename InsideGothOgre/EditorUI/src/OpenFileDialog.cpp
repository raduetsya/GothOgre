#include "EditorUI_Precompiled.h"
#include "EditorUI_CommonInclude.h"
#include "OpenFileDialog.h"

namespace GothOgre
{
	OpenFileDialog__::Config::Config(const String& _section)
		: CfgWindowS::Config(_section)
	{
	}
	//------------------------------------------------------------------------------
	void OpenFileDialog__::Config::setCurrentPath(const String& _currentPath)
	{
		static const String sCurrentPath = "CurrentPath";
		UISettings::getSingleton().setSetting<String>(sCurrentPath, getSection(), _currentPath);
	}
	//------------------------------------------------------------------------------
	String OpenFileDialog__::Config::getCurrentPath() const
	{
		static const String sCurrentPath = "CurrentPath";
		return UISettings::getSingleton().getSetting<String>(sCurrentPath, getSection());
	}
	//------------------------------------------------------------------------------
	void OpenFileDialog__::Config::setCurrentFilter(size_t _filterIndex)
	{
		static const String sCurrentFilter = "CurrentFilter";
		UISettings::getSingleton().setSetting<size_t>(sCurrentFilter, getSection(), _filterIndex);
	}
	//------------------------------------------------------------------------------
	size_t OpenFileDialog__::Config::getCurrentFilter() const
	{
		static const String sCurrentFilter = "CurrentFilter";
		return UISettings::getSingleton().getSetting<size_t>(sCurrentFilter, getSection());
	}
	//------------------------------------------------------------------------------
	void OpenFileDialog__::Config::setMovableSeparatorPosition(float _position)
	{
		static const String sMovableSeparatorPosition = "MovableSeparatorPosition";
		UISettings::getSingleton().setSetting<float>(sMovableSeparatorPosition, getSection(), _position);
	}
	//------------------------------------------------------------------------------
	float OpenFileDialog__::Config::getMovableSeparatorPosition() const
	{
		static const String sMovableSeparatorPosition = "MovableSeparatorPosition";
		return UISettings::getSingleton().getSetting<float>(sMovableSeparatorPosition, getSection());
	}
	//--------------------------------------------------------------------------
	class ResourceGroupManagerHack : public ResourceGroupManager
	{
	public:
		FileInfoListPtr findResourceFileInfo (const String& _groupName, const String& _pattern, bool _recursive, bool _dirs)
		{
			OGRE_LOCK_AUTO_MUTEX
			// MEMCATEGORY_GENERAL is the only category supported for SharedPtr
			FileInfoListPtr vec(OGRE_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);

			// Try to find in resource index first
			ResourceGroup* grp = getResourceGroup(_groupName);
			if (!grp)
			{
				OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, 
					"Cannot locate a resource group called '" + _groupName + "'", 
					"ResourceGroupManager::findResourceFileInfo");
			}

			OGRE_LOCK_MUTEX(grp->OGRE_AUTO_MUTEX_NAME) // lock group mutex

			// Iterate over the archives
			LocationList::iterator i, iend;
			iend = grp->locationList.end();
			for (i = grp->locationList.begin(); i != iend; ++i)
			{
				FileInfoListPtr lst = (*i)->archive->findFileInfo(_pattern, _recursive, _dirs);
				vec->insert(vec->end(), lst->begin(), lst->end());
			}
			return vec;
		}
	};
	//--------------------------------------------------------------------------
	OpenFileDialog::OpenFileDialog()
	{
		mWindow              = nullptr;
		mTab                 = nullptr;
		mTabAlphabeticList   = nullptr;
		mTabFoldersAndFiles  = nullptr;
		mMovableSeparator    = nullptr;
		mPanelVContainer     = nullptr;
		mPanelVElement0      = nullptr;
		mPanelVElement1      = nullptr;
		mListFiles           = nullptr;
		mListFilesInFolder   = nullptr;
		mTreeFolders         = nullptr;
		mStaticCurrentPath   = nullptr;
		mEditBasename        = nullptr;
		mComboFilters        = nullptr;
		mButtonCancel        = nullptr;
		mButtonDoIt          = nullptr;

		mType = LOAD;
		mCaption = StringUtil::BLANK;
		mButtonCaption = StringUtil::BLANK;
		mCurrentBasename = StringUtil::BLANK;
		mCurrentPath = StringUtil::BLANK;
		mCurrentFilter = 0;
		
		mNeedUpdateCaption = false;
		mNeedUpdateButtonCaption = false;
		mNeedUpdateFilters = false;
		mNeedUpdateCurrentFilter = false;
		mNeedUpdateAlphabeticList = false;
		mNeedUpdateFolders = false;
		mNeedUpdateCurrentFolder = false;
		mNeedUpdateCurrentPath = false;
		mNeedUpdateFilesInFolder = false;
		mNeedUpdateBasenameInAlphabeticList = false;
		mNeedUpdateBasenameInFilesInFolder = false;
		mNeedUpdateBasenameInEdit = false;
		mNeedUpdateFoldersAndFilesCoord = false;
		mNeedUpdateAlphabeticListCoord = false;
		mFrameStartNotificationEnabled = false;
	}
	//--------------------------------------------------------------------------
	OpenFileDialog::~OpenFileDialog()
	{
		disableFrameStartNotification();
	}
	//--------------------------------------------------------------------------
	void OpenFileDialog::createWidgets()
	{
		if(!mWindow)
		{
			mPrefix = MyGUI::utility::toString(this, "_");
			MyGUI::LayoutManager& lm = MyGUI::LayoutManager::getInstance();

			mWindow              = lm.loadLayout("OpenFileDialog.layout", mPrefix).at(0)->castType<MyGUI::Window>();
			mTab                 = mWindow->findWidget(mPrefix + "tab")->castType<MyGUI::Tab>();
			mTabAlphabeticList   = mWindow->findWidget(mPrefix + "tabAlphabeticList")->castType<MyGUI::TabItem>();
			mTabFoldersAndFiles	 = mWindow->findWidget(mPrefix + "tabFoldersAndFiles")->castType<MyGUI::TabItem>();
			mMovableSeparator    = mWindow->findWidget(mPrefix + "movableSeparator")->castType<MyGUI::VScroll>();
			mPanelVContainer     = mWindow->findWidget(mPrefix + "panelVContainer")->castType<MyGUI::Widget>();
			mPanelVElement0      = mWindow->findWidget(mPrefix + "panelVElement0")->castType<MyGUI::Widget>();
			mPanelVElement1      = mWindow->findWidget(mPrefix + "panelVElement1")->castType<MyGUI::Widget>();
			mTreeFolders         = mWindow->findWidget(mPrefix + "treeFolders")->castType<MyGUI::TreeControl>();
			mListFiles           = mWindow->findWidget(mPrefix + "listFiles")->castType<MyGUI::MultiList>();
			mListFilesInFolder   = mWindow->findWidget(mPrefix + "listFilesInFolder")->castType<MyGUI::MultiList>();
			mStaticCurrentPath   = mWindow->findWidget(mPrefix + "staticCurrentPath")->castType<MyGUI::StaticText>();
			mEditBasename        = mWindow->findWidget(mPrefix + "editBasename")->castType<MyGUI::Edit>();
			mComboFilters        = mWindow->findWidget(mPrefix + "comboFilters")->castType<MyGUI::ComboBox>();
			mButtonDoIt          = mWindow->findWidget(mPrefix + "buttonDoIt")->castType<MyGUI::Button>();
			mButtonCancel        = mWindow->findWidget(mPrefix + "buttonCancel")->castType<MyGUI::Button>();

			mListFiles->eventListChangePosition = MyGUI::newDelegate(this, &OpenFileDialog::notifyListChangePosition);
			mListFiles->eventListSelectAccept = MyGUI::newDelegate(this, &OpenFileDialog::notifyListSelectAccept);
			mListFilesInFolder->eventListChangePosition = MyGUI::newDelegate(this, &OpenFileDialog::notifyListChangePosition);
			mListFilesInFolder->eventListSelectAccept = MyGUI::newDelegate(this, &OpenFileDialog::notifyListSelectAccept);
			mComboFilters->eventComboAccept = MyGUI::newDelegate(this, &OpenFileDialog::notifyComboAccept);
			mTreeFolders->eventTreeNodeSelected = MyGUI::newDelegate(this, &OpenFileDialog::notifyTreeControlNodeSelected);
			mEditBasename->eventEditTextChange = MyGUI::newDelegate(this, &OpenFileDialog::notifyEditBasenameChange);
			mEditBasename->eventEditSelectAccept = MyGUI::newDelegate(this, &OpenFileDialog::notifyEditBasenameAccept);
			mButtonDoIt->eventMouseButtonClick = MyGUI::newDelegate(this, &OpenFileDialog::notifyButtonDoIt);
			mButtonCancel->eventMouseButtonClick = MyGUI::newDelegate(this, &OpenFileDialog::notifyButtonCancel);
			mTab->eventTabChangeSelect = MyGUI::newDelegate(this, &OpenFileDialog::notifyTabChangeSelect);
			mMovableSeparator->eventScrollChangePosition = MyGUI::newDelegate(this, &OpenFileDialog::notifyMovableSeparatorChangePosition);
			mWindow->eventWindowButtonPressed = MyGUI::newDelegate(this, &OpenFileDialog::notifyWindowButtonPressed);
			mWindow->eventWindowChangeCoord = MyGUI::newDelegate(this, &OpenFileDialog::notifyWindowChangeCoord);

			mWindow->setVisible(false);
			MyGUI::InputManager::getInstance().addWidgetModal(mWindow);

			setNeedUpdateCaption();
			setNeedUpdateButtonCaption();
			setNeedUpdateFilters();
			setNeedUpdateCurrentFilter();
			setNeedUpdateAlphabeticList();
			setNeedUpdateFolders();
			setNeedUpdateCurrentFolder();
			setNeedUpdateCurrentPath();
			setNeedUpdateFilesInFolder();
			setNeedUpdateBasenameInAlphabeticList();
			setNeedUpdateBasenameInFilesInFolder();
			setNeedUpdateBasenameInEdit();
			setNeedUpdateFoldersAndFilesCoord();
			setNeedUpdateAlphabeticListCoord();
		}
	}
	//--------------------------------------------------------------------------
	void OpenFileDialog::destroyWidgets()
	{
		if(mWindow)
		{
			if(MyGUI::InputManager::getInstancePtr())
				MyGUI::InputManager::getInstance().removeWidgetModal(mWindow);

			if(MyGUI::WidgetManager::getInstancePtr())
				MyGUI::WidgetManager::getInstance().destroyWidget(mWindow);

			mWindow              = nullptr;
			mTab                 = nullptr;
			mTabAlphabeticList   = nullptr;
			mTabFoldersAndFiles  = nullptr;
			mMovableSeparator    = nullptr;
			mPanelVContainer     = nullptr;
			mPanelVElement0      = nullptr;
			mPanelVElement1      = nullptr;
			mListFiles           = nullptr;
			mListFilesInFolder   = nullptr;
			mTreeFolders         = nullptr;
			mStaticCurrentPath   = nullptr;
			mEditBasename        = nullptr;
			mComboFilters        = nullptr;
			mButtonCancel        = nullptr;
			mButtonDoIt          = nullptr;
		}
	}
	//--------------------------------------------------------------------------	
	MyGUI::Widget* OpenFileDialog::getCfgWidget()
	{
		return mWindow;
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::setType(Type _type)
	{
		if(mType != _type)
		{
			mType = _type;
			setNeedUpdateButtonCaption();
		}
	}
	//--------------------------------------------------------------------------	
	OpenFileDialog::Type OpenFileDialog::getType() const
	{
		return mType;
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::setCaption(const String& _caption)
	{
		if(mCaption != _caption)
		{
			mCaption = _caption;
			setNeedUpdateCaption();
		}
	}
	//--------------------------------------------------------------------------	
	const String& OpenFileDialog::getCaption() const
	{
		return mCaption;
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::setButtonCaption(const String& _buttonCaption)
	{
		if(mButtonCaption != _buttonCaption)
		{
			mButtonCaption = _buttonCaption;
			setNeedUpdateButtonCaption();
		}
	}
	//--------------------------------------------------------------------------	
	const String& OpenFileDialog::getButtonCaption() const
	{
		return mButtonCaption;
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::setResourceGroup(const String& _resourceGroup)
	{
		if(mResourceGroup != _resourceGroup)
		{
			mResourceGroup = _resourceGroup;
			ResourceLocationScanner::getSingleton().scan(mResourceGroup);
			setNeedUpdateFolders();
			setNeedUpdateCurrentFolder();
			setNeedUpdateCurrentPath();
			setNeedUpdateFilesInFolder();
			setNeedUpdateAlphabeticList();
		}
	}
	//--------------------------------------------------------------------------	
	const String& OpenFileDialog::getResourceGroup() const
	{
		return mResourceGroup;
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::addFilter(const Filter& _filter)
	{
		mFilters.push_back(_filter);
		setNeedUpdateFilters();
		setNeedUpdateFilesInFolder();
		setNeedUpdateAlphabeticList();
	}
	//--------------------------------------------------------------------------	
	size_t OpenFileDialog::getNumFilters() const
	{
		return mFilters.size();
	}
	//--------------------------------------------------------------------------	
	const OpenFileDialog::Filter& OpenFileDialog::getFilter(size_t _index) const
	{
		return mFilters[_index];
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::setCurrentPath(const String& _path)
	{
		if(mCurrentPath != _path)
		{
			mCurrentPath = _path;
			setNeedUpdateCurrentPath();
			setNeedUpdateCurrentFolder();
			setNeedUpdateFilesInFolder();
		}
	}
	//--------------------------------------------------------------------------	
	String OpenFileDialog::getCurrentPath() const
	{
		return mCurrentPath;
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::setCurrentBasename(const String& _basename)
	{
		if(mCurrentBasename != _basename)
		{
			mCurrentBasename = _basename;
			setNeedUpdateBasenameInFilesInFolder();
			setNeedUpdateBasenameInAlphabeticList();
			setNeedUpdateBasenameInEdit();
		}
	}
	//--------------------------------------------------------------------------	
	const String& OpenFileDialog::getCurrentBasename() const
	{
		return mCurrentBasename;
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::setCurrentFilename(const String& _filename)
	{
		String path, basename;
		size_t pos = _filename.find_first_of("\\/");
		if(pos != String::npos)
		{
			path = _filename.substr(0, pos + 1);
			basename = _filename.substr(pos + 1);
		}
		else
		{
			path = StringUtil::BLANK;
			basename = _filename;
		}
		setCurrentPath(path);
		setCurrentBasename(basename);
	}
	//--------------------------------------------------------------------------	
	String OpenFileDialog::getCurrentFilename() const
	{
		return getCurrentPath() + getCurrentBasename();
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::setCurrentFilter(size_t _filterIndex)
	{
		if(mCurrentFilter != _filterIndex)
		{
			mCurrentFilter = _filterIndex;
			setNeedUpdateCurrentFilter();
			setNeedUpdateFilesInFolder();
			setNeedUpdateAlphabeticList();
		}
	}
	//--------------------------------------------------------------------------	
	size_t OpenFileDialog::getCurrentFilter() const
	{
		return mCurrentFilter;
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::setMovableSeparatorPosition(float _position)
	{
		int n = int( _position * (mMovableSeparator->getScrollRange() - 1));
		mMovableSeparator->setScrollPosition(n);
		setNeedUpdateFoldersAndFilesCoord();
	}
	//--------------------------------------------------------------------------	
	float OpenFileDialog::getMovableSeparatorPosition() const
	{
		return float(mMovableSeparator->getScrollPosition()) / (mMovableSeparator->getScrollRange() - 1);
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::setNeedUpdateCaption()
	{
		mNeedUpdateCaption = true;
		enableFrameStartNotification();
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::setNeedUpdateButtonCaption()
	{
		mNeedUpdateButtonCaption = true;
		enableFrameStartNotification();
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::setNeedUpdateFilters()
	{
		mNeedUpdateFilters = true;
		enableFrameStartNotification();
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::setNeedUpdateCurrentFilter()
	{
		mNeedUpdateCurrentFilter = true;
		enableFrameStartNotification();
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::setNeedUpdateAlphabeticList()
	{
		mNeedUpdateAlphabeticList = true;
		enableFrameStartNotification();
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::setNeedUpdateFolders()
	{
		mNeedUpdateFolders = true;
		enableFrameStartNotification();
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::setNeedUpdateCurrentFolder()
	{
		mNeedUpdateCurrentFolder = true;
		enableFrameStartNotification();
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::setNeedUpdateCurrentPath()
	{
		mNeedUpdateCurrentPath = true;
		enableFrameStartNotification();
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::setNeedUpdateFilesInFolder()
	{
		mNeedUpdateFilesInFolder = true;
		enableFrameStartNotification();
	}
	//-----------------------------------------------------------------------------
	void OpenFileDialog::setNeedUpdateBasenameInFilesInFolder()
	{
		mNeedUpdateBasenameInFilesInFolder = true;
		enableFrameStartNotification();
	}
	//-----------------------------------------------------------------------------
	void OpenFileDialog::setNeedUpdateBasenameInAlphabeticList()
	{
		mNeedUpdateBasenameInAlphabeticList = true;
		enableFrameStartNotification();
	}
	//-----------------------------------------------------------------------------
	void OpenFileDialog::setNeedUpdateBasenameInEdit()
	{
		mNeedUpdateBasenameInEdit = true;
		enableFrameStartNotification();
	}
	//-----------------------------------------------------------------------------
	void OpenFileDialog::setNeedUpdateFoldersAndFilesCoord()
	{
		mNeedUpdateFoldersAndFilesCoord = true;
		enableFrameStartNotification();
	}
	//-----------------------------------------------------------------------------
	void OpenFileDialog::setNeedUpdateAlphabeticListCoord()
	{
		mNeedUpdateAlphabeticListCoord = true;
		enableFrameStartNotification();
	}
	//-----------------------------------------------------------------------------
	void OpenFileDialog::enableFrameStartNotification()
	{
		if(!mFrameStartNotificationEnabled)
		{
			MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &OpenFileDialog::notifyFrameStart);
			mFrameStartNotificationEnabled = true;
		}
	}
	//-----------------------------------------------------------------------------
	void OpenFileDialog::disableFrameStartNotification()
	{
		if(mFrameStartNotificationEnabled)
		{
			MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &OpenFileDialog::notifyFrameStart);
			mFrameStartNotificationEnabled = false;
		}
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::notifyFrameStart(float _time)
	{
		if(!ResourceGroupManager::getSingleton().resourceGroupExists(mResourceGroup)
			|| !getNumFilters())
			return;

		if(!mWindow->isVisible())
			mWindow->setVisible(true);

		if(mNeedUpdateCaption)
		{
			if(updateCaption())
				mNeedUpdateCaption = false;
		}
		if(mNeedUpdateButtonCaption)
		{
			if(updateButtonCaption())
				mNeedUpdateButtonCaption = false;
		}
		if(mNeedUpdateFilters)
		{
			if(updateFilters())
				mNeedUpdateFilters = false;
		}
		if(mNeedUpdateCurrentFilter)
		{
			if(updateCurrentFilter())
				mNeedUpdateCurrentFilter = false;
		}
		if(mNeedUpdateAlphabeticList)
		{
			if(updateAlphabeticList())
				mNeedUpdateAlphabeticList = false;
		}
		if(mNeedUpdateFolders)
		{
			if(updateFolders())
				mNeedUpdateFolders = false;
		}
		if(mNeedUpdateCurrentFolder)
		{
			if(updateCurrentFolder())
				mNeedUpdateCurrentFolder = false;
		}
		if(mNeedUpdateCurrentPath)
		{
			if(updateCurrentPath())
				mNeedUpdateCurrentPath = false;
		}
		if(mNeedUpdateFilesInFolder)
		{
			if(updateFilesInFolder())
				mNeedUpdateFilesInFolder = false;
		}
		if(mNeedUpdateBasenameInAlphabeticList)
		{
			if(updateBasenameInAlphabeticList())
				mNeedUpdateBasenameInAlphabeticList = false;
		}
		if(mNeedUpdateBasenameInFilesInFolder)
		{
			if(updateBasenameInFilesInFolder())
				mNeedUpdateBasenameInFilesInFolder = false;
		}
		if(mNeedUpdateBasenameInEdit)
		{
			if(updateBasenameInEdit())
				mNeedUpdateBasenameInEdit = false;
		}
		if(mNeedUpdateFoldersAndFilesCoord)
		{
			if(updateFoldersAndFilesCoord())
				mNeedUpdateFoldersAndFilesCoord = false;
		}
		if(mNeedUpdateAlphabeticListCoord)
		{
			if(updateAlphabeticListCoord())
				mNeedUpdateAlphabeticListCoord = false;
		}
		disableFrameStartNotification();
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::notifyTabChangeSelect(MyGUI::Tab* _sender, size_t _index)
	{
		enableFrameStartNotification();
	}
	//-----------------------------------------------------------------------------
	void OpenFileDialog::notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name)
	{
		if(_name == "close")
		{
			cancel();
		}
	}
	//-----------------------------------------------------------------------------
	void OpenFileDialog::notifyWindowChangeCoord(MyGUI::Window* _sender)
	{
		setNeedUpdateFoldersAndFilesCoord();
		setNeedUpdateAlphabeticListCoord();
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::notifyMovableSeparatorChangePosition(MyGUI::VScroll* _sender, size_t _position)
	{
		setNeedUpdateFoldersAndFilesCoord();
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::notifyTreeControlNodeSelected(MyGUI::TreeControl* _sender, MyGUI::TreeControl::Node* _node)
	{
		if(_node)
		{
			String newPath = *(_node->getData<String>());
			setCurrentPath(newPath);
			if(mCurrentPath != newPath)
			{
				mCurrentPath = newPath;
				setNeedUpdateCurrentPath();
				setNeedUpdateFilesInFolder();
			}
		}
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::notifyComboAccept(MyGUI::ComboBox* _sender, size_t _index) 
	{
		if(_index < getNumFilters() && _index != mCurrentFilter)
		{
			mCurrentFilter = _index;
			setNeedUpdateFilesInFolder();
			setNeedUpdateAlphabeticList();
		}
	}
	//--------------------------------------------------------------------------
	void OpenFileDialog::notifyListChangePosition(MyGUI::MultiList* _sender, size_t _index)
	{
		if(_index < _sender->getItemCount())
		{
			if(_sender == mListFiles)
			{
				mCurrentBasename = mListFiles->getItemNameAt(_index);
				mCurrentPath = mListFiles->getSubItemNameAt(1, _index);
				setNeedUpdateCurrentFolder();
				setNeedUpdateFilesInFolder();
				setNeedUpdateBasenameInFilesInFolder();
				setNeedUpdateBasenameInEdit();
			}
			else if(_sender == mListFilesInFolder)
			{
				mCurrentBasename = mListFilesInFolder->getItemNameAt(_index);
				setNeedUpdateBasenameInAlphabeticList();
				setNeedUpdateBasenameInEdit();
			}
		}
	}
	//--------------------------------------------------------------------------
	void OpenFileDialog::notifyListSelectAccept(MyGUI::MultiList* _sender, size_t _index)
	{
		if(_index < _sender->getItemCount())
		{
			notifyListChangePosition(_sender, _index);
			doIt();
		}
	}
	//--------------------------------------------------------------------------
	void OpenFileDialog::notifyEditBasenameChange(MyGUI::Edit* _sender)
	{
		mCurrentBasename = mEditBasename->getCaption();
	}
	//--------------------------------------------------------------------------
	void OpenFileDialog::notifyEditBasenameAccept(MyGUI::Edit* _sender)
	{
		doIt();
	}
	//--------------------------------------------------------------------------
	void OpenFileDialog::notifyButtonDoIt(MyGUI::Widget* _sender)
	{
		doIt();
	}
	//--------------------------------------------------------------------------
	void OpenFileDialog::notifyButtonCancel(MyGUI::Widget* _sender)
	{
		cancel();
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::notifyFileOverwriteMessageButton(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
	{
		if(_result == MyGUI::MessageBoxStyle::Yes)
			doIt_final();
	}
	//--------------------------------------------------------------------------
	void OpenFileDialog::cancel()
	{
		mWindow->setVisible(false);
		eventEndDialog(StringUtil::BLANK);
	}
	//--------------------------------------------------------------------------
	void OpenFileDialog::doIt()
	{
		MyGUI::LanguageManager& lang = MyGUI::LanguageManager::getInstance();
		if(mCurrentBasename.empty())
		{
			MyGUI::MessagePtr message = MyGUI::Message::createMessageBox(
				"Message", lang.replaceTags("#{Error}"), makeMessage("#{SelectFileBefore}"), 
				MyGUI::MessageBoxStyle::IconWarning | MyGUI::MessageBoxStyle::Ok);
			return;
		}

		size_t pos = mCurrentBasename.rfind('.');
		if(pos == String::npos)
		{
			if(mCurrentFilter < getNumFilters())
			{
				const Filter& filter = getFilter(mCurrentFilter);
				mCurrentBasename += filter.getDefaultExt();
			}
		}

		String filename = getCurrentFilename();
		if(mType == LOAD)
		{
			if(!ResourceGroupManager::getSingleton().resourceExists(mResourceGroup, filename))
			{
				MyGUI::MessagePtr message = MyGUI::Message::createMessageBox(
					"Message", lang.replaceTags("#{Error}"), makeMessage("#{FileNotFound}", filename), 
					MyGUI::MessageBoxStyle::IconWarning | MyGUI::MessageBoxStyle::Ok);
				return;
			}
		}
		else if(mType == SAVE)
		{
			if(ResourceGroupManager::getSingleton().resourceExists(mResourceGroup, filename))
			{
				MyGUI::MessagePtr message = MyGUI::Message::createMessageBox(
					"Message", lang.replaceTags("#{Warning}"), makeMessage("#{FileOverwrite}"), 
					MyGUI::MessageBoxStyle::IconQuest | MyGUI::MessageBoxStyle::Yes | MyGUI::MessageBoxStyle::No);
				message->eventMessageBoxResult = MyGUI::newDelegate(this, &OpenFileDialog::notifyFileOverwriteMessageButton);
				return;
			}
		}

		doIt_final();
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::doIt_final()
	{
		mWindow->setVisible(false);
		eventEndDialog(getCurrentFilename());
	}
	//--------------------------------------------------------------------------
	String OpenFileDialog::makeMessage(const String& _template, const String& _arg1)
	{
		MyGUI::LanguageManager& lang = MyGUI::LanguageManager::getInstance();
		lang.addUserTag("1", _arg1);
		String msg = lang.replaceTags(lang.replaceTags(_template)); // double tag replacing
		return msg;
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::fileSizeToString(size_t _fileSize, String* _outString)
	{
		*_outString = StringConverter::toString(_fileSize);
		size_t len = _outString->length();
		_outString->insert(0, 10 - len, ' ');
	}
	//--------------------------------------------------------------------------	
	bool OpenFileDialog::updateCaption()
	{
		if(!mWindow)
			return false;

		String caption = mCaption;
		caption = MyGUI::LanguageManager::getInstance().replaceTags(caption);
		mWindow->setCaption(caption);
		return true;
	}
	//--------------------------------------------------------------------------	
	bool OpenFileDialog::updateButtonCaption()
	{
		if(!mButtonDoIt)
			return false;

		String buttonCaption = mButtonCaption;
		if(buttonCaption.empty())
		{
			switch(mType)
			{
				case LOAD: buttonCaption = "#{LOAD}"; break;
				case SAVE: buttonCaption = "#{SAVE}"; break;
			}
		}
		buttonCaption = MyGUI::LanguageManager::getInstance().replaceTags(buttonCaption);
		mButtonDoIt->setCaption(buttonCaption);
		return true;
	}
	//--------------------------------------------------------------------------	
	bool OpenFileDialog::updateFilters()
	{
		if(!mComboFilters)
			return false;

		mComboFilters->deleteAllItems();
		for(size_t i = 0; i != getNumFilters(); ++i)
		{
			const Filter& filter = getFilter(i);
			String desc = filter.getDescription();
			desc = MyGUI::LanguageManager::getInstance().replaceTags(desc);
			mComboFilters->addItem(desc);
		}
		
		return true;
	}
	//--------------------------------------------------------------------------	
	bool OpenFileDialog::updateCurrentFilter()
	{
		if(!mComboFilters)
			return false;

		if(mComboFilters->getItemCount() != 0)
		{
			if(mCurrentFilter >= mComboFilters->getItemCount())
				mCurrentFilter = 0;
			mComboFilters->setIndexSelected(mCurrentFilter);
		}
		else
		{
			mComboFilters->clearIndexSelected();
			mCurrentFilter = -1;
		}

		return true;
	}
	//--------------------------------------------------------------------------	
	bool OpenFileDialog::updateAlphabeticList()
	{
		if(!mTabAlphabeticList || mTab->getItemSelected() != mTabAlphabeticList)
			return false;

		// Setup columns
		if(mListFiles->getColumnCount() == 0)
		{
			int width = mListFiles->getWidth();
			MyGUI::LanguageManager& lang = MyGUI::LanguageManager::getInstance();
			mListFiles->addColumn(lang.replaceTags("#{FileName}"), 0);
			mListFiles->addColumn(lang.replaceTags("#{FilePath}"), 0);
			mListFiles->addColumn(lang.replaceTags("#{FileSize}"), 0);
			mListFiles->addColumn(lang.replaceTags("#{ArchiveType}"), 0);
			mListFiles->sortByColumn(0);
		}

		// Fill the list
		mListFiles->removeAllItems();

		// Add information about every file matching one of patterns 
		// corresponding the current filter.
		size_t filterIndex = mComboFilters->getIndexSelected();
		if(filterIndex != -1)
		{
			String fileSizeAsStr;
			const Filter& filter = getFilter(filterIndex);
			for(size_t i = 0; i != filter.getNumPatterns(); ++i)
			{
				const String& pattern = filter.getPattern(i);
				FileInfoListPtr pFileInfoList = ResourceGroupManager::getSingleton().findResourceFileInfo(mResourceGroup, pattern, false);
				const FileInfoList& fileInfoList = *pFileInfoList;
				for(size_t j = 0; j != fileInfoList.size(); ++j)
				{
					const FileInfo& fileInfo = fileInfoList[j];
					mListFiles->addItem(fileInfo.basename);
					fileSizeToString(fileInfo.uncompressedSize, &fileSizeAsStr);
					mListFiles->setSubItemNameAt(1, j, fileInfo.path);
					mListFiles->setSubItemNameAt(2, j, fileSizeAsStr);
					mListFiles->setSubItemNameAt(3, j, fileInfo.archive->getType());
				}
			}
		}
		
		return true;
	}
	//--------------------------------------------------------------------------	
	bool OpenFileDialog::updateFolders()
	{
		if(!mTabFoldersAndFiles || mTab->getItemSelected() != mTabFoldersAndFiles)
			return false;

		// Setup tree of folders
		StringVectorPtr pFolders = ResourceGroupManager::getSingleton().listResourceNames(mResourceGroup, true);
		StringVector& folders = *pFolders;
		mNodeByFolder.clear();

		// Add root folder
		String rootFolder = "<root>";
		MyGUI::TreeControl::Node* rootNode = mTreeFolders->getRoot();
		rootNode->setText(rootFolder);
		rootNode->setData(StringUtil::BLANK);
		mNodeByFolder.insert(std::make_pair(StringUtil::BLANK, rootNode));
		mTreeFolders->setRootVisible(true);

		String folder, basefolder, folderpath;
		for(size_t i = 0; i != folders.size(); ++i)
		{
			folder = folders[i];
			folder.erase(folder.find_last_not_of("\\/") + 1);
			size_t pos = folder.find_last_of("\\/");

			if(pos != String::npos)
			{
				basefolder = folder.substr(pos + 1);
				folderpath = folder.substr(0, pos + 1);
			}
			else
			{
				basefolder = folder;
				folderpath.clear();
			}

			MyGUI::TreeControl::Node* parentNode = mTreeFolders->getRoot();
			NodeByFolder::iterator it = mNodeByFolder.find(folderpath);
			if(it != mNodeByFolder.end())
				parentNode = it->second;

			MyGUI::TreeControl::Node* node = new MyGUI::TreeControl::Node(basefolder, parentNode);
			folder += '/';
			node->setData(folder);
			mNodeByFolder.insert(std::make_pair(folder, node));
		}

		return true;
	}
	//--------------------------------------------------------------------------	
	bool OpenFileDialog::updateCurrentFolder()
	{
		if(!mTabFoldersAndFiles || mTab->getItemSelected() != mTabFoldersAndFiles)
			return false;

		NodeByFolder::iterator it = mNodeByFolder.find(mCurrentPath);
		if(it != mNodeByFolder.end())
		{
			MyGUI::TreeControl::Node* node = it->second;
			mTreeFolders->setSelection(node);
		}
		else
		{
			mCurrentPath.clear();
		}
		return true;
	}
	//--------------------------------------------------------------------------	
	bool OpenFileDialog::updateCurrentPath()
	{
		if(!mTabFoldersAndFiles || mTab->getItemSelected() != mTabFoldersAndFiles)
			return false;

		if(!mCurrentPath.empty())
			mStaticCurrentPath->setCaption(mCurrentPath);
		else
			mStaticCurrentPath->setCaption("<root>");
		return true;
	}
	//--------------------------------------------------------------------------	
	bool OpenFileDialog::updateFilesInFolder()
	{
		if(!mTabFoldersAndFiles || mTab->getItemSelected() != mTabFoldersAndFiles)
			return false;

		// Setup columns
		if(mListFilesInFolder->getColumnCount() == 0)
		{
			int width = mListFilesInFolder->getWidth();
			MyGUI::LanguageManager& lang = MyGUI::LanguageManager::getInstance();
			mListFilesInFolder->addColumn(lang.replaceTags("#{FileName}"), 0);
			mListFilesInFolder->addColumn(lang.replaceTags("#{FileSize}"), 0); 
			mListFilesInFolder->addColumn(lang.replaceTags("#{ArchiveType}"), 0);
			mListFilesInFolder->sortByColumn(0);
		}

		// Fill the list
		mListFilesInFolder->removeAllItems();

		// Add information about every file matching one of patterns 
		// corresponding the current filter.
		size_t filterIndex = mComboFilters->getIndexSelected();
		if(filterIndex != -1)
		{
			String fileSizeAsStr;
			const Filter& filter = getFilter(filterIndex);
			for(size_t i = 0; i != filter.getNumPatterns(); ++i)
			{
				const String& pattern = filter.getPattern(i);
				ResourceGroupManagerHack& rgmh = reinterpret_cast<ResourceGroupManagerHack&>(ResourceGroupManager::getSingleton());
				FileInfoListPtr pFileInfoList = rgmh.findResourceFileInfo(mResourceGroup, mCurrentPath + pattern, false, false);
				const FileInfoList& fileInfoList = *pFileInfoList;
				for(size_t j = 0; j != fileInfoList.size(); ++j)
				{
					const FileInfo& fileInfo = fileInfoList[j];
					mListFilesInFolder->addItem(fileInfo.basename);
					fileSizeToString(fileInfo.uncompressedSize, &fileSizeAsStr);
					mListFilesInFolder->setSubItemNameAt(1, j, fileSizeAsStr);
					mListFilesInFolder->setSubItemNameAt(2, j, fileInfo.archive->getType());
				}
			}
		}
		
		return true;
	}
	//--------------------------------------------------------------------------	
	bool OpenFileDialog::updateBasenameInFilesInFolder()
	{
		if(!mTabFoldersAndFiles || mTab->getItemSelected() != mTabFoldersAndFiles)
			return false;

		for(size_t i = 0; i != mListFilesInFolder->getItemCount(); ++i)
		{
			if(mListFilesInFolder->getItemNameAt(i) == mCurrentBasename)
			{
				mListFilesInFolder->setIndexSelected(i);
				break;
			}
		}

		return true;
	}
	//--------------------------------------------------------------------------	
	bool OpenFileDialog::updateBasenameInAlphabeticList()
	{
		if(!mTabAlphabeticList || mTab->getItemSelected() != mTabAlphabeticList)
			return false;

		for(size_t i = 0; i != mListFiles->getItemCount(); ++i)
		{
			if(mListFiles->getItemNameAt(i) == mCurrentBasename)
			{
				mListFiles->setIndexSelected(i);
				break;
			}
		}

		return true;
	}
	//--------------------------------------------------------------------------	
	bool OpenFileDialog::updateBasenameInEdit()
	{
		if(!mEditBasename)
			return false;

		mEditBasename->setCaption(mCurrentBasename);
		return true;
	}
	//--------------------------------------------------------------------------	
	bool OpenFileDialog::updateFoldersAndFilesCoord()
	{
		if(!mTabFoldersAndFiles || mTab->getItemSelected() != mTabFoldersAndFiles)
			return false;

		float f = float(mMovableSeparator->getScrollPosition()) / (mMovableSeparator->getScrollRange() - 1);
		int delta = mPanelVElement1->getTop() - mPanelVElement0->getBottom();
		int w = mPanelVContainer->getWidth();
		int h = mPanelVContainer->getHeight();
		int h0 = int(h * f) - delta / 2;
		mPanelVElement0->setCoord(0, 0, w, h0);
		mPanelVElement1->setCoord(0, h0 + delta, w, h - (h0 + delta));

		int lw = mListFilesInFolder->getWidth();
		mListFilesInFolder->setColumnWidth(0, int(lw * 0.66f));
		mListFilesInFolder->setColumnWidth(1, int(lw * 0.16f)); 
		mListFilesInFolder->setColumnWidth(2, int(lw * 0.18f));

		return true;
	}
	//--------------------------------------------------------------------------	
	bool OpenFileDialog::updateAlphabeticListCoord()
	{
		if(!mTabAlphabeticList || mTab->getItemSelected() != mTabAlphabeticList)
			return false;

		int lw = mListFiles->getWidth();
		mListFiles->setColumnWidth(0, int(lw * 0.45f));
		mListFiles->setColumnWidth(1, int(lw * 0.21f)); 
		mListFiles->setColumnWidth(2, int(lw * 0.16f));
		mListFiles->setColumnWidth(3, int(lw * 0.18f));

		return true;
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::updateFromConfig()
	{
		OpenFileDialog_::updateFromConfig();
		setCurrentPath(_getConfig().getCurrentPath());
		setCurrentFilter(_getConfig().getCurrentFilter());
		setMovableSeparatorPosition(_getConfig().getMovableSeparatorPosition());
	}
	//--------------------------------------------------------------------------	
	void OpenFileDialog::preDestroyWidgets()
	{
		OpenFileDialog_::preDestroyWidgets();
		_getConfig().setMovableSeparatorPosition(getMovableSeparatorPosition());
		_getConfig().setCurrentPath(getCurrentPath());
		_getConfig().setCurrentFilter(getCurrentFilter());
	}

} // namespace GothOgre