#ifndef FAST_CONTAINERS_H
#define FAST_CONTAINERS_H


namespace GothOgre
{
	class BspPart;
	class NonBspPart;

	class ListOfSceneNodes : public std::list<SceneNode*, MemoryPoolAllocator<SceneNode*>>
	{
	public:
		ListOfSceneNodes()
			: std::list<SceneNode*, MemoryPoolAllocator<SceneNode*>>(sAllocator) {}

		static MemoryPoolAllocator<SceneNode*> sAllocator;
	};

	typedef ListOfSceneNodes::iterator IteratorOfSceneNode;
	
	struct PositionInNonBspPart
	{
		PositionInNonBspPart() {nonBspPart = nullptr;}
		NonBspPart*			nonBspPart;
		IteratorOfSceneNode	iterator;
	};

	struct PositionInBspLeaf
	{
		PositionInBspLeaf() {bspLeaf = nullptr;}
		Bsp::Leaf*			bspLeaf;
		IteratorOfSceneNode	iterator;
	};

	class PositionInBspPart : public std::list<PositionInBspLeaf, MemoryPoolAllocator<PositionInBspLeaf>>
	{
	public:
		PositionInBspPart()
			: std::list<PositionInBspLeaf, MemoryPoolAllocator<PositionInBspLeaf>>(sAllocator) {}

		static MemoryPoolAllocator<PositionInBspLeaf> sAllocator;
	};

} // namespace GothOgre

#endif // FAST_CONTAINERS_H