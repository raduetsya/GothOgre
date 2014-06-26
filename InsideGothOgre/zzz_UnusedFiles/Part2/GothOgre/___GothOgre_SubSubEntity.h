#ifndef GOTHOGRE_SUBSUBENTITY_H
#define GOTHOGRE_SUBSUBENTITY_H

namespace GothOgre
{
	/** Part of subentity.
	A single subentity can contain many such parts.
	These subsubentities are not used in the rendering process,
	however they can be used in the raycasting algrorithm. */
	struct SubSubEntity
	{
		SubEntity*      parent;
		AxisAlignedBox  bounds;
		size_t          indexStart;
		size_t          indexCount;
	};

	typedef vector<SubSubEntity>::type             SubSubEntityList;
	typedef ConstVectorIterator<SubSubEntityList>  SubSubEntityIterator;
};

#endif // GOTHOGRE_SUBSUBENTITY_H