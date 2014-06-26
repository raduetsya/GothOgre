#ifndef RAY_SCENE_QUERY_EX_IMPL_H
#define RAY_SCENE_QUERY_EX_IMPL_H

#include "SceneManagerExImpl.h"


namespace GothOgre
{
	class RaySceneQueryExImpl : public RaySceneQueryEx, BoundingSphereTreeListener
	{
	public:
		RaySceneQueryExImpl(SceneManagerExImpl* _sceneManager);
		~RaySceneQueryExImpl();

		void executeImpl();

	private:
		void removeMultipleIntersections();
		void enumerateRayIntersects(SceneNodeBST::ElementHandle _handle, Real _distance, void* _param);
		void intersectRayEntity();
		void inspectMaterial();
		void intersectRaySubMesh();
		void intersectRayIndices();
		bool intersectRayFace();
		void addResultEntry();

	private:
		Ray                            mLocalRay;
		Matrix4                        mRayToWorldTransform;
		Matrix4                        mWorldToRayTransform;
		Matrix4                        mLocalToRayTransform;
		Entity*                        mEntity;
		SubEntity*                     mSubEntity;
		size_t                         mSubEntityIndex;
		SubMesh*                       mSubMesh;
		size_t                         mSubMeshIndex;
		MaterialPtr                    mMaterial;
		bool                           mMaterialTransparent;
		CullingMode                    mMaterialCullingMode;
		Real                           mClockwiseCoef;

		MeshReadHelper                 mMeshReadHelper;

		size_t                         mFaceIndex;
		Vector3                        mFaceVertices[3];
		Vector3                        mFaceNormal;
		Real                           mDistance;
	};

} // namespace GothOgre

#endif // RAY_SCENE_QUERY_EX_IMPL_H