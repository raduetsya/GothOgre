#ifndef ZEN_OBJECT_PROPERTIES_PARAM_VIEWS_2_H
#define ZEN_OBJECT_PROPERTIES_PARAM_VIEWS_2_H

#include "ZenObjectPropertiesParamViews1.h"


namespace GUISystem
{
	namespace ZenObjectProperties
	{
		//----------------------------------------------------------------------------
		// StringView
		//----------------------------------------------------------------------------
		class StringView : public EditView
		{
		public:
			void updateValue()
			{
				String s;
				if(hasMultipleValues())
				{
					s = Util::MULTIPLE_VALUES;
				}
				else
				{
					ZenStringInterface* obj = getFirstObject();
					if(obj)
						s = obj->getParameter<String>(getParamDef()->getName());
				}
				mEditValue->setCaption(s);
			}
		};



		//----------------------------------------------------------------------------
		// StringButtonView
		//----------------------------------------------------------------------------
		class StringButtonView : public EditButtonView
		{
		public:
			void updateValue()
			{
				String s;
				if(hasMultipleValues())
				{
					s = Util::MULTIPLE_VALUES;
				}
				else
				{
					ZenStringInterface* obj = getFirstObject();
					if(obj)
						s = obj->getParameter<String>(getParamDef()->getName());
				}
				mEditValue->setCaption(s);
			}
		};



		//----------------------------------------------------------------------------
		// IntView
		//----------------------------------------------------------------------------
		class IntView : public EditView
		{
			void updateValue()
			{
				String s;
				if(hasMultipleValues())
				{
					s = Util::MULTIPLE_VALUES;
				}
				else
				{
					ZenStringInterface* obj = getFirstObject();
					if(obj)
					{
						int val = obj->getParameter<int>(getParamDef()->getName());
						s = StrConv::toString(val);
					}
				}
				mEditValue->setCaption(s);
			}
		};



		//----------------------------------------------------------------------------
		// RealView
		//----------------------------------------------------------------------------
		class RealView : public EditView
		{
			void updateValue()
			{
				String s;
				if(hasMultipleValues())
				{
					s = Util::MULTIPLE_VALUES;
				}
				else
				{
					ZenStringInterface* obj = getFirstObject();
					if(obj)
					{
						Real val = obj->getParameter<Real>(getParamDef()->getName());
						s = StrConv::toString(val);
					}
				}
				mEditValue->setCaption(s);
			}
		};



		//----------------------------------------------------------------------------
		// BoolView
		//----------------------------------------------------------------------------
		class BoolView : public ComboView
		{
		public:
			void createWidgets(MyGUI::Widget* _parent)
			{
				ComboView::createWidgets(_parent);
				mComboValue->addItem("FALSE");
				mComboValue->addItem("TRUE");
			}
				
			void updateValue()
			{
				if(hasMultipleValues())
				{
					mComboValue->setCaption(Util::MULTIPLE_VALUES);
				}
				else
				{
					ZenStringInterface* obj = getFirstObject();
					if(obj)
					{
						bool val = obj->getParameter<bool>(getParamDef()->getName());
						mComboValue->setIndexSelected(val ? 1 : 0);
					}
					else
					{
						mComboValue->setCaption(StringUtil::BLANK);
					}
				}
			}
		};




		//----------------------------------------------------------------------------
		// EnumView
		//----------------------------------------------------------------------------
		class EnumView : public ComboView
		{
		public:
			void createWidgets(MyGUI::Widget* _parent)
			{
				ComboView::createWidgets(_parent);
				
				ZenParamEnum* paramDef = (ZenParamEnum*) getParamDef();
				const ZenParamEnum::ValueList& allValues = paramDef->getAllValues();
				const ZenParamEnum::ValueNameMap& valueNameMap = paramDef->getValueNameMap();
				for(size_t i = 0; i != allValues.size(); ++i)
				{
					int val = allValues[i];
					StrStream ss;
					EnumUtil::output(ss, val, valueNameMap);
					const String& str = ss.str();
					mComboValue->addItem(str, (MyGUI::Any) val);
				}
			}
				
			void updateValue()
			{
				if(hasMultipleValues())
				{
					mComboValue->setCaption(Util::MULTIPLE_VALUES);
				}
				else
				{
					ZenStringInterface* obj = getFirstObject();
					if(obj)
					{
						ZenParamEnum* paramDef = (ZenParamEnum*) getParamDef();
						int val = obj->getParameter<int>(paramDef->getName());
						const ZenParamEnum::ValueNameMap& valueNameMap = paramDef->getValueNameMap();
						StrStream ss;
						EnumUtil::output(ss, val, valueNameMap);
						const String& str = ss.str();
						size_t index = mComboValue->findItemIndexWith(str);
						if(index != MyGUI::ITEM_NONE)
							mComboValue->setIndexSelected(index);
						else
							mComboValue->setCaption(str);
					}
					else
					{
						mComboValue->setCaption(StringUtil::BLANK);
					}
				}
			}
		};



		//----------------------------------------------------------------------------
		// Vector3View
		//----------------------------------------------------------------------------
		class Vector3View : public EditButtonView
		{
		public:
			void updateValue()
			{
				String s;
				if(hasMultipleValues())
				{
					s = Util::MULTIPLE_VALUES;
				}
				else
				{
					ZenStringInterface* obj = getFirstObject();
					if(obj)
					{
						Vector3 val = obj->getParameter<Vector3>(getParamDef()->getName());
						s = StrConv::toString(val);
					}
				}
				mEditValue->setCaption(s);
			}
		};



		//----------------------------------------------------------------------------
		// AxisAlignedBoxView
		//----------------------------------------------------------------------------
		class AxisAlignedBoxView : public EditButtonView
		{
		public:
			void updateValue()
			{
				String s;
				if(hasMultipleValues())
				{
					s = Util::MULTIPLE_VALUES;
				}
				else
				{
					ZenStringInterface* obj = getFirstObject();
					if(obj)
					{
						AxisAlignedBox val = obj->getParameter<AxisAlignedBox>(getParamDef()->getName());
						s = StrConv::toString(val);
					}
				}
				mEditValue->setCaption(s);
			}
		};



		//----------------------------------------------------------------------------
		// ColourView
		//----------------------------------------------------------------------------
		class ColourView : public EditButtonView
		{
		public:
			void updateValue()
			{
				String s;
				if(hasMultipleValues())
				{
					s = Util::MULTIPLE_VALUES;
				}
				else
				{
					ZenStringInterface* obj = getFirstObject();
					if(obj)
					{
						ColourValue val = obj->getParameter<ColourValue>(getParamDef()->getName());
						s = StrConv::toString(val);
					}
				}
				mEditValue->setCaption(s);
			}
		};



		//----------------------------------------------------------------------------
		// QuaternionView
		//----------------------------------------------------------------------------
		class QuaternionView : public EditButtonView
		{
		public:
			void updateValue()
			{
				String s;
				if(hasMultipleValues())
				{
					s = Util::MULTIPLE_VALUES;
				}
				else
				{
					ZenStringInterface* obj = getFirstObject();
					if(obj)
					{
						Quaternion val = obj->getParameter<Quaternion>(getParamDef()->getName());
						s = StrConv::toString(val);
					}
				}
				mEditValue->setCaption(s);
			}
		};



		//----------------------------------------------------------------------------
		// TimeView
		//----------------------------------------------------------------------------
		class TimeView : public EditButtonView
		{
		public:
			void updateValue()
			{
				String s;
				if(hasMultipleValues())
				{
					s = Util::MULTIPLE_VALUES;
				}
				else
				{
					ZenStringInterface* obj = getFirstObject();
					if(obj)
					{
						Time val = obj->getParameter<Time>(getParamDef()->getName());
						s = StrConv::toString(val);
					}
				}
				mEditValue->setCaption(s);
			}
		};



		//----------------------------------------------------------------------------
		// VisualNameView
		//----------------------------------------------------------------------------
		class VisualNameView : public StringButtonView
		{
		};



		//----------------------------------------------------------------------------
		// SoundNameView
		//----------------------------------------------------------------------------
		class SoundNameView : public StringButtonView
		{
		};
	}
}

#endif // ZEN_OBJECT_PROPERTIES_PARAM_VIEWS_2_H