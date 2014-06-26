#ifndef GOTHOGRE_MTL_MANAGER_EX_H
#define GOTHOGRE_MTL_MANAGER_EX_H

#include "GothOgre_ResourceManagerEx.h"


namespace GothOgre
{
	//----------------------------------------------------------------------------------
	/** Ogre-styled resource manager for new resource type "Mtl" */
	class GOTHOGRE_EXPORT MtlManager : public ResourceManager, public Singleton<MtlManager>
	{
	public:
		/** Default constructor. */
		MtlManager();

		/** Default destructor. */
		virtual ~MtlManager();

	protected:
		Resource* createImpl(const String& _name, ResourceHandle _handle, 
			const String& _group, bool _isManual, ManualResourceLoader* _loader, 
			const NameValuePairList* _createParams);
	};


	//----------------------------------------------------------------------------------
	/** Resource manager for new resource type "Mtl",
	with extended support for load/save functionality. */
	class GOTHOGRE_EXPORT MtlManagerEx : public ResourceManagerEx,
		public Singleton<MtlManagerEx>
	{
	public:
		MtlManagerEx();
		~MtlManagerEx();
		
		Ogre::ResourceManager* getResourceManager() const;
	};

} // namespace GothOgre

#endif // GOTHOGRE_MTL_MANAGER_EX_H