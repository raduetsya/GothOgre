#include "GothOgre_Precompiled.h"
#include "GothOgre_EntitySubtypes.h"
#include "GothOgre_SceneManager.h"


namespace GothOgre
{
	//-----------------------------------------------------------------------
	// BspEntity

	String BspEntityFactory::FACTORY_TYPE_NAME = "BspEntity";
	
	BspEntityFactory::BspEntityFactory()
	{
		Root::getSingleton().addMovableObjectFactory(this);
		SceneManager::BSP_ENTITY_TYPE_MASK = getTypeFlags();
	}

	BspEntityFactory::~BspEntityFactory()
	{
		Root::getSingleton().removeMovableObjectFactory(this);
	}

	bool BspEntityFactory::requestTypeFlags() const
	{
		return true;
	}

	const String& BspEntityFactory::getType() const
	{
		return FACTORY_TYPE_NAME;
	}

	uint32 BspEntity::getTypeFlags() const
	{
		return SceneManager::BSP_ENTITY_TYPE_MASK;
	}



	//-----------------------------------------------------------------------
	// LightEntity

	String LightEntityFactory::FACTORY_TYPE_NAME = "LightEntity";
	
	LightEntityFactory::LightEntityFactory()
	{
		Root::getSingleton().addMovableObjectFactory(this);
		SceneManager::LIGHT_ENTITY_TYPE_MASK = getTypeFlags();
	}

	LightEntityFactory::~LightEntityFactory()
	{
		Root::getSingleton().removeMovableObjectFactory(this);
	}

	bool LightEntityFactory::requestTypeFlags() const
	{
		return true;
	}

	const String& LightEntityFactory::getType() const
	{
		return FACTORY_TYPE_NAME;
	}

	uint32 LightEntity::getTypeFlags() const
	{
		return SceneManager::LIGHT_ENTITY_TYPE_MASK;
	}



	//-----------------------------------------------------------------------
	// SoundEntity

	String SoundEntityFactory::FACTORY_TYPE_NAME = "SoundEntity";
	
	SoundEntityFactory::SoundEntityFactory()
	{
		Root::getSingleton().addMovableObjectFactory(this);
		SceneManager::SOUND_ENTITY_TYPE_MASK = getTypeFlags();
	}

	SoundEntityFactory::~SoundEntityFactory()
	{
		Root::getSingleton().removeMovableObjectFactory(this);
	}

	bool SoundEntityFactory::requestTypeFlags() const
	{
		return true;
	}

	const String& SoundEntityFactory::getType() const
	{
		return FACTORY_TYPE_NAME;
	}

	uint32 SoundEntity::getTypeFlags() const
	{
		return SceneManager::SOUND_ENTITY_TYPE_MASK;
	}




	//-----------------------------------------------------------------------
	// WaynetEntity

	String WaynetEntityFactory::FACTORY_TYPE_NAME = "WaynetEntity";
	
	WaynetEntityFactory::WaynetEntityFactory()
	{
		Root::getSingleton().addMovableObjectFactory(this);
		SceneManager::WAYNET_ENTITY_TYPE_MASK = getTypeFlags();
	}

	WaynetEntityFactory::~WaynetEntityFactory()
	{
		Root::getSingleton().removeMovableObjectFactory(this);
	}

	bool WaynetEntityFactory::requestTypeFlags() const
	{
		return true;
	}

	const String& WaynetEntityFactory::getType() const
	{
		return FACTORY_TYPE_NAME;
	}

	uint32 WaynetEntity::getTypeFlags() const
	{
		return SceneManager::WAYNET_ENTITY_TYPE_MASK;
	}
}