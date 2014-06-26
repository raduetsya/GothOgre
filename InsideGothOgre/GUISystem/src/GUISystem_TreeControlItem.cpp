/*!
	@file
	@author     Pavel Turin
	@date       08/2009
*/

#include "GUISystem_Precompiled.h"
#include "GUISystem_TreeControlItem.h"

namespace GUISystem
{
	TreeControlItem::TreeControlItem() :
		mpButtonExpandCollapse(nullptr),
		mpIcon(nullptr)
	{
	}

	void TreeControlItem::initialiseOverride()
	{
		Base::initialiseOverride();

		assignWidget(mpButtonExpandCollapse, "ButtonExpandCollapse");
		if (mpButtonExpandCollapse != nullptr)
		{
			mpButtonExpandCollapse->eventMouseSetFocus += newDelegate(this, &TreeControlItem::notifyMouseSetFocus);
			mpButtonExpandCollapse->eventMouseLostFocus += newDelegate(this, &TreeControlItem::notifyMouseLostFocus);
			mpButtonExpandCollapse->eventMouseWheel += newDelegate(this, &TreeControlItem::notifyMouseWheel);
		}

		assignWidget(mpIcon, "Image");
		MYGUI_ASSERT(nullptr != mpButtonExpandCollapse, "Child ButtonExpandCollapse not found in skin (TreeControlItem must have ButtonExpandCollapse)");

		// Added by Kerrax
		assignWidget(mpText, "Text");
		MYGUI_ASSERT(nullptr != mpText, "Child Text not found in skin (TreeControlItem must have Text)");

		// Added by Kerrax
		assignWidget(mpContents, "Contents");
		MYGUI_ASSERT(nullptr != mpContents, "Child Contents not found in skin (TreeControlItem must have Contents)");
	}

	void TreeControlItem::shutdownOverride()
	{
		mpButtonExpandCollapse = nullptr;

		Base::shutdownOverride();
	}

	void TreeControlItem::notifyMouseSetFocus(Widget* pSender, Widget* pPreviousWidget)
	{
		if (pSender && pSender->getParent() == this)
			onMouseSetFocus(pPreviousWidget);
	}

	void TreeControlItem::notifyMouseLostFocus(Widget* pSender, Widget* pNextWidget)
	{
		if (pSender && pSender->getParent() == this)
			onMouseLostFocus(pNextWidget);
	}

	void TreeControlItem::notifyMouseWheel(Widget* pSender, int nValue)
	{
		if (pSender && pSender->getParent() == this)
			onMouseWheel(nValue);
	}

	TreeControl::Node* TreeControlItem::getNode() const
	{
		return *(const_cast<TreeControlItem*>(this)->getUserData<TreeControl::Node*>());
	}
}
