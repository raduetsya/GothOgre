#ifndef GOTHOGRE_BSP_ENTITY_FACTORY_H
#define GOTHOGRE_BSP_ENTITY_FACTORY_H

#include "GothOgre_BspEntity.h"

namespace GothOgre
{
	
	/** Factory object for creating BspEntity instances */
	class GOTHOGRE_EXPORT BspEntityFactory : public MovableObjectFactory
	{
	protected:
		MovableObject* createInstanceImpl( const String& _name, const NameValuePairList* _params);

	public:
		BspEntityFactory();
		~BspEntityFactory();

		void initialize();
		void shutdown();

		static String FACTORY_TYPE_NAME;

		const String& getType() const;
		void destroyInstance( MovableObject* _obj);
	};

} // namespace GothOgre

#endif // GOTHOGRE_BSP_ENTITY_FACTORY_H