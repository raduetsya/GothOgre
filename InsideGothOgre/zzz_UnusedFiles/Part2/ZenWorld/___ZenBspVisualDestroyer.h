#ifndef ZEN_BSP_VISUAL_DESTROYER_H
#define ZEN_BSP_VISUAL_DESTROYER_H


namespace GothOgre
{
	//---------------------------------------------------------------------------
	/** Special class to create scene nodes to display BSP-tree.*/
	class ZenBspVisualDestroyer
	{
	public:
		ZenBspVisualDestroyer(SceneNode* _mainSceneNode);
		void execute();

	private:
		SceneNode*	mMainSceneNode;
	};

} // namespace GothOgre

#endif // ZEN_BSP_VISUAL_DESTROYER_H