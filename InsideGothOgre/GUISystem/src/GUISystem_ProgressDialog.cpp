#include "GUISystem_Precompiled.h"
#include "GUISystem_ProgressDialog.h"
#include "GUISystem_UISettings.h"

//-------------------------------------------------------------------------
GUISystem::ProgressDialog* 
	Ogre::Singleton<GUISystem::ProgressDialog>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------


namespace GUISystem
{
	//------------------------------------------------------------------------
	enum {PROGRESSBAR_MAX_RANGE = 10000};
	//------------------------------------------------------------------------
	BaseProgressDialog::BaseProgressDialog()
	{
	}
	//------------------------------------------------------------------------
	void BaseProgressDialog::initialise2(BaseProgressDialog* _owner, MyGUI::Widget* _parent)
	{
		BaseLayout::initialiseByAttributes( _owner, _parent );

		if(!mTextCaption && getMainWidget()->isType<MyGUI::Window>())
		{
			MyGUI::Window* win = getMainWidget()->castType<MyGUI::Window>();
			mTextCaption = win->getCaptionWidget();
		}

		if(mProgressBar)
			mProgressBar->setProgressRange(PROGRESSBAR_MAX_RANGE);

		if(mButtonClose)
			mButtonClose->eventMouseButtonClick += MyGUI::newDelegate(this, &BaseProgressDialog::notifyButtonClose);

		if(mButtonStopTimer)
			mButtonStopTimer->eventMouseButtonClick += MyGUI::newDelegate(this, &BaseProgressDialog::notifyButtonStopTimer);

		// Progress began already (because otherwise this dialog was not been shown).
		setVisible(true);
		setModal(true);

		mWaitProgressEnd = true;
		updateButtons();

		registerFrameListener();
		registerProgressListener();
		
		if(mListLog)
		{
			mListLog->removeAllItems();
			registerLogListener();
		}

		mNumInfos = 0;
		mNumWarnings = 0;
		mNumErrors = 0;
	}
	//------------------------------------------------------------------------
	void BaseProgressDialog::progressEnded()
	{
		unregisterLogListener();
		mWaitProgressEnd = false;

		if(mListLog)
		{
			String templ = mListLog->getUserString("TotalFormat");
			MyGUI::LanguageManager& lang = MyGUI::LanguageManager::getInstance();
			lang.addUserTag("1", StrConv::toString(mNumInfos));
			lang.addUserTag("2", StrConv::toString(mNumWarnings));
			lang.addUserTag("3", StrConv::toString(mNumErrors));
			MyGUI::UString total = lang.replaceTags(templ);
			mListLog->addItem(total);
			mListLog->beginToItemLast();
		}

		if(mProgressBar)
		{
			mProgressBar->setProgressPosition(PROGRESSBAR_MAX_RANGE);
		}

		mTimeout = 0;
		if(mButtonClose)
		{
			if(mNumErrors != 0)
			{
				Real defaultTimeout = StrConv::fromString<Real>( 
					mButtonClose->getUserString("DefaultErrorTimeout"));
				
				mTimeout = UISettings::getSingleton().getSetting<Real>(
					"ErrorTimeout", getConfigSection(), defaultTimeout);
			}
			else if(mNumWarnings != 0)
			{
				Real defaultTimeout = StrConv::fromString<Real>(
					mButtonClose->getUserString("DefaultWarningTimeout"));

				mTimeout = UISettings::getSingleton().getSetting<Real>(
					"WarningTimeout", getConfigSection(), defaultTimeout);
			}
			else if(mNumInfos != 0)
			{
				Real defaultTimeout = StrConv::fromString<Real>(
					mButtonClose->getUserString("DefaultInfoTimeout"));

				mTimeout = UISettings::getSingleton().getSetting<Real>(
					"InfoTimeout", getConfigSection(), defaultTimeout);
			}
		}

		if(mTimeout <= 0)
			close();
		else
			updateButtons();
	}
	//------------------------------------------------------------------------
	BaseProgressDialog::~BaseProgressDialog()
	{
	}
	//------------------------------------------------------------------------
	void BaseProgressDialog::messageLogged(const String& _module, LogLevel _lvl, const String& _message)
	{
		if(mListLog)
		{
			String colorName;
			switch(_lvl)
			{
				case LogLevel::INFO: 
				{
					colorName = "ColorInfo"; 
					++mNumInfos;
					break;
				}
				case LogLevel::WARNING: 
				{
					colorName = "ColorWarning"; 
					++mNumWarnings;
					break;
				}
				case LogLevel::ERROR: 
				{
					colorName = "ColorError"; 
					++mNumErrors;
					break;
				}
			}

			String color = mListLog->getUserString(colorName);
			mListLog->addItem(color + _message);
			mListLog->beginToItemLast();
		}
	}
	//------------------------------------------------------------------------
	void BaseProgressDialog::frameStarted(const FrameEvent& _evt)
	{
		Progress& progress = Progress::getSingleton();
	
		if(mTextCaption)
		{
			const String& caption = progress.getCaption();
			mTextCaption->setCaptionWithReplacing(caption);
		}

		if(mTextMessage)
		{
			const String& message = progress.getMessage();
			mTextMessage->setCaptionWithReplacing(message);
		}
	
		if(mProgressBar)
		{
			size_t pos = (size_t) (progress.getRealPosition() * PROGRESSBAR_MAX_RANGE);
			mProgressBar->setProgressPosition(pos);
		}
		
		if(!mWaitProgressEnd)
		{
			mTimeout -= _evt.timeSinceLastFrame;
			if(mTimeout <= 0)
				close();
			else
				updateButtons();
		}
	}
	//----------------------------------------------------------------------
	void BaseProgressDialog::updateButtons()
	{
		MyGUI::LanguageManager& lang = MyGUI::LanguageManager::getInstance();

		if(mWaitProgressEnd)
		{
			if(mButtonStopTimer)
				mButtonStopTimer->setEnabled(false);

			if(mButtonClose)
			{
				String caption = mButtonClose->getUserString("CaptionDefault");
				MyGUI::UString caption2 = lang.replaceTags(caption);
				mButtonClose->setCaption(caption2);
				mButtonClose->setEnabled(false);
			}
		}
		else
		{
			bool waitTimeout = (mTimeout < 1000);
			
			if(mButtonStopTimer)
				mButtonStopTimer->setEnabled( waitTimeout );
			
			if(mButtonClose)
			{
				mButtonClose->setEnabled(true);
				MyGUI::UString caption2;
				if(waitTimeout)
				{
					int seconds = Math::ICeil(mTimeout);
					lang.addUserTag("1", StrConv::toString(seconds));
					String caption = mButtonClose->getUserString("CaptionWithTimeout");
					caption2 = lang.replaceTags(caption);
				}
				else
				{
					String caption = mButtonClose->getUserString("CaptionDefault");
					caption2 = lang.replaceTags(caption);
				}
				mButtonClose->setCaption(caption2);
			}
		}
	}
	//------------------------------------------------------------------------
	void BaseProgressDialog::close()
	{
		eventEndDialog();
	}
	//----------------------------------------------------------------------
	void BaseProgressDialog::notifyButtonStopTimer(MyGUI::Widget* _sender)
	{
		mTimeout = Math::POS_INFINITY;
		updateButtons();
	}
	//----------------------------------------------------------------------
	void BaseProgressDialog::notifyButtonClose(MyGUI::Widget* _sender)
	{
		close();
	}
	//----------------------------------------------------------------------
	void BaseProgressDialog::notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name)
	{
		if(_name == "close")
			close();
	}

}