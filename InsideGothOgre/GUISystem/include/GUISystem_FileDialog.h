#ifndef GUISYSTEM_FILE_DIALOG_H
#define GUISYSTEM_FILE_DIALOG_H

#include "GUISystem_BaseLayout.h"
#include "GUISystem_TreeControl.h"
#include "GUISystem_MessageBox.h"


namespace GUISystem
{
	//------------------------------------------------------------------------------
	// BaseFileDialog
	//------------------------------------------------------------------------------
	// Class attributes
	ATTRIBUTE_CLASS_LAYOUT( BaseFileDialog, "#{FileDialog_Layout}" );

	/** Base class for the "FileDialog" class and others. */
	class GUISYSTEM_EXPORT BaseFileDialog : public BaseLayout
	{
	public:
		/** Types of the dialog. */
		enum Type
		{
			/** This is a dialog to enter file name (and folder) to be saved. */
			SAVE,

			/** This is a dialog to pick file to be loaded. */
			LOAD
		};

		class Filter
		{
		public:
			void setDescription(const String& _description) {mDescription = _description;}
			const String& getDescription() const {return mDescription;}
			
			void addPattern(const String& _pattern) {mPatterns.push_back(_pattern);}
			size_t getNumPatterns() const {return mPatterns.size();}
			const String& getPattern(size_t _index) const {return mPatterns[_index];}

			void setDefaultExt(const String& _defaultExt) {mDefaultExt = _defaultExt;}
			const String& getDefaultExt() const {return mDefaultExt;}

		private:
			String               mDescription;
			vector<String>::type mPatterns;
			String               mDefaultExt;
		};

	public:
		BaseFileDialog();
		~BaseFileDialog();

		// Sets the type of the dialog.
		void setType(Type _type);

		// Returns the type of the dialog.
		Type getType() const;

		/** Sets the dialog's caption. */
		void setCaption(const String& _caption);

		/** Returns the dialog's caption. */
		const String& getCaption() const;

		/** Sets the "OK" button's caption. 
		You can leave the default caption based on the dialog's type. */
		void setOkText(const String& _text);

		/** Returns the "OK" button's caption. */
		const String& getOkText() const;

		/** Sets the "Cancel" button's caption. 
		You can leave the default caption based on the dialog's type. */
		void setCancelText(const String& _text);

		/** Returns the "Cancel" button's caption. */
		const String& getCancelText() const;

		/** Sets resource group. */
		void setResourceGroup(const String& _resourceGroup);

		/** Returns a resource group specified 
		by the previous call BaseFileDialog::setResourceGroup. */
		const String& getResourceGroup() const;

		/** Sets the file filters.*/
		void addFilter(const Filter& _filter);
		size_t getNumFilters() const;
		const Filter& getFilter(size_t _index) const;

		void setDefaultFilter(size_t _index);
		void setCurrentFilter(size_t _index);
		size_t getCurrentFilter() const;

		void setDefaultPath(const String& _path);
		void setCurrentPath(const String& _path);
		String getCurrentPath() const;

		void setCurrentBasename(const String& _basename);
		const String& getCurrentBasename() const;

		void setCurrentFilename(const String& _filename);
		String getCurrentFilename() const;

		/** Event which is raised when an user clicks the main button.
		Signature: void method(const String& _filename). */
		typedef MyGUI::delegates::CDelegate1<const String&> EventHandle_Result;
		EventHandle_Result eventEndDialog;

	protected:
		// Call the function in constructor of a derived class.
		template <typename T>
		void initialiseByAttributes(T* _owner, MyGUI::Widget* _parent = nullptr);

	private:
		void initialise2(BaseFileDialog* _owner, MyGUI::Widget* _parent);
		void readConfig();
		void writeConfig();

		void setSplitterHPosition(float _position);
		float getSplitterHPosition() const;

		void setNeedUpdateCaption();
		void setNeedUpdateOkText();
		void setNeedUpdateCancelText();
		void setNeedUpdateFilters();
		void setNeedUpdateCurrentFilter();
		void setNeedUpdateAlphabeticList();
		void setNeedUpdateFolders();
		void setNeedUpdateCurrentFolder();
		void setNeedUpdateCurrentPath();
		void setNeedUpdateFilesInFolder();
		void setNeedUpdateBasenameInAlphabeticList();
		void setNeedUpdateBasenameInFilesInFolder();
		void setNeedUpdateBasenameInEdit();
		void setNeedUpdateFoldersAndFilesCoord();
		void setNeedUpdateAlphabeticListCoord();

		GOTHOGRE_DECLARE_FRAME_STARTED( BaseFileDialog );

		void notifyTabChangeSelect(MyGUI::TabControl* _sender, size_t _index);
		void notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name);
		void notifyWindowChangeCoord(MyGUI::Window* _sender);
		void notifySplitterHChangePosition(MyGUI::ScrollBar* _sender, size_t _position);
		void notifyListChangePosition(MyGUI::MultiListBox* _sender, size_t _index);
		void notifyListSelectAccept(MyGUI::MultiListBox* _sender, size_t _index);
		void notifyComboAccept(MyGUI::ComboBox* _sender, size_t _index);
		void notifyTreeControlNodeSelected(TreeControl* _sender, TreeControl::Node* _node);
		void notifyTreeControlNodeActivated(TreeControl* _sender, TreeControl::Node* _node);
		void notifyFileOverwriteMessageButton(MessageBoxButton _choice);
		void notifyEditBasenameChange(MyGUI::EditBox* _sender);
		void notifyEditBasenameAccept(MyGUI::EditBox* _sender);
		void notifyButtonCancel(MyGUI::Widget* _sender);
		void notifyButtonOk(MyGUI::Widget* _sender);

		void ok();
		void ok_final();
		void cancel();
		
		static String makeMessage(const String& _template, const String& _arg1 = StringUtil::BLANK);
		static void fileSizeToString(size_t _fileSize, String* _outString);

		bool updateCaption();
		bool updateOkText();
		bool updateCancelText();
		bool updateFilters();
		bool updateCurrentFilter();
		bool updateAlphabeticList();
		bool updateFolders();
		bool updateCurrentFolder();
		bool updateCurrentPath();
		bool updateFilesInFolder();
		bool updateBasenameInAlphabeticList();
		bool updateBasenameInFilesInFolder();
		bool updateBasenameInEdit();
		bool updateFoldersAndFilesCoord();
		bool updateAlphabeticListCoord();

	private:
		typedef vector<Filter>::type Filters;

		Type  				mType;
		String				mResourceGroup;
		String				mCaption;
		String              mOkText;
		String              mCancelText;
		Filters             mFilters;
		String              mCurrentBasename;
		size_t              mCurrentFilter;
		size_t              mDefaultFilter;
		String              mCurrentPath;
		String              mDefaultPath;

		ATTRIBUTE_FIELD_WIDGET_NAME(BaseFileDialog, mTab, "tab");
		MyGUI::TabControl*         mTab;

		ATTRIBUTE_FIELD_WIDGET_NAME(BaseFileDialog, mTabAlphabeticList, "tabAlphabeticList");
		MyGUI::TabItem*     mTabAlphabeticList;

		ATTRIBUTE_FIELD_WIDGET_NAME(BaseFileDialog, mTabFoldersAndFiles, "tabFoldersAndFiles");
		MyGUI::TabItem*     mTabFoldersAndFiles;

		ATTRIBUTE_FIELD_WIDGET_NAME(BaseFileDialog, mSplitterH, "splitterH");
		MyGUI::ScrollBar*   mSplitterH;

		ATTRIBUTE_FIELD_WIDGET_NAME(BaseFileDialog, mPanelVContainer, "panelVContainer");
		MyGUI::Widget*      mPanelVContainer;

		ATTRIBUTE_FIELD_WIDGET_NAME(BaseFileDialog, mPanelVElement0, "panelVElement0");
		MyGUI::Widget*      mPanelVElement0;

		ATTRIBUTE_FIELD_WIDGET_NAME(BaseFileDialog, mPanelVElement1, "panelVElement1");
		MyGUI::Widget*      mPanelVElement1;

		ATTRIBUTE_FIELD_WIDGET_NAME(BaseFileDialog, mTreeFolders, "treeFolders");
		TreeControl*        mTreeFolders;

		ATTRIBUTE_FIELD_WIDGET_NAME(BaseFileDialog, mListFiles, "listFiles");
		MyGUI::MultiListBox*   mListFiles;

		ATTRIBUTE_FIELD_WIDGET_NAME(BaseFileDialog, mListFilesInFolder, "listFilesInFolder");
		MyGUI::MultiListBox*   mListFilesInFolder;

		ATTRIBUTE_FIELD_WIDGET_NAME(BaseFileDialog, mTextCurrentPath, "textCurrentPath");
		MyGUI::TextBox*        mTextCurrentPath;

		ATTRIBUTE_FIELD_WIDGET_NAME(BaseFileDialog, mEditBasename, "editBasename");
		MyGUI::EditBox*        mEditBasename;

		ATTRIBUTE_FIELD_WIDGET_NAME(BaseFileDialog, mComboFilters, "comboFilters");
		MyGUI::ComboBox*    mComboFilters;

		ATTRIBUTE_FIELD_WIDGET_NAME(BaseFileDialog, mButtonOk, "buttonOk");
		MyGUI::Button*      mButtonOk;

		ATTRIBUTE_FIELD_WIDGET_NAME(BaseFileDialog, mButtonCancel, "buttonCancel");
		MyGUI::Button*      mButtonCancel;

		bool mNeedUpdateCaption;
		bool mNeedUpdateOkText;
		bool mNeedUpdateCancelText;
		bool mNeedUpdateFilters;
		bool mNeedUpdateCurrentFilter;
		bool mNeedUpdateAlphabeticList;
		bool mNeedUpdateFolders;
		bool mNeedUpdateCurrentFolder;
		bool mNeedUpdateCurrentPath;
		bool mNeedUpdateFilesInFolder;
		bool mNeedUpdateBasenameInAlphabeticList;
		bool mNeedUpdateBasenameInFilesInFolder;
		bool mNeedUpdateBasenameInEdit;
		bool mNeedUpdateFoldersAndFilesCoord;
		bool mNeedUpdateAlphabeticListCoord;

		typedef map<String, TreeControl::Node*>::type NodeByFolder;
		NodeByFolder        mNodeByFolder;
	};
	//--------------------------------------------------------------------------------------
	template <typename T>
	void BaseFileDialog::initialiseByAttributes(T* _owner, MyGUI::Widget* _parent)
	{
		applyClassAttributes(_owner);
		initialise2(_owner, _parent);
	}




	//--------------------------------------------------------------------------------------
	// FileDialog
	//--------------------------------------------------------------------------------------
	/** Class representing a dialog to select a file for loading or saving. */
	class GUISYSTEM_EXPORT FileDialog : public BaseFileDialog
	{
	public:
		FileDialog()
		{
			initialiseByAttributes(this);
		}
	};


} // namespace GUISystem

#endif // GUISYSTEM_FILE_DIALOG_H
