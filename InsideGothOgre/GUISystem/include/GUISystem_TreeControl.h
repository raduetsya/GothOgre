/*!
	@file
	@author     Pavel Turin
	@date       08/2009
*/
#ifndef GUISYSTEM_TREE_CONTROL_H
#define GUISYSTEM_TREE_CONTROL_H

#include "GUISystem_TreeControlGenericNode.h"


namespace GUISystem
{
	class TreeControlItem;

	class GUISYSTEM_EXPORT TreeControl : public MyGUI::Widget
	{
		MYGUI_RTTI_DERIVED( TreeControl )

	public:
		class Node;

		typedef MyGUI::delegates::CMultiDelegate2<TreeControl*, Node*> EventHandle_TreeControlPtrNodePtr;
		typedef MyGUI::delegates::CMultiDelegate2<TreeControl*, size_t> EventHandle_TreeControlPtrSizeT;

		class GUISYSTEM_EXPORT Node :
			public GenericNode<Node, TreeControl>
		{
		public:
			Node();
			Node(TreeControl* pOwner);
			Node(const MyGUI::UString& strText, Node* pParent = nullptr);
			Node(const MyGUI::UString& strText, const MyGUI::UString& strImage, Node* pParent = nullptr);
			virtual ~Node();

			bool isPrepared() const;
			void setPrepared(bool bIsPrepared);
			void prepare();
			size_t prepareChildren();

			bool isExpanded() const;
			void setExpanded(bool bIsExpanded);

			const MyGUI::UString& getImage() const;
			void setImage(const MyGUI::UString& strImage);

			void setData(MyGUI::Any Data);
			template <typename TYPE>
			TYPE* getData() const;

		private:
			bool mbIsPrepared;
			bool mbIsExpanded;
			MyGUI::UString mstrImage;
			MyGUI::Any mData;
		};

		typedef Node::VectorGenericNodePtr VectorNodePtr;

		TreeControl();
		~TreeControl();

		Node* getRoot() const;
		void setRootVisible(bool bValue);
		bool isRootVisible() const;

		Node* getSelection() const;
		void setSelection(Node* pSelection);

		void invalidate();

		virtual void setSize(const MyGUI::IntSize& Size);
		virtual void setCoord(const MyGUI::IntCoord& Bounds);

		EventHandle_TreeControlPtrNodePtr eventTreeNodeMouseSetFocus;
		EventHandle_TreeControlPtrNodePtr eventTreeNodeMouseLostFocus;
		EventHandle_TreeControlPtrNodePtr eventTreeNodeSelected;
		EventHandle_TreeControlPtrNodePtr eventTreeNodeActivated;
		EventHandle_TreeControlPtrNodePtr eventTreeNodeContextMenu;
		EventHandle_TreeControlPtrNodePtr eventTreeNodePrepare;
		EventHandle_TreeControlPtrSizeT eventTreeScrolled;

	protected:
		virtual void initialiseOverride();
		virtual void shutdownOverride();

		void notifyMousePressed(MyGUI::Widget* pSender, int nLeft, int nTop, MyGUI::MouseButton nID);
		void notifyMouseWheel(MyGUI::Widget* pSender, int nValue);
		void notifyMouseDoubleClick(MyGUI::Widget* pSender);
		void notifyMouseSetFocus(MyGUI::Widget* pSender, MyGUI::Widget* pPreviousWidget);
		void notifyMouseLostFocus(MyGUI::Widget* pSender, MyGUI::Widget* pNextWidget);
		void notifyScrollChangePosition(MyGUI::ScrollBar* pSender, size_t nPosition);
		void notifyExpandCollapse(MyGUI::Widget* pSender);
		void notifyFrameEntered(float nTime);

		virtual void onMouseWheel(int nValue);
		virtual void onKeyButtonPressed(MyGUI::KeyCode Key, MyGUI::Char Character);

	private:
		typedef std::vector<TreeControlItem*> VectorTreeItemPtr;

		void validate();

		void updateScroll();
		void updateItems();

		void scrollTo(size_t nPosition);
		void sendScrollingEvents(size_t nPosition);

		MyGUI::ScrollBar* mpWidgetScroll;
		VectorTreeItemPtr mItemWidgets;
		MyGUI::UString mstrSkinLine;
		bool mbScrollAlwaysVisible;
		bool mbInvalidated;
		bool mbRootVisible;
		int mnItemHeight;
		int mnScrollRange;
		int mnTopIndex;
		int mnTopOffset;
		size_t mnFocusIndex;
		Node* mpSelection;
		Node* mpRoot;
		size_t mnExpandedNodes;
		int mnLevelOffset;

		MyGUI::Widget* mClient;
	};


	inline TreeControl::Node::Node() :
		mbIsPrepared(false),
		mbIsExpanded(true)
	{ }
	inline bool TreeControl::Node::isPrepared() const
	{
		return mbIsPrepared;
	}
	inline void TreeControl::Node::setPrepared(bool bIsPrepared)
	{
		mbIsPrepared = bIsPrepared;
	}
	inline bool TreeControl::Node::isExpanded() const
	{
		return mbIsExpanded;
	}
	inline const MyGUI::UString& TreeControl::Node::getImage() const
	{
		return mstrImage;
	}
	inline void TreeControl::Node::setData(MyGUI::Any Data)
	{
		mData = Data;
	}
	template <typename TYPE>
	TYPE* TreeControl::Node::getData() const
	{
		return mData.castType<TYPE>(true);
	}

	inline TreeControl::Node* TreeControl::getRoot() const
	{
		return mpRoot;
	}
	inline bool TreeControl::isRootVisible() const
	{
		return mbRootVisible;
	}
	inline TreeControl::Node* TreeControl::getSelection() const
	{
		return mpSelection;
	}
}


#endif // GUISYSTEM_TREE_CONTROL_H
