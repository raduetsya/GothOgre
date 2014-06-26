#ifndef CAMERA_INFO_H
#define CAMERA_INFO_H


namespace GothOgre
{
	// Information which is used to determine 
	// if a scene node or a bsp node is visible for the specified camera.
	class CameraInfo
	{
	public:
		CameraInfo(Camera* _camera, Real _farClipDistance);
		~CameraInfo();

		Camera* getCamera() const {return mCamera;}

		bool isVisibleBox(const Vector3& _boxCenter, const Vector3& _boxHalfSize, 
			const size_t* _planeIndices, size_t* _planeIndices2) const;

		bool isVisibleBox(const Vector3& _boxCenter, const Vector3& _boxHalfSize, 
			const size_t* _planeIndices, Real* _distance) const;

		const size_t* getAllPlaneIndices() const {return mAllPlaneIndices;}

		enum {MAX_FRUSTUM_PLANES = 6};
		typedef size_t PlaneIndices[MAX_FRUSTUM_PLANES + 1];

	private:
		void init(Camera* _camera, Real _farClipDistance);

	private:
		Camera*		mCamera;
		Plane		mPlanes[MAX_FRUSTUM_PLANES];
		Vector3		mPlaneAbsNormals[MAX_FRUSTUM_PLANES];
		size_t		mAllPlaneIndices[MAX_FRUSTUM_PLANES + 1];
	};

} // namespace GothOgre

#endif // CAMERA_INFO_H