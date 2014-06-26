#ifndef GOTHOGRE_SAFE_LIST_H
#define GOTHOGRE_SAFE_LIST_H


namespace GothOgre
{
	/** List with safe operations for add and remove elements.
	These operations can be safety done in any time even 
	while iterating through the list. */
	template<typename T>
	class SafeList
	{
	public:
		typedef typename SafeList<T>                   Container;
		typedef typename T                             ValueType;
		typedef typename list<T>::type                 InternalList;
		typedef typename InternalList::const_iterator  ElementHandle;

	public:
		/** Iterator. */
		class Iterator
		{
		private:
			Container*   mContainer;
			size_t       mIteratorIndex;

		public:
			/** Constructor.
			@remark
			Provide a container to initialise. */
			Iterator(Container* _container)
			{
				mContainer = _container;
				mIteratorIndex = -1;

				Container::ItUseInfoVector& vec = mContainer->mItUseInfoVector;
				for(size_t i = 0; i != vec.size(); ++i)
				{
					if(!vec[i].inUse)
					{
						mIteratorIndex = i;
						break;
					}
				}

				if(mIteratorIndex == -1)
				{
					if(vec.size() > 100)
						GOTHOGRE_WARNING("Too many iterators (> 100) for SafeList.");
					mIteratorIndex = vec.size();
					vec.push_back(Container::ItUseInfo());
				}
				
				Container::ItUseInfo& itUseInfo = mContainer->mItUseInfoVector[mIteratorIndex];
				itUseInfo.inUse = true;
				itUseInfo.elementHandle = mContainer->mList.begin();
			}

			~Iterator()
			{
				Container::ItUseInfo& itUseInfo = mContainer->mItUseInfoVector[mIteratorIndex];
				itUseInfo.inUse = false;
			}

			/** Returns true if there are more items in the collection. */
			bool hasMoreElements() const
			{
				Container::ItUseInfo& itUseInfo = mContainer->mItUseInfoVector[mIteratorIndex];
				return itUseInfo.elementHandle != mContainer->mList.end();
			}
  
			/** Moves the elementHandle on one element. */
			void moveNext()
			{
				Container::ItUseInfo& itUseInfo = mContainer->mItUseInfoVector[mIteratorIndex];
				++itUseInfo.elementHandle;
			}
    
			/** Returns the next(=current) value element in the collection, and advances to the next. */
			ValueType getNext()
			{
				Container::ItUseInfo& itUseInfo = mContainer->mItUseInfoVector[mIteratorIndex];
				return *(itUseInfo.elementHandle++);
			}

			/** Returns the next(=current) value element in the collection, without advancing to the next. */
			ValueType peekNextValue()
			{
				Container::ItUseInfo& itUseInfo = mContainer->mItUseInfoVector[mIteratorIndex];
				return *(itUseInfo.elementHandle);
			}

			/** Returns the element handle by the iterator */
			ElementHandle getElementHandle() const
			{
				Container::ItUseInfo& itUseInfo = mContainer->mItUseInfoVector[mIteratorIndex];
				return itUseInfo.elementHandle;
			}
		};

	private:
		friend class Iterator;

		struct ItUseInfo
		{
			ElementHandle  elementHandle;
			bool           inUse;
		};
		typedef typename vector<ItUseInfo>::type ItUseInfoVector;

		InternalList      mList;
		ItUseInfoVector   mItUseInfoVector;

	public:
		/** Constructor */
		SafeList() 
		{
		}

		/** Destructor. */
		~SafeList()
		{
			clear();
		}

		/** Adds new element to the list */
		ElementHandle add(ValueType _newElem)
		{
			mList.push_back(_newElem);
			ElementHandle handle = mList.end();
			return --handle;
		}

		/** Removes an element from the list */
		void remove(ElementHandle _h)
		{
			if(_h == mList.end())
				return;
				
			for(size_t i = 0; i != mItUseInfoVector.size(); ++i)
			{
				ItUseInfo& itUseInfo = mItUseInfoVector[i];
				if(itUseInfo.inUse && itUseInfo.elementHandle == _h)
					++itUseInfo.elementHandle;
			}
			mList.erase(_h);
		}

		/** Removes an element from the list */
		void remove(Iterator _iterator)
		{
			remove(_iterator.getElementHandle());
		}

		/** Gets an elementHandle over this list */
		Iterator getIterator() const
		{
			return Iterator(const_cast<Container*>(this));
		}

		/** Check if the container is empty */
		bool empty() const
		{
			return mList.empty();
		}

		/** Clear the container */
		void clear()
		{
			mList.clear();
			for(size_t i = 0; i != mItUseInfoVector.size(); ++i)
			{
				ItUseInfo& itUseInfo = mItUseInfoVector[i];
				itUseInfo.elementHandle = mList.end();
			}
		}

		/** Returns a specially made invalid handle,
		i.e. a handle points to nothing. */
		ElementHandle getNullHandle() const
		{
			return mList.end();
		}
	};

} // namespace GothOgre

#endif // GOTHOGRE_SAFE_LIST_H