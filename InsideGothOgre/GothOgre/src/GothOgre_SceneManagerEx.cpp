#include "GothOgre_Precompiled.h"
#include "GothOgre_SceneManagerEx.h"
#include "GothOgre_SceneQueryMask.h"
#include "GothOgre_SoundSystem.h"


namespace GothOgre
{
	//-----------------------------------------------------------------------
	SceneManagerEx::SceneManagerEx(const String& _instanceName)
		: SceneManager(_instanceName)
	{
		mEastDirection  = Vector3::UNIT_X;
		mNorthDirection = Vector3::UNIT_Y;
		mUpDirection    = Vector3::UNIT_Z;
	}
	//-----------------------------------------------------------------------
	SceneManagerEx::~SceneManagerEx()
	{
	}
	//-----------------------------------------------------------------------
	RaySceneQueryExPtr SceneManagerEx::createRayQueryEx(const Ray& _ray, uint32 _queryMask)
	{
		RaySceneQueryExPtr query = createRayQueryEx();
		query->setRay(_ray);
		query->setQueryMask(_queryMask);
		return query;
	}
	//-----------------------------------------------------------------------
	RaySceneQueryExPtr SceneManagerEx::createRayQueryEx(Real _relx, Real _rely, Camera* _camera, uint32 _queryMask)
	{
		Ray ray = _camera->getCameraToViewportRay(_relx, _rely);
		return createRayQueryEx(ray, _queryMask);
	}
	//-----------------------------------------------------------------------
	RaySceneQueryExPtr SceneManagerEx::createRayQueryEx(int _x, int _y, const ViewportAbsCoord& _vpAbsCoord, Camera* _camera, uint32 _queryMask)
	{
		Real relx = Real(_x - _vpAbsCoord.getLeft()) / _vpAbsCoord.getWidth();
		Real rely = Real(_y - _vpAbsCoord.getTop()) / _vpAbsCoord.getHeight();
		return createRayQueryEx(relx, rely, _camera, _queryMask);
	}

} // namespace GothOgre
