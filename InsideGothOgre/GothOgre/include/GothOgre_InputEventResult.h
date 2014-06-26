#ifndef GOTHOGRE_INPUT_EVENT_RESULT_H
#define GOTHOGRE_INPUT_EVENT_RESULT_H

namespace GothOgre
{
	class GOTHOGRE_EXPORT InputEventResult
	{
	public:
		enum Enum
		{
			NOT_PROCESSED,
			PROCESSED
		};

		InputEventResult(int _value = NOT_PROCESSED) : mValue((Enum)_value) {}
		operator int() const {return mValue;}
	private:
		Enum mValue;
	};

	typedef InputEventResult KeyEventResult;
	typedef InputEventResult MouseEventResult;
};

#endif // GOTHOGRE_INPUT_EVENT_RESULT_H