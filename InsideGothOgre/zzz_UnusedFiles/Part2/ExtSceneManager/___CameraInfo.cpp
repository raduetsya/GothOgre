#include "Precompiled.h"
#include "CameraInfo.h"

namespace GothOgre
{

	CameraInfo::CameraInfo(Camera* _camera, Real _farClipDistance)
	{
		init(_camera, _farClipDistance);
	}
	//----------------------------------------------------------------------------
	void CameraInfo::init(Camera* _camera, Real _farClipDistance)
	{
		mCamera = _camera;
		if(_farClipDistance == 0) 
			_farClipDistance = Math::POS_INFINITY;

		size_t numPlanes = 0;
		const Plane* planes = _camera->getFrustumPlanes();
		for(size_t i = 0; i != MAX_FRUSTUM_PLANES; ++i)
		{
			Plane plane = planes[i];
			if(i == FRUSTUM_PLANE_FAR)
			{
				if(_farClipDistance == Math::POS_INFINITY)
					continue;

				Vector3 normal = -_camera->getDirection();
				Vector3 point = _camera->getDerivedPosition() + _camera->getDirection() * _farClipDistance;
				plane = Plane(normal, point);
			}
			mPlanes[i] = plane;
			const Vector3& n = plane.normal;
			Vector3 an = Vector3(Math::Abs(n.x), Math::Abs(n.y), Math::Abs(n.z));
			mPlaneAbsNormals[i] = an;
			mAllPlaneIndices[numPlanes++] = i;
		}

		mAllPlaneIndices[numPlanes] = -1;
	}
	//----------------------------------------------------------------------------
	CameraInfo::~CameraInfo()
	{
	}
	//----------------------------------------------------------------------------
	bool CameraInfo::isVisibleBox(const Vector3& _boxCenter, const Vector3& _boxHalfSize, 
		const size_t* _planeIndices, size_t* _planeIndices2) const
	{
		// For each plane, see if all points are on the negative side
		// If so, object is not visible
		size_t planeIndex;
		while((planeIndex = *_planeIndices++) != -1)
		{
			// Calculate the distance between box centre and the plane
			const Plane& plane = mPlanes[planeIndex];
			Real dist = plane.getDistance(_boxCenter);

			// Calculate the maximise allows absolute distance for
			// the distance between box centre and plane
			const Vector3& an = mPlaneAbsNormals[planeIndex];
			Real maxAbsDist = an.dotProduct(_boxHalfSize);

			if (dist < -maxAbsDist) // NEGATIVE_SIDE
				return false;

			if (dist < +maxAbsDist) // BOTH_SIDE
			{
				*_planeIndices2++ = planeIndex;
			}
		}
		*_planeIndices2 = -1;
		return true;
	}
	//----------------------------------------------------------------------------
	bool CameraInfo::isVisibleBox(const Vector3& _boxCenter, const Vector3& _boxHalfSize, 
		const size_t* _planeIndices, Real* _distance) const
	{
		// For each plane, see if all points are on the negative side
		// If so, object is not visible
		size_t planeIndex = FRUSTUM_PLANE_NEAR;
		do
		{
			// Calculate the distance between box centre and the plane
			const Plane& plane = mPlanes[planeIndex];
			Real dist = plane.getDistance(_boxCenter);

			// Calculate the maximise allows absolute distance for
			// the distance between box centre and plane
			const Vector3& an = mPlaneAbsNormals[planeIndex];
			Real maxAbsDist = an.dotProduct(_boxHalfSize);

			if (dist < -maxAbsDist) // NEGATIVE_SIDE
				return false;

			if(planeIndex == FRUSTUM_PLANE_NEAR)
			{
				*_distance = std::max(dist - maxAbsDist, (Real)0);
			}

			planeIndex = *_planeIndices++;
			if(planeIndex == FRUSTUM_PLANE_NEAR)
				planeIndex = *_planeIndices++;
		}
		while(planeIndex != -1);

		return true;		
	}


} // namespace GothOgre