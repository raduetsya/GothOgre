#ifndef GOTHOGRE_MATH_2_H
#define GOTHOGRE_MATH_2_H


namespace GothOgre
{
	//---------------------------------------------------------------------------
	struct GOTHOGRE_EXPORT Math2
	{
		/** Performs a linear interpolation. */
		template<typename VALUE_TYPE, typename REAL_TYPE>
		static VALUE_TYPE lerp(const VALUE_TYPE& _x, const VALUE_TYPE& _y, REAL_TYPE _s)
		{
			return VALUE_TYPE( _x * (1 - _s) + _y * _s );
		}

		/* Example of usage:
		ColourValue clr1(1, 0, 0.3); // Red
		ColourValue clr2(0, 1, 0.5); // Green
		ColourValue clr = lerp(clr1, clr2, 0.2);
		// now clr == ColourValue(0.8, 0.2, 0.34);
		*/
	};

} // namespace GothOgre

#endif // GOTHOGRE_MATH_2_H