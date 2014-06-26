#ifndef GUISYSTEM_TOOL_OPERATION_H
#define GUISYSTEM_TOOL_OPERATION_H

namespace GUISystem
{
	class GUISYSTEM_EXPORT ToolOperation
	{
	public:
		ToolOperation();
		virtual ~ToolOperation();
		
		Tool* getTool() const {return mTool;}

		void deactivateTool();

	private:
		friend class ToolManager;
		void init(Tool* _tool);

	private:
		Tool* mTool;
		bool  mDeactivated;
	};

}

#endif // GUISYSTEM_TOOL_OPERATION_H
