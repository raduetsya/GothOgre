#include "GUISystem_Precompiled.h"
#include "GUISystem_ToolManager.h"
#include "GUISystem_ToolManagerList.h"

//-------------------------------------------------------------------------
GUISystem::ToolManagerList* 
	Ogre::Singleton<GUISystem::ToolManagerList>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------

namespace GUISystem
{
	ToolManagerList::ToolManagerList()
	{
		mActiveOperation = nullptr;
		mDelayedActTool = nullptr;
	}
	//-------------------------------------------------------------------------
	ToolManagerList::~ToolManagerList()
	{
	}
	//-------------------------------------------------------------------------
	ToolOperation* ToolManagerList::getActiveOperation() const
	{
		return mActiveOperation;
	}
	//-------------------------------------------------------------------------
	void ToolManagerList::setActiveOperation(ToolOperation* _toolOperation)
	{
		mActiveOperation = _toolOperation;
	}
	//-------------------------------------------------------------------------
	void ToolManagerList::addToolManager(ToolManager* _toolManager)
	{
		const String& name = _toolManager->getName();
		mToolManagersByName.insert(std::make_pair(name, _toolManager));
	}
	//-------------------------------------------------------------------------
	void ToolManagerList::removeToolManager(ToolManager* _toolManager)
	{
		const String& name = _toolManager->getName();
		ToolManagersByName::iterator it = mToolManagersByName.find(name);
		if(it != mToolManagersByName.end())
			mToolManagersByName.erase(it);
	}
	//-------------------------------------------------------------------------
	ToolManager* ToolManagerList::getToolManager(const String& _name) const
	{
		ToolManagersByName::const_iterator it = mToolManagersByName.find(_name);
		if(it == mToolManagersByName.end())
		{
			GOTHOGRE_EXCEPT("Tool manager '" << _name << "' not found"); 
		}
		return it->second;
	}
	//-------------------------------------------------------------------------
	void ToolManagerList::setDelayedActTool(Tool* _tool, const Any& _toolParameter)
	{
		mDelayedActTool = _tool;
		mDelayedActToolParameter = _toolParameter;
	}
	//-------------------------------------------------------------------------
	Tool* ToolManagerList::getDelayedActTool() const
	{
		return mDelayedActTool;
	}
	//-------------------------------------------------------------------------
	const Any& ToolManagerList::getDelayedActToolParameter() const
	{
		return mDelayedActToolParameter;
	}

} // namespace GUISystem
