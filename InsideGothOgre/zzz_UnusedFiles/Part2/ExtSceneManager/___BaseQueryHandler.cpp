#include "Precompiled.h"
#include "SceneManagerExImpl.h"
#include "QueryHandlerBase.h"


namespace GothOgre
{
	//-------------------------------------------------------------------------
	QueryHandlerBase::QueryHandlerBase(SceneManagerExImpl* _sm)
		: mSceneManager(_sm)
	{
	}
	//-------------------------------------------------------------------------
	QueryHandlerBase::~QueryHandlerBase()
	{
	}
	//-------------------------------------------------------------------------
	RaySceneQueryEx* QueryHandlerBase::createRayQueryEx()
	{

	}


} // namespace GothOgre