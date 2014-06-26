#ifndef GOTHOGRE_INTERPOLATION_H
#define GOTHOGRE_INTERPOLATION_H


namespace GothOgre
{
	//---------------------------------------------------------------------------
	struct GOTHOGRE_EXPORT Interpolation
	{
		//------------------------------------------------------------------
		/** Performs a linear interpolation. */
		template<typename ValueType, typename RealType>
		static ValueType lerp(const ValueType& _x, const ValueType& _y, RealType _s)
		{
			return ValueType( _x * (1 - _s) + _y * _s );
		}



		//------------------------------------------------------------------
		// Result of the findClosestPoints function. 
		template<typename ValueType>
		struct FindClosestPointsResult
		{
			// true if the last search was successful
			bool                  success;

			// index of the nearest smaller element to the searched one.
			size_t                lowerBound;

			// index of the nearest greater element to the searched one.
			size_t                upperBound;
			
			// ratio which can be used to interpolate values.
			Real                  ratio;
		};

		/** Utility for searching of a smallest interval 
		containing a specified value. This helper function can be used
		to find source values for a later linear interpolation. */
		template<typename ValueType, typename ElementType, typename ContainerType>
		static FindClosestPointsResult<ValueType> findClosestPoints(
			ValueType _x, 
			ConstVectorIterator<ContainerType> _startIterator, 
			ValueType (ElementType::* _valueInElement), 
			ValueType _period = ValueType(0.0f) )
		{
			size_t valueOffsetInBytes  = (size_t) (void*) &(((ElementType*)0)->*_valueInElement);
			return _findClosestPoints(_x, _startIterator, valueOffsetInBytes, -1, _period );
		}

		/** Utility for searching of a smallest interval 
		containing a specified value. This helper function can be used
		to find source values for a later linear interpolation. */
		template<typename ValueType, typename ElementType, typename ContainerType>
		static FindClosestPointsResult<ValueType> findClosestPoints(
			ValueType _x, 
			ConstVectorIterator<ContainerType> _startIterator,
			ValueType (ElementType::* _valueInElement), 
			ValueType (ElementType::* _periodInElement) )
		{
			size_t valueOffsetInBytes  = (size_t) (void*) &(((ElementType*)0)->*_valueInElement);
			size_t periodOffsetInBytes = (size_t) (void*) &(((ElementType*)0)->*_periodInElement);
			return _findClosestPoints(_x, _startIterator, valueOffsetInBytes, periodOffsetInBytes, ValueType() );
		}

		/** Utility for searching of a smallest interval 
		containing a specified value. This helper function can be used
		to find source values for a later linear interpolation. */
		template<typename ValueType, typename ElementType, typename AllocatorType>
		static FindClosestPointsResult<ValueType> findClosestPoints(
			ValueType _x, const std::vector<ElementType, AllocatorType> _vector, 
			ValueType (ElementType::* _valueInElement), 
			ValueType _period = ValueType(0.0f) )
		{
			return findClosestPoints( _x, 
				(ConstVectorIterator<std::vector<ElementType, AllocatorType>>) _vector,
				_valueInElement, _period );
		}

		/** Utility for searching of a smallest interval 
		containing a specified value. This helper function can be used
		to find source values for a later linear interpolation. */
		template<typename ValueType, typename ElementType, typename AllocatorType>
		static FindClosestPointsResult<ValueType> findClosestPoints(
			ValueType _x, const std::vector<ElementType, AllocatorType> _vector, 
			ValueType (ElementType::* _valueInElement), 
			ValueType (ElementType::* _periodInElement) )
		{
			return findClosestPoints( _x, 
				(ConstVectorIterator<std::vector<ElementType, AllocatorType>>) _vector,
				_valueInElement, _periodInElement );
		}

	private:
		template<typename ValueType, typename IteratorType>
		static FindClosestPointsResult<ValueType> _findClosestPoints(
			ValueType _x, 
			IteratorType _startIterator,
			size_t _valueOffsetInBytes, 
			size_t _periodOffsetInBytes, 
			ValueType _period )
		{
			FindClosestPointsResult<ValueType> result;

			ValueType zero( 0.0f );
			ValueType infinity( Math::POS_INFINITY );
			ValueType distFromLowerBound = infinity;
			ValueType distToUpperBound   = infinity;
			
			IteratorType iterator = _startIterator;
			for(size_t index = 0; iterator.hasMoreElements(); ++index)
			{
				const IteratorType::ValueType& element = iterator.getNext();

				ValueType cur  = *(ValueType*)(((size_t)(void*)(&element)) + _valueOffsetInBytes);
				ValueType prev = cur;
				ValueType next = cur;
				
				ValueType period;
				if(_periodOffsetInBytes == -1)
					period = *(ValueType*)(void*) (((size_t)(void*)(&element)) + _periodOffsetInBytes);
				else
					period = _period;
				
				if(period != infinity && period > zero && _x >= cur)
				{
					prev = ValueType( floor((_x - cur)/period) * period + cur );
					if(prev < cur)
						next = ValueType( prev + period );
					else
						next = prev = cur;
				}
				
				ValueType distFromLowerBound2 = cur - prev;
				ValueType distToUpperBound2   = next - cur;
				
				if(zero <= distFromLowerBound2 && distFromLowerBound2 < distFromLowerBound)
				{
					result.lowerBound = index;
					distFromLowerBound = distFromLowerBound2;
				}
				
				if(zero <= distToUpperBound2 && distToUpperBound2 < distToUpperBound)
				{
					result.upperBound = index;
					distToUpperBound = distToUpperBound2;
				}
			}
			
			result.success = true;
			ValueType intervalLength = distFromLowerBound + distToUpperBound;
			if(intervalLength == zero)
			{
				result.ratio = 0;
			}
			else if(distFromLowerBound != infinity)
			{
				result.ratio = Real(distFromLowerBound / intervalLength);
			}
			else if(distToUpperBound != infinity)
			{
				result.ratio = 1;
			}
			else
			{
				result.success = false;
			}
			return result;
		}

	};



	//--------------------------------------------------------------------------
	/* Example of usage:

	ColourValue clr1(1, 0, 0.3); // Red
	ColourValue clr2(0, 1, 0.5); // Green
	ColourValue clr = lerp(clr1, clr2, 0.2);
	// now clr == ColourValue(0.8, 0.2, 0.34);
	*/

	//--------------------------------------------------------------------------
	/* Example of usage:
	
	struct MyStruct
	{
		Degree angle;
		int    x; // value, which depends on angle
		... // many other fields

		// Constructor
		MyStruct(Degree _angle, int _value) : angle(_angle), value(_value) {}
	};

	// typedefs
	typedef vector<MyStruct>::type MyStructList;
	typedef ConstVectorIterator<MyStructList> MyStructIterator;

	// set functional dependency
	MyStructList list;
	list.push_back(MyStruct(0, -5));   // #0
	list.push_back(MyStruct(30, 6));   // #1
	list.push_back(MyStruct(60, 110)); // #2
	list.push_back(MyStruct(75, 20));  // #3
	list.push_back(MyStruct(90, 50));  // #4

	// searching
	SmallestIntervalSearch<Degree> sis( &MyStruct::angle, 360 ); // The period is equal to 360 degrees.
	bool ok = sis.execute( (MyStructIterator) list, 65 );    // ok == true, i.e. success
	size_t prev = sis.getLowerBound();  // prev == 2 (angle: 60)
	size_t next = sis.getUpperBound();  // next == 3 (angle: 75)
	Real ratio = sis.getRatio();        // ratio == (65-60)/(75-60) == 1/3 == 0.33333
	int interpolated_x = Math2::lerp( list[prev].x, list[next].x, ratio ); // result == 110*(1-1/3)+20*1/3 == 80
	*/


} // namespace GothOgre

#endif // GOTHOGRE_INTERPOLATION_H