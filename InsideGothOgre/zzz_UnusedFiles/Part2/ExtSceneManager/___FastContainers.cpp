#include "Precompiled.h"
#include "FastContainers.h"

namespace GothOgre
{
	MemoryPoolAllocator<SceneNode*> ListOfSceneNodes::sAllocator(16384);
	MemoryPoolAllocator<PositionInBspLeaf> PositionInBspPart::sAllocator(16384);

} // namespace GothOgre