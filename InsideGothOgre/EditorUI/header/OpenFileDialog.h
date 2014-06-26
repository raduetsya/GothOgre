#ifndef OPEN_FILE_DIALOG_H
#define OPEN_FILE_DIALOG_H

#include "CfgWindowS.h"
#include "TreeControl.h"

namespace GothOgre
{
	/** Helper class. Used internally by class CfgWindowSX. */
	class OpenFileDialog__ : public CfgWindowS
	{
	public:
		/** Class to use configuration file. */
		class Config : public CfgWindowS::Config
		{
		public:
			Config(const String& _section);

			void setCurrentPath(const String& _currentPath);
			String getCurrentPath() const;

			void setCurrentFilter(size_t _filterIndex);
			size_t getCurrentFilter() const;

			void setMovableSeparatorPosition(float _position);
			float getMovableSeparatorPosition() const;
		};

	protected:
		virtual Config& __getConfig() = 0;

	private:
		virtual CfgWindowS::Config& _getConfig() {return __getConfig();}
	};

	//------------------------------------------------------------------------------

	/** Helper class. Used internally by class CfgWindowSX. */
	class OpenFileDialog_ : public OpenFileDialog__
	{
	protected:
		virtual Config& _getConfig() = 0;

	private:
		virtual Config& __getConfig() {return _getConfig();}
	};

	//------------------------------------------------------------------------------

	/** Class representing a dialog to select file to be opened/saved. */
	class OpenFileDialog : public OpenFileDialog_
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

		typedef vector<Filter>::type Filters;

	public:
		OpenFileDialog();
		virtual ~OpenFileDialog();

		// Sets the type of the dialog.
		void setType(Type _type);

		// Returns the type of the dialog.
		Type getType() const;

		/** Sets the dialog's caption. */
		void setCaption(const String& _caption);

		/** Returns the dialog's caption. */
		const String& getCaption() const;

		/** Sets the main button's caption. 
		You can leave the default caption based on the dialog's type. */
		void setButtonCaption(const String& _buttonCaption);

		/** Returns the main button's caption. */
		const String& getButtonCaption() const;

		/** Sets resource group. */
		void setResourceGroup(const String& _resourceGroup);

		/** Returns a resource group specified 
		by the previous call OpenFileDialog::setResourceGroup. */
		const String& getResourceGroup() const;

		/** Sets a filter.
		@remark
		A filter is a list of pairs of strings. 
		Sets the first string in each pair is a display string 
		that describes the filter (for example, "Text Files"), 
		and the second string specifies the filter pattern (for example, "*.TXT"). 
		A pattern string can be a combination of valid file name characters 
		and the asterisk (*) wildcard character. 
		You can specify multiple filter patterns for a single display string. 
		Examples: 
		"All Files (*.*)" | "*.*"
		"3D Studio Meshes (*.3ds)" | "*.3ds" 
		"All Supported Files | "*.txt" | "*.doc" | "*.html"
		*/
		void addFilter(const Filter& _filter);
		size_t getNumFilters() const;
		const Filter& getFilter(size_t _index) const;
		void setCurrentFilter(size_t _index);
		size_t getCurrentFilter() const;

		void setCurrentPath(const String& _path);
		String getCurrentPath() const;

		void setCurrentBasename(const String& _basename);
		const String& getCurrentBasename() const;

		void setCurrentFilename(const String& _filename);
		String getCurrentFilename() const;

		void setMovableSeparatorPosition(float _position);
		float getMovableSeparatorPosition() const;

		/** Event which is raised when an user clicks the main button.
		Signature: void method(const String& _filename). */
		typedef MyGUI::delegates::CDelegate1<const String&> EventHandle_Result;
		EventHandle_Result eventEndDialog;

	private:
		virtual void createWidgets();
		virtual void destroyWidgets();
		virtual MyGUI::Widget* getCfgWidget();

		void setNeedUpdateCaption();
		void setNeedUpdateButtonCaption();
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

		void enableFrameStartNotification();
		void disableFrameStartNotification();

		void notifyFrameStart(float _time);
		void notifyTabChangeSelect(MyGUI::Tab* _sender, size_t _index);
		void notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name);
		void notifyWindowChangeCoord(MyGUI::Window* _sender);
		void notifyMovableSeparatorChangePosition(MyGUI::VScroll* _sender, size_t _position);
		void notifyListChangePosition(MyGUI::MultiList* _sender, size_t _index);
		void notifyListSelectAccept(MyGUI::MultiList* _sender, size_t _index);
		void notifyComboAccept(MyGUI::ComboBox* _sender, size_t _index);
		void notifyTreeControlNodeSelected(MyGUI::TreeControl* _sender, MyGUI::TreeControl::Node* _node);
		void notifyFileOverwriteMessageButton(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);
		void notifyEditBasenameChange(MyGUI::Edit* _sender);
		void notifyEditBasenameAccept(MyGUI::Edit* _sender);
		void notifyButtonCancel(MyGUI::Widget* _sender);
		void notifyButtonDoIt(MyGUI::Widget* _sender);

		void doIt();
		void doIt_final();
		void cancel();
		
		static String makeMessage(const String& _template, const String& _arg1 = StringUtil::BLANK);
		static void fileSizeToString(size_t _fileSize, String* _outString);

		bool updateCaption();
		bool updateButtonCaption();
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

		void updateFromConfig();
		void preDestroyWidgets();

	private:
		Type  				mType;
		String				mResourceGroup;
		String				mCaption;
		String              mButtonCaption;
		Filters             mFilters;
		String              mCurrentBasename;
		size_t              mCurrentFilter;
		String              mCurrentPath;

		String				mPrefix;
		MyGUI::WindowPtr	mWindow;
		MyGUI::TabPtr       mTab;
		MyGUI::TabItemPtr	mTabAlphabeticList;
		MyGUI::TabItemPtr	mTabFoldersAndFiles;
		MyGUI::MultiListPtr	mListFiles;
		MyGUI::MultiListPtr	mListFilesInFolder;
		MyGUI::TreeControl* mTreeFolders;
		MyGUI::StaticText*  mStaticCurrentPath;
		MyGUI::VScrollPtr   mMovableSeparator;
		MyGUI::WidgetPtr    mPanelVContainer;
		MyGUI::WidgetPtr    mPanelVElement0;
		MyGUI::WidgetPtr    mPanelVElement1;
		MyGUI::EditPtr		mEditBasename;
		MyGUI::ComboBoxPtr	mComboFilters;
		MyGUI::ButtonPtr	mButtonDoIt;
		MyGUI::ButtonPtr	mButtonCancel;

		bool mNeedUpdateCaption;
		bool mNeedUpdateButtonCaption;
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
		bool mFrameStartNotificationEnabled;

		typedef map<String, MyGUI::TreeControl::Node*>::type NodeByFolder;
		NodeByFolder        mNodeByFolder;
	};

} // namespace GothOgre

#endif // OPEN_FILE_DIALOG_H