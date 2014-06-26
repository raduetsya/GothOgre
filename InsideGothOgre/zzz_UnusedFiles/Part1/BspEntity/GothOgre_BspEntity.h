#ifndef GOTHOGRE_BSP_ENTITY_H
#define GOTHOGRE_BSP_ENTITY_H

#include "GothOgre_BspPtr.h"

namespace GothOgre
{

	class GOTHOGRE_EXPORT BspEntity : public Entity
	{
	public:
		BspEntity();
		BspEntity(const String& _name, const BspPtr& _bsp);
		virtual ~BspEntity();

		// Sets the distance at which the object is no longer rendered.
		virtual void setRenderingDistance(Real _dist);

		// Gets the Mesh that this Entity is based on.  
		const BspPtr& getBsp() const {return mBsp;}

		/** Retrieves the local axis-aligned bounding box for this object. */
		virtual const AxisAlignedBox& getBoundingBox() const;

		/** Retrieves the radius of the origin-centered bounding sphere for this object. */
		virtual Real getBoundingRadius() const;

		/** Internal method by which the movable object 
		must add Renderable subclass instances to the rendering queue. */
		virtual void _updateRenderQueue( RenderQueue *queue );

		/** Try to initialise the BspEntity from the underlying resources. */
		void _initialise( bool forceReinitialise = false );

	private:
		void destroyMesh();
		void createMesh();
		void createSharedVertices();
		void createSubMeshes();
		void setNeedFindVisibleFaces(bool _needFind);
		void findVisibleFaces();
		void addSubEntitiesToRenderQueue(RenderQueue* _queue);
		static vector<Plane>::type getFrustumPlanes(Camera* _camera);

	private:
		BspPtr						mBsp;
		size_t						mBspStateCount;
		vector<IndexData*>::type	mIndexDatas;

		bool						mNeedFindVisibleFaces;
		Vector3						mLastCameraPosition;
		Quaternion					mLastCameraOrientation;
		Vector3						mLastBspPosition;
		Quaternion					mLastBspOrientation;

	private:
		/** Special class to find visible faces by using BSP tree. */
		class VisibleFacesSearch
		{
		public:
			VisibleFacesSearch(const BspPtr& _bsp, IndexData** _indexDatas, const vector<Plane>::type& _frustumPlanes);
			~VisibleFacesSearch();

			/** Runs the search visible faces. */
			void run();

		private:
			void lockBuffers();
			void unlockBuffers();
			bool isVisibleBox(const Vector3& _boxCenter, const Vector3& _boxHalfSize, const size_t* _planeIndices, size_t* _planeIndices2) const;
			void enumVisibleFaces(Bsp::BspNode* _bspNode, const size_t* _planeIndices);
			void addFaces(size_t _firstFaceIndex, size_t _numFaceIndices);

		private:
			BspPtr		mBsp;
			Camera*		mCamera;

			enum {NUM_FRUSTUM_PLANES = 6};
			Plane		mFrustumPlanes[NUM_FRUSTUM_PLANES];
			Vector3		mFrustumPlaneAbsNormals[NUM_FRUSTUM_PLANES];
			size_t		mFrustumPlaneIndices[NUM_FRUSTUM_PLANES + 1];

			size_t		mNumFaceIndices;
			size_t*		mFaceIndices;

			size_t		mNumFaces;
			Bsp::Face*	mFaces;
			uint8*		mFaceVisibilities;

			size_t		mNumMaterials;
			IndexData**	mIndexDatas;
			uint32**	mIndexBuffers;

			size_t		mNumVisibleFaces;
		};
	};

} // namespace GothOgre

#endif // GOTHOGRE_BSP_ENTITY_H