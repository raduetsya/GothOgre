#include "Precompiled.h"
#include "ZenObjectPropertiesParamView.h"
#include "ZenObjectPropertiesGroup.h"


namespace GUISystem
{
	namespace ZenObjectProperties
	{

		ParamView::ParamView()
		{
			mParamDef = nullptr;
		}
		//----------------------------------------------------------------------------
		void ParamView::baseinit(ZenParamDef* _paramDef, Group* _group)
		{
			mParamDef = _paramDef;
			mGroup = _group;
			setLayoutName(_group->getLayoutName());
		}
		//----------------------------------------------------------------------------
		ParamView::~ParamView()
		{
		}
		//----------------------------------------------------------------------------
		const MyGUI::IntCoord& ParamView::getCoord() const
		{
			return getMainWidget()->getCoord();
		}
		//----------------------------------------------------------------------------
		void ParamView::setCoord(const MyGUI::IntCoord& _coord)
		{
			getMainWidget()->setCoord(_coord);
		}
		//----------------------------------------------------------------------------
		Window* ParamView::getWindow() const 
		{
			return mGroup->getWindow();
		}
		//----------------------------------------------------------------------------
		ParamView::ObjectIterator ParamView::getObjectIterator() const
		{
			return getWindow()->getObjectIterator();
		}
		//----------------------------------------------------------------------------
		ZenStringInterface* ParamView::getFirstObject() const 
		{
			return getGroup()->getFirstObject();
		}

	}
}