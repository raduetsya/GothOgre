#ifndef GUISYSTEM_INPUT_PRIORITY_H
#define GUISYSTEM_INPUT_PRIORITY_H


namespace GUISystem
{
	struct InputPriority
	{
		enum
		{
			/** The priority of a hotkey manager which want a key-releasing event. */
			HOTKEY_MANAGER_HIGH = Priority::HIGHEST, 
			
			/** The priority of a tool which has captured mouse. */
			TOOL_HIGH           = Priority::HIGH, 
			
			/** The priority of MyGUI widgets. */
			GUI_WIDGET          = Priority::ABOVE_NORMAL, 

			/** The priority of a hotkey manager in normal state. */
			HOTKEY_MANAGER      = Priority::NORMAL, 

			/** The priority of a tool which has NOT captured input. */
			TOOL                = Priority::BELOW_NORMAL 
		};
	};
}

#endif // GUISYSTEM_INPUT_PRIORITY_H