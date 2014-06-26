#include "GothOgre_Precompiled.h"
#include "GothOgre_DefaultExtSceneManager.h"


namespace GothOgre
{
	//-----------------------------------------------------------------------
	class BridgeQueryListener : public ExtRaySceneQueryListener
	{
	public:
		BridgeQueryListener( ExtRaySceneQueryListener* _listener )
			: mDestListener( _listener )
		{
		}

		bool queryResult( MovableObject *obj, Real distance )
		{
			ExtRaySceneQueryResultEntry entry;
			entry.distance = distance;
			entry.movable = obj;
	
			entry.subEntityIndex = 0;
			entry.faceIndex = 0;
			entry.faceCoefs[0] = 0;
			entry.faceCoefs[1] = 0;
			entry.faceCoefs[2] = 0;
			entry.textureCoords = Vector2::ZERO;

			return mDestListener->queryResult(entry);
		}

		bool queryResult(SceneQuery::WorldFragment* fragment, Real distance)
		{
			return true;
		}

	private:
		ExtRaySceneQueryListener* mDestListener;
	};



	//-----------------------------------------------------------------------
	DefaultExtRaySceneQuery::DefaultExtRaySceneQuery(ExtSceneManager* _sceneManager)
		: ExtRaySceneQuery(_sceneManager), mOgreQuery(nullptr)
	{
		mOgreQuery = _sceneManager->createRayQuery(Ray());
	}
	//-----------------------------------------------------------------------
	DefaultExtRaySceneQuery::~DefaultExtRaySceneQuery()
	{
		mParentSceneMgr->destroyQuery(mOgreQuery);
	}
	//-----------------------------------------------------------------------
	void DefaultExtRaySceneQuery::execute( ExtRaySceneQueryListener* _listener )
	{
		mOgreQuery->setQueryMask( getQueryMask() );
		mOgreQuery->setQueryTypeMask( getQueryTypeMask() );
		mOgreQuery->setWorldFragmentType( getWorldFragmentType() );
		mOgreQuery->setRay( getRay() );

		BridgeQueryListener bql( _listener );
		mOgreQuery->execute( &bql );
	}



	//-----------------------------------------------------------------------
	DefaultExtSceneManager::DefaultExtSceneManager(const String& _instanceName)
		: ExtSceneManager(_instanceName)
	{
		GOTHOGRE_WARNING("The default scene manager used. Some features will be unavailable.");
	}

	DefaultExtSceneManager::~DefaultExtSceneManager()
	{
	}

	const String& DefaultExtSceneManager::getTypeName() const
	{
		return DefaultExtSceneManagerFactory::FACTORY_TYPE_NAME;
	}

	void DefaultExtSceneManager::addBsp(const BspPtr& _bsp)
	{
	}

	void DefaultExtSceneManager::removeBsp(const BspPtr& _bsp)
	{
	}

	void DefaultExtSceneManager::removeAllBsps()
	{
	}

	vector<BspPtr>::type DefaultExtSceneManager::getAllBsps() const
	{
		return vector<BspPtr>::type();
	}

	void DefaultExtSceneManager::setNonBspPartMaxSize(const Vector3& _maxSize)
	{
	}

	const Vector3& DefaultExtSceneManager::getNonBspPartMaxSize() const
	{
		return Vector3::ZERO;
	}

	void DefaultExtSceneManager::setFarClipDistance(const Real _farClipDistance)
	{
	}

	Real DefaultExtSceneManager::getFarClipDistance() const
	{
		return 0;
	}

	ExtRaySceneQuery* DefaultExtSceneManager::createRayQuery()
	{
		return new DefaultExtRaySceneQuery(this);
	}

	const String& DefaultExtSceneManager::getSector(MovableObject* _obj)
	{
		return StringUtil::BLANK;
	}

	bool DefaultExtSceneManager::isUnderwater(MovableObject* _obj)
	{
		return false;
	}


	//-----------------------------------------------------------------------
	const String DefaultExtSceneManagerFactory::FACTORY_TYPE_NAME = "GothOgre::DefaultExtSceneManager";

	void DefaultExtSceneManagerFactory::initMetaData() const
	{
		mMetaData.typeName = FACTORY_TYPE_NAME;
		mMetaData.description = FACTORY_TYPE_NAME;
		mMetaData.sceneTypeMask = ST_GOTHOGRE;
		mMetaData.worldGeometrySupported = false;
	}

	SceneManager* DefaultExtSceneManagerFactory::createInstance(const String& _instanceName)
	{
		return new DefaultExtSceneManager(_instanceName);
	}

	void DefaultExtSceneManagerFactory::destroyInstance(SceneManager* _instance)
	{
		delete _instance;
	}

	DefaultExtSceneManagerFactory::DefaultExtSceneManagerFactory()
	{
		bool registeredByPlugin = false;
		SceneManagerEnumerator::MetaDataIterator it = SceneManagerEnumerator::getSingleton().getMetaDataIterator();
		while(it.hasMoreElements())
		{
			const SceneManagerMetaData* meta = it.getNext();
			if(meta->sceneTypeMask & ST_GOTHOGRE)
			{
				registeredByPlugin = true;
				break;
			}
		}

		if(!registeredByPlugin)
			SceneManagerEnumerator::getSingleton().addFactory(this);
	}

	DefaultExtSceneManagerFactory::~DefaultExtSceneManagerFactory()
	{
		SceneManagerEnumerator::getSingleton().removeFactory(this);
	}

} // namespace GothOgre

