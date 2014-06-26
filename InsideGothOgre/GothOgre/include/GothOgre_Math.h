#ifndef GOTHOGRE_MATH_H
#define GOTHOGRE_MATH_H


namespace GothOgre
{
	//----------------------------------------------------------------------
	// Divide operators for angles
	inline Real operator /(const Degree& _a, const Degree& _b) 
	{
		return _a.valueDegrees() / _b.valueDegrees();
	}
	
	inline Real operator /(const Radian& _a, const Radian& _b) 
	{
		return _a.valueRadians() / _b.valueRadians();
	}

	inline Real operator /(const Degree& _a, const Radian& _b) 
	{
		return _a.valueDegrees() / _b.valueDegrees();
	}

	inline Real operator /(const Radian& _a, const Degree& _b) 
	{
		return _a.valueDegrees() / _b.valueDegrees();
	}

} // namespace GothOgre

#endif // GOTHOGRE_MATH_H