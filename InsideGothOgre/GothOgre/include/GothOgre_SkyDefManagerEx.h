#ifndef GOTHOGRE_SKY_DEF_MANAGER_EX_H
#define GOTHOGRE_SKY_DEF_MANAGER_EX_H

#include "GothOgre_ResourceManagerEx.h"


namespace GothOgre
{
	//----------------------------------------------------------------------------------
	/** Ogre-styled resource manager for new resource type "SkyDef" */
	class GOTHOGRE_EXPORT SkyDefManager : public ResourceManager, public Singleton<SkyDefManager>
	{
	public:
		/** Default constructor. */
		SkyDefManager();

		/** Default destructor. */
		virtual ~SkyDefManager();

	protected:
		Resource* createImpl(const String& _name, ResourceHandle _handle, 
			const String& _group, bool _isManual, ManualResourceLoader* _loader, 
			const NameValuePairList* _createParams);
	};


	//----------------------------------------------------------------------------------
	/** Resource manager for new resource type "SkyDef",
	with extended support for load/save functionality. */
	class GOTHOGRE_EXPORT SkyDefManagerEx : public ResourceManagerEx,
		public Singleton<SkyDefManagerEx>
	{
	public:
		SkyDefManagerEx();
		~SkyDefManagerEx();
		
		Ogre::ResourceManager* getResourceManager() const;
		const String& getDefaultResourceGroup() const;
	};

	//------------------------------------------------------------------------
	/** Prototype for future skydef loaders. */
	class GOTHOGRE_EXPORT SkyDefLoader : public ResourceLoader
	{
	public:
		SkyDefLoader(const String& _name, Priority _priority = Priority::LOWEST);
	};

} // namespace GothOgre

#endif // GOTHOGRE_SKY_DEF_MANAGER_EX_H