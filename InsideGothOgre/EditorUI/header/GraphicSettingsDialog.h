#ifndef GRAPHIC_SETTINGS_DIALOG_H
#define GRAPHIC_SETTINGS_DIALOG_H

namespace GUISystem
{
	ATTRIBUTE_CLASS_LAYOUT        ( GraphicSettingsDialog, "GraphicSettingsDialog.layout");
	ATTRIBUTE_CLASS_CONFIG_SECTION( GraphicSettingsDialog, "GraphicSettingsDialog");

	class GraphicSettingsDialog : public BaseLayout
	{
	public:
		GraphicSettingsDialog();
		~GraphicSettingsDialog();

		// event for pressed "close" button
		typedef MyGUI::delegates::CDelegate0 EventHandle_Result;
		EventHandle_Result eventEndDialog;

	private:
		// Common part
		void storeOrigValues();
		void restoreOrigValues();
		void initialise();
		void update();
		void apply();
		void requestApply();

		void notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name);
		void notifyWindowChangeCoord(MyGUI::Window* _sender);
		void notifyButtonOK(MyGUI::Widget* _sender);
		void notifyButtonCancel(MyGUI::Widget* _sender);

	private:
		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mButtonOK, "buttonOK");
		MyGUI::Button*         mButtonOK;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mButtonCancel, "buttonCancel");
		MyGUI::Button*         mButtonCancel;

	private:
		bool                   mNeedApply;

	private:
		// Page: "Rendering distances"
		void storeOrigValues_RD();
		void restoreOrigValues_RD();
		void initialise_RD();
		void update_RD();
		void apply_RD();

		void notifyWindowChangeCoord_RD(MyGUI::Window* _sender);
		void notifyAutoUpdateOtherDistances(MyGUI::Widget* _sender);
		void notifyFogEnabled(MyGUI::Widget* _sender);
		void notifyDistanceChanged(MyGUI::VScroll* _sender, size_t _position);
		void notifyVobSizeChanged(MyGUI::EditBox* _sender);

		void updateOtherDistances();
		void setDistance(MyGUI::VScroll* _scroll, Real _distance);
		Real getDistance(MyGUI::VScroll* _scroll) const;
		void setVobSize(MyGUI::TextBox* _staticText, Real _vobSize);
		Real getVobSize(MyGUI::TextBox* _staticText) const;
		MyGUI::TextBox* getTextCtrlBySlider(MyGUI::VScroll* _scroll) const;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mScrollView_RD, "scrollViewRenderingDistances");
		MyGUI::ScrollView*     mScrollView_RD;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mCheckAutoUpdateOtherDistances, "checkAutoUpdateOtherDistances");
		MyGUI::Button*         mCheckAutoUpdateOtherDistances;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mCheckFogEnabled, "checkFogEnabled");
		MyGUI::Button*         mCheckFogEnabled;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mSliderFarClipDistance, "sliderFarClipDistance");
		MyGUI::HScroll*        mSliderFarClipDistance;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mSliderFogNearDistance, "sliderFogNearDistance");
		MyGUI::HScroll*        mSliderFogNearDistance;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mSliderFogFarDistance, "sliderFogFarDistance");
		MyGUI::HScroll*        mSliderFogFarDistance;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mSliderTinyVobDistance, "sliderTinyVobDistance");
		MyGUI::HScroll*        mSliderTinyVobDistance;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mSliderSmallVobDistance, "sliderSmallVobDistance");
		MyGUI::HScroll*        mSliderSmallVobDistance;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mSliderNormalVobDistance, "sliderNormalVobDistance");
		MyGUI::HScroll*        mSliderNormalVobDistance;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mSliderLargeVobDistance, "sliderLargeVobDistance");
		MyGUI::HScroll*        mSliderLargeVobDistance;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mSliderExtraLargeVobDistance, "sliderExtraLargeVobDistance");
		MyGUI::HScroll*        mSliderExtraLargeVobDistance;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mSliderHugeVobDistance, "sliderHugeVobDistance");
		MyGUI::HScroll*        mSliderHugeVobDistance;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mTextFarClipDistance, "textFarClipDistance");
		MyGUI::TextBox*     mTextFarClipDistance;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mTextFogNearDistance, "textFogNearDistance");
		MyGUI::TextBox*     mTextFogNearDistance;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mTextFogFarDistance, "textFogFarDistance");
		MyGUI::TextBox*     mTextFogFarDistance;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mTextTinyVobDistance, "textTinyVobDistance");
		MyGUI::TextBox*     mTextTinyVobDistance;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mTextSmallVobDistance, "textSmallVobDistance");
		MyGUI::TextBox*     mTextSmallVobDistance;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mTextNormalVobDistance, "textNormalVobDistance");
		MyGUI::TextBox*     mTextNormalVobDistance;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mTextLargeVobDistance, "textLargeVobDistance");
		MyGUI::TextBox*     mTextLargeVobDistance;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mTextExtraLargeVobDistance, "textExtraLargeVobDistance");
		MyGUI::TextBox*     mTextExtraLargeVobDistance;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mTextHugeVobDistance, "textHugeVobDistance");
		MyGUI::TextBox*     mTextHugeVobDistance;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mEditTinyVobSize, "editTinyVobSize");
		MyGUI::EditBox*           mEditTinyVobSize;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mEditSmallVobSize, "editSmallVobSize");
		MyGUI::EditBox*           mEditSmallVobSize;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mEditNormalVobSize, "editNormalVobSize");
		MyGUI::EditBox*           mEditNormalVobSize;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mEditLargeVobSize, "editLargeVobSize");
		MyGUI::EditBox*           mEditLargeVobSize;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mEditExtraLargeVobSize, "editExtraLargeVobSize");
		MyGUI::EditBox*           mEditExtraLargeVobSize;

		ATTRIBUTE_FIELD_WIDGET_NAME(GraphicSettingsDialog, mEditHugeVobSize, "editHugeVobSize");
		MyGUI::EditBox*           mEditHugeVobSize;

	private:
		Real   mOrigFarClipDistance;
		bool   mOrigAutoUpdateOtherDistances;
		bool   mOrigFogEnabled;
		Real   mOrigFogNearDistance;
		Real   mOrigFogFarDistance;
		Real   mOrigTinyVobDistance;
		Real   mOrigSmallVobDistance;
		Real   mOrigNormalVobDistance;
		Real   mOrigLargeVobDistance;
		Real   mOrigExtraLargeVobDistance;
		Real   mOrigHugeVobDistance;
		Real   mOrigTinyVobSize;
		Real   mOrigSmallVobSize;
		Real   mOrigNormalVobSize;
		Real   mOrigLargeVobSize;
		Real   mOrigExtraLargeVobSize;
		Real   mOrigHugeVobSize;
	};

} // namespace GUISystem

#endif // GRAPHIC_SETTINGS_DIALOG_H