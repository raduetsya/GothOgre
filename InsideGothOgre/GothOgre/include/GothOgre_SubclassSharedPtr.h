#ifndef GOTHOGRE_SUBCLASS_SHARED_PTR_H
#define GOTHOGRE_SUBCLASS_SHARED_PTR_H


namespace GothOgre
{
	//----------------------------------------------------------------------
    /** Template for shared pointer to resource.
	We cannot simply use SharedPtr because we need operator =.
	See also note for class TexturePtr in the Ogre API Reference. */
    template<typename T, typename BASE>
	class SubclassSharedPtr : public BASE
    {
    public:
        SubclassSharedPtr() : BASE() {}
        explicit SubclassSharedPtr(T* rep) : BASE(rep) {}
        SubclassSharedPtr(const SubclassSharedPtr& r) : BASE(r) {} 
        SubclassSharedPtr(const BASE& r) : BASE()
        {
			// lock & copy other mutex pointer
            OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
            {
			    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
			    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
                pRep = static_cast<T*>(r.getPointer());
                pUseCount = r.useCountPointer();
                if (pUseCount)
                {
                    ++(*pUseCount);
                }
            }
        }

        /// Operator =
        SubclassSharedPtr& operator=(const BASE& r)
        {
            if (pRep == static_cast<T*>(r.getPointer()))
                return *this;
            release();
			// lock & copy other mutex pointer
            OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
            {
			    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
			    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
                pRep = static_cast<T*>(r.getPointer());
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

		T* get() const
		{
			return static_cast<T*>( BASE::get() );
		}

		T& operator*() const
		{
			T* p = get();
			assert(p);
			return p;
		}
		
		T* operator->() const
		{
			T* p = get();
			assert(p);
			return p;
		}
    };

} // namespace GothOgre

#endif // GOTHOGRE_SUBCLASS_SHARED_PTR_H