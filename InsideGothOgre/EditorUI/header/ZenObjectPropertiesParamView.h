#ifndef ZEN_OBJECT_PROPERTIES_PARAM_VIEW_H
#define ZEN_OBJECT_PROPERTIES_PARAM_VIEW_H

#include "ZenObjectPropertiesGroup.h"


namespace GUISystem
{
	namespace ZenObjectProperties
	{
		class ParamView : public BaseLayout
		{
		public:
			ParamView();
			virtual ~ParamView();

			void baseinit(ZenParamDef* _paramDef, Group* _group);

			virtual void createWidgets(MyGUI::Widget* _parent) = 0;
			virtual void updateValue() {}

			virtual const MyGUI::IntCoord& getCoord() const;
			virtual void setCoord(const MyGUI::IntCoord& _coord);

			ZenParamDef* getParamDef() const {return mParamDef;}
			Group* getGroup() const {return mGroup;}
			Window* getWindow() const;

			void setNumUses(size_t _numUses) {mNumUses = _numUses;}
			size_t getNumUses() const {return mNumUses;}
			void setHasMultipleValues(bool _hasMultipleValues) {mHasMultipleValues = _hasMultipleValues;}
			bool hasMultipleValues() const {return mHasMultipleValues;}

			typedef Window::ObjectIterator ObjectIterator;
			ObjectIterator getObjectIterator() const;
			ZenStringInterface* getFirstObject() const;

		private:
			ZenParamDef*		mParamDef;
			Group*				mGroup;
			size_t				mNumUses;
			bool				mHasMultipleValues;
		};
	}
}

#endif // ZEN_OBJECT_PROPERTIES_PARAM_VIEW_H