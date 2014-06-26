#include "GUISystem_Precompiled.h"
#include "GUISystem_FileDialog.h"
#include "GUISystem_UISettings.h"

namespace GUISystem
{
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
				GOTHOGRE_EXCEPT(
					"Cannot locate a resource group called '" << _groupName << "'");
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
	BaseFileDialog::BaseFileDialog()
	{
		mType = LOAD;
		mCaption = StringUtil::BLANK;
		mOkText = StringUtil::BLANK;
		mCancelText = StringUtil::BLANK;
		mDefaultFilter = 0;
		mDefaultPath = StringUtil::BLANK;
		
		mNeedUpdateCaption = false;
		mNeedUpdateOkText = false;
		mNeedUpdateCancelText = false;
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
	}
	//--------------------------------------------------------------------------
	void BaseFileDialog::initialise2(BaseFileDialog* _owner, MyGUI::Widget* _parent) // Called after initialiseByAttributes
	{
		BaseLayout::initialiseByAttributes( _owner, _parent);
		setModal(true);
		readConfig();

		mListFiles->eventListChangePosition += MyGUI::newDelegate(this, &BaseFileDialog::notifyListChangePosition);
		mListFiles->eventListSelectAccept += MyGUI::newDelegate(this, &BaseFileDialog::notifyListSelectAccept);
		mListFilesInFolder->eventListChangePosition += MyGUI::newDelegate(this, &BaseFileDialog::notifyListChangePosition);
		mListFilesInFolder->eventListSelectAccept += MyGUI::newDelegate(this, &BaseFileDialog::notifyListSelectAccept);
		mComboFilters->eventComboAccept += MyGUI::newDelegate(this, &BaseFileDialog::notifyComboAccept);
		mTreeFolders->eventTreeNodeSelected += MyGUI::newDelegate(this, &BaseFileDialog::notifyTreeControlNodeSelected);
		mTreeFolders->eventTreeNodeActivated += MyGUI::newDelegate(this, &BaseFileDialog::notifyTreeControlNodeActivated);
		mEditBasename->eventEditTextChange += MyGUI::newDelegate(this, &BaseFileDialog::notifyEditBasenameChange);
		mEditBasename->eventEditSelectAccept += MyGUI::newDelegate(this, &BaseFileDialog::notifyEditBasenameAccept);
		mButtonOk->eventMouseButtonClick += MyGUI::newDelegate(this, &BaseFileDialog::notifyButtonOk);
		mButtonCancel->eventMouseButtonClick += MyGUI::newDelegate(this, &BaseFileDialog::notifyButtonCancel);
		mTab->eventTabChangeSelect += MyGUI::newDelegate(this, &BaseFileDialog::notifyTabChangeSelect);
		mSplitterH->eventScrollChangePosition += MyGUI::newDelegate(this, &BaseFileDialog::notifySplitterHChangePosition);

		setNeedUpdateCaption();
		setNeedUpdateOkText();
		setNeedUpdateCancelText();
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
	//--------------------------------------------------------------------------
	BaseFileDialog::~BaseFileDialog()
	{
		writeConfig();
		unregisterFrameListener();
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::setType(Type _type)
	{
		if(mType != _type)
		{
			mType = _type;
			setNeedUpdateOkText();
		}
	}
	//--------------------------------------------------------------------------	
	BaseFileDialog::Type BaseFileDialog::getType() const
	{
		return mType;
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::setCaption(const String& _caption)
	{
		if(mCaption != _caption)
		{
			mCaption = _caption;
			setNeedUpdateCaption();
		}
	}
	//--------------------------------------------------------------------------	
	const String& BaseFileDialog::getCaption() const
	{
		return mCaption;
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::setOkText(const String& _text)
	{
		if(mOkText != _text)
		{
			mOkText = _text;
			setNeedUpdateOkText();
		}
	}
	//--------------------------------------------------------------------------	
	const String& BaseFileDialog::getOkText() const
	{
		return mOkText;
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::setCancelText(const String& _text)
	{
		if(mCancelText != _text)
		{
			mCancelText = _text;
			setNeedUpdateCancelText();
		}
	}
	//--------------------------------------------------------------------------	
	const String& BaseFileDialog::getCancelText() const
	{
		return mCancelText;
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::setResourceGroup(const String& _resourceGroup)
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
	const String& BaseFileDialog::getResourceGroup() const
	{
		return mResourceGroup;
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::addFilter(const Filter& _filter)
	{
		mFilters.push_back(_filter);
		setNeedUpdateFilters();
		setNeedUpdateFilesInFolder();
		setNeedUpdateAlphabeticList();
	}
	//--------------------------------------------------------------------------	
	size_t BaseFileDialog::getNumFilters() const
	{
		return mFilters.size();
	}
	//--------------------------------------------------------------------------	
	const BaseFileDialog::Filter& BaseFileDialog::getFilter(size_t _index) const
	{
		return mFilters[_index];
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::setDefaultFilter(size_t _index)
	{
		mDefaultFilter = _index;
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::setDefaultPath(const String& _path)
	{
		mDefaultPath = _path;
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::setCurrentPath(const String& _path)
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
	String BaseFileDialog::getCurrentPath() const
	{
		return mCurrentPath;
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::setCurrentBasename(const String& _basename)
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
	const String& BaseFileDialog::getCurrentBasename() const
	{
		return mCurrentBasename;
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::setCurrentFilename(const String& _filename)
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
	String BaseFileDialog::getCurrentFilename() const
	{
		return getCurrentPath() + getCurrentBasename();
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::setCurrentFilter(size_t _filterIndex)
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
	size_t BaseFileDialog::getCurrentFilter() const
	{
		return mCurrentFilter;
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::setSplitterHPosition(float _position)
	{
		int n = int( _position * (mSplitterH->getScrollRange() - 1));
		mSplitterH->setScrollPosition(n);
		setNeedUpdateFoldersAndFilesCoord();
	}
	//--------------------------------------------------------------------------	
	float BaseFileDialog::getSplitterHPosition() const
	{
		return float(mSplitterH->getScrollPosition()) / (mSplitterH->getScrollRange() - 1);
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::setNeedUpdateCaption()
	{
		mNeedUpdateCaption = true;
		registerFrameListener();
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::setNeedUpdateOkText()
	{
		mNeedUpdateOkText = true;
		registerFrameListener();
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::setNeedUpdateCancelText()
	{
		mNeedUpdateCancelText = true;
		registerFrameListener();
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::setNeedUpdateFilters()
	{
		mNeedUpdateFilters = true;
		registerFrameListener();
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::setNeedUpdateCurrentFilter()
	{
		mNeedUpdateCurrentFilter = true;
		registerFrameListener();
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::setNeedUpdateAlphabeticList()
	{
		mNeedUpdateAlphabeticList = true;
		registerFrameListener();
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::setNeedUpdateFolders()
	{
		mNeedUpdateFolders = true;
		registerFrameListener();
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::setNeedUpdateCurrentFolder()
	{
		mNeedUpdateCurrentFolder = true;
		registerFrameListener();
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::setNeedUpdateCurrentPath()
	{
		mNeedUpdateCurrentPath = true;
		registerFrameListener();
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::setNeedUpdateFilesInFolder()
	{
		mNeedUpdateFilesInFolder = true;
		registerFrameListener();
	}
	//-----------------------------------------------------------------------------
	void BaseFileDialog::setNeedUpdateBasenameInFilesInFolder()
	{
		mNeedUpdateBasenameInFilesInFolder = true;
		registerFrameListener();
	}
	//-----------------------------------------------------------------------------
	void BaseFileDialog::setNeedUpdateBasenameInAlphabeticList()
	{
		mNeedUpdateBasenameInAlphabeticList = true;
		registerFrameListener();
	}
	//-----------------------------------------------------------------------------
	void BaseFileDialog::setNeedUpdateBasenameInEdit()
	{
		mNeedUpdateBasenameInEdit = true;
		registerFrameListener();
	}
	//-----------------------------------------------------------------------------
	void BaseFileDialog::setNeedUpdateFoldersAndFilesCoord()
	{
		mNeedUpdateFoldersAndFilesCoord = true;
		registerFrameListener();
	}
	//-----------------------------------------------------------------------------
	void BaseFileDialog::setNeedUpdateAlphabeticListCoord()
	{
		mNeedUpdateAlphabeticListCoord = true;
		registerFrameListener();
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::frameStarted(const FrameEvent& _evt)
	{
		if(!ResourceGroupManager::getSingleton().resourceGroupExists(mResourceGroup)
			|| !getNumFilters() || !getMainWidget())
			return;

		if(mNeedUpdateCaption)
		{
			if(updateCaption())
				mNeedUpdateCaption = false;
		}
		if(mNeedUpdateOkText)
		{
			if(updateOkText())
				mNeedUpdateOkText = false;
		}
		if(mNeedUpdateCancelText)
		{
			if(updateCancelText())
				mNeedUpdateCancelText = false;
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
		unregisterFrameListener();
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::notifyTabChangeSelect(MyGUI::TabControl* _sender, size_t _index)
	{
		registerFrameListener();
	}
	//-----------------------------------------------------------------------------
	void BaseFileDialog::notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name)
	{
		if(_name == "close")
		{
			cancel();
		}
	}
	//-----------------------------------------------------------------------------
	void BaseFileDialog::notifyWindowChangeCoord(MyGUI::Window* _sender)
	{
		setNeedUpdateFoldersAndFilesCoord();
		setNeedUpdateAlphabeticListCoord();
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::notifySplitterHChangePosition(MyGUI::ScrollBar* _sender, size_t _position)
	{
		setNeedUpdateFoldersAndFilesCoord();
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::notifyTreeControlNodeSelected(TreeControl* _sender, TreeControl::Node* _node)
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
	void BaseFileDialog::notifyTreeControlNodeActivated(TreeControl* _sender, TreeControl::Node* _node)
	{
		if(_node)
		{
			_node->setExpanded(!_node->isExpanded());
		}
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::notifyComboAccept(MyGUI::ComboBox* _sender, size_t _index) 
	{
		if(_index < getNumFilters() && _index != mCurrentFilter)
		{
			mCurrentFilter = _index;
			setNeedUpdateFilesInFolder();
			setNeedUpdateAlphabeticList();
		}
	}
	//--------------------------------------------------------------------------
	void BaseFileDialog::notifyListChangePosition(MyGUI::MultiListBox* _sender, size_t _index)
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
	void BaseFileDialog::notifyListSelectAccept(MyGUI::MultiListBox* _sender, size_t _index)
	{
		if(_index < _sender->getItemCount())
		{
			notifyListChangePosition(_sender, _index);
			ok();
		}
	}
	//--------------------------------------------------------------------------
	void BaseFileDialog::notifyEditBasenameChange(MyGUI::EditBox* _sender)
	{
		mCurrentBasename = mEditBasename->getCaption();
	}
	//--------------------------------------------------------------------------
	void BaseFileDialog::notifyEditBasenameAccept(MyGUI::EditBox* _sender)
	{
		ok();
	}
	//--------------------------------------------------------------------------
	void BaseFileDialog::notifyButtonOk(MyGUI::Widget* _sender)
	{
		ok();
	}
	//--------------------------------------------------------------------------
	void BaseFileDialog::notifyButtonCancel(MyGUI::Widget* _sender)
	{
		cancel();
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::notifyFileOverwriteMessageButton(MessageBoxButton _choice)
	{
		if(_choice == MessageBoxButton::YES)
			ok_final();
	}
	//--------------------------------------------------------------------------
	void BaseFileDialog::cancel()
	{
		eventEndDialog(StringUtil::BLANK);
	}
	//--------------------------------------------------------------------------
	void BaseFileDialog::ok()
	{
		MyGUI::LanguageManager& lang = MyGUI::LanguageManager::getInstance();
		if(mCurrentBasename.empty())
		{
			MessageBox* msgbox = new MessageBox;
			msgbox->setIcon(MessageBoxIcon::INFO);
			msgbox->setMessage("#{SelectFileBefore}");
			msgbox->addButton(MessageBoxButton::OK);
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
				MessageBox* msgbox = new MessageBox;
				msgbox->setIcon(MessageBoxIcon::ERROR);
				msgbox->setMessage("#{FileNotFound}", "1", filename);
				msgbox->addButton(MessageBoxButton::OK);
				return;
			}
		}
		else if(mType == SAVE)
		{
			if(ResourceGroupManager::getSingleton().resourceExists(mResourceGroup, filename))
			{
				MessageBox* msgbox = new MessageBox;
				msgbox->setIcon(MessageBoxIcon::QUEST);
				msgbox->setMessage("#{ConfirmOverwriteFile}", "1", filename);
				msgbox->addButton(MessageBoxButton::YES);
				msgbox->addButton(MessageBoxButton::NO);
				msgbox->eventChoice = MyGUI::newDelegate(this, &BaseFileDialog::notifyFileOverwriteMessageButton);
				return;
			}
		}

		ok_final();
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::ok_final()
	{
		eventEndDialog(getCurrentFilename());
	}
	//--------------------------------------------------------------------------	
	void BaseFileDialog::fileSizeToString(size_t _fileSize, String* _outString)
	{
		*_outString = StrConv::toString(_fileSize);
		size_t len = _outString->length();
		_outString->insert(0, 10 - len, ' ');
	}
	//--------------------------------------------------------------------------	
	bool BaseFileDialog::updateCaption()
	{
		String caption = mCaption;
		caption = MyGUI::LanguageManager::getInstance().replaceTags(caption);
		getWindow()->setCaption(caption);
		return true;
	}
	//--------------------------------------------------------------------------	
	bool BaseFileDialog::updateOkText()
	{
		if(!mButtonOk)
			return false;

		String text = mOkText;
		if(text.empty())
		{
			switch(mType)
			{
				case LOAD: text = "#{Load}"; break;
				case SAVE: text = "#{Save}"; break;
			}
		}
		text = MyGUI::LanguageManager::getInstance().replaceTags(text);
		mButtonOk->setCaption(text);
		return true;
	}
	//--------------------------------------------------------------------------	
	bool BaseFileDialog::updateCancelText()
	{
		if(!mButtonCancel)
			return false;

		String text = mCancelText;
		if(text.empty())
		{
			text = "#{Cancel}";
		}
		text = MyGUI::LanguageManager::getInstance().replaceTags(text);
		mButtonCancel->setCaption(text);
		return true;
	}
	//--------------------------------------------------------------------------	
	bool BaseFileDialog::updateFilters()
	{
		if(!mComboFilters)
			return false;

		mComboFilters->removeAllItems();
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
	bool BaseFileDialog::updateCurrentFilter()
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
	bool BaseFileDialog::updateAlphabeticList()
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
	bool BaseFileDialog::updateFolders()
	{
		if(!mTabFoldersAndFiles || mTab->getItemSelected() != mTabFoldersAndFiles)
			return false;

		// Setup tree of folders
		StringVectorPtr pFolders = ResourceGroupManager::getSingleton().listResourceNames(mResourceGroup, true);
		StringVector& folders = *pFolders;
		mNodeByFolder.clear();

		// Add root folder
		String rootFolder = "<root>";
		TreeControl::Node* rootNode = mTreeFolders->getRoot();
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

			TreeControl::Node* parentNode = mTreeFolders->getRoot();
			NodeByFolder::iterator it = mNodeByFolder.find(folderpath);
			if(it != mNodeByFolder.end())
				parentNode = it->second;

			TreeControl::Node* node = new TreeControl::Node(basefolder, parentNode);
			folder += '/';
			node->setData(folder);
			mNodeByFolder.insert(std::make_pair(folder, node));
		}

		return true;
	}
	//--------------------------------------------------------------------------	
	bool BaseFileDialog::updateCurrentFolder()
	{
		if(!mTabFoldersAndFiles || mTab->getItemSelected() != mTabFoldersAndFiles)
			return false;

		NodeByFolder::iterator it = mNodeByFolder.find(mCurrentPath);
		if(it != mNodeByFolder.end())
		{
			TreeControl::Node* node = it->second;
			mTreeFolders->setSelection(node);
		}
		else
		{
			mCurrentPath.clear();
		}
		return true;
	}
	//--------------------------------------------------------------------------	
	bool BaseFileDialog::updateCurrentPath()
	{
		if(!mTabFoldersAndFiles || mTab->getItemSelected() != mTabFoldersAndFiles)
			return false;

		if(!mCurrentPath.empty())
			mTextCurrentPath->setCaption(mCurrentPath);
		else
			mTextCurrentPath->setCaption("<root>");
		return true;
	}
	//--------------------------------------------------------------------------	
	bool BaseFileDialog::updateFilesInFolder()
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
	bool BaseFileDialog::updateBasenameInFilesInFolder()
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
	bool BaseFileDialog::updateBasenameInAlphabeticList()
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
	bool BaseFileDialog::updateBasenameInEdit()
	{
		if(!mEditBasename)
			return false;

		mEditBasename->setCaption(mCurrentBasename);
		return true;
	}
	//--------------------------------------------------------------------------	
	bool BaseFileDialog::updateFoldersAndFilesCoord()
	{
		if(!mTabFoldersAndFiles || mTab->getItemSelected() != mTabFoldersAndFiles)
			return false;

		float f = float(mSplitterH->getScrollPosition()) / (mSplitterH->getScrollRange() - 1);
		int delta = mPanelVElement1->getTop() - mPanelVElement0->getBottom();
		int w = mPanelVContainer->getWidth();
		int h = mPanelVContainer->getHeight();
		int h0 = int(h * f) - delta / 2;
		mPanelVElement0->setCoord(0, 0, w, h0);
		mPanelVElement1->setCoord(0, h0 + delta, w, h - (h0 + delta));

		int lw = mListFilesInFolder->getWidth();
		mListFilesInFolder->setColumnWidthAt(0, int(lw * 0.66f));
		mListFilesInFolder->setColumnWidthAt(1, int(lw * 0.16f)); 
		mListFilesInFolder->setColumnWidthAt(2, int(lw * 0.18f));

		return true;
	}
	//--------------------------------------------------------------------------	
	bool BaseFileDialog::updateAlphabeticListCoord()
	{
		if(!mTabAlphabeticList || mTab->getItemSelected() != mTabAlphabeticList)
			return false;

		int lw = mListFiles->getWidth();
		mListFiles->setColumnWidthAt(0, int(lw * 0.45f));
		mListFiles->setColumnWidthAt(1, int(lw * 0.21f)); 
		mListFiles->setColumnWidthAt(2, int(lw * 0.16f));
		mListFiles->setColumnWidthAt(3, int(lw * 0.18f));

		return true;
	}
	//--------------------------------------------------------------------------
	void BaseFileDialog::readConfig()
	{
		Real defaultSplitterPosition = StrConv::fromString<Real>(
			mSplitterH->getUserString("DefaultPosition"));

		if(!getConfigSection().empty())
		{
			UISettings& ui = UISettings::getSingleton();
			setCurrentPath( ui.getSetting<String>("CurrentPath", getConfigSection(), mDefaultPath));
			setCurrentFilter( ui.getSetting<size_t>("CurrentFilter", getConfigSection(), mDefaultFilter));
			setSplitterHPosition( ui.getSetting<float>("SplitterPosition", getConfigSection(), defaultSplitterPosition));
		}
		else
		{
			setSplitterHPosition( defaultSplitterPosition );
			setCurrentPath( mDefaultPath );
			setCurrentFilter( mDefaultFilter );
		}
	}
	//--------------------------------------------------------------------------
	void BaseFileDialog::writeConfig()
	{
		if(!getConfigSection().empty())
		{
			UISettings& ui = UISettings::getSingleton();
			ui.beginBatch();
			ui.setSetting<String>("CurrentPath", getConfigSection(), getCurrentPath());
			ui.setSetting<size_t>("CurrentFilter", getConfigSection(), getCurrentFilter());
			ui.setSetting<float>("SplitterHPosition", getConfigSection(), getSplitterHPosition());
			ui.endBatch();
		}
	}

} // namespace GUISystem
