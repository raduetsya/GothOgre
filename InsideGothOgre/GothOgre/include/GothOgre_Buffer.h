#ifndef GOTHOGRE_BUFFER_H
#define GOTHOGRE_BUFFER_H

namespace GothOgre
{
	// This template is an alternative to std::vector<T>,
	// however Buffer<T> is most faster if its size less 
	// than its prealloc. size
	template<class T, int _PREALLOC_SIZE>
	class Buffer
	{
	public:
		typedef T value_type;
		static const int PREALLOC_SIZE = _PREALLOC_SIZE;

		Buffer(size_t _size = 0);
		Buffer(const Buffer& _src);

		template<int PREALLOC_SIZE2>
		Buffer(const Buffer<T, PREALLOC_SIZE2>& _src);

		Buffer(const T* _data, size_t _size);
		~Buffer();

		const Buffer& operator =(const Buffer& _src);

		template<int PREALLOC_SIZE2>
		const Buffer& operator =(const Buffer<T, PREALLOC_SIZE2>& _src);
		
		void assign(const T* _data, size_t _size);

		void resize(size_t _size, bool _copyOldData = false);
		size_t size() const;

		void clear();
		bool empty() const;

		const T* data() const;
		T* data();

		const T& operator[](size_t _index) const;
		T& operator[](size_t _index);

		const T& back() const;
		T& back();
		void push_back(const T& _val);
		void pop_back();

	private:
		T* alloc(size_t _size);
		void free(T* _ptr, size_t _size);
		static void copy(T* _dest, const T* _src, size_t _size);

	private:
		size_t          mCapacity;
		size_t          mSize;
		T*              mData;
		T               mPreallocData[PREALLOC_SIZE];
		STLAllocator<T, GeneralAllocPolicy> mAllocator;
	};
	//------------------------------------------------------------------------------------
	template<class T, int PREALLOC_SIZE>
	inline Buffer<T, PREALLOC_SIZE>::Buffer(size_t _size)
		: mSize(0), mCapacity(0), mData(nullptr)
	{
		resize(_size);
	}
	//------------------------------------------------------------------------------------
	template<class T, int PREALLOC_SIZE>
	inline Buffer<T, PREALLOC_SIZE>::Buffer(const Buffer& _src)
		: mSize(0), mCapacity(0), mData(nullptr)
	{
		assign(_src.data(), _src.size());
	}
	//------------------------------------------------------------------------------------
	template<class T, int PREALLOC_SIZE> template<int PREALLOC_SIZE2>
	inline Buffer<T, PREALLOC_SIZE>::Buffer(const Buffer<T, PREALLOC_SIZE2>& _src)
		: mSize(0), mCapacity(0), mData(nullptr)
	{
		assign(_src.data(), _src.size());
	}
	//------------------------------------------------------------------------------------
	template<class T, int PREALLOC_SIZE>
	inline Buffer<T, PREALLOC_SIZE>::Buffer(const T* _data, size_t _size)
		: mSize(0), mCapacity(0), mData(nullptr)
	{
		assign(_data, _size);
	}
	//------------------------------------------------------------------------------------
	template<class T, int PREALLOC_SIZE>
	inline Buffer<T, PREALLOC_SIZE>::~Buffer()
	{
		clear();
	}
	//------------------------------------------------------------------------------------
	template<class T, int PREALLOC_SIZE>
	inline const Buffer<T, PREALLOC_SIZE>& Buffer<T, PREALLOC_SIZE>::operator =(const Buffer& _src)
	{
		assign(_src.data(), _src.size());
		return *this;
	}
	//------------------------------------------------------------------------------------
	template<class T, int PREALLOC_SIZE>
	template<int PREALLOC_SIZE2>
	inline const Buffer<T, PREALLOC_SIZE>& Buffer<T, PREALLOC_SIZE>::operator =(const Buffer<T, PREALLOC_SIZE2>& _src)
	{
		assign(_src.data(), _src.size());
		return *this;
	}
	//------------------------------------------------------------------------------------
	template<class T, int PREALLOC_SIZE>
	void Buffer<T, PREALLOC_SIZE>::assign(const T* _data, size_t _size)
	{
		if(mData != _data)
		{
			resize(_size, false);
			copy(mData, _data, _size);
		}
	}
	//------------------------------------------------------------------------------------
	template<class T, int PREALLOC_SIZE>
	inline void Buffer<T, PREALLOC_SIZE>::clear()
	{
		resize(0);
	}
	//------------------------------------------------------------------------------------
	template<class T, int PREALLOC_SIZE>
	inline bool Buffer<T, PREALLOC_SIZE>::empty() const
	{
		return !mSize;
	}
	//------------------------------------------------------------------------------------
	template<class T, int PREALLOC_SIZE>
	inline size_t Buffer<T, PREALLOC_SIZE>::size() const
	{
		return mSize;
	}
	//------------------------------------------------------------------------------------
	template<class T, int PREALLOC_SIZE>
	void Buffer<T, PREALLOC_SIZE>::resize(size_t _size, bool _copyOldData)
	{
		if(_size != mSize)
		{
			if(_size > mCapacity)
			{
				T* newData;
				if(!_size)
					newData = nullptr;
				else if(_size <= PREALLOC_SIZE)
					newData = mPreallocData;
				else
					newData = alloc(_size);

				if(_copyOldData && newData && newData != mData)
					copy(newData, mData, std::min(mSize, _size));

				if(mData && mData != mPreallocData)
					free(mData, mCapacity);

				mData = newData;
				mCapacity = _size;
			}
			mSize = _size;
		}
	}
	//------------------------------------------------------------------------------------
	template<class T, int PREALLOC_SIZE>
	inline const T* Buffer<T, PREALLOC_SIZE>::data() const
	{
		return mData;
	}
	//------------------------------------------------------------------------------------
	template<class T, int PREALLOC_SIZE>
	inline T* Buffer<T, PREALLOC_SIZE>::data()
	{
		return mData;
	}
	//------------------------------------------------------------------------------------
	template<class T, int PREALLOC_SIZE>
	inline const T& Buffer<T, PREALLOC_SIZE>::operator[](size_t _index) const
	{
#ifdef _DEBUG
		GOTHOGRE_ASSERT(_index <= mSize, "Index out of range");
#endif
		return mData[_index];
	}
	//------------------------------------------------------------------------------------
	template<class T, int PREALLOC_SIZE>
	inline T& Buffer<T, PREALLOC_SIZE>::operator[](size_t _index)
	{
#ifdef _DEBUG
		GOTHOGRE_ASSERT(_index <= mSize, "Index out of range");
#endif
		return mData[_index];
	}
	//------------------------------------------------------------------------------------
	template<class T, int PREALLOC_SIZE>
	const T& Buffer<T, PREALLOC_SIZE>::back() const
	{
#ifdef _DEBUG
		GOTHOGRE_ASSERT(mSize != 0, "Cannot get the last element of empty buffer.");
#endif
		return mData[mSize - 1];
	}
	//------------------------------------------------------------------------------------
	template<class T, int PREALLOC_SIZE>
	T& Buffer<T, PREALLOC_SIZE>::back()
	{
#ifdef _DEBUG
		GOTHOGRE_ASSERT(mSize != 0, "Cannot get the last element of empty buffer.");
#endif
		return mData[mSize - 1];
	}
	//------------------------------------------------------------------------------------
	template<class T, int PREALLOC_SIZE>
	void Buffer<T, PREALLOC_SIZE>::pop_back()
	{
#ifdef _DEBUG
		GOTHOGRE_ASSERT(mSize != 0, "Cannot get the last element of empty buffer.");
#endif
		--mSize;
	}
	//------------------------------------------------------------------------------------
	template<class T, int PREALLOC_SIZE>
	void Buffer<T, PREALLOC_SIZE>::push_back(const T& _val)
	{
		size_t sz = mSize;
		resize(sz + 1, true);
		mData[sz] = _val;
	}
	//------------------------------------------------------------------------------------
	template<class T, int PREALLOC_SIZE>
	T* Buffer<T, PREALLOC_SIZE>::alloc(size_t _size)
	{
		return mAllocator.allocate(_size);
	}
	//------------------------------------------------------------------------------------
	template<class T, int PREALLOC_SIZE>
	void Buffer<T, PREALLOC_SIZE>::free(T* _ptr, size_t _size)
	{
		mAllocator.deallocate(_ptr, _size);
	}
	//------------------------------------------------------------------------------------
	template<class T, int PREALLOC_SIZE>
	void Buffer<T, PREALLOC_SIZE>::copy(T* _dest, const T* _src, size_t _size)
	{
		while(_size--)
			*_dest++ = *_src++;
	}

}


#endif // GOTHOGRE__BUFFER_H