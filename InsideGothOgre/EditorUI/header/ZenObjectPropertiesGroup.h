#ifndef ZEN_OBJECT_PROPERTIES_GROUP_H
#define ZEN_OBJECT_PROPERTIES_GROUP_H

#include "ZenObjectPropertiesWindow.h"


namespace GUISystem
{
	namespace ZenObjectProperties
	{
		class ParamView;

		ATTRIBUTE_CLASS_MAIN_WIDGET (Group, "_Group");
		
		class Group : public BaseLayout
		{
		public:
			Group(const String& _name, Window* _propWindow);
			~Group();

			void createWidgets(MyGUI::Widget* _parent);

			const String& getName() const {return mName;}
			Window* getWindow() const {return mWindow;}

			bool empty() const {return mParamViews.empty();}
			ParamView* findParamView(const String& _paramName) const;
			ParamView* addParamView(ZenParamDef* _paramDef);
			void removeParamView(ParamView* _paramView);
			void removeParamViewsByNumUses(size_t _minNumUses);
			
			void setNumUses(size_t _numUses);
			void setHasMultipleValues(bool _multipleValues);

			void calcMinMaxHeights();
			int getMinHeight() const {return mMinHeight;}
			int getMaxHeight() const {return mMaxHeight;}

			const MyGUI::IntCoord& getCoord() const;
			void setCoord(const MyGUI::IntCoord& _coord);

			bool isMinimized() const;
			void setMinimized(bool _minimized, bool _smoothly);

			void updateSize();
			void updateValues();

			typedef Window::ObjectIterator ObjectIterator;
			ObjectIterator getObjectIterator() const;
			ZenStringInterface* getFirstObject() const;

		private:
			void createMinimizeController();
			void destroyMinimizeController();

			void notifyButtonMinimize(MyGUI::Widget* _sender);
			void notifyMinimizeControllerUpdated(MyGUI::Widget* _sender);

		private:
			ATTRIBUTE_FIELD_WIDGET_NAME(Group, mButtonMinimize, "_Group_buttonMinimize");
			MyGUI::Button*          mButtonMinimize;

			ATTRIBUTE_FIELD_WIDGET_NAME(Group, mTextCaption, "_Group_textCaption");
			MyGUI::TextBox*      mTextCaption;

			ATTRIBUTE_FIELD_WIDGET_NAME(Group, mClientArea, "_Group_clientArea");
			MyGUI::Widget*          mClientArea;

		private:
			String					    mName;
			Window*						mWindow;
			list<ParamView*>::type	    mParamViews;
			int						    mMinHeight; // height minimized
			int						    mMaxHeight; // height normal
			MyGUI::ControllerPosition*	mMinimizeController;
		};
	}
}

#endif // ZEN_OBJECT_PROPERTIES_GROUP_H