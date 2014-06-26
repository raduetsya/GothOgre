#ifndef WORLD_OBJECT_TREE_WINDOW_H
#define WORLD_OBJECT_TREE_WINDOW_H

namespace GUISystem
{
	ATTRIBUTE_CLASS_LAYOUT        ( WorldObjectTreeWindow, "WorldObjectTreeWindow.layout");
	ATTRIBUTE_CLASS_CONFIG_SECTION( WorldObjectTreeWindow, "WorldObjectTree");
	
	class WorldObjectTreeWindow : public BaseLayout
	{
	public:
		WorldObjectTreeWindow();
		~WorldObjectTreeWindow();

	private:
		void fillTreeControl();
		void addNode(WorldObject* _worldObject);
		void removeNode(WorldObject* _worldObject);

		WorldObject* getWorldObjectByNode(TreeControl::Node* _node) const;
		TreeControl::Node* getNodeByWorldObject(WorldObject* _worldObject) const;

		GOTHOGRE_DECLARE_WORLD_OBJECT_ADDED_REMOVED_CHANGED_SELECT_CHANGED( WorldObjectTreeWindow );
		GOTHOGRE_DECLARE_WORLD_ACTIVE_CHANGED( WorldObjectTreeWindow );

		void notifyTreeControlNodeSelected(TreeControl* _sender, TreeControl::Node* _node);
		void notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name);

	private:
		ATTRIBUTE_FIELD_WIDGET_NAME(WorldObjectTreeWindow, mTreeControl, "treeControl");
		TreeControl*        mTreeControl;

	private:
		typedef map<WorldObject*, TreeControl::Node*>::type WorldObjectNodeMap;
		WorldObjectNodeMap		mWorldObjectNodeMap;
	};

} // namespace GUISystem

#endif // WORLD_OBJECT_TREE_WINDOW_H