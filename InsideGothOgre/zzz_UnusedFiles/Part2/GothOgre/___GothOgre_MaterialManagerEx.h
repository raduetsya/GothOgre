#ifndef GOTHOGRE_MATERIAL_MANAGER_EX_H
#define GOTHOGRE_MATERIAL_MANAGER_EX_H

#include "GothOgre_ResourceManagerEx.h"


namespace GothOgre
{
	//------------------------------------------------------------------------
	/** Version of the MaterialManager class with better function "createManual". */
	class GOTHOGRE_EXPORT MaterialManagerEx : public Singleton<MaterialManagerEx>, 
		public ResourceManagerEx
	{
	public:
		MaterialManagerEx();
		~MaterialManagerEx();
		
		virtual const String& getDefaultResourceGroup() const;
		virtual Ogre::ResourceManager* getResourceManager() const;
		virtual bool isCaseSensitive() const;
	};

} //  namespace GothOgre

#endif // GOTHOGRE_MATERIAL_MANAGER_EX_H