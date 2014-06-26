#ifndef GOTHOGRE_PRIORITY_H
#define GOTHOGRE_PRIORITY_H


namespace GothOgre
{
	/** Symbolic definitions for priorities. The priority is an integer value
	which can be not equal to one of the following values. 
	The priorities are used in InputSystem, UIModeManager. */
	class Priority
	{
	public:
		enum Enum
		{
			LOWEST       = 0,
			LOW          = 100,
			BELOW_NORMAL = 200,
			NORMAL       = 300,
			ABOVE_NORMAL = 400,
			HIGH         = 500,
			HIGHEST      = 0x7FFFFFFF
		};

		Priority(int _value = LOWEST) : mValue(Enum(_value)) {}
		operator int() const {return mValue;}
	private:
		Enum mValue;
	};
}

#endif // GOTHOGRE_PRIORITY_H