#include "GothOgre_StdHeaders.h"
#include "GothOgre_BspEntityFactory.h"
#include "GothOgre_BspManager.h"

namespace GothOgre
{
	//-----------------------------------------------------------------------
	String BspEntityFactory::FACTORY_TYPE_NAME = "BspEntity";
	//-----------------------------------------------------------------------
	const String& BspEntityFactory::getType(void) const
	{
		return FACTORY_TYPE_NAME;
	}
	//-----------------------------------------------------------------------
	BspEntityFactory::BspEntityFactory()
	{
	}
	//-----------------------------------------------------------------------
	BspEntityFactory::~BspEntityFactory()
	{
	}
	//-----------------------------------------------------------------------
	void BspEntityFactory::initialize()
	{
		Root::getSingleton().addMovableObjectFactory(this);
	}
	//-----------------------------------------------------------------------
	void BspEntityFactory::shutdown()
	{
		Root::getSingleton().removeMovableObjectFactory(this);
	}
	//-----------------------------------------------------------------------
	MovableObject* BspEntityFactory::createInstanceImpl( const String& _name,
		const NameValuePairList* _params)
	{
		// must have mesh parameter
		BspPtr pBsp;
		if (_params != 0)
		{
			String groupName = ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME;

			NameValuePairList::const_iterator ni;
			ni = _params->find("resourceGroup");
			if (ni != _params->end())
			{
				groupName = ni->second;
			}

			ni = _params->find("bsp");
			if (ni != _params->end())
			{
				// Get bsp (load if required)
				pBsp = BspManager::getSingleton().createOrRetrieve( ni->second, groupName ).first;
			}

		}

		if(pBsp.isNull())
		{
			GOTHOGRE_EXCEPT(
				"'bsp' parameter required when constructing a BspEntity.");
		}
		return OGRE_NEW BspEntity(_name, pBsp);
	}
	//-----------------------------------------------------------------------
	void BspEntityFactory::destroyInstance( MovableObject* _obj)
	{
		OGRE_DELETE _obj;
	}



} // namespace GothOgre