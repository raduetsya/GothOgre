#ifndef GOTHOGRE_MEMORY_POOL_ALLOCATOR_H
#define GOTHOGRE_MEMORY_POOL_ALLOCATOR_H

#include "GothOgre_MemoryPool.h"
#include "GothOgre_MemoryPoolPtr.h"

namespace GothOgre
{
	/// This allocator can be used instead of std::allocator.
	/// This allocator is ideal for node-based containers (std::list, std::map, std::multimap, std::set),
	/// but this allocator is NOT general-purpose allocator:
	/// it can allocate only 1 block of storage per one time.
	template<typename T>
	class MemoryPoolAllocator
	{
	public:
		/// Element type.
		typedef T value_type;

		/// Pointer to element.
		typedef T* pointer;

		/// Reference to element.
		typedef T& reference;

		/// Constant pointer to element.
		typedef const T* const_pointer;

		/// Constant reference to element.
		typedef const T& const_reference;

		/// Quantities of elements.
		typedef size_t size_type;

		/// Difference between two pointers.
		typedef ptrdiff_t difference_type;

		/// Constructs an allocator object.
		MemoryPoolAllocator(size_t _numBlocksPerAllocate)
		{
			mMemoryPool = MemoryPoolPtr( new MemoryPool(sizeof(T), _numBlocksPerAllocate) );
		}

		/// Constructs an allocator object.
		MemoryPoolAllocator(const MemoryPoolAllocator& _src)
		{
			mMemoryPool = _src.getMemoryPool();
		}

		/// Constructs an allocator object.
		template<typename U>
		MemoryPoolAllocator(const MemoryPoolAllocator<U>& _src)
		{
			mMemoryPool = _src.getMemoryPool();
			mMemoryPool->setBlockSize(std::max(sizeof(T), mMemoryPool->getBlockSize()));
		}

		/// Allocator destructor.
		~MemoryPoolAllocator()
		{
		}

		/// Returns the address of x.
		pointer address(reference x) const
		{
			return &x;
		}

		/// Returns the address of x.
		const_pointer address(const_reference x) const
		{
			return &x;
		}

		/// Assignment operator.
		const MemoryPoolAllocator& operator =(const MemoryPoolAllocator& _src)
		{
			mMemoryPool = _src.getMemoryPool();
			return *this;
		}

		/// Assignment operator.
		template<typename U>
		const MemoryPoolAllocator& operator =(const MemoryPoolAllocator<U>& _src)
		{
			mMemoryPool = _src.getMemoryPool();
			mMemoryPool->setBlockSize(std::max(sizeof(T), mMemoryPool->getBlockSize()));
			return *this;
		}

		/// Returns maximum size possible to allocate.
		size_type max_size() const
		{
			return std::numeric_limits<size_type>::max() / sizeof(T);
		}

		/// Allocates a block of storage with a size large enough to contain 1 elements 
		/// of type T (the allocator's template parameter), 
		/// but this element is not constructed (constructor T::T is not called).
		pointer allocate(size_type _n = 1, const void* _hint = 0)
		{
			GOTHOGRE_ASSERT(_n == 1, "MemoryPoolAllocator can allocate only 1 element per call!");
			return reinterpret_cast<pointer>( mMemoryPool->allocate() );
		}

		/// Release block of storage. Destructor T::~T is not called.
		void deallocate(pointer _p, size_type _n = 1)
		{
			mMemoryPool->deallocate(_p);
		}

		/// Constructs an object of type T (the template parameter) on the location pointed by _p 
		/// using its copy constructor to initialise its value to _val.
		void construct(pointer _p, const_reference _val)
		{
			new ((void*)_p) T(_val);
		}

		/// Constructs an object of type T (the template parameter) on the location pointed by _p 
		/// using its default constructor.
		void construct(pointer _p)
		{
			new ((void*)_p) T();
		}

		/// Destroys the object of type T (the template parameter) pointed by _p.
		void destroy(pointer _p)
		{
			_p->~T();
		}

		/// Constructs an object of type T (the template parameter) on a new allocated block of storage.
		T* construct_new(const_reference _val)
		{
			T* p = allocate(1);
			construct(p, _val);
			return p;
		}

		/// Constructs an object of type T (the template parameter) on a new allocated block of storage.
		T* construct_new()
		{
			T* p = allocate(1);
			construct(p);
			return p;
		}

		/// Destroys the object of type T (the template parameter) pointed by _p and release block of storage.
		void del(pointer _p)
		{
			destroy(_p);
			deallocate(_p);
		}

		/// Comparison operator.
		bool operator ==(const MemoryPoolAllocator& _other) 
		{ 
			return mMemoryPool == _other.mMemoryPool; 
		}

		/// Comparison operator.
		template<typename U>
		bool operator ==(const MemoryPoolAllocator<U>& _other) 
		{ 
			return mMemoryPool == _other.mMemoryPool; 
		}

		/// Comparison operator.
		bool operator !=(const MemoryPoolAllocator& _other) 
		{
			return !operator==(_other); 
		}

		/// Comparison operator.
		template<typename U>
		bool operator !=(const MemoryPoolAllocator<U>& _other) 
		{
			return !operator==(_other); 
		}

		/// The rebind functionality.
		template<typename U> 
		struct rebind
		{ 
			typedef MemoryPoolAllocator<U> other;
		};

		MemoryPoolPtr getMemoryPool() const
		{
			return mMemoryPool;
		}

	private:
		MemoryPoolPtr mMemoryPool;
	};

}; // namespace GothOgre

#endif // GOTHOGRE_MEMORY_POOL_ALLOCATOR_H