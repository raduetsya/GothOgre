#ifndef HACK_RESOURCE_GROUP_MANAGER_H
#define HACK_RESOURCE_GROUP_MANAGER_H

namespace GothOgre
{
	namespace OgreHacks
	{

StringVectorPtr  listResourceNames (const String &groupName, bool dirs=false) 
  List all file or directory names in a resource group. 
 
FileInfoListPtr  listResourceFileInfo (const String &groupName, bool dirs=false) 
  List all files in a resource group with accompanying information.  


		// Find all file or directory names matching a given pattern in a resource group.  
		StringVectorPtr  findResourceNames (const String &groupName, const String &pattern, bool dirs=false) 

		// Find all files or directories matching a given pattern in a group and get some detailed information about them.  
		void FileInfoListPtr findResourceFileInfo (const String &group, const String &pattern, bool dirs=false) 



	};
}

#endif // HACK_RESOURCE_GROUP_MANAGER_H