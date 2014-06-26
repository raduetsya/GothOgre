#ifndef GOTHOGRE_ZEN_MANAGER_H
#define GOTHOGRE_ZEN_MANAGER_H

#include "GothOgre_ZenObject.h"

namespace GothOgre
{
	class ZenFactory;

	/** Class containing a list of registered ZenFactory-derived objects.
	This is a singleton, there is an only one instance of the class. */
	class GOTHOGRE_EXPORT ZenManager : public Singleton<ZenManager>
	{
	public:
		ZenManager();
		~ZenManager();

		/** Find a factory by type name. The function can use as the normal type name
		(see ZenFactory::getType) as any of alternative names 
		(see ZenFactory::getSupportedNames).
		\return A found factory or nullptr if the specified type was not found. */
		ZenFactory* getZenFactory(const String& _typeName);

		/** Create an object by type name. 
		The function searchs an appropriate factory and then it creates an object.
		\return A newly created object or nullptr if the specified type was not found. */
		ZenObjectPtr createZenObject(const String& _typeName);

	private:
		friend class ZenFactory;

		/** Register a factory */
		void addZenFactory(ZenFactory* _zenObjectFactory);
		
		/** Unregister a factory */
		void removeZenFactory(ZenFactory* _zenObjectFactory);

		/** Scan supported type names for every factory. */
		void scanSupportedTypeNames();

	private:
		typedef HashMap<String, ZenFactory*> TypeNameFactoryMap;
		TypeNameFactoryMap		mTypeNameFactoryMap;
	};

} // namespace GothOgre

#endif // GOTHOGRE_ZEN_MANAGER_H