#ifndef MESH_AND_BSP_LOAD_CONTEXT_H
#define MESH_AND_BSP_LOAD_CONTEXT_H

#include "MshFileLoadContext.h"

namespace GothOgre
{
	class MeshAndBspLoadContext : public MshFileLoadContext
	{
	public:
		MeshAndBspLoadContext();
		~MeshAndBspLoadContext();

		void load(const DataStreamExPtr& _stream);
		void apply(Resource* _destResource);
		void skip(const DataStreamExPtr& _stream);

	protected:
		#pragma pack(push, 1)

		enum Environment : uint32
		{
			Indoor	= 0,
			Outdoor = 1
		};

		struct BspNodeFlags
		{
			union
			{
				struct
				{
					bool frontExist:1;
					bool backExist:1;
					bool frontIsLeaf:1;
					bool backIsLeaf:1;
				};
				uint8	allbits;
			};
		};

		struct BspNodeBase
		{
			BspNodeBase() {firstPolygonIndex = 0; numPolygonIndices = 0;}

			bool    isLeaf;
			AABB	aabb;              // don't change 
			uint32	firstPolygonIndex; // the order of
			uint32	numPolygonIndices; // these three values
		};

		struct BspLeaf : public BspNodeBase
		{
			BspLeaf() {isLeaf = true;}
		};

		struct BspNode : public BspNodeBase
		{
			BspNode() {isLeaf = false; lod = 0; front = nullptr; back = nullptr;}

			Plane        separationPlane;
			uint8        lod;
			BspNodeBase* front;
			BspNodeBase* back;
		};

		#pragma pack(pop)

		struct Sector
		{
			String name;
			vector<uint32>::type bspLeafIndices;
			vector<uint32>::type polygonIndices;
		};

		struct PolyToFace
		{
			size_t firstFace;
			size_t numFaces;
		};

		struct PolyIndexToFaceIndex
		{
			size_t firstFaceIndex;
			size_t numFaceIndices;
		};

	protected:
		void readMeshAndBspHeader();
		void readBspInfo(Chunk& _chunk);
		void readPolygonIndices(Chunk& _chunk);
		void readBspNodeBase(BspNodeBase* _bspNodeBase);
		void readBspLeaf(BspLeaf* _bspLeaf);
		void readBspNode(BspNode* _bspNode);
		void readBspNodes(Chunk& _chunk);
		void readLights(Chunk& _chunk);
		void readSectors(Chunk& _chunk);
		void loadFromDataStream(const DataStreamExPtr& _dataStream);

		void countPolygonUsesInLod0(BspNodeBase* _nodeBase);
		void countPolygonUsesInLod0();
		void removePolygonsNotUsedInLod0();

		void logStatistics();
		
		void clear();

	protected:
		uint32					mMeshAndBspVersion; 
		uint32					mMeshAndBspSize;
		uint16					mBspVersion;

		// INDOOR or OUTDOOR
		Environment				mEnvironment;

		// Indices of polygons used in BSP
		vector<uint32>::type	mPolygonIndices;

		// Root BSP node
		BspNode*				mRootBspNode;

		// All BSP nodes are stored in this vector
		vector<BspNode>::type	mBspNodes;

		// All BSP leafs are stored in this vector
		vector<BspLeaf>::type	mBspLeaves;

		// Sectors
		vector<Sector>::type	mSectors;

		// Number of uses in LOD 0 for each polygon
		// (if a polygon is used in LOD 1 only then the polygon must not be used)
		vector<size_t>::type	mPolygonUsesInLod0;

		// Child operations for better progress bar
		Progress::Operation* mChildOperation;
		Progress::Operation* mChildOperation2;
	};

} // namespace GothOgre

#endif // MESH_AND_BSP_LOAD_CONTEXT_H