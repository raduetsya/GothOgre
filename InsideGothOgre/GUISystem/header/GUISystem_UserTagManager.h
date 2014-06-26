#ifndef GUISYSTEM_USER_TAG_MANAGER_H
#define GUISYSTEM_USER_TAG_MANAGER_H


namespace GUISystem
{
	/** This class makes possible to read xml files containing tags. 
	These tags are used as constants.
	The "MyGUI" library also can use tags in language files,
	however I need a possibility to read tags which are not associated
	with any language. */
	class UserTagManager
	{
	public:
		UserTagManager();
		~UserTagManager();

		void initialise();
		void shutdown();

		void _load(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version);
	};
}

#endif // GUISYSTEM_USER_TAG_MANAGER_H