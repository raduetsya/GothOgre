#ifndef GOTHOGRE_SOUND_DEF_MANAGER_EX_H
#define GOTHOGRE_SOUND_DEF_MANAGER_EX_H

#include "GothOgre_ResourceManagerEx.h"


namespace GothOgre
{
	//----------------------------------------------------------------------------------
	/** Ogre-styled resource manager for new resource type "SoundDef" */
	class GOTHOGRE_EXPORT SoundDefManager : public ResourceManager, public Singleton<SoundDefManager>
	{
	public:
		/** Default constructor. */
		SoundDefManager();

		/** Default destructor. */
		virtual ~SoundDefManager();

	protected:
		Resource* createImpl(const String& _name, ResourceHandle _handle, 
			const String& _group, bool _isManual, ManualResourceLoader* _loader, 
			const NameValuePairList* _createParams);
	};


	//----------------------------------------------------------------------------------
	/** Resource manager for new resource type "SoundDef",
	with extended support for load/save functionality. */
	class GOTHOGRE_EXPORT SoundDefManagerEx : public ResourceManagerEx,
		public Singleton<SoundDefManagerEx>
	{
	public:
		SoundDefManagerEx();
		~SoundDefManagerEx();
	
		Ogre::ResourceManager* getResourceManager() const;
		const String& getDefaultResourceGroup() const;
	};

} // namespace GothOgre

#endif // GOTHOGRE_SOUND_DEF_MANAGER_EX_H