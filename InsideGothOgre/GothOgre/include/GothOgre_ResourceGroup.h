#ifndef GOTHOGRE_RESOURCE_GROUP_H
#define GOTHOGRE_RESOURCE_GROUP_H

namespace GothOgre
{
	/** Strings identifying resource groups. 
	Resource group defines a set of pathes where resources can be found. */
	struct GOTHOGRE_EXPORT ResourceGroup
	{
		// Game resource groups
		static const String TEXTURES;
		static const String MESHES;
		static const String SKIES;
		static const String WORLDS;
		static const String SOUND;
		static const String MUSIC;

		// Resource groups with gui elements
		static const String GUIMEDIA;

		// Resource group for resources which were created 
		// programmatically, exist in the computer's memory only, 
		// and cannot be loaded from files or saved.
		static const String MANUAL;
	};
}

#endif // GOTHOGRE_RESOURCE_GROUP_H