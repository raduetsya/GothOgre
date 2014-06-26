#ifndef GUISYSTEM_TOOL_FACTORY_H
#define GUISYSTEM_TOOL_FACTORY_H

namespace GUISystem
{
	class Tool;
	class ToolManager;

	class GUISYSTEM_EXPORT ToolFactory
	{
	public:
		ToolFactory(const String& _toolName, const String& _toolManagerName);
		virtual ~ToolFactory();

		/** Returns name of the type of an instance created by the factory */
		const String& getName() const {return mName;}

		/** Creates an instance */
		virtual Tool* createTool() = 0;

		/** Destroys an instance */
		virtual void destroyTool(Tool* _tool);

	private:
		String       mName;
		ToolManager* mToolManager;
	};
}

#endif // GUISYSTEM_TOOL_FACTORY_H
