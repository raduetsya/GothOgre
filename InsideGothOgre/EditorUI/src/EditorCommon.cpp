#include "EditorUI_Precompiled.h"
#include "EditorUI_CommonInclude.h"
#include "EditorCommon.h"
#include "TreeControl.h"
#include "TreeControlItem.h"
#include "OpenFileDialog.h"

namespace GothOgre
{
	EditorCommon::EditorCommon()
	{
		mInitializer = new Initializer(this);
		mShutdowner  = new Shutdowner(this);
	}
	//-------------------------------------------------------------------------------
	EditorCommon::~EditorCommon()
	{
		if(mInitializer)
			delete mInitializer;

		if(mShutdowner)
			delete mShutdowner;
	}
	//-------------------------------------------------------------------------------
	void EditorCommon::_initialize()
	{
		MyGUI::ResourceManager::getInstance().load("EditorUI.xml");

		MyGUI::FactoryManager& factory = MyGUI::FactoryManager::getInstance();
		factory.registerFactory<MyGUI::TreeControl>("Widget");
		factory.registerFactory<MyGUI::TreeControlItem>("Widget");
	}
	//-------------------------------------------------------------------------------
	void EditorCommon::_shutdown()
	{
		MyGUI::FactoryManager& factory = MyGUI::FactoryManager::getInstance();
		factory.unregisterFactory<MyGUI::TreeControl>("Widget");
		factory.unregisterFactory<MyGUI::TreeControlItem>("Widget");
	}
	//-------------------------------------------------------------------------------
	EditorCommon::Initializer::Initializer(EditorCommon* _common)
	{
		mEditorCommon = _common;
		UIModeSwitcher::getSingleton().addListener(this, UIModeOrder::AfterFirst);
	}

	EditorCommon::Initializer::~Initializer()
	{
		UIModeSwitcher::getSingleton().removeListener(this);
	}

	void EditorCommon::Initializer::switchUIMode(const UIModeEvent& _evt)
	{
		if(_evt.getNewMode() != UIMode::Shutdown)
			mEditorCommon->_initialize();
	}
	//-----------------------------------------------------------------------------
	EditorCommon::Shutdowner::Shutdowner(EditorCommon* _common)
	{
		mEditorCommon = _common;
		UIModeSwitcher::getSingleton().addListener(this, UIModeOrder::BeforeLast);
	}

	EditorCommon::Shutdowner::~Shutdowner()
	{
		UIModeSwitcher::getSingleton().removeListener(this);
	}

	void EditorCommon::Shutdowner::switchUIMode(const UIModeEvent& _evt)
	{
		if(_evt.getNewMode() == UIMode::Shutdown)
			mEditorCommon->_shutdown();
	}

} // namespace GothOgre