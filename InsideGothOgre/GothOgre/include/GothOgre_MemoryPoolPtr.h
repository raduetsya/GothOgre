#ifndef GOTHOGRE_MEMORY_POOL_PTR_H
#define GOTHOGRE_MEMORY_POOL_PTR_H

#include "GothOgre_MemoryPool.h"

namespace GothOgre
{
	/// Shared pointer to MemoryPool.
	class GOTHOGRE_EXPORT MemoryPoolPtr
	{
	public:
		MemoryPoolPtr()
		{
			mPtr = nullptr;
		}

		MemoryPoolPtr(const MemoryPoolPtr& _src)
		{
			mPtr = _src.mPtr;
			if(mPtr)
				++mPtr->mRefCount;
		}

		explicit MemoryPoolPtr(MemoryPool* _ptr)
		{
			mPtr = _ptr;
			if(mPtr)
				++mPtr->mRefCount;
		}

		~MemoryPoolPtr()
		{
			release();
		}

		const MemoryPoolPtr& operator =(const MemoryPoolPtr& _src)
		{
			if(mPtr != _src.mPtr)
			{
				release();
				mPtr = _src.mPtr;
				if(mPtr)
					++mPtr->mRefCount;
			}
			return *this;
		}

		bool isNull() const
		{
			return !mPtr;
		}

		void setNull()
		{
			release();
			mPtr = nullptr;
		}

		MemoryPool*	getPointer() const
		{
			return mPtr;
		}

		MemoryPool* get() const
		{
			return mPtr;
		}

		MemoryPool* operator->() const
		{
			return get();
		}

		MemoryPool& operator *() const
		{
			return *get();
		}

		size_t useCount() const
		{
			return mPtr ? mPtr->mRefCount : 0;
		}

		bool operator ==(const MemoryPoolPtr& _b) const
		{
			return mPtr == _b.mPtr;
		}

		bool operator !=(const MemoryPoolPtr& _b) const
		{
			return !operator==(_b);
		}

	protected:
		void release()
		{
			if(mPtr)
			{
				if(!--mPtr->mRefCount)
					destroy();
			}			
		}

		void destroy()
		{
			delete mPtr;
		}

	private:
		MemoryPool* mPtr;
	};

}; // namespace GothOgre

#endif // GOTHOGRE_MEMORY_POOL_PTR_H