#ifndef GOTHOGRE_SAFE_ORDERED_LIST_H
#define GOTHOGRE_SAFE_ORDERED_LIST_H


namespace GothOgre
{
	/** List with safe operations for add and remove elements.
	These operations can be safety done in any time even 
	while iterating through the list.
	This class looks like class SafeList.
	There is one difference: the "add" function of this class 
	gets an additional parameter: order.
	The order is used to find a place for a new element. */
	template<typename KEY, typename T>
	class SafeOrderedList
	{
	public:
		typedef typename SafeOrderedList<KEY, T>             Container;
		typedef typename KEY                                 KeyType;
		typedef typename T                                   ValueType;
		typedef typename multimap<KeyType, ValueType>::type  InternalMultiMap;
		typedef typename InternalMultiMap::const_iterator    ElementHandle;

	public:
		/** Iterator. */
		class Iterator
		{
		private:
			Container*  mContainer;
			size_t      mIteratorIndex;

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
					mIteratorIndex = vec.size();
					vec.push_back(Container::ItUseInfo());
				}
				
				Container::ItUseInfo& itUseInfo = mContainer->mItUseInfoVector[mIteratorIndex];
				itUseInfo.inUse = true;
				itUseInfo.elementHandle = mContainer->mMultiMap.begin();
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
				return itUseInfo.elementHandle != mContainer->mMultiMap.end();
			}
  
			/** Returns the next(=current) key element in the collection, without advancing to the next. */
			KeyType peekNextKey() const
			{
				Container::ItUseInfo& itUseInfo = mContainer->mItUseInfoVector[mIteratorIndex];
				return (itUseInfo.elementHandle)->first;
			}

			/** Returns the next(=current) value element in the collection, without advancing to the next. */
			ValueType peekNextValue()
			{
				Container::ItUseInfo& itUseInfo = mContainer->mItUseInfoVector[mIteratorIndex];
				return (itUseInfo.elementHandle)->second;
			}   
    
			/** Returns the next(=current) value element in the collection, and advances to the next. */
			ValueType getNext()
			{
				Container::ItUseInfo& itUseInfo = mContainer->mItUseInfoVector[mIteratorIndex];
				return (itUseInfo.elementHandle++)->second;
			}

			/** Moves the iterator on one element. */
			void moveNext()
			{
				Container::ItUseInfo& itUseInfo = mContainer->mItUseInfoVector[mIteratorIndex];
				++itUseInfo.elementHandle;
			}

			/** Returns the element handle by the iterator */
			ElementHandle getElementHandle() const
			{
				Container::ItUseInfo& itUseInfo = mContainer->mItUseInfoVector[mIteratorIndex];
				return itUseInfo.elementHandle;
			}
		};

	public:
		/** Reverse iterator. */
		class ReverseIterator
		{
		private:
			Container*  mContainer;
			size_t      mIteratorIndex;

		public:
			/** Constructor.
			@remark
			Provide a container to initialise. */
			ReverseIterator(Container* _container)
			{
				mContainer = _container;
				mIteratorIndex = -1;

				Container::ItUseInfoVector& vec = mContainer->mRItUseInfoVector;
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
						GOTHOGRE_WARNING("Too many iterators (> 100) for SafeOrderedList.");
					mIteratorIndex = vec.size();
					vec.push_back(Container::ItUseInfo());
				}
				
				Container::ItUseInfo& itUseInfo = mContainer->mRItUseInfoVector[mIteratorIndex];
				itUseInfo.inUse = true;
				itUseInfo.elementHandle = mContainer->mMultiMap.end();
			}

			~ReverseIterator()
			{
				Container::ItUseInfo& itUseInfo = mContainer->mRItUseInfoVector[mIteratorIndex];
				itUseInfo.inUse = false;
			}

			/** Returns true if there are more items in the collection. */
			bool hasMoreElements() const
			{
				Container::ItUseInfo& itUseInfo = mContainer->mRItUseInfoVector[mIteratorIndex];
				return itUseInfo.elementHandle != mContainer->mMultiMap.begin();
			}
  
			/** Moves the iterator on one element. */
			void moveNext()
			{
				Container::ItUseInfo& itUseInfo = mContainer->mRItUseInfoVector[mIteratorIndex];
				--itUseInfo.elementHandle;
			}
    
			/** Returns the next(=current) value element in the collection, and advances to the next. */
			ValueType getNext()
			{
				Container::ItUseInfo& itUseInfo = mContainer->mRItUseInfoVector[mIteratorIndex];
				return (--itUseInfo.elementHandle)->second;
			}

			/** Returns the element handle by the iterator */
			ElementHandle getElementHandle() const
			{
				Container::ItUseInfo& itUseInfo = mContainer->mItUseInfoVector[mIteratorIndex];
				ElementHandle h = itUseInfo.elementHandle;
				return --h;
			}
		};

	private:
		friend class Iterator;
		friend class ReverseIterator;

		struct ItUseInfo
		{
			ElementHandle  elementHandle;
			bool           inUse;
		};
		typedef typename vector<ItUseInfo>::type ItUseInfoVector;

		InternalMultiMap  mMultiMap;
		ItUseInfoVector   mItUseInfoVector;
		ItUseInfoVector   mRItUseInfoVector;

	public:
		/** Constructor */
		SafeOrderedList() 
		{
		}

		/** Destructor. */
		~SafeOrderedList()
		{
			clear();
		}

		/** Adds new element to the list */
		ElementHandle add(ValueType _newElem, KeyType _order)
		{
			return mMultiMap.insert(std::make_pair(_order, _newElem));
		}

		/** Removes an element from the list */
		void remove(ElementHandle _h)
		{
			if(_h == mMultiMap.end())
				return;

			size_t i;
			for(i = 0; i != mItUseInfoVector.size(); ++i)
			{
				ItUseInfo& itUseInfo = mItUseInfoVector[i];
				if(itUseInfo.inUse && itUseInfo.elementHandle == _h)
					++itUseInfo.elementHandle; // move to the next element
			}

			for(i = 0; i != mRItUseInfoVector.size(); ++i)
			{
				ItUseInfo& itUseInfo = mRItUseInfoVector[i];
				if(itUseInfo.inUse && itUseInfo.elementHandle == _h)
				{
					if(itUseInfo.elementHandle != mMultiMap.begin())
						--itUseInfo.elementHandle; // move to the previous element
					else
						++itUseInfo.elementHandle; // after deletion will point to begin() again
				}
			}

			mMultiMap.erase(_h);
		}

		/** Removes an element from the list */
		void remove(Iterator _iterator)
		{
			remove(_iterator.getElementHandle());
		}

		/** Removes an element from the list */
		void remove(ReverseIterator _iterator)
		{
			remove(_iterator.getElementHandle());
		}

		/** Gets an iterator over this list */
		Iterator getIterator() const
		{
			return Iterator(const_cast<Container*>(this));
		}

		/** Gets a reverse iterator over this list */
		ReverseIterator getReverseIterator() const
		{
			return ReverseIterator(const_cast<Container*>(this));
		}

		/** Check if the container is empty */
		bool empty() const
		{
			return mMultiMap.empty();
		}

		/** Clear the container */
		void clear()
		{
			mMultiMap.clear();
			size_t i;
			for(i = 0; i != mItUseInfoVector.size(); ++i)
			{
				ItUseInfo& itUseInfo = mItUseInfoVector[i];
				itUseInfo.elementHandle = mMultiMap.end();
			}

			for(i = 0; i != mRItUseInfoVector.size(); ++i)
			{
				ItUseInfo& itUseInfo = mRItUseInfoVector[i];
				itUseInfo.elementHandle = mMultiMap.begin();
			}
		}

		/** Returns a specially made invalid handle,
		i.e. a handle points to nothing. */
		ElementHandle getNullHandle() const
		{
			return mMultiMap.end();
		}
	};

} // namespace GothOgre

#endif // GOTHOGRE_SAFE_ORDERED_LIST_H