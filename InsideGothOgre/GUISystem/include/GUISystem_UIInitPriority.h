#ifndef GUISYSTEM_UI_INIT_PRIORITY_H
#define GUISYSTEM_UI_INIT_PRIORITY_H


namespace GUISystem
{

	struct UIInitPriority
	{
		enum
		{
			GUISYSTEM    = Priority::HIGHEST,
			SKIN         = Priority::HIGH,
			AFTER_SKIN   = Priority::ABOVE_NORMAL,
			BEFORE_TOOLS = Priority::NORMAL,
			TOOLS        = Priority::LOW,
		};
	};
}

#endif // GUISYSTEM_UI_INIT_PRIORITY_H