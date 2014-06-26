#ifndef GUISYSTEM_BASE_LAYOUT_H
#define GUISYSTEM_BASE_LAYOUT_H

#include "GUISystem_WrapsAttribute2.h"

namespace GUISystem
{
	/** This class is designed to be a base class for any class
	which represents a window and a dialog. */
	class GUISYSTEM_EXPORT BaseLayout
	{
	public:
		BaseLayout();
		virtual ~BaseLayout();

		/* Returns name of section in configuration file "ui.cfg". */
		const String& getConfigSection() const {return mConfigSection;}

		/** Returns filename for loading layout. The function returns
		a value which has been read from the class attributes or 
		has been set with function setLayoutName. */
		const String& getLayoutName() const {return mLayoutName;}

		/* Returns a pointer to main widget. */
		MyGUI::Widget* getMainWidget() const {return mMainWidget;}
		operator MyGUI::Widget*() const {return getMainWidget();}

		/** Returns a pointer to main widget if it is a window. 
		If this is not a window the function returns nullptr. */
		MyGUI::Window* getWindow() const;

		void setVisible(bool _visible);
		bool isVisible() const;

		void setModal(bool _modal);
		bool isModal() const;

		/** Returns the coordinates of the layout 
		(as fractions of the rendering window's width and height). */
		MyGUI::FloatCoord getCoord() const;

		/** Sets the coordinates of the layout. */
		void setCoord(const MyGUI::FloatCoord& _fcoord);

	protected:
		/** Initialises the window/dialog by attributes. 
		Normally the function should be called in constructor
		of derived class. */
		template <typename T>
		void initialiseByAttributes(T* _owner, MyGUI::Widget* _parent = nullptr);

		/** Gets class attributes and write them to the member variables. 
		Normally the function should be used internally 
		by the "initialiseByAttributes" function. */
		template<typename T>
		void applyClassAttributes(T* _owner);

		/** Load layout file and create the main widget and its children.
		Normally the function should be used internally 
		by the "initialiseByAttributes" function. */
		void loadLayout(MyGUI::Widget* _parent);

		/** Assign member variables to pointers to child widgets.
		Normally the function should be used internally 
		by the "initialiseByAttributes" function. */
		template<typename T>
		void assignWidgets(T* _owner);

		/** Sets filename for loading layout. Usually there is not
		need to call the function because the filename is read
		from the class attributes. */ 
		void setLayoutName(const String& _layoutName);

		/** Sets name of section in configuration file "ui.cfg". 
		Usually there is not need to call the function because 
		the section's name is read from the class attributes. */ 
		void setConfigSection(const String& _configSection);

		/** Sets the name of the main widget, as it stored in .layout file.
		Usually there is not need to call the function because 
		the section's name is read from the class attributes. */
		void setMainWidgetName(const String& _mainWidgetName);

		/** Can be overriden in derived class. 
		Note: this function is used only if the main widget's type is MyGUI::Window. */
		virtual void notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name) {}

		/** Can be overriden in derived class. 
		Note: this function is used only if the main widget's type is MyGUI::Window. */
		virtual void notifyWindowChangeCoord(MyGUI::Window* _sender) {}

	private:
		// Load widget and its children.
		static MyGUI::Widget* loadLayoutPart(const String& _layoutName, const String& _mainWidgetName, const String& _prefix = "", MyGUI::Widget* _parent = nullptr);

		// Finds widget info.
		static const MyGUI::WidgetInfo* findWidgetInfo(const String& _widgetName, const MyGUI::VectorWidgetInfo& _vectorInfo);

		// Assign 'pointer-to-widget' member variable.
		void assignSingleWidget(MyGUI::Widget*& _widget, const String& _name, bool _throwExceptionIfNotFound);

		// Reads coordinates of the layout to the configuration file.
		void readConfig();

		// Writes coordinates of the layout to the configuration file.
		void writeConfig();

	private:
		String            mLayoutName;
		String            mMainWidgetName;
		String			  mPrefix;
		String			  mConfigSection;
		MyGUI::FloatCoord mDefaultCoord;
		bool              mStoreClientCoordToConfig;
		MyGUI::Widget*	  mMainWidget;
		bool              mModal;
	};
	//--------------------------------------------------------------------------------------
	template <typename T>
	void BaseLayout::applyClassAttributes(T* _owner)
	{
		const String& layout = attribute::AttributeLayout<T>::getData();
		const String& configSection = attribute::AttributeConfigSection<T>::getData();
		const String& mainWidget = attribute::AttributeMainWidget<T>::getData();
		if(!layout.empty())
			setLayoutName( layout);

		if(!mainWidget.empty())
			setMainWidgetName(mainWidget);

		if(!configSection.empty())
			setConfigSection( configSection);
	}
	//--------------------------------------------------------------------------------------
	template <typename T>
	void BaseLayout::initialiseByAttributes(T* _owner, MyGUI::Widget* _parent)
	{
		// apply class attributes
		applyClassAttributes(_owner);

		// load layout
		loadLayout(_parent);

		// assign widgets
		assignWidgets(_owner);

		// read coordinates
		readConfig();
	}
	//--------------------------------------------------------------------------------------
	template <typename T>
	void BaseLayout::assignWidgets(T* _owner)
	{
		typename attribute::AttributeFieldWidgetName<T>::VectorBindPair& data = attribute::AttributeFieldWidgetName<T>::getData();
		for (typename attribute::AttributeFieldWidgetName<T>::VectorBindPair::iterator item=data.begin(); item!=data.end(); ++item)
		{
			MyGUI::Widget* value = 0;
			assignSingleWidget(value, item->second, true);
			item->first->set(_owner, value);
		}

		// assign optional widgets
		typename attribute::AttributeFieldWidgetNameOptional<T>::VectorBindPair& data2 = attribute::AttributeFieldWidgetNameOptional<T>::getData();
		for (typename attribute::AttributeFieldWidgetNameOptional<T>::VectorBindPair::iterator item2=data2.begin(); item2!=data2.end(); ++item2)
		{
			MyGUI::Widget* value = 0;
			assignSingleWidget(value, item2->second, false);
			item2->first->set(_owner, value);
		}
	}

} // namespace GUISystem

#endif // GUISYSTEM_BASE_LAYOUT_H
