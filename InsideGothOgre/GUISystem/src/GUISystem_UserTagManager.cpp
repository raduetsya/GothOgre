#include "GUISystem_Precompiled.h"
#include "GUISystem_UserTagManager.h"


namespace GUISystem
{
	//-------------------------------------------------------------------------
	const std::string XML_TYPE("UserTags");
	//-------------------------------------------------------------------------
	UserTagManager::UserTagManager()
	{
	}
	//-------------------------------------------------------------------------
	UserTagManager::~UserTagManager()
	{
	}
	//-------------------------------------------------------------------------
	void UserTagManager::initialise()
	{
		MyGUI::ResourceManager::getInstance().registerLoadXmlDelegate(XML_TYPE) 
			= MyGUI::newDelegate(this, &UserTagManager::_load);
	}
	//-------------------------------------------------------------------------
	void UserTagManager::shutdown()
	{
		if(MyGUI::ResourceManager::getInstancePtr())
			MyGUI::ResourceManager::getInstance().unregisterLoadXmlDelegate(XML_TYPE);
	}
	//-------------------------------------------------------------------------
	void UserTagManager::_load(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version)
	{
		MyGUI::LanguageManager::getInstance().loadUserTags(_file);
	}

} // namespace GUISystem