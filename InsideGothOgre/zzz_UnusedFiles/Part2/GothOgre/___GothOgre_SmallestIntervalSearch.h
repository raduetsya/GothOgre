#ifndef GOTHOGRE_SMALLEST_INTERVAL_SEARCH_H
#define GOTHOGRE_SMALLEST_INTERVAL_SEARCH_H


namespace GothOgre
{

	//---------------------------------------------------------------------------
	/** Utility class template for searching of a smallest interval 
	containing a specified value. This helper template class can be used
	to find source values for a later linear interpolation. */
	template<typename ValueType>
	class GOTHOGRE_EXPORT SmallestIntervalSearch
	{
	public:
		
		/** Constructor. 
		@param _offsetValue Offset of a value from beginning of an element 
		of the vector which will be later specified with the "doSearch" function.
		@param _period Period for the value. */
		template<typename ElementType>
		SmallestIntervalSearch( ValueType (ElementType::* _offsetValue), ValueType _period = 0)
		{
			mOffsetValue = (size_t) (void*) &(((ElementType*)0)->*_offsetValue);
			mPeriod = _period;
			mUseConstPeriod = true;
			mOk = false;
		}

		/** Constructor. 
		@param _offsetValue Offset of a value from beginning of an element 
		of the vector which will be later specified with the "doSearch" function.
		@param _offsetValue Offset of a period from beginning of an element 
		of the vector which will be later specified with the "doSearch" function. */
		template<typename ElementType>
		SmallestIntervalSearch( ValueType (ElementType::* _offsetValue), ValueType (ElementType::* _offsetPeriod))
		{
			mOffsetValue = (size_t) (void*) &(((ElementType*)0)->*_offsetValue);
			mOffsetPeriod = (size_t) (void*) &(((ElementType*)0)->*_offsetPeriod);
			mUseConstPeriod = false;
			mOk = false;
		}

		/** Searchs a smallest interval containing a value. */
		template<typename IteratorType>
		bool execute( IteratorType _iterator, ValueType _x )
		{
			mRatio      = -1;
			mLowerBound = -1;
			mUpperBound = -1;

			ValueType zero( 0 );
			ValueType infinity( Math::POS_INFINITY );
			ValueType distFromLowerBound = infinity;
			ValueType distToUpperBound   = infinity;
			
			for(size_t index = 0; _iterator.hasMoreElements(); ++index)
			{
				const IteratorType::ValueType& element = _iterator.getNext();

				ValueType cur  = *(ValueType*)(((size_t)(void*)(&element)) + mOffsetValue);
				ValueType prev = cur;
				ValueType next = cur;
				
				ValueType period;
				if(mUseConstPeriod)
					period = mPeriod;
				else
					period = *(ValueType*)(void*) (((size_t)(void*)(&element)) + mOffsetPeriod);
				
				if(period != infinity && period > zero && _x >= cur)
				{
					prev = ValueType( floor( double( (_x - cur) / period) ) * period + cur );
					if(prev < cur)
						next = ValueType( prev + period );
					else
						next = prev = cur;
				}
				
				ValueType distFromLowerBound2 = cur - prev;
				ValueType distToUpperBound2   = next - cur;
				
				if(zero <= distFromLowerBound2 && distFromLowerBound2 < distFromLowerBound)
				{
					mLowerBound = index;
					distFromLowerBound = distFromLowerBound2;
				}
				
				if(zero <= distToUpperBound2 && distToUpperBound2 < distToUpperBound)
				{
					mUpperBound = index;
					distToUpperBound = distToUpperBound2;
				}
			}
			
			mOk = true;
			ValueType intervalLength = distFromLowerBound + distToUpperBound;
			if(intervalLength == zero)
			{
				mRatio = 0;
			}
			else if(distFromLowerBound != infinity)
			{
				mRatio = Real(distFromLowerBound / intervalLength);
			}
			else if(distToUpperBound != infinity)
			{
				mRatio = 1;
			}
			else
			{
				mOk = false;
			}
			return mOk;
		}

		/** Returns true if the last search was successful. */
		bool ok() const {return mOk;}

		/** Returns an index of the nearest smaller element to the searched one. */
		size_t getLowerBound() const {return mLowerBound;}

		/** Returns an index of the nearest greater element to the searched one. */
		size_t getUpperBound() const {return mUpperBound;}
		
		/** Returns  */
		Real getRatio() const {return mRatio;}

	private:
		size_t                mOffsetValue;
		size_t                mOffsetPeriod;
		ValueType             mPeriod;
		bool                  mUseConstPeriod;
		size_t                mLowerBound;
		size_t                mUpperBound;
		Real                  mRatio;
		bool                  mOk;
	};





} // namespace GothOgre

#endif // GOTHOGRE_SMALLEST_INTERVAL_SEARCH_H