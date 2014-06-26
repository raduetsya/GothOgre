#include "GothOgre_Precompiled.h"
#include "GothOgre_MatGenParams.h"


namespace GothOgre
{
	//------------------------------------------------------------------------
	// MatGenParams
	//------------------------------------------------------------------------
	MatGenParams::MatGenParams()
	{
		mFlags = 0;
	}
	//------------------------------------------------------------------------
	void MatGenParams::setFrameIndex( size_t _frameIndex ) 
	{
		mFrameIndex = _frameIndex; 
		mFrameIndexDefined = true;
	}
	//------------------------------------------------------------------------
	void MatGenParams::setScrollSpeed( const Vector2& _loopsPerSecond ) 
	{
		mScrollSpeed = _loopsPerSecond; 
		mScrollSpeedDefined = true;
	}
	//------------------------------------------------------------------------
	void MatGenParams::setColourFactor( const ColourValue& _factor ) 
	{
		mColourFactor = _factor; 
		mColourFactorDefined = true;
	}
	//------------------------------------------------------------------------
	void MatGenParams::setSpecularIntensityFactor( Real _factor ) 
	{
		mSpecularIntensityFactor = _factor; 
		mSpecularIntensityFactorDefined = true;
	}
	//------------------------------------------------------------------------
	void MatGenParams::setSpecularColourFactor( const ColourValue& _factor ) 
	{
		mSpecularColourFactor = _factor; 
		mSpecularColourFactorDefined = true;
	}
	//------------------------------------------------------------------------
	void MatGenParams::setReflectivityFactor( Real _factor ) 
	{
		mReflectivityFactor = _factor; 
		mReflectivityFactorDefined = true;
	}
	//------------------------------------------------------------------------
	void MatGenParams::setSectorName(const String& _sectorName) 
	{
		mSectorName = _sectorName; 
		mSectorNameDefined = true;
	}
	//------------------------------------------------------------------------
	void MatGenParams::setMorph(bool _morph) 
	{
		mMorph = _morph;
	}
	//------------------------------------------------------------------------
	void MatGenParams::setSkin(bool _skin) 
	{
		mSkin = _skin;
	}
	//------------------------------------------------------------------------
	void MatGenParams::setSelected(bool _selected) 
	{
		mIsSelected = _selected;
	}
	//------------------------------------------------------------------------
	void MatGenParams::setParentSelected(bool _parentSelected) 
	{
		mIsParentSelected = _parentSelected;
	}
	//------------------------------------------------------------------------
	bool MatGenParams::operator < (const MatGenParams& _rhs) const
	{
		int icmp = mFlags - _rhs.mFlags;
		if( icmp != 0 )
			return icmp < 0;

		if( mFrameIndexDefined )
		{
			icmp = compare( mFrameIndex, _rhs.mFrameIndex );
			if( icmp != 0 )
				return icmp < 0;
		}

		if( mScrollSpeedDefined )
		{
			icmp = compare( mScrollSpeed, _rhs.mScrollSpeed );
			if( icmp != 0 )
				return icmp < 0;
		}

		if( mColourFactorDefined )
		{
			icmp = compare( mColourFactor, _rhs.mColourFactor );
			if(icmp != 0)
				return icmp < 0;
		}

		if( mSpecularIntensityFactorDefined )
		{
			icmp = compare( mSpecularIntensityFactor, _rhs.mSpecularIntensityFactor );
			if(icmp != 0)
				return icmp < 0;
		}

		if( mSpecularColourFactorDefined )
		{
			icmp = compare( mSpecularColourFactor, _rhs.mSpecularColourFactor );
			if(icmp != 0)
				return icmp < 0;
		}

		if( mReflectivityFactorDefined )
		{
			icmp = compare( mReflectivityFactor, _rhs.mReflectivityFactor );
			if(icmp != 0)
				return icmp < 0;
		}

		if(mSectorNameDefined)
		{
			icmp = compare( mSectorName, _rhs.mSectorName );
			if(icmp != 0)
				return icmp < 0;
		}

		return false; // (*this == _rhs)
	}
	//------------------------------------------------------------------------
	int MatGenParams::compare( Real _first, Real _second )
	{
		Real fcmp = (_first - _second);
		return fcmp < 0 ? -1 : 1;
	}
	//------------------------------------------------------------------------
	int MatGenParams::compare( size_t _first, size_t _second )
	{
		return int(_first - _second);
	}
	//------------------------------------------------------------------------
	int MatGenParams::compare( const String& _first, const String& _second )
	{
		return _first.compare( _second );
	}
	//------------------------------------------------------------------------
	int MatGenParams::compare( const Vector2& _first, const Vector2& _second )
	{
		Real fcmp = (_first.x - _second.x);
		if(fcmp != 0)
			return fcmp < 0 ? -1 : 1;

		fcmp = (_first.y - _second.y);
		return fcmp < 0 ? -1 : 1;
	}
	//------------------------------------------------------------------------
	int MatGenParams::compare( const ColourValue& _first, const ColourValue& _second )
	{
		float fcmp = (_first.r - _second.r);
		if(fcmp != 0)
			return fcmp < 0 ? -1 : 1;

		fcmp = (_first.g - _second.g);
		if(fcmp != 0)
			return fcmp < 0 ? -1 : 1;

		fcmp = (_first.b - _second.b);
		if(fcmp != 0)
			return fcmp < 0 ? -1 : 1;

		fcmp = (_first.a - _second.a);
		if(fcmp != 0)
			return fcmp < 0 ? -1 : 1;

		return 0;
	}

} // namespace GothOgre