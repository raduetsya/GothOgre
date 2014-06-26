#ifndef GUISYSTEM_TOOL_MANAGER_H
#define GUISYSTEM_TOOL_MANAGER_H

namespace GUISystem
{
	class Tool;
	class ToolFactory;
	class ToolOperation;
	
	/** Manages the tools */
	class GUISYSTEM_EXPORT ToolManager
	{
	public:
		ToolManager(const String& _name);
		~ToolManager();

		/** Returns the name of this tool manager. */
		const String& getName() const;

		void createTools();
		void destroyTools();

		/** Find a tool by type name
		@remark
		The functions throws an exception if a tool is not found. */
		Tool* getTool(const String& _name) const;

		/** Activates a specified tool with setting a custom parameter for the tool
		@param _tool Tool to activate. nullptr is allowed.
		@param _toolData Custom parameter. Can be used by tool.
		@return true if success
		@par false if Tool::deactivating for the current active tool
		or Tool::activating for the specified tool returns false. */
		ToolOperation* activateTool(Tool* _tool, const Any& _toolParameter = Any());

		/** Returns current active tool operation.
		@remark
		nullptr is returned if there is no active tool */
		ToolOperation* getActiveOperation() const;

		/** Deactivates the tool. */
		void deactivateTool(ToolOperation* _toolOperation);

	private:
		friend class ToolFactory;
		void addToolFactory(ToolFactory* _toolFactory);
		void removeToolFactory(ToolFactory* _toolFactory);
		void setActiveOperation(ToolOperation* _toolOperation);

	private:
		typedef list<Tool*>::type				Tools;
		typedef map<String, Tool*>::type		ToolsByName;
		typedef list<ToolFactory*>::type		ToolFactories;

		struct ToolAndParam
		{
			Tool*  tool;
			Any    parameter;
		};
		typedef list<ToolAndParam>::type PreviousTools;

	private:		
		ToolFactories			mToolFactories;
		Tools					mToolsByAddOrder;
		ToolsByName				mToolsByName;
		PreviousTools			mPreviousTools;
		String                  mName;
	};

} // namespace GUISystem

#endif // GUISYSTEM_TOOL_MANAGER_H
