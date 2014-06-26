#include "GothOgre_Precompiled.h"
#include "GothOgre_RaySceneQueryEx.h"
#include "GothOgre_SceneManagerEx.h"


namespace GothOgre
{
	//------------------------------------------------------------------------
	// RaySceneQueryEx
	//------------------------------------------------------------------------
	RaySceneQueryEx::RaySceneQueryEx(SceneManagerEx* _sceneManager)
		: SceneQuery(_sceneManager)
	{
        mMaxResults = 1;
		mMultipleIntersections = true;
	}
	//------------------------------------------------------------------------
	RaySceneQueryEx::~RaySceneQueryEx()
	{
	}
	//------------------------------------------------------------------------
	void RaySceneQueryEx::setRay(const Ray& _ray)
	{
		mRay = _ray;
	}
	//------------------------------------------------------------------------
	const Ray& RaySceneQueryEx::getRay() const
	{
		return mRay;
	}
	//------------------------------------------------------------------------
	void RaySceneQueryEx::setMaxResults(ushort _maxResults)
	{
		mMaxResults = _maxResults;
	}
	//------------------------------------------------------------------------
	ushort RaySceneQueryEx::getMaxResults() const
	{
		return mMaxResults;
	}
	//------------------------------------------------------------------------
	void RaySceneQueryEx::setMultipleIntersections(bool _multiple)
	{
		mMultipleIntersections = _multiple;
	}
	//------------------------------------------------------------------------
	bool RaySceneQueryEx::getMultipleIntersections() const
	{
		return mMultipleIntersections;
	}
	//------------------------------------------------------------------------
	RaySceneQueryExResult& RaySceneQueryEx::execute(RaySceneQueryExListener* _listener)
	{
		clearResults();
		mListener = _listener;
		if(!mListener)
		{
			static RaySceneQueryExListener defaultListener;
			mListener = &defaultListener;
		}
		
		executeImpl();

		return getLastResults();
	}
	//------------------------------------------------------------------------
	RaySceneQueryExResult& RaySceneQueryEx::getLastResults()
	{
		return mResult;
	}
	//------------------------------------------------------------------------
	void RaySceneQueryEx::clearResults()
    {
        // C++ idiom to free vector buffer: swap with empty vector
        RaySceneQueryExResult().swap(mResult);
    }



	//------------------------------------------------------------------------
	// RaySceneQueryExPtr
	//------------------------------------------------------------------------
	void RaySceneQueryExPtr::destroy()
	{
		if(pRep)
		{
			SceneManager* sceneMgr = pRep->mParentSceneMgr;
			sceneMgr->destroyQuery(pRep);
		}
		OGRE_FREE(pUseCount, MEMCATEGORY_GENERAL);
		OGRE_DELETE_AUTO_SHARED_MUTEX
	}

} // namespace GothOgre