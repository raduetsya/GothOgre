#ifndef GOTHOGRE_FLAG_SET_H
#define GOTHOGRE_FLAG_SET_H


namespace GothOgre
{
	//------------------------------------------------------------------------
	/** Helpful class to declare wrappers around flags' combination. 
	This class contains one integer value. */
	class GOTHOGRE_EXPORT FlagSet
	{
	public:
		FlagSet(int _value) : mValue(_value) {}
		operator int() const {return mValue;}

		void set(int _f) {mValue |= _f;}
		void clear(int _f) {mValue &= ~_f;}
		bool check(int _f) {return (mValue & _f) != 0;}

	private:
		int mValue;
	};

} // namespace GothOgre

#endif // GOTHOGRE_FLAG_SET_H