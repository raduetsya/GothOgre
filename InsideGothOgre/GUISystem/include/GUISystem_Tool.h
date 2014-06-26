#ifndef GUISYSTEM_TOOL_H
#define GUISYSTEM_TOOL_H


namespace GUISystem
{
	class ToolFactory;
	class ToolManager;
	class ToolOperation;

	/** Tool represents link between an user and an object controlling by the user.
	@remark
	The controlling object can be camera, vob, vertex of mesh in edit mode and so on.
	@par
	The "Tool" class is designed to be a base class in hierarchy of tools.
	@par
	Only one tool can be active at the same time.
	Use ToolManager to operate with tool.
	*/
	class GUISYSTEM_EXPORT Tool
	{
	public:
		ToolFactory* getToolFactory() const {return mToolFactory;}

		/** Returns a tool manager which uses the tool */
		ToolManager* getToolManager() const {return mToolManager;}

		/** Returns the tool's name. This name is used for searching tools by name */
		const String& getName() const;

		/** Activates the tool. The previous active tool will be deactivated automatically */
		ToolOperation* activateTool(const Any& _toolParameter = Any());

		/** Is the tool active now? */
		bool isActive() const;

		/** Constructor */
		Tool();

		/** Destructor */
		virtual ~Tool();
		
	protected:
		friend class ToolManager;

		/** Called by ToolManager to finish the tool's activation.
		@remark
		The function can add listeners for event MouseEvent, 
		KeyEvent, ControlSettingsEvent, and so on, if it wants. */
		virtual ToolOperation* activate(const Any& _toolParameter) = 0;

	private:
		friend class ToolManager;
		void init(ToolFactory* _toolFactory, ToolManager* _toolManager);

	private:
		ToolManager*	mToolManager;
		ToolFactory*    mToolFactory;
	};
}

#endif // GUISYSTEM_TOOL_H