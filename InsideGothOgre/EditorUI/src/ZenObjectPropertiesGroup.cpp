#include "Precompiled.h"
#include "ZenObjectPropertiesGroup.h"
#include "ZenObjectPropertiesWindow.h"
#include "ZenObjectPropertiesParamView.h"
#include "ZenObjectPropertiesUtil.h"

namespace GUISystem
{
	namespace ZenObjectProperties
	{

		Group::Group(const String& _groupName, Window* _window)
		{
			mName = _groupName;
			mWindow = _window;
			setLayoutName(_window->getLayoutName());
		}
		//----------------------------------------------------------------------------
		Group::~Group()
		{
			for(list<ParamView*>::type::iterator it = mParamViews.begin(); it != mParamViews.end(); ++it)
			{
				ParamView* paramView = *it;
				if(paramView->getMainWidget())
					getWindow()->widgetDestroyed();
				delete paramView;
			}

			if(getMainWidget())
				getWindow()->widgetDestroyed();
		}
		//----------------------------------------------------------------------------
		ParamView* Group::findParamView(const String& _paramName) const
		{
			for(list<ParamView*>::type::const_iterator it = mParamViews.begin(); 
				it != mParamViews.end(); ++it)
			{
				ParamView* paramView = *it;
				if(paramView->getParamDef()->getName() == _paramName)
					return paramView;
			}
			return nullptr;
		}
		//----------------------------------------------------------------------------
		ParamView* Group::addParamView(ZenParamDef* _paramDef)
		{	
			ParamView* newParamView = Util::createParamView(_paramDef, this);
			mParamViews.push_back(newParamView);
			return newParamView;
		}
		//----------------------------------------------------------------------------
		void Group::removeParamView(ParamView* _paramView)
		{	
			mParamViews.remove(_paramView);
			delete _paramView;
		}
		//----------------------------------------------------------------------------
		void Group::removeParamViewsByNumUses(size_t _minNumUses)
		{
			list<ParamView*>::type::iterator it = mParamViews.begin(); 
			while(it != mParamViews.end())
			{
				list<ParamView*>::type::iterator it2 = it; ++it2;
				ParamView* paramView = *it;
				if(paramView->getNumUses() < _minNumUses)
				{
					mParamViews.erase(it);
					delete paramView;
				}
				it = it2;
			}		
		}
		//----------------------------------------------------------------------------
		void Group::setNumUses(size_t _numUses)
		{	
			for(list<ParamView*>::type::iterator it = mParamViews.begin(); it != mParamViews.end(); ++it)
			{
				ParamView* paramView = *it;
				paramView->setNumUses(_numUses);
			}
		}
		//----------------------------------------------------------------------------
		void Group::setHasMultipleValues(bool _hasMultipleValues)
		{	
			for(list<ParamView*>::type::iterator it = mParamViews.begin(); it != mParamViews.end(); ++it)
			{
				ParamView* paramView = *it;
				paramView->setHasMultipleValues(_hasMultipleValues);
			}
		}
		//----------------------------------------------------------------------------
		Group::ObjectIterator Group::getObjectIterator() const
		{
			return getWindow()->getObjectIterator();
		}
		//----------------------------------------------------------------------------
		ZenStringInterface* Group::getFirstObject() const 
		{
			return getWindow()->getFirstObject();
		}
		//----------------------------------------------------------------------------
		void Group::createWidgets(MyGUI::Widget* _parent)
		{
			if(!getMainWidget())
			{
				initialiseByAttributes(this, _parent);

				// Disable aligning because we are setting coordinates manually.
				getMainWidget()->setAlign(MyGUI::Align::Default);

				mButtonMinimize->eventMouseButtonClick = MyGUI::newDelegate(this, &Group::notifyButtonMinimize);
				mTextCaption->eventMouseButtonDoubleClick = MyGUI::newDelegate(this, &Group::notifyButtonMinimize);
				mTextCaption->setCaption(mName);
				getWindow()->widgetCreated();
			}

			for(list<ParamView*>::type::iterator it = mParamViews.begin(); it != mParamViews.end(); ++it)
			{
				ParamView* paramView = *it;
				if(!paramView->getMainWidget())
				{
					paramView->createWidgets(mClientArea);
					paramView->getMainWidget()->setAlign(MyGUI::Align::Default);
					getWindow()->widgetCreated();
				}
			}
		}
		//----------------------------------------------------------------------------
		void Group::notifyButtonMinimize(MyGUI::Widget* _sender)
		{
			setMinimized(!isMinimized(), true);
		}
		//----------------------------------------------------------------------------
		bool Group::isMinimized() const
		{
			return mButtonMinimize->getStateCheck();
		}
		//----------------------------------------------------------------------------
		void Group::setMinimized(bool _minimized, bool _smoothly)
		{
			mButtonMinimize->setStateCheck(_minimized);
			if(_smoothly)
			{
				mMinimizeController = MyGUI::ControllerManager::getInstance().createItem("ControllerPosition")->castType<MyGUI::ControllerPosition>();
				mMinimizeController->eventUpdateAction = MyGUI::newDelegate(this, &Group::notifyMinimizeControllerUpdated);
				MyGUI::ControllerManager::getInstance().addItem(getMainWidget(), mMinimizeController);

				MyGUI::IntSize finalSize = getMainWidget()->getSize();
				if(_minimized)
					finalSize.height = mMinHeight;
				else
					finalSize.height = mMaxHeight;

				mMinimizeController->setSize(finalSize);
				mMinimizeController->setTime(0.5f);
				mMinimizeController->setFunction("Inertional");
			}
			else
			{
				updateSize();
				getWindow()->setNeedUpdateGroupCoords();
			}
		}
		//----------------------------------------------------------------------------
		void Group::notifyMinimizeControllerUpdated(MyGUI::Widget* _sender)
		{
			getWindow()->updateGroupCoords();
		}
		//----------------------------------------------------------------------------
		void Group::updateSize()
		{
			MyGUI::ControllerManager::getInstance().removeItem(getMainWidget());
			MyGUI::IntCoord coord = getMainWidget()->getCoord();
			coord.height = (isMinimized() ? mMinHeight : mMaxHeight);
			getMainWidget()->setCoord(coord);
		}
		//----------------------------------------------------------------------------
		void Group::calcMinMaxHeights()
		{
			int clientHeight = 0;
			list<ParamView*>::type::iterator it;
			for(it = mParamViews.begin(); it != mParamViews.end(); ++it)
			{
				ParamView* paramView = *it;
				const MyGUI::IntCoord& paramViewCoord = paramView->getCoord();
				clientHeight += paramViewCoord.height;
			}
			MyGUI::IntSize dSize = getMainWidget()->getSize() - mClientArea->getSize();
			mMinHeight = dSize.height;
			mMaxHeight = dSize.height + clientHeight;
		}
		//----------------------------------------------------------------------------
		const MyGUI::IntCoord& Group::getCoord() const 
		{
			return getMainWidget()->getCoord();
		}
		//----------------------------------------------------------------------------
		void Group::setCoord(const MyGUI::IntCoord& _coord)
		{
			getMainWidget()->setCoord(_coord);
			MyGUI::IntSize clientSize = mClientArea->getSize();
			int y = 0;
			list<ParamView*>::type::iterator it;
			for(it = mParamViews.begin(); it != mParamViews.end(); ++it)
			{
				ParamView* paramView = *it;
				MyGUI::IntCoord paramViewCoord = paramView->getCoord();
				paramViewCoord.top = y;
				y += paramViewCoord.height;
				paramViewCoord.left = 0;
				paramViewCoord.width = clientSize.width;
				paramView->setCoord(paramViewCoord);
			}
		}
		//----------------------------------------------------------------------------
		void Group::updateValues()
		{	
			for(list<ParamView*>::type::iterator it = mParamViews.begin(); it != mParamViews.end(); ++it)
			{
				ParamView* paramView = *it;
				paramView->updateValue();
			}
		}

	}
}