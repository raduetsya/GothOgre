#include "Precompiled.h"
#include "EditorInitBeforeTools.h"
#include "EditorMenuBar.h"
#include "EditorBackground.h"
#include "WorldViewWindowManager.h"
#include "WorldObjectPropertiesWindowManager.h"
#include "WorldObjectTreeWindowManager.h"


namespace GUISystem
{
	//-------------------------------------------------------------------------
	EditorInitBeforeTools::EditorInitBeforeTools()
		: mEditorMenuBar(nullptr), mEditorBackground(nullptr),
		  mWorldViewWindowManager(nullptr),
		  mWorldObjectPropertiesWindowManager(nullptr),
		  mWorldObjectTreeWindowManager(nullptr)
	{
		registerUIModeListener( UIInitPriority::BEFORE_TOOLS );
	}
	//-------------------------------------------------------------------------
	EditorInitBeforeTools::~EditorInitBeforeTools()
	{
		shutdown();
	}
	//-------------------------------------------------------------------------
	void EditorInitBeforeTools::uiModeEntered(const UIModeEvent& _evt)
	{
		if(_evt.getNewUIMode() == UIMode::EDITOR)
			initialise();
	}
	//-------------------------------------------------------------------------
	void EditorInitBeforeTools::uiModeExited(const UIModeEvent& _evt)
	{
		if(_evt.getOldUIMode() == UIMode::EDITOR)
			shutdown();
	}
	//-------------------------------------------------------------------------
	void EditorInitBeforeTools::initialise()
	{
		if(!mEditorMenuBar)
			mEditorMenuBar = new EditorMenuBar;

		if(!mEditorBackground)
			mEditorBackground = new EditorBackground;

		if(!mWorldViewWindowManager)
			mWorldViewWindowManager = new WorldViewWindowManager;

		if(!mWorldObjectPropertiesWindowManager)
			mWorldObjectPropertiesWindowManager = new WorldObjectPropertiesWindowManager;

		if(!mWorldObjectTreeWindowManager)
			mWorldObjectTreeWindowManager = new WorldObjectTreeWindowManager;
	}
	//-------------------------------------------------------------------------
	void EditorInitBeforeTools::shutdown()
	{
		if(mEditorMenuBar)
		{
			delete mEditorMenuBar;
			mEditorMenuBar = nullptr;
		}

		if(mEditorBackground)
		{
			delete mEditorBackground;
			mEditorBackground = nullptr;
		}

		if(mWorldViewWindowManager)
		{
			delete mWorldViewWindowManager;
			mWorldViewWindowManager = nullptr;
		}

		if(mWorldObjectPropertiesWindowManager)
		{
			delete mWorldObjectPropertiesWindowManager;
			mWorldObjectPropertiesWindowManager = nullptr;
		}

		if(mWorldObjectTreeWindowManager)
		{
			delete mWorldObjectTreeWindowManager;
			mWorldObjectTreeWindowManager = nullptr;
		}
	}

}
