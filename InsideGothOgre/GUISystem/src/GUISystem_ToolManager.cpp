#include "GUISystem_Precompiled.h"
#include "GUISystem_ToolManager.h"
#include "GUISystem_ToolManagerList.h"
#include "GUISystem_ToolFactory.h"
#include "GUISystem_ToolOperation.h"
#include "GUISystem_Tool.h"

namespace GUISystem
{
	//---------------------------------------------------------------------
	ToolManager::ToolManager(const String& _name)
	{
		mName = _name;
		ToolManagerList::getSingleton().addToolManager(this);
	}
	//---------------------------------------------------------------------
	ToolManager::~ToolManager()
	{
		destroyTools();
		ToolManagerList::getSingleton().removeToolManager(this);
	}
	//---------------------------------------------------------------------
	const String& ToolManager::getName() const
	{
		return mName;
	}
	//---------------------------------------------------------------------
	void ToolManager::addToolFactory(ToolFactory* _toolFactory)
	{
		GOTHOGRE_ASSERT(mToolsByAddOrder.empty(), 
			_toolFactory->getName() << ": It's impossible to add tool factory at this moment.");

		if(std::find(mToolFactories.begin(), mToolFactories.end(), _toolFactory)
			== mToolFactories.end())
		{
			mToolFactories.push_back(_toolFactory);
			GOTHOGRE_INFO("Tool factory " << _toolFactory->getName()
				<< " added to tool manager " << getName());
		}
	}
	//---------------------------------------------------------------------
	void ToolManager::removeToolFactory(ToolFactory* _toolFactory)
	{
		GOTHOGRE_ASSERT(mToolsByAddOrder.empty(), 
			_toolFactory->getName() << ": It's impossible to remove tool factory at this moment.");

		ToolFactories::iterator it = std::find(mToolFactories.begin(), mToolFactories.end(), _toolFactory);
		if(it != mToolFactories.end())
		{
			mToolFactories.erase(it);
			GOTHOGRE_INFO("Tool factory " << _toolFactory->getName()
				<< " removed from tool manager " << getName());
		}
	}
	//---------------------------------------------------------------------
	void ToolManager::createTools()
	{
		if(!mToolFactories.empty() && mToolsByAddOrder.empty())
		{
			GOTHOGRE_INFO("Tool manager " << getName() << " creating tools");
		
			for(ToolFactories::iterator it = mToolFactories.begin(); 
				it != mToolFactories.end(); ++it)
			{
				ToolFactory* toolFactory = *it;
				Tool* tool = toolFactory->createTool();
				tool->init(toolFactory, this);
				mToolsByAddOrder.push_back(tool);
				mToolsByName.insert(std::make_pair(tool->getName(), tool));
				
				Tool* delayedActTool = ToolManagerList::getSingleton().getDelayedActTool();
				if(delayedActTool == tool)
				{
					const Any& toolParameter = ToolManagerList::getSingleton().getDelayedActToolParameter();
					delayedActTool->activateTool(toolParameter);
					ToolManagerList::getSingleton().setDelayedActTool(nullptr);
				}
			}
	
			GOTHOGRE_INFO("Tool manager " << getName() << " created tools successfully");
		}
	}
	//---------------------------------------------------------------------
	void ToolManager::destroyTools()
	{
		if(!mToolsByAddOrder.empty())
		{
			GOTHOGRE_INFO("Tool manager " << getName() << " destroying tools");
			
			ToolOperation* toolOperation = getActiveOperation();
			if(toolOperation && toolOperation->getTool()->getToolManager() == this)
			{
				delete toolOperation;
				setActiveOperation(nullptr);
			}

			for(Tools::reverse_iterator it = mToolsByAddOrder.rbegin(); 
				it != mToolsByAddOrder.rend(); ++it)
			{
				Tool* tool = *it;
				delete tool;
			}
			mToolsByAddOrder.clear();
			mToolsByName.clear();
			mPreviousTools.clear();
			
			GOTHOGRE_INFO("Tool manager " << getName() << " destroyed tools successfully");
		}
	}
	//---------------------------------------------------------------------
	Tool* ToolManager::getTool(const String& _typeName) const
	{
		ToolsByName::const_iterator it = mToolsByName.find(_typeName);
		if(it != mToolsByName.end())
			return it->second;
		else
			return nullptr;
	}
	//---------------------------------------------------------------------
	ToolOperation* ToolManager::getActiveOperation() const
	{
		return ToolManagerList::getSingleton().getActiveOperation();
	}
	//---------------------------------------------------------------------
	void ToolManager::setActiveOperation(ToolOperation* _toolOperation)
	{
		ToolManagerList::getSingleton().setActiveOperation(_toolOperation);
	}
	//---------------------------------------------------------------------
	ToolOperation* ToolManager::activateTool(Tool* _tool, const Any& _toolParameter)
	{
		ToolOperation* toolOperation = getActiveOperation();
		if(toolOperation && toolOperation->getTool() == _tool)
			return toolOperation;

		if(toolOperation)
		{
			delete toolOperation;
			setActiveOperation(nullptr);
		}

		for(PreviousTools::iterator it = mPreviousTools.begin();
			it != mPreviousTools.end(); ++it)
		{
			if((*it).tool == _tool)
			{
				mPreviousTools.erase(it);
				break;
			}
		}

		mPreviousTools.push_front(ToolAndParam());
		ToolAndParam& tap = mPreviousTools.front();
		tap.tool = _tool;
		tap.parameter = _toolParameter;

		GOTHOGRE_INFO("Tool " << _tool->getName() << " activating");
		toolOperation = _tool->activate(_toolParameter);
		setActiveOperation(toolOperation);

		if(toolOperation && !toolOperation->mDeactivated)
		{
			toolOperation->init(_tool);
			return toolOperation;
		}
		else
		{
			deactivateTool(toolOperation);
			return nullptr;
		}
	}
	//---------------------------------------------------------------------
	void ToolManager::deactivateTool(ToolOperation* _toolOperation)
	{
		GOTHOGRE_ASSERT(getActiveOperation() == _toolOperation, "Error");
		
		if(_toolOperation)
		{
			delete _toolOperation;
			setActiveOperation(nullptr);
		}
		
		ToolAndParam tap = mPreviousTools.front();
		mPreviousTools.pop_front();
		
		if(!mPreviousTools.empty())
		{
			tap = mPreviousTools.front();
			activateTool(tap.tool, tap.parameter);
		}
	}

} // namespace GUISystem
