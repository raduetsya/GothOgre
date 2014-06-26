#include "Precompiled.h"
#include "WorldViewWindow.h"
#include "WorldViewWindowManager.h"


namespace GUISystem
{
	//---------------------------------------------------------------------------
	WorldViewWindow::WorldViewWindow(World* _world)
		: mRenderTarget(nullptr), mWorld(nullptr), 
		  mCamera(nullptr), mViewport(nullptr), 
		  mClosing(false)
	{
		mWorld = _world;
		mWorldName = mWorld->getName();	
		
		initialiseByAttributes(this);

		// Read the window's coordinates from the configuration file worldedit.cfg
		WorldEditSettings& wes = WorldEditSettings::getSingleton();
		if(wes.hasSection(mWorldName))
		{
			MyGUI::FloatCoord fcoord;
			fcoord.left   = wes.getSetting<float>("WindowLeft",   mWorldName, 0.1f);
			fcoord.top    = wes.getSetting<float>("WindowTop",    mWorldName, 0.1f);
			fcoord.width  = wes.getSetting<float>("WindowWidth",  mWorldName, 0.5f);
			fcoord.height = wes.getSetting<float>("WindowHeight", mWorldName, 0.5f);
			setCoord(fcoord);
		}
		else
		{
			static int sWindowIndex = 0;
			MyGUI::FloatPoint newWindowOffset = MyGUI::FloatPoint::parse(getWindow()->getUserString("NewWindowOffset"));
			MyGUI::FloatCoord fcoord = getCoord();
			fcoord.left += newWindowOffset.left * sWindowIndex;
			fcoord.top  += newWindowOffset.top * sWindowIndex;
			sWindowIndex++;
			if(fcoord.left < 1.0f && fcoord.top < 1.0f)
			{
				fcoord.width  = std::min(fcoord.left + fcoord.width,  1.0f) - fcoord.left;
				fcoord.height = std::min(fcoord.top  + fcoord.height, 1.0f) - fcoord.top;
				setCoord(fcoord);
			}
		}
		
		getWindow()->eventKeySetFocus += MyGUI::newDelegate(this, &WorldViewWindow::notifyKeySetFocus);
		
		// TRM_PT_VIEW_ALL
		mCanvas->createTexture(MyGUI::Canvas::TRM_PT_VIEW_ALL, MyGUI::TextureUsage::RenderTarget);
		mCanvas->eventPreTextureChanges += MyGUI::newDelegate(this, &WorldViewWindow::notifyPreTextureChanges);
		mCanvas->requestUpdateCanvas = MyGUI::newDelegate(this, &WorldViewWindow::requestUpdateCanvas);
		mCanvas->updateTexture();

		updateCaption();
		updateViewportAbsCoord();

		registerWorldListener();
		registerWorldObjectListener();
	}
	//---------------------------------------------------------------------------
	WorldViewWindow::~WorldViewWindow()
	{
		// Write the window's coordinates to the configuration file worldedit.cfg
		WorldEditSettings& wes = WorldEditSettings::getSingleton();
		MyGUI::FloatCoord fcoord = getCoord();
		wes.setSetting( "WindowLeft",   mWorldName, fcoord.left );
		wes.setSetting( "WindowTop",    mWorldName, fcoord.top );
		wes.setSetting( "WindowWidth",  mWorldName, fcoord.width );
		wes.setSetting( "WindowHeight", mWorldName, fcoord.height );

		if(mCanvas)
			mCanvas->destroyTexture();

		if(mWorld)
			mWorld->setViewport(nullptr);
	}
	//---------------------------------------------------------------------------
	void WorldViewWindow::notifyPreTextureChanges(MyGUI::Canvas* _canvas)
	{
		if (mRenderTarget != nullptr)
		{
			mRenderTarget->removeAllViewports();
			mRenderTarget = nullptr;
		}
	}
	//---------------------------------------------------------------------------
	void WorldViewWindow::requestUpdateCanvas(MyGUI::Canvas* _canvas, MyGUI::Canvas::Event _event)
	{
		if (_event.textureChanged || _event.requested)
		{
			TexturePtr texture = MyGUITextureUtil::textureFromMyGUI(mCanvas->getTexture());
			RenderTexture* target = texture->getBuffer()->getRenderTarget();

			if (mRenderTarget != target && target != nullptr)
			{
				mRenderTarget = target;
				mRenderTarget->removeAllViewports();

				mCamera = mWorld->getCamera();
				mViewport = mRenderTarget->addViewport(mCamera, -1);
				mViewport->setClearEveryFrame(true);
				mViewport->setOverlaysEnabled(false);
				mWorld->setViewport(mViewport);
			}
		}
	}
	//---------------------------------------------------------------------------
	void WorldViewWindow::notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name)
	{
		if(_name == "close")
		{
			setClosing(true);
			WorldViewWindowManager::getSingleton()._processWindowClosing();
		}
	}
	//---------------------------------------------------------------------------
	void WorldViewWindow::notifyWindowChangeCoord(MyGUI::Window* _sender)
	{
		updateViewportAbsCoord();
	}
	//---------------------------------------------------------------------------
	void WorldViewWindow::notifyKeySetFocus(MyGUI::Widget* _sender, MyGUI::Widget* _old)
	{
		WorldManager::getSingleton().setActiveWorld(getWorld());
	}
	//---------------------------------------------------------------------------
	void WorldViewWindow::worldLoaded(const WorldEvent& _evt)
	{
		if(mWorldChanged != mWorld->isChanged() || mWorldName != mWorld->getName())
			updateCaption();
	}
	//---------------------------------------------------------------------------
	void WorldViewWindow::worldSaved(const WorldEvent& _evt)
	{
		if(mWorldChanged != mWorld->isChanged() || mWorldName != mWorld->getName())
			updateCaption();
	}
	//---------------------------------------------------------------------------
	void WorldViewWindow::worldActiveChanged(const WorldEvent& _evt)
	{
		if(mWorldActive != mWorld->isActive())
		{
			updateCaption();
			updateInactiveShower();
			if(mWorld->isActive())
				getWindow()->upLayerItem();
		}
	}
	//---------------------------------------------------------------------------
	void WorldViewWindow::worldObjectAdded(const WorldObjectEvent& _evt)
	{
		if(mWorldChanged != mWorld->isChanged())
			updateCaption();
	}
	//---------------------------------------------------------------------------
	void WorldViewWindow::worldObjectRemoved(const WorldObjectEvent& _evt)
	{
		if(mWorldChanged != mWorld->isChanged())
			updateCaption();
	}
	//---------------------------------------------------------------------------
	void WorldViewWindow::worldObjectChanged(const WorldObjectEvent& _evt)
	{
		if(mWorldChanged != mWorld->isChanged())
			updateCaption();
	}
	//---------------------------------------------------------------------------
	void WorldViewWindow::updateCaption()
	{
		mWorldChanged = mWorld->isChanged();
		mWorldActive = mWorld->isActive();
		mWorldName = mWorld->getName();

		getWindow()->setCaption(generateWindowCaption(mWorld));
	}
	//----------------------------------------------------------------------------
	String WorldViewWindow::generateWindowCaption(World* _world)
	{
		String caption = _world->getShortName();
		if(_world->isChanged())
			caption = caption + "*";
		if(_world->isActive())
			caption = "<A> " + caption;
		return caption;
	}
	//---------------------------------------------------------------------------
	void WorldViewWindow::updateInactiveShower()
	{
		mPanelInactiveShower->setVisible(!mWorld->isActive());
	}
	//---------------------------------------------------------------------------
	void WorldViewWindow::updateViewportAbsCoord()
	{
		MyGUI::IntCoord coord = mCanvas->getAbsoluteCoord();
		ViewportAbsCoord coord2(coord.left, coord.top, coord.width, coord.height);
		mWorld->setViewportAbsCoord(coord2);
	}

}