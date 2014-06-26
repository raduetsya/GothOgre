#ifndef NEW_SCENE_NODE_H
#define NEW_SCENE_NODE_H


namespace GothOgre
{
	class NewSceneNode;
	typedef BoundingSphereTree<NewSceneNode*> SceneNodeBST;


	class NewSceneNode : public SceneNode
	{
	public:
        NewSceneNode(SceneManager* _creator);
        NewSceneNode(SceneManager* _creator, const String& _name);
		~NewSceneNode();

		void _updateBounds();
		void setParent(Node* _parent);
		void removeFromBST();
		void checkNoObjects();
		void checkNoChildren();

	private:
		SceneNodeBST::ElementHandle  mBstElemHandle;
	};

} // namespace GothOgre

#endif // NEW_SCENE_NODE_H