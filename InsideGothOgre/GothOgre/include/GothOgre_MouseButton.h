#ifndef GOTHOGRE_MOUSE_BUTTON_H
#define GOTHOGRE_MOUSE_BUTTON_H

#include "GothOgre_EnumIO.h"


namespace GothOgre
{
	/** Class represents button identifier for mouse device.
	@remark
	The class support reading key code from string
	and writing it to string in readable form.
	*/
	class GOTHOGRE_EXPORT MouseButton
	{
	public:
		enum Enum
		{
			LEFT = 0,
			RIGHT,
			MIDDLE,

			BUTTON0 = 0,
			BUTTON1,
			BUTTON2,
			BUTTON3,
			BUTTON4,
			BUTTON5,
			BUTTON6,
			BUTTON7,

			NONE = -1
		};

		MouseButton(int _value = NONE) : mValue((Enum)_value) {}
		operator int() const {return mValue;}
		GOTHOGRE_DECLARE_ENUM_IO( MouseButton )
	private:
		Enum  mValue;
	};
}


#endif // GOTHOGRE_MOUSE_BUTTON_H