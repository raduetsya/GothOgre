#include "Precompiled.h"
#include "PointInformator.h"
#include "SceneManagerExImpl.h"


namespace GothOgre
{
	PointInformator::PointInformator(SceneManagerExImpl *_sm)
		: mSceneManager(_sm)
	{
	}
	//-----------------------------------------------------------------------
	PointInformator::~PointInformator()
	{
	}
	//--------------------------------------------------------------------------------------
	SceneUnderwaterInfo PointInformator::getUnderwaterInfo(const Vector3& _point)
	{
		// Default values
		SceneUnderwaterInfo info;
		info.waterMaterial.setNull();
		info.immersionDepth = Math::NEG_INFINITY;
		info.position = _point;
			
		// Upward raycast - searching ceiling.
		Vector3 upVector = mSceneManager->getUpDirection();
		RaySceneQueryExPtr query = mSceneManager->createRayQueryEx();
		query->setRay( Ray(_point, upVector) );
		query->setQueryMask(SceneQueryMask::LEVEL_BSP);
		query->setMaxResults(1);
		
		// Execute query.
		UnderwaterListener listener(mSceneManager);
		RaySceneQueryExResult& qr = query->execute(&listener);

		// Analyze the result.
		if(qr.size() == 1)
		{
			info.waterMaterial = qr[0].material;
			info.immersionDepth = qr[0].distance;
		}
		return info;
	}
	//--------------------------------------------------------------------------------------
	PointInformator::UnderwaterListener::UnderwaterListener(SceneManagerExImpl* _sm)
		: mSceneManager(_sm)
	{
	}

	bool PointInformator::UnderwaterListener::checkSubEntity(SubEntity* _subEnt)
	{
		const MaterialPtr& material = _subEnt->getMaterial();
		if(material.isNull())
			return false;
		
		const MatGenInfo& matGenInfo = mSceneManager->getMatGenInfo( material );
		return (matGenInfo.mtl->getMatGroup() == MatGroup::WATER);
	}
	//--------------------------------------------------------------------------------------
	SceneSectorInfo PointInformator::getSectorInfo(const Vector3& _point)
	{
		// Default values
		SceneSectorInfo info;
		info.sectorName.clear();
		info.position = _point;
			
		// Upward raycast - searching ceiling.
		Vector3 downVector = -mSceneManager->getUpDirection();
		RaySceneQueryExPtr query = mSceneManager->createRayQueryEx();
		query->setRay( Ray(_point, downVector) );
		query->setQueryMask(SceneQueryMask::LEVEL_BSP);
		query->setMaxResults(1);
		
		// Execute query.
		SectorListener listener(mSceneManager);
		RaySceneQueryExResult& qr = query->execute(&listener);

		// Analyze the result.
		if(qr.size() == 1)
		{
			MaterialPtr material = qr[0].material;
			const MatGenInfo& matGenInfo = mSceneManager->getMatGenInfo(material);
			info.sectorName = matGenInfo.matGenParams.getSectorName();
		}
		return info;
	}
	//--------------------------------------------------------------------------------------
	PointInformator::SectorListener::SectorListener(SceneManagerExImpl* _sm)
		: mSceneManager(_sm)
	{
	}

	bool PointInformator::SectorListener::checkSubEntity(SubEntity* _subEnt)
	{
		const MaterialPtr& material = _subEnt->getMaterial();
		if(material.isNull())
			return false;

		const MatGenInfo& matGenInfo = mSceneManager->getMatGenInfo( material );
		return !matGenInfo.matGenParams.getSectorName().empty();
	}
	//--------------------------------------------------------------------------------------
	SceneFloorInfo PointInformator::getFloorInfo(const Vector3& _point)
	{
		// Default values
		SceneFloorInfo info;
		info.position = _point;
		info.floorMaterial.setNull();
		info.heightAboveFloor = Math::NEG_INFINITY;
			
		// Upward raycast - searching ceiling.
		Vector3 downVector = -mSceneManager->getUpDirection();
		RaySceneQueryExPtr query = mSceneManager->createRayQueryEx();
		query->setRay( Ray(_point, downVector) );
		query->setMaxResults(1);
		
		// Execute query.
		RaySceneQueryExResult& qr = query->execute();

		// Analyze the result.
		if(qr.size() == 1)
		{
			info.floorMaterial = qr[0].material;
			info.heightAboveFloor = qr[0].distance;
		}
		return info;
	}

} // namespace GothOgre