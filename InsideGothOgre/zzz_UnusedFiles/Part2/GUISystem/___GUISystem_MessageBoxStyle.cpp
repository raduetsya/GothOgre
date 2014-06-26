#include "GUISystem_Precompiled.h"
#include "GUISystem_MessageBoxStyle.h"


namespace GUISystem
{
	//-------------------------------------------------------------------------
	size_t MessageBoxStyle::getIconIndex() const
	{
		size_t index = 0;
		int num = (*this) >> _INDEX_ICON1;

		while (num != 0)
		{
			if ((num & 1) == 1)
				return index;

			++index;
			num >>= 1;
		}

		return -1;
	}
	//-------------------------------------------------------------------------
	size_t MessageBoxStyle::getButtonIndex() const
	{
		size_t index = 0;
		int num = (*this);

		while (num != 0)
		{
			if ((num & 1) == 1)
				return index;

			++index;
			num >>= 1;
		}

		return -1;
	}
	//-------------------------------------------------------------------------
	vector<MessageBoxStyle>::type MessageBoxStyle::getButtons() const
	{
		vector<MessageBoxStyle>::type buttons;

		size_t index = 0;
		int num = (*this);
		while (index < _INDEX_ICON1)
		{
			if ((num & 1) == 1)
				buttons.push_back(index);

			++index;
			num >>= 1;
		}

		return buttons;
	}


} // namespace GUISystem