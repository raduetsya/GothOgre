#include "Precompiled.h"
#include "GraphicSettingsDialog.h"

namespace GUISystem
{
	//--------------------------------------------------------------------------
	// Common part
	//--------------------------------------------------------------------------
	GraphicSettingsDialog::GraphicSettingsDialog()
	{
		initialiseByAttributes(this);
		initialise();
		storeOrigValues();
		update();
		mNeedApply = false;
	}
	//--------------------------------------------------------------------------
	GraphicSettingsDialog::~GraphicSettingsDialog()
	{
	}
	//----------------------------------------------------------------------------
	void GraphicSettingsDialog::storeOrigValues()
	{
		storeOrigValues_RD();
	}
	//----------------------------------------------------------------------------
	void GraphicSettingsDialog::restoreOrigValues()
	{
		GraphicSettings& gs = GraphicSettings::getSingleton();
		gs.beginBatch();
		restoreOrigValues_RD();
		gs.endBatch();
	}
	//----------------------------------------------------------------------------
	void GraphicSettingsDialog::initialise()
	{
		initialise_RD();

		setModal(true);
		mButtonOK->eventMouseButtonClick = MyGUI::newDelegate(this, &GraphicSettingsDialog::notifyButtonOK);
		mButtonCancel->eventMouseButtonClick = MyGUI::newDelegate(this, &GraphicSettingsDialog::notifyButtonOK);
	}
	//----------------------------------------------------------------------------
	void GraphicSettingsDialog::update()
	{
		update_RD();
	}
	//----------------------------------------------------------------------------
	void GraphicSettingsDialog::requestApply()
	{
		mNeedApply = true;
		
		// The following line can be commented to
		// apply changed only when the dialog is closed.
		apply();
	}
	//----------------------------------------------------------------------------
	void GraphicSettingsDialog::apply()
	{
		if(!mNeedApply)
			return;
		mNeedApply = false;

		apply_RD();		
	}
	//--------------------------------------------------------------------------
	void GraphicSettingsDialog::notifyWindowChangeCoord(MyGUI::Window* _sender)
	{
		notifyWindowChangeCoord_RD(_sender);
	}
	//----------------------------------------------------------------------------
	void GraphicSettingsDialog::notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name)
	{
		if (_name == "close")
		{
			apply();
			notifyButtonOK(nullptr);
		}
	}
	//----------------------------------------------------------------------------
	void GraphicSettingsDialog::notifyButtonOK(MyGUI::Widget* _sender)
	{
		eventEndDialog();
	}
	//----------------------------------------------------------------------------
	void GraphicSettingsDialog::notifyButtonCancel(MyGUI::Widget* _sender)
	{
		restoreOrigValues();
		eventEndDialog();
	}













	//--------------------------------------------------------------------------
	// Page: Rendering distances
	//--------------------------------------------------------------------------
	static const Real MIN_DISTANCE = 0;
	static const Real MAX_DISTANCE = 50000;
	static const Real DISTANCE_STEP = 500;
	static const Real INVALID_VALUE = -123;
	//--------------------------------------------------------------------------
	static String distance_to_string(Real _distance)
	{
		String text;
		if(_distance == Math::POS_INFINITY)
		{
			text = "infinity";
		}
		else
		{
			text = StrConv::toString(_distance);
		}
		return text;
	}
	//--------------------------------------------------------------------------
	static Real string_to_distance(const String& _text)
	{
		Real distance;
		if(_text == "infinity")
		{
			distance = Math::POS_INFINITY;
		}
		else
		{
			bool ok;
			distance = StrConv::fromString<Real>(_text, &ok);
			if(!ok)
				distance = INVALID_VALUE;
		}
		return distance;
	}
	//--------------------------------------------------------------------------
	static String vobSize_to_string(Real _maxSize)
	{
		return distance_to_string(_maxSize);
	}
	//--------------------------------------------------------------------------
	static Real string_to_vobSize(const String& _text)
	{
		return string_to_distance(_text);
	}
	//--------------------------------------------------------------------------
	void GraphicSettingsDialog::initialise_RD()
	{
		mCheckAutoUpdateOtherDistances->eventMouseButtonClick = MyGUI::newDelegate(this, &GraphicSettingsDialog::notifyAutoUpdateOtherDistances);
		mCheckFogEnabled->eventMouseButtonClick = MyGUI::newDelegate(this, &GraphicSettingsDialog::notifyFogEnabled);
		
		Real fRange = (MAX_DISTANCE - MIN_DISTANCE) / DISTANCE_STEP + 1;
		fRange += 1; // +infinity value
		size_t nRange = (size_t) floor(fRange + 0.5);

		MyGUI::VScroll* sliders[] = {mSliderFarClipDistance,
			mSliderFogNearDistance, mSliderFogFarDistance, mSliderTinyVobDistance,
			mSliderSmallVobDistance, mSliderNormalVobDistance,
			mSliderLargeVobDistance, mSliderExtraLargeVobDistance,
			mSliderHugeVobDistance};

		MyGUI::EditBox* editCtrls[] = {mEditTinyVobSize, mEditSmallVobSize,
			mEditNormalVobSize, mEditLargeVobSize, mEditExtraLargeVobSize,
			mEditHugeVobSize};

		size_t i;
		for(i = 0; i != sizeof(sliders) / sizeof(sliders[0]); ++i)
		{
			MyGUI::VScroll* slider = sliders[i];
			slider->setScrollRange(nRange);
			slider->eventScrollChangePosition = MyGUI::newDelegate(this, &GraphicSettingsDialog::notifyDistanceChanged);
		}

		for(i = 0; i != sizeof(editCtrls) / sizeof(editCtrls[0]); ++i)
		{
			MyGUI::EditBox* editCtrl = editCtrls[i];
			editCtrl->eventEditTextChange = MyGUI::newDelegate(this, &GraphicSettingsDialog::notifyVobSizeChanged);
		}
	}
	//----------------------------------------------------------------------------
	void GraphicSettingsDialog::storeOrigValues_RD()
	{
		GraphicSettings& gs = GraphicSettings::getSingleton();
		
		mOrigFarClipDistance          = gs.getSetting<Real>("FarClipDistance",          "Camera", 20000);
		mOrigAutoUpdateOtherDistances = gs.getSetting<bool>("AutoUpdateOtherDistances", "Camera", true);

		mOrigFogEnabled      = gs.getSetting<bool>("Enabled",      "Fog", true);
		mOrigFogNearDistance = gs.getSetting<Real>("NearDistance", "Fog", 12000);
		mOrigFogFarDistance  = gs.getSetting<Real>("FarDistance",  "Fog", 20000);

		mOrigTinyVobDistance       = gs.getSetting<Real>("RenderingDistance", "TinyVob",        1500);
		mOrigSmallVobDistance      = gs.getSetting<Real>("RenderingDistance", "SmallVob",       3000);
		mOrigNormalVobDistance     = gs.getSetting<Real>("RenderingDistance", "NormalVob",      5000);
		mOrigLargeVobDistance      = gs.getSetting<Real>("RenderingDistance", "LargeVob",       8000);
		mOrigExtraLargeVobDistance = gs.getSetting<Real>("RenderingDistance", "ExtraLargeVob", 11000);
		mOrigHugeVobDistance       = gs.getSetting<Real>("RenderingDistance", "HugeVob",       14000);
	
		mOrigTinyVobSize       = gs.getSetting<Real>("MaxSize", "TinyVob",         10);
		mOrigSmallVobSize      = gs.getSetting<Real>("MaxSize", "SmallVob",        30);
		mOrigNormalVobSize     = gs.getSetting<Real>("MaxSize", "NormalVob",      100);
		mOrigLargeVobSize      = gs.getSetting<Real>("MaxSize", "LargeVob",       300);
		mOrigExtraLargeVobSize = gs.getSetting<Real>("MaxSize", "ExtraLargeVob", 1000);
		mOrigHugeVobSize       = gs.getSetting<Real>("MaxSize", "HugeVob",       Math::POS_INFINITY);
	}
	//----------------------------------------------------------------------------
	void GraphicSettingsDialog::restoreOrigValues_RD()
	{
		GraphicSettings& gs = GraphicSettings::getSingleton();
		gs.beginBatch();
		
		gs.setSetting<Real>("FarClipDistance",          "Camera", mOrigFarClipDistance);
		gs.setSetting<bool>("AutoUpdateOtherDistances", "Camera", mOrigAutoUpdateOtherDistances);

		gs.setSetting<bool>("Enabled",      "Fog", mOrigFogEnabled);
		gs.setSetting<Real>("NearDistance", "Fog", mOrigFogNearDistance);
		gs.setSetting<Real>("FarDistance",  "Fog", mOrigFogFarDistance);

		gs.setSetting<Real>("RenderingDistance", "TinyVob",       mOrigTinyVobDistance);
		gs.setSetting<Real>("RenderingDistance", "SmallVob",      mOrigSmallVobDistance);
		gs.setSetting<Real>("RenderingDistance", "NormalVob",     mOrigNormalVobDistance);
		gs.setSetting<Real>("RenderingDistance", "LargeVob",      mOrigLargeVobDistance);
		gs.setSetting<Real>("RenderingDistance", "ExtraLargeVob", mOrigExtraLargeVobDistance);
		gs.setSetting<Real>("RenderingDistance", "HugeVob",       mOrigHugeVobDistance);

		gs.setSetting<Real>("MaxSize", "TinyVob",       mOrigTinyVobSize);
		gs.setSetting<Real>("MaxSize", "SmallVob",      mOrigSmallVobSize);
		gs.setSetting<Real>("MaxSize", "NormalVob",     mOrigNormalVobSize);
		gs.setSetting<Real>("MaxSize", "LargeVob",      mOrigLargeVobSize);
		gs.setSetting<Real>("MaxSize", "ExtraLargeVob", mOrigExtraLargeVobSize);
		gs.setSetting<Real>("MaxSize", "HugeVob",       mOrigHugeVobSize);

		gs.endBatch();
	}
	//----------------------------------------------------------------------------
	void GraphicSettingsDialog::update_RD()
	{
		GraphicSettings& gs = GraphicSettings::getSingleton();
		setDistance(mSliderFarClipDistance, gs.getSetting<Real>("FarClipDistance", "Camera"));
		mCheckAutoUpdateOtherDistances->setStateCheck(gs.getSetting<bool>("AutoUpdateOtherDistances", "Camera"));

		mCheckFogEnabled->setStateCheck(gs.getSetting<bool>("Enabled", "Fog"));
		setDistance(mSliderFogNearDistance, gs.getSetting<Real>("NearDistance", "Fog"));
		setDistance(mSliderFogFarDistance,  gs.getSetting<Real>("FarDistance",  "Fog"));

		setDistance(mSliderTinyVobDistance,       gs.getSetting<Real>("RenderingDistance", "TinyVob"));
		setDistance(mSliderSmallVobDistance,      gs.getSetting<Real>("RenderingDistance", "SmallVob"));
		setDistance(mSliderNormalVobDistance,     gs.getSetting<Real>("RenderingDistance", "NormalVob"));
		setDistance(mSliderLargeVobDistance,      gs.getSetting<Real>("RenderingDistance", "LargeVob"));
		setDistance(mSliderExtraLargeVobDistance, gs.getSetting<Real>("RenderingDistance", "ExtraLargeVob"));
		setDistance(mSliderHugeVobDistance,       gs.getSetting<Real>("RenderingDistance", "HugeVob"));

		setVobSize(mEditTinyVobSize,       gs.getSetting<Real>("MaxSize", "TinyVob"));
		setVobSize(mEditSmallVobSize,      gs.getSetting<Real>("MaxSize", "SmallVob"));
		setVobSize(mEditNormalVobSize,     gs.getSetting<Real>("MaxSize", "NormalVob"));
		setVobSize(mEditLargeVobSize,      gs.getSetting<Real>("MaxSize", "LargeVob"));
		setVobSize(mEditExtraLargeVobSize, gs.getSetting<Real>("MaxSize", "ExtraLargeVob"));
		setVobSize(mEditHugeVobSize,       gs.getSetting<Real>("MaxSize", "HugeVob"));
	}
	//--------------------------------------------------------------------------
	void GraphicSettingsDialog::apply_RD()
	{
		GraphicSettings& gs = GraphicSettings::getSingleton();
		gs.setSetting("FarClipDistance",          "Camera", getDistance(mSliderFarClipDistance));
		gs.setSetting("AutoUpdateOtherDistances", "Camera", mCheckAutoUpdateOtherDistances->getStateCheck());

		gs.setSetting("Enabled",      "Fog", mCheckFogEnabled->getStateCheck());
		gs.setSetting("NearDistance", "Fog", getDistance(mSliderFogNearDistance));
		gs.setSetting("FarDistance",  "Fog", getDistance(mSliderFogFarDistance));

		gs.setSetting("RenderingDistance", "TinyVob",       getDistance(mSliderTinyVobDistance));
		gs.setSetting("RenderingDistance", "SmallVob",      getDistance(mSliderSmallVobDistance));
		gs.setSetting("RenderingDistance", "NormalVob",     getDistance(mSliderNormalVobDistance));
		gs.setSetting("RenderingDistance", "LargeVob",      getDistance(mSliderLargeVobDistance));
		gs.setSetting("RenderingDistance", "ExtraLargeVob", getDistance(mSliderExtraLargeVobDistance));
		gs.setSetting("RenderingDistance", "HugeVob",       getDistance(mSliderHugeVobDistance));

		Real maxSize;
		maxSize = getVobSize(mEditTinyVobSize);
		if(maxSize != INVALID_VALUE)
			gs.setSetting("MaxSize", "TinyVob", maxSize);

		maxSize = getVobSize(mEditSmallVobSize);
		if(maxSize != INVALID_VALUE)
			gs.setSetting("MaxSize", "SmallVob", maxSize);

		maxSize = getVobSize(mEditNormalVobSize);
		if(maxSize != INVALID_VALUE)
			gs.setSetting("MaxSize", "NormalVob", maxSize);

		maxSize = getVobSize(mEditLargeVobSize);
		if(maxSize != INVALID_VALUE)
			gs.setSetting("MaxSize", "LargeVob", maxSize);

		maxSize = getVobSize(mEditExtraLargeVobSize);
		if(maxSize != INVALID_VALUE)
			gs.setSetting("MaxSize", "ExtraLargeVob", maxSize);
	}
	//--------------------------------------------------------------------------
	void GraphicSettingsDialog::notifyDistanceChanged(MyGUI::VScroll* _sender, size_t _position)
	{
		Real distance;
		size_t scrollPosition = _sender->getScrollPosition();
		if(scrollPosition == _sender->getScrollRange() - 1)
			distance = Math::POS_INFINITY;
		else
			distance = Real(scrollPosition) * DISTANCE_STEP + MIN_DISTANCE;

		getTextCtrlBySlider(_sender)->setCaption(distance_to_string(distance));

		if(_sender == mSliderFarClipDistance && mCheckAutoUpdateOtherDistances->getStateCheck())
			updateOtherDistances();

		requestApply();
	}
	//--------------------------------------------------------------------------
	void GraphicSettingsDialog::notifyVobSizeChanged(MyGUI::EditBox* _sender)
	{
		requestApply();
	}
	//--------------------------------------------------------------------------
	void GraphicSettingsDialog::notifyAutoUpdateOtherDistances(MyGUI::Widget* _sender)
	{
		bool enabled = !mCheckAutoUpdateOtherDistances->getStateCheck();
		mCheckAutoUpdateOtherDistances->setStateCheck(enabled);
		if(enabled)
			updateOtherDistances();

		requestApply();
	}
	//--------------------------------------------------------------------------
	void GraphicSettingsDialog::notifyFogEnabled(MyGUI::Widget* _sender)
	{
		bool enabled = !mCheckFogEnabled->getStateCheck();
		mCheckFogEnabled->setStateCheck(enabled);
		requestApply();
	}
	//--------------------------------------------------------------------------
	void GraphicSettingsDialog::notifyWindowChangeCoord_RD(MyGUI::Window* _sender)
	{
		// fix canvas size
		MyGUI::IntSize canvasSize = mScrollView_RD->getCanvasSize();
		MyGUI::IntSize viewSize = mScrollView_RD->getViewCoord().size();//mScrollView_RD->getClientCoord().size();
		canvasSize.width = viewSize.width;
		mScrollView_RD->setCanvasSize(canvasSize);
	}
	//----------------------------------------------------------------------------
	void GraphicSettingsDialog::setDistance(MyGUI::VScroll* _scroll, Real _distance)
	{
		size_t scrollPosition;
		if(_distance == Math::POS_INFINITY)
		{
			scrollPosition = _scroll->getScrollRange() - 1;
		}
		else
		{
			Real f = (_distance - MIN_DISTANCE) / DISTANCE_STEP;
			scrollPosition = (size_t) floor(f + 0.5f);
			scrollPosition = std::min(scrollPosition, _scroll->getScrollRange() - 1);
		}
		_scroll->setScrollPosition(scrollPosition);
		getTextCtrlBySlider(_scroll)->setCaption(distance_to_string(_distance));
	}
	//--------------------------------------------------------------------------
	Real GraphicSettingsDialog::getDistance(MyGUI::VScroll* _scroll) const
	{
		const String& text = getTextCtrlBySlider(_scroll)->getCaption();
		Real distance = string_to_distance(text);
		return distance;
	}
	//--------------------------------------------------------------------------
	MyGUI::TextBox* GraphicSettingsDialog::getTextCtrlBySlider(MyGUI::VScroll* _scroll) const
	{
		MyGUI::TextBox* ret = nullptr;
		if(_scroll == mSliderFarClipDistance)
			ret = mTextFarClipDistance;
		else if(_scroll == mSliderFogNearDistance)
			ret = mTextFogNearDistance;
		else if(_scroll == mSliderFogFarDistance)
			ret = mTextFogFarDistance;
		else if(_scroll == mSliderTinyVobDistance)
			ret = mTextTinyVobDistance;
		else if(_scroll == mSliderSmallVobDistance)
			ret = mTextSmallVobDistance;
		else if(_scroll == mSliderNormalVobDistance)
			ret = mTextNormalVobDistance;
		else if(_scroll == mSliderLargeVobDistance)
			ret = mTextLargeVobDistance;
		else if(_scroll == mSliderExtraLargeVobDistance)
			ret = mTextExtraLargeVobDistance;
		else if(_scroll == mSliderHugeVobDistance)
			ret = mTextHugeVobDistance;
		return ret;
	}
	//--------------------------------------------------------------------------
	void GraphicSettingsDialog::setVobSize(MyGUI::TextBox* _staticText, Real _maxSize)
	{
		_staticText->setCaption(vobSize_to_string(_maxSize));
	}
	//--------------------------------------------------------------------------
	Real GraphicSettingsDialog::getVobSize(MyGUI::TextBox* _staticText) const
	{
		const String& text = _staticText->getCaption();
		Real size = string_to_vobSize(text);
		return size;
	}
	//--------------------------------------------------------------------------
	void GraphicSettingsDialog::updateOtherDistances()
	{
		Real distance = getDistance(mSliderFarClipDistance);
		setDistance(mSliderFogNearDistance,       std::max( distance * 0.8f, MIN_DISTANCE));
		setDistance(mSliderFogFarDistance,        std::max( distance * 1.0f, MIN_DISTANCE));

		setDistance(mSliderTinyVobDistance,       std::max( distance * 0.2f, MIN_DISTANCE));
		setDistance(mSliderSmallVobDistance,      std::max( distance * 0.4f, MIN_DISTANCE));
		setDistance(mSliderNormalVobDistance,     std::max( distance * 0.6f, MIN_DISTANCE));
		setDistance(mSliderLargeVobDistance,      std::max( distance * 0.8f, MIN_DISTANCE));
		setDistance(mSliderExtraLargeVobDistance, std::max( distance * 0.9f, MIN_DISTANCE));
		setDistance(mSliderHugeVobDistance,       std::max( distance * 1.0f, MIN_DISTANCE));
	}

} // namespace GUISystem
