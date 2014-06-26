#ifndef GOTHOGRE_BSP_MANAGER_EX_H
#define GOTHOGRE_BSP_MANAGER_EX_H

#include "GothOgre_ResourceManagerEx.h"


namespace GothOgre
{

	//------------------------------------------------------------------------
	/** Ogre-styled resource manager for new resource type "Bsp" */
	class GOTHOGRE_EXPORT BspManager : public ResourceManager, public Singleton<BspManager>
	{
	public:
		/** Default constructor. */
       BspManager();

		/** Default destructor. */
		virtual ~BspManager();

	protected:
		virtual Resource* createImpl(const String& _name, ResourceHandle _handle, 
			const String& _group, bool _isManual, ManualResourceLoader* _loader, 
			const NameValuePairList* _createParams);
	};


	//------------------------------------------------------------------------
	/** Resource manager for new resource type "Bsp",
	with extended support for load/save functionality. */
	class GOTHOGRE_EXPORT BspManagerEx : public Singleton<BspManagerEx>, 
		public ResourceManagerEx
	{
	public:
		BspManagerEx();
		~BspManagerEx();
		
		virtual const String& getDefaultResourceGroup() const;
		virtual Ogre::ResourceManager* getResourceManager() const;
		virtual bool isCaseSensitive() const;
	};

} // namespace GothOgre

#endif // GOTHOGRE_BSP_MANAGER_EX_H