/*!
	@file
	@author     Pavel Turin
	@date       08/2009
*/
#ifndef GUISYSTEM_TREE_CONTROL_ITEM_H
#define GUISYSTEM_TREE_CONTROL_ITEM_H

#include "GUISystem_TreeControl.h"


namespace GUISystem
{

	class GUISYSTEM_EXPORT TreeControlItem :
		public MyGUI::Button
	{
		MYGUI_RTTI_DERIVED( TreeControlItem )

	public:
		TreeControlItem();

		TreeControl::Node* getNode() const;
		MyGUI::Button* getButtonExpandCollapse() const
		{
			return mpButtonExpandCollapse;
		}
		MyGUI::ImageBox* getIcon() const
		{
			return mpIcon;
		}

		// Added by Kerrax
		void setCaption(const MyGUI::UString& _caption)
		{
			mpText->setCaption(_caption);
		}

		// Added by Kerrax
		void setPosition(const MyGUI::IntPoint& _point)
		{
			mpContents->setPosition(MyGUI::IntPoint(_point.left, 0));
			MyGUI::Button::setPosition(MyGUI::IntPoint(0, _point.top));
		}

		
	protected:
		virtual void initialiseOverride();
		virtual void shutdownOverride();

		void notifyMouseSetFocus(Widget* pSender, Widget* pPreviousWidget);
		void notifyMouseLostFocus(Widget* pSender, Widget* pNextWidget);
		void notifyMouseWheel(Widget* pSender, int nValue);

	private:
		MyGUI::Button* mpButtonExpandCollapse;
		MyGUI::ImageBox* mpIcon;
		MyGUI::TextBox*  mpText; // Added by Kerrax
		MyGUI::Widget*   mpContents; // Added by Kerrax
		//int mnLevelOffset;
	};

}

#endif // GUISYSTEM_TREE_CONTROL_ITEM_H
