#ifndef GOTHOGRE_RANGE_H
#define GOTHOGRE_RANGE_H


namespace GothOgre
{

	template<typename T>
	class Range
	{
	public:
		Range() {}
		Range(T _minmax) {mMin = _minmax; mMax = _minmax;}
		Range(T _min, T _max) {mMin = _min; mMax = _max;}
		Range(const Range<T>& _src) {mMin = _src.mMin; mMax = _src.mMax;}

		const Range<T>& operator =(const Range<T>& _src) {mMin = _src.mMin; mMax = _src.mMax; return *this;}

		bool operator ==(const Range<T>& _other) const {return (mMin == _other.mMin) && (mMax == _other.mMax);}
		bool operator !=(const Range<T>& _other) const {return !(*this == _other);}

		T getMin() const {return mMin;}
		T getMax() const {return mMax;}
		void setMin(T _min) {mMin = _min;}
		void setMax(T _max) {mMax = _max;}

	private:
		T mMin;
		T mMax;
	};

}; // namespace GothOgre

#endif // GOTHOGRE_RANGE_H