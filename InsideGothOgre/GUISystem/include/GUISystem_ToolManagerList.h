#ifndef GUISYSTEM_TOOL_MANAGER_LIST_H
#define GUISYSTEM_TOOL_MANAGER_LIST_H

namespace GUISystem
{
	class ToolManager;
	class ToolOperation;

	class GUISYSTEM_EXPORT ToolManagerList : public Singleton<ToolManagerList>
	{
	public:
		ToolManagerList();
		~ToolManagerList();

		/** Returns a tool manager which has been created for the specified
		UI mode. This function throws an exception if there is no tool manager. */
		ToolManager* getToolManager(const String& _name) const;

		/** Returns the active operation. */
		ToolOperation* getActiveOperation() const;

	private:
		friend class ToolManager;
		void addToolManager(ToolManager* _toolManager);
		void removeToolManager(ToolManager* _toolManager);
		void setActiveOperation(ToolOperation* _toolOperation);

	private:
		friend class Tool;
		friend class ToolManager;
		void setDelayedActTool(Tool* _tool, const Any& _toolParameter = Any());
		Tool* getDelayedActTool() const;
		const Any& getDelayedActToolParameter() const;

	private:
		typedef map<String, ToolManager*>::type ToolManagersByName;
		ToolManagersByName    mToolManagersByName;
		ToolOperation*		  mActiveOperation;
		Tool*                 mDelayedActTool;
		Any                   mDelayedActToolParameter;
	};

} // namespace GUISystem

#endif // GUISYSTEM_TOOL_MANAGER_LIST_H