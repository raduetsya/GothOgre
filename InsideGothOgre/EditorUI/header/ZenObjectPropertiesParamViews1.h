#ifndef ZEN_OBJECT_PROPERTIES_PARAM_VIEWS_1_H
#define ZEN_OBJECT_PROPERTIES_PARAM_VIEWS_1_H

#include "ZenObjectPropertiesParamView.h"
#include "ZenObjectPropertiesUtil.h"

namespace GUISystem
{
	namespace ZenObjectProperties
	{

		//----------------------------------------------------------------------------
		// EditView
		//----------------------------------------------------------------------------
		ATTRIBUTE_CLASS_MAIN_WIDGET(EditView, "_Edit");
		
		class EditView : public ParamView
		{
		public:
			void createWidgets(MyGUI::Widget* _parent)
			{
				initialiseByAttributes(this, _parent);
				mTextName->setCaption(getParamDef()->getName());
			}

		protected:
			ATTRIBUTE_FIELD_WIDGET_NAME( EditView, mTextName, "_Edit_textName" );
			MyGUI::TextBox*   mTextName;

			ATTRIBUTE_FIELD_WIDGET_NAME( EditView, mEditValue, "_Edit_editValue" );
			MyGUI::EditBox*         mEditValue;
		};



		//----------------------------------------------------------------------------
		// EditButtonView
		//----------------------------------------------------------------------------
		ATTRIBUTE_CLASS_MAIN_WIDGET(EditButtonView, "_EditButton");
		
		class EditButtonView : public ParamView
		{
		public:
			void createWidgets(MyGUI::Widget* _parent)
			{
				initialiseByAttributes(this, _parent);
				mTextName->setCaption(getParamDef()->getName());
			}

		protected:
			ATTRIBUTE_FIELD_WIDGET_NAME( EditButtonView, mTextName, "_EditButton_textName" );
			MyGUI::TextBox*   mTextName;

			ATTRIBUTE_FIELD_WIDGET_NAME( EditButtonView, mEditValue, "_EditButton_editValue" );
			MyGUI::EditBox*         mEditValue;

			ATTRIBUTE_FIELD_WIDGET_NAME( EditButtonView, mButton, "_EditButton_button" );
			MyGUI::Button*       mButton;
		};




		//----------------------------------------------------------------------------
		// ComboView
		//----------------------------------------------------------------------------
		ATTRIBUTE_CLASS_MAIN_WIDGET( ComboView, "_Combo");
		
		class ComboView : public ParamView
		{
		public:
			void createWidgets(MyGUI::Widget* _parent)
			{
				initialiseByAttributes(this, _parent);
				mTextName->setCaption(getParamDef()->getName());
			}

		protected:
			ATTRIBUTE_FIELD_WIDGET_NAME( ComboView, mTextName, "_Combo_textName" );
			MyGUI::TextBox*   mTextName;

			ATTRIBUTE_FIELD_WIDGET_NAME( ComboView, mComboValue, "_Combo_comboValue" );
			MyGUI::ComboBox*     mComboValue;
		};





		//----------------------------------------------------------------------------
		// ThreeEditsView
		//----------------------------------------------------------------------------
		ATTRIBUTE_CLASS_MAIN_WIDGET(ThreeEditsView, "_ThreeEdits");
		
		class ThreeEditsView : public ParamView
		{
		public:
			void createWidgets(MyGUI::Widget* _parent)
			{
				initialiseByAttributes(this, _parent);
				mTextName->setCaption(getParamDef()->getName());

				mMainWidgetInitSize = getMainWidget()->getClientCoord().size();
				mTextNameInitCoord = mTextName->getCoord();
				mText1InitCoord = mText1->getCoord();
				mEdit1InitCoord = mEdit1->getCoord();
				mText2InitCoord = mText2->getCoord();
				mEdit2InitCoord = mEdit2->getCoord();
				mText3InitCoord = mText3->getCoord();
				mEdit3InitCoord = mEdit3->getCoord();
			}

			void setCoord(const MyGUI::IntCoord& _coord)
			{
				ParamView::setCoord(_coord);
				MyGUI::IntSize size = getMainWidget()->getClientCoord().size();
				int dx = size.width - mMainWidgetInitSize.width;
				MyGUI::IntCoord coord;

				mTextName->setCoord(mTextNameInitCoord);
				mText1->setCoord(mText1InitCoord);

				coord = mEdit1InitCoord;
				coord.width += dx / 3;
				mEdit1->setCoord(coord);

				coord = mText2InitCoord;
				coord.left += dx / 3;
				mText2->setCoord(coord);
				
				coord = mEdit2InitCoord;
				coord.left += dx / 3;
				coord.width += dx / 3;
				mEdit2->setCoord(coord);

				coord = mText3InitCoord;
				coord.left += 2 * dx / 3;
				mText3->setCoord(coord);
				
				coord = mEdit3InitCoord;
				coord.left += 2 * dx / 3;
				coord.width += dx - 2 * dx / 3;
				mEdit3->setCoord(coord);
			}

		protected:
			ATTRIBUTE_FIELD_WIDGET_NAME( ThreeEditsView, mTextName, "_ThreeEdits_textName" );
			MyGUI::TextBox*   mTextName;

			ATTRIBUTE_FIELD_WIDGET_NAME( ThreeEditsView, mText1, "_ThreeEdits_text1" );
			MyGUI::TextBox*   mText1;

			ATTRIBUTE_FIELD_WIDGET_NAME( ThreeEditsView, mEdit1, "_ThreeEdits_edit1" );
			MyGUI::EditBox*         mEdit1;

			ATTRIBUTE_FIELD_WIDGET_NAME( ThreeEditsView, mText2, "_ThreeEdits_text2" );
			MyGUI::TextBox*   mText2;

			ATTRIBUTE_FIELD_WIDGET_NAME( ThreeEditsView, mEdit2, "_ThreeEdits_edit2" );
			MyGUI::EditBox*         mEdit2;

			ATTRIBUTE_FIELD_WIDGET_NAME( ThreeEditsView, mText3, "_ThreeEdits_text3" );
			MyGUI::TextBox*   mText3;

			ATTRIBUTE_FIELD_WIDGET_NAME( ThreeEditsView, mEdit3, "_ThreeEdits_edit3" );
			MyGUI::EditBox*         mEdit3;

		private:
			MyGUI::IntSize		mMainWidgetInitSize;
			MyGUI::IntCoord		mTextNameInitCoord;
			MyGUI::IntCoord		mText1InitCoord;
			MyGUI::IntCoord		mEdit1InitCoord;
			MyGUI::IntCoord		mText2InitCoord;
			MyGUI::IntCoord		mEdit2InitCoord;
			MyGUI::IntCoord		mText3InitCoord;
			MyGUI::IntCoord		mEdit3InitCoord;
		};

	}
}

#endif // ZEN_OBJECT_PROPERTIES_PARAM_VIEWS_1_H