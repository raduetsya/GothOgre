#include "Precompiled.h"
#include "ZenObjectPropertiesWindow.h"
#include "ZenObjectPropertiesGroup.h"
#include "ZenObjectPropertiesParamView.h"
#include "ZenObjectPropertiesUtil.h"


namespace GUISystem
{
	namespace ZenObjectProperties
	{
		Window::Window()
		{
		}
		//----------------------------------------------------------------------------
		void Window::initialise2(Window* _owner, MyGUI::Widget* _parent)
		{
			BaseLayout::initialiseByAttributes(_owner, _parent);
			
			mScrollView->setCanvasAlign(MyGUI::Align::Default);
			widgetCreated();

			updateGroups();
			updateValues();
		}
		//----------------------------------------------------------------------------
		Window::~Window()
		{
			destroyGroups();
			unregisterFrameListener();
		}
		//----------------------------------------------------------------------------
		void Window::removeAllObjects()
		{
			mObjects.clear();
			setNeedUpdateGroups();
		}
		//----------------------------------------------------------------------------
		void Window::addObject(ZenStringInterface* _object)
		{
			mObjects.insert(_object);
			setNeedUpdateGroups();
		}
		//----------------------------------------------------------------------------
		void Window::removeObject(ZenStringInterface* _object)
		{
			mObjects.erase(_object);
			setNeedUpdateGroups();
		}
		//----------------------------------------------------------------------------
		Window::ObjectIterator Window::getObjectIterator() const
		{
			return (ObjectIterator) mObjects;
		}
		//----------------------------------------------------------------------------
		void Window::setNeedUpdateValues()
		{
			mNeedUpdateValues = true;
			registerFrameListener();
		}
		//----------------------------------------------------------------------------
		void Window::setNeedUpdateGroups()
		{
			mNeedUpdateGroups = true;
			registerFrameListener();
		}
		//----------------------------------------------------------------------------
		void Window::setNeedUpdateGroupCoords()
		{
			mNeedUpdateGroupCoords = true;
			registerFrameListener();
		}
		//----------------------------------------------------------------------------
		void Window::frameStarted(const FrameEvent& _evt)
		{
			updateGeneric();
			unregisterFrameListener();
		}
		//----------------------------------------------------------------------------
		void Window::updateGeneric()
		{
			if(mNeedUpdateGroups)
			{
				updateGroups();
				updateGroupCoords();
				updateValues();
			}
			else
			{
				if(mNeedUpdateValues)
				{
					updateValues();
				}
				if(mNeedUpdateGroupCoords)
				{
					updateGroupCoords();
				}
			}

			mNeedUpdateGroups = false;
			mNeedUpdateValues = false;
			mNeedUpdateGroupCoords = false;
		}
		//----------------------------------------------------------------------------
		void Window::updateGroups()
		{
			mNumCreatedWidgets = 0;
			mNumDestroyedWidgets = 0;

			updateGroupList();

			for(list<Group*>::type::iterator it = mGroups.begin(); it != mGroups.end(); ++it)
			{
				Group* group = *it;
				group->createWidgets(mScrollView);
			}

			if(mNumCreatedWidgets != 0 || mNumDestroyedWidgets != 0)
			{
				calcMinMaxHeights();
				updateGroupSizes();
			}
		}
		//----------------------------------------------------------------------------
		void Window::destroyGroups()
		{
			list<Group*>::type::iterator it;
			for(it = mGroups.begin(); it != mGroups.end(); ++it)
			{
				delete *it;
			}
			mGroups.clear();
		}
		//----------------------------------------------------------------------------
		void Window::updateGroupList()
		{
			if(mObjects.empty())
			{
				destroyGroups();
				return;
			}

			// set "numUses" to zero for all paramViews
			list<Group*>::type::iterator itG;
			for(itG = mGroups.begin(); itG != mGroups.end(); ++itG)
			{
				Group* group = *itG;
				group->setNumUses(0);
				group->setHasMultipleValues(false);
			}

			// go through all paramViews of all objects
			size_t numObjects = 0;
			for(set<ZenStringInterface*>::type::iterator itO = mObjects.begin(); itO != mObjects.end(); ++itO)
			{
				ZenStringInterface* obj = *itO;
				++numObjects;
				ZenParamNameList paramNames = obj->getParamNameList();
				for(ZenParamNameList::iterator itP = paramNames.begin(); itP != paramNames.end(); ++itP)
				{
					const String& paramName = *itP;
					ZenParamDef* paramDef = obj->getParamDef(paramName);
					if(!paramDef->isInternal())
					{
						const String& groupName = paramDef->getGroup();
						Group* group = findGroup(groupName);
						if(!group)
							group = addGroup(groupName);

						const String& paramType = paramDef->getType();
						const String& paramSubType = paramDef->getSubType();
						
						ParamView* paramView = group->findParamView(paramName);
						if(paramView)
						{
							if(paramView->getParamDef()->getType() == paramType
								&& paramView->getParamDef()->getSubType() == paramSubType)
							{
								paramView->setNumUses(paramView->getNumUses() + 1);
								if(itO != mObjects.begin() && !paramView->hasMultipleValues())
								{
									if(!Util::equalsParamValues(paramDef, *mObjects.begin(), obj))
										paramView->setHasMultipleValues(true);
								}
							}
							else
							{
								group->removeParamView(paramView);
								paramView = nullptr;
							}
						}

						if(!paramView && itO == mObjects.begin())
						{
							paramView = group->addParamView(paramDef);
							paramView->setNumUses(1);
							paramView->setHasMultipleValues(false);
						}
					}
				}
			}

			size_t minNumUses = numObjects;

			// Remove unused paramViews and empty groups
			itG = mGroups.begin(); 
			while(itG != mGroups.end())
			{
				list<Group*>::type::iterator itG2 = itG; ++itG2;
				Group* group = *itG;
				group->removeParamViewsByNumUses(minNumUses);
				if(group->empty())
					removeGroup(group);
				itG = itG2;
			}
		}
		//----------------------------------------------------------------------------
		Group* Window::findGroup(const String& _groupName) const
		{
			for(list<Group*>::type::const_iterator it = mGroups.begin(); 
				it != mGroups.end(); ++it)
			{
				Group* group = *it;
				if(group->getName() == _groupName)
					return group;
			}
			return nullptr;
		}
		//----------------------------------------------------------------------------
		Group* Window::addGroup(const String& _groupName)
		{
			Group* newGroup = new Group(_groupName, this);
			mGroups.push_back(newGroup);
			return newGroup;
		}
		//----------------------------------------------------------------------------
		void Window::removeGroup(Group* _group)
		{
			mGroups.remove(_group);
			delete _group;
		}
		//----------------------------------------------------------------------------
		void Window::calcMinMaxHeights()
		{
			for(list<Group*>::type::iterator it = mGroups.begin(); it != mGroups.end(); ++it)
			{
				Group* group = *it;
				group->calcMinMaxHeights();
			}
		}
		//----------------------------------------------------------------------------
		void Window::updateGroupSizes()
		{
			for(list<Group*>::type::iterator it = mGroups.begin(); it != mGroups.end(); ++it)
			{
				Group* group = *it;
				group->updateSize();
			}
		}
		//----------------------------------------------------------------------------
		void Window::notifyWindowChangeCoord(MyGUI::Window* _sender)
		{
			setNeedUpdateGroupCoords();
		}
		//----------------------------------------------------------------------------
		void Window::updateGroupCoords()
		{
			// Calculate position of each group and total height of all groups.
			// The total height will be used later to set canvas size of scrollview.
			MyGUI::IntSize canvasSize(0, 0);
			list<Group*>::type::iterator it;
			for(it = mGroups.begin(); it != mGroups.end(); ++it)
			{
				Group* group = *it;
				MyGUI::IntCoord groupCoord = group->getCoord();
				canvasSize.height += groupCoord.height;
			}

			// Set the canvas size.
			mScrollView->setCanvasSize(canvasSize);

			// Width of the client could be changed because 
			// a vertical scroll bar could be shown or hidden.
			// So we must recalculate width of the canvas size.
			MyGUI::IntSize viewSize = mScrollView->getViewCoord().size();
			canvasSize.width = viewSize.width;
			mScrollView->setCanvasSize(canvasSize);

			// Change widths of all groups.
			int y = 0;
			for(it = mGroups.begin(); it != mGroups.end(); ++it)
			{
				// Set a new width of a group.
				Group* group = *it;
				MyGUI::IntCoord groupCoord = group->getCoord();
				groupCoord.width = canvasSize.width;
				groupCoord.left = 0;
				groupCoord.top = y;
				y += groupCoord.height;
				group->setCoord(groupCoord);
			}
		}
		//----------------------------------------------------------------------------
		void Window::updateValues()
		{
			for(list<Group*>::type::iterator it = mGroups.begin(); it != mGroups.end(); ++it)
			{
				Group* group = *it;
				group->updateValues();
			}
		}
		//----------------------------------------------------------------------------
		ZenStringInterface* Window::getFirstObject() const
		{
			if(mObjects.empty())
				return nullptr;
			else
				return *(mObjects.begin());
		}

	}
}