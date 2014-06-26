#include "GUISystem_Precompiled.h"
#include "GUISystem_BaseLayout.h"
#include "GUISystem_UISettings.h"

namespace GUISystem
{
	BaseLayout::BaseLayout()
	{
		mMainWidget = nullptr;
		mStoreClientCoordToConfig = false;
		mModal = false;
	}
	//--------------------------------------------------------------------------------------
	BaseLayout::~BaseLayout()
	{
		UISettings::getSingleton().beginBatch();
		writeConfig();
		UISettings::getSingleton().endBatch(false); // It's recommender to not fire UI settings listeners from any destructor

		if(mMainWidget)
		{
			setModal(false);
			MyGUI::WidgetManager::getInstance().destroyWidget(mMainWidget);
		}
	}
	//--------------------------------------------------------------------------------------
	void BaseLayout::setLayoutName(const String& _layoutName)
	{
		mLayoutName = MyGUI::LanguageManager::getInstance().replaceTags(
			_layoutName);
	}
	//--------------------------------------------------------------------------------------
	void BaseLayout::setConfigSection(const String& _configSection)
	{
		mConfigSection = MyGUI::LanguageManager::getInstance().replaceTags(
			_configSection);
	}
	//--------------------------------------------------------------------------------------
	void BaseLayout::setMainWidgetName(const String& _mainWidgetName)
	{
		mMainWidgetName = _mainWidgetName;
	}
	//--------------------------------------------------------------------------------------
	const MyGUI::WidgetInfo* BaseLayout::findWidgetInfo(const String& _widgetName, const MyGUI::VectorWidgetInfo& _vectorInfo)
	{
		for (MyGUI::VectorWidgetInfo::const_iterator iter = _vectorInfo.begin(); iter != _vectorInfo.end(); ++iter)
		{
			// check name
			if(iter->name == _widgetName)
				return &*iter;

			// iterate children
			const MyGUI::WidgetInfo* childFind = findWidgetInfo(_widgetName, iter->childWidgetsInfo);
			if(childFind)
				return childFind;
		}

		// found nothing
		return nullptr;
	}
	//--------------------------------------------------------------------------------------
	MyGUI::Widget* BaseLayout::loadLayoutPart(const String& _layoutName, const String& _mainWidgetName, const String& _prefix, MyGUI::Widget* _parent)
	{
		// load "*.layout" file
		MyGUI::ResourceLayout* resourceLayout = MyGUI::LayoutManager::getInstance().getByName(_layoutName, false);
		if (!resourceLayout)
		{
			MyGUI::ResourceManager::getInstance().load(_layoutName);
			resourceLayout = MyGUI::LayoutManager::getInstance().getByName(_layoutName, false);
		}
		GOTHOGRE_ASSERT(resourceLayout != nullptr, "Layout '" << _layoutName << "' couldn't be loaded");

		// find main widget's info
		const MyGUI::WidgetInfo* widgetInfo = findWidgetInfo(_mainWidgetName, resourceLayout->getLayoutData());
		GOTHOGRE_ASSERT(widgetInfo != nullptr, "Root widget '" << _mainWidgetName << "' in layout '" << _layoutName << "' not found.");

		// create main widget and its children
		MyGUI::Widget* widget = resourceLayout->createWidget(*widgetInfo, _prefix, _parent);
		GOTHOGRE_ASSERT(widget != nullptr, "Could not create root widget '" << _mainWidgetName << "' in layout '" << _layoutName << "'.");

		return widget;
	}
	//--------------------------------------------------------------------------------------
	void BaseLayout::loadLayout(MyGUI::Widget* _parent)
	{
		if(mMainWidgetName.empty())
			mMainWidgetName = "_Main";

		mPrefix = MyGUI::utility::toString(this, "_");

		mMainWidget = loadLayoutPart(mLayoutName, mMainWidgetName, mPrefix, _parent);
		mMainWidget->setVisible(true);

		// get parameters from user strings of the main widget
		if(mConfigSection.empty())
		{
			String userStr = mMainWidget->getUserString("ConfigSection");
			if(!userStr.empty())
				setConfigSection(userStr);
		}

		if(mDefaultCoord == MyGUI::FloatCoord())
		{
			String userStr = mMainWidget->getUserString("DefaultCoord");
			if(!userStr.empty())
			{
				StrStream ss(userStr, StrStream::DONT_COPY);
				ss >> mDefaultCoord.left;
				ss >> mDefaultCoord.top;
				ss >> mDefaultCoord.width;
				ss >> mDefaultCoord.height;
			}
		}

		if(mStoreClientCoordToConfig == false)
		{
			String userStr = mMainWidget->getUserString("StoreClientCoordToConfig");
			if(!userStr.empty())
				mStoreClientCoordToConfig = StrConv::fromString<bool>(userStr);
		}

		// register window message handlers
		if(mMainWidget->isType<MyGUI::Window>())
		{
			MyGUI::Window* win = mMainWidget->castType<MyGUI::Window>();
			win->eventWindowButtonPressed += MyGUI::newDelegate(this, &BaseLayout::notifyWindowButtonPressed);
			win->eventWindowChangeCoord += MyGUI::newDelegate(this, &BaseLayout::notifyWindowChangeCoord);
		}
	}
	//--------------------------------------------------------------------------------------
	void BaseLayout::assignSingleWidget(MyGUI::Widget*& _widget, const String& _name, bool _throwException)
	{
		_widget = mMainWidget->findWidget(mPrefix + _name);
		if(!_widget && _throwException)
			GOTHOGRE_EXCEPT("Widget '" << _name << "' in layout '" << mLayoutName << "' not found.");
	}
	//--------------------------------------------------------------------------------------
	void BaseLayout::readConfig()
	{
		if(!mMainWidget)
			return;

		MyGUI::FloatCoord fcoord = mDefaultCoord;
		if(!mConfigSection.empty())
		{
			UISettings& ui = UISettings::getSingleton();
			static const String sLeft   = "Left";
			static const String sTop    = "Top";
			static const String sWidth  = "Width";
			static const String sHeight = "Height";
			fcoord.left   = ui.getSetting<float>(sLeft,   mConfigSection, mDefaultCoord.left);
			fcoord.top    = ui.getSetting<float>(sTop,    mConfigSection, mDefaultCoord.top);
			fcoord.width  = ui.getSetting<float>(sWidth,  mConfigSection, mDefaultCoord.width);
			fcoord.height = ui.getSetting<float>(sHeight, mConfigSection, mDefaultCoord.height);
		}

		setCoord(fcoord);
	}
	//--------------------------------------------------------------------------------------
	void BaseLayout::setCoord(const MyGUI::FloatCoord& _fcoord)
	{
		if(_fcoord != MyGUI::FloatCoord())
		{
			MyGUI::IntSize viewSize = MyGUI::RenderManager::getInstance().getViewSize();
			MyGUI::IntCoord newCoord;
			newCoord.left   = int(   _fcoord.left * viewSize.width  );
			newCoord.top    = int(    _fcoord.top * viewSize.height );
			newCoord.width  = int(  _fcoord.width * viewSize.width  );
			newCoord.height = int( _fcoord.height * viewSize.height );

			MyGUI::IntCoord curCoord;
			MyGUI::FloatCoord curFcoord = getCoord();
			curCoord.left   = int(   curFcoord.left * viewSize.width  );
			curCoord.top    = int(    curFcoord.top * viewSize.height );
			curCoord.width  = int(  curFcoord.width * viewSize.width  );
			curCoord.height = int( curFcoord.height * viewSize.height );

			if(newCoord != curCoord)
			{
				MyGUI::IntCoord mwCoord = mMainWidget->getCoord();
				int mwRight  = mwCoord.left + mwCoord.width;
				int mwBottom = mwCoord.top  + mwCoord.height;
				int newRight  = newCoord.left + newCoord.width;
				int newBottom = newCoord.top  + newCoord.height;
				int curRight  = curCoord.left + curCoord.width;
				int curBottom = curCoord.top  + curCoord.height;

				mwCoord.left  += newCoord.left - curCoord.left;
				mwCoord.top   += newCoord.top  - curCoord.top;
				mwCoord.width  = mwRight  + newRight  - curRight  - mwCoord.left;
				mwCoord.height = mwBottom + newBottom - curBottom - mwCoord.top;
				
				mMainWidget->setCoord(mwCoord);
				if(mMainWidget->isType<MyGUI::Window>())
				{
					MyGUI::Window* win = mMainWidget->castType<MyGUI::Window>();			
					notifyWindowChangeCoord(win);
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------
	void BaseLayout::writeConfig()
	{
		if(!mMainWidget || mConfigSection.empty())
			return;

		MyGUI::FloatCoord fcoord = getCoord();
		
		UISettings& ui = UISettings::getSingleton();
		ui.beginBatch();
		static const String sLeft   = "Left";
		static const String sTop    = "Top";
		static const String sWidth  = "Width";
		static const String sHeight = "Height";
		ui.setSetting<float>(sLeft,   mConfigSection, fcoord.left);
		ui.setSetting<float>(sTop,    mConfigSection, fcoord.top);
		ui.setSetting<float>(sWidth,  mConfigSection, fcoord.width);
		ui.setSetting<float>(sHeight, mConfigSection, fcoord.height);
		ui.endBatch();
	}
	//--------------------------------------------------------------------------------------
	MyGUI::FloatCoord BaseLayout::getCoord() const
	{
		MyGUI::Widget* widget = mMainWidget;
		if(mStoreClientCoordToConfig)
		{
			widget = widget->getClientWidget();
			GOTHOGRE_ASSERT(widget, "Client of widget '" << mMainWidget << "' of layout '" << mLayoutName << "' not found.");
		}
		MyGUI::IntCoord icoord = widget->getAbsoluteCoord();

		MyGUI::IntSize viewSize = MyGUI::RenderManager::getInstance().getViewSize();
		MyGUI::FloatCoord fcoord(
			float(icoord.left) / viewSize.width,
			float(icoord.top) / viewSize.height,
			float(icoord.width) / viewSize.width,
			float(icoord.height) / viewSize.height);

		return fcoord;
	}
	//--------------------------------------------------------------------------------------
	MyGUI::Window* BaseLayout::getWindow() const
	{
		if(mMainWidget->isType<MyGUI::Window>())
			return mMainWidget->castType<MyGUI::Window>();
		else
			return nullptr;
	}
	//--------------------------------------------------------------------------------------
	void BaseLayout::setVisible(bool _visible)
	{
		mMainWidget->setVisible(_visible);
	}
	//--------------------------------------------------------------------------------------
	bool BaseLayout::isVisible() const
	{
		return mMainWidget->getVisible();
	}
	//--------------------------------------------------------------------------------------
	void BaseLayout::setModal(bool _modal)
	{
		if(mModal != _modal)
		{
			mModal = _modal;
			if(_modal)
				MyGUI::InputManager::getInstance().addWidgetModal(mMainWidget);
			else
				MyGUI::InputManager::getInstance().removeWidgetModal(mMainWidget);
		}
	}
	//--------------------------------------------------------------------------------------
	bool BaseLayout::isModal() const
	{
		return mModal;
	}

} // namespace GUISystem