#include "GothOgre_Precompiled.h"
#include "GothOgre_BspPtr.h"
#include "GothOgre_Bsp.h"


namespace GothOgre
{
	//------------------------------------------------------------------------
	BspPtr::BspPtr(const ResourcePtr& r) 
		: SharedPtr<Bsp>()
	{
		// lock & copy other mutex pointer
        OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
        {
		    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
		    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
		    pRep = (Bsp*) (r.getPointer());
		    pUseCount = r.useCountPointer();
		    if (pUseCount)
		    {
			    ++(*pUseCount);
		    }
        }
	}
	//------------------------------------------------------------------------
	BspPtr& BspPtr::operator=(const ResourcePtr& r)
	{
		if (pRep == r.getPointer())
			return *this;
		release();
		// lock & copy other mutex pointer
        OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
        {
		    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
		    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
		    pRep = (Bsp*) (r.getPointer());
		    pUseCount = r.useCountPointer();
		    if (pUseCount)
		    {
			    ++(*pUseCount);
		    }
        }
		else
		{
			// RHS must be a null pointer
			assert(r.isNull() && "RHS must be null if it has no mutex!");
			setNull();
		}
		return *this;
	}

} // namespace GothOgre
