#ifndef FILEFORMATS_DYN_ARRAY_H
#define FILEFORMATS_DYN_ARRAY_H


namespace GothOgre
{

	/** Class template DynArray is a wrapper around C++ dynamic array.
	@remark 
	It can change its size and its elements can be accesible with operator [].
	The DynArray class template looks like the std::vector class template 
	but there are differences:
	@par 
	1. The "resize" function. The std::vector<T>::resize 
	initializes new elements with default values when a vector expands.
	DynArray<T>::resize don't initialize new elements.
	Also when the second parameter of DynArray<T>::resize is false
	the function discards old data containing in the DynArray<T>.
	@par
	2. The DynArray class template don't have special capacity member field,
	so it always reallocate memory when its size is changed.
	Also the DynArray class template don't have function which require often
	resizing (push_back, pop_back, insert, remove.
	*/
	template<typename T>
	class DynArray
	{
	public:
		typedef T			value_type;
		typedef size_t		size_type;
		typedef T*			iterator;
		typedef const T*	const_iterator;

		// Constructors
		DynArray() 
		{
			mData = nullptr;
			mSize = 0;
		}

		DynArray(const DynArray<T>& _src) 
		{
			mData = nullptr;
			mSize = 0;
			*this = _src;
		}

		// Destructor
		~DynArray() 
		{
			clear();
		}

		// Copy operator
		const DynArray& operator =(const DynArray& _src)
		{
			if(this != &_src)
			{
				size_t sz = _src.size();
				resize(sz);
				T* pdst = mData;
				const T* psrc = _src.mData;
				while(sz--)
					*pdst++ = *psrc++;
			}
			return *this;
		}

		// Capacity
		size_t size() const 
		{
			return mSize;
		}

		size_t sizeInBytes() const
		{
			return size() * sizeof(T);
		}

		bool empty() const
		{
			return !mSize;
		}

		// Iterators
		iterator begin()
		{
			return mData;
		}

		iterator end()
		{
			return mData + mSize;
		}

		const_iterator begin() const
		{
			return mData;
		}

		// Element access
		const_iterator end() const
		{
			return mData + mSize;
		}

		T& operator [](size_t _index)
		{
			return mData[_index];
		}

		const T& operator [](size_t _index) const
		{
			return mData[_index];
		}

		// Modifiers
		size_t resize(size_t _newsize, bool _keepData = true) 
		{
			size_t oldsize = mSize;
			if(mSize != _newsize)
			{
				T* newdata = nullptr;
				if(_newsize)
				{
					newdata = new T[_newsize];
					if(_keepData)
					{
						size_t minsz = std::min(_newsize, mSize);
						T* pdst = newdata;
						const T* psrc = mData;
						while(minsz--)
							*pdst++ = *psrc++;
					}
				}
				if(mData)
				{
					delete[] mData;
				}
				mData = newdata;
				mSize = _newsize;
			}
			return oldsize;
		}

		void clear()
		{
			resize(0);
		}

	private:
		T*		mData;
		size_t	mSize;
	};


} // namespace GothOgre

#endif // FILEFORMATS_DYN_ARRAY_H