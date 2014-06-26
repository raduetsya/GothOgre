#ifndef GOTHOGRE_PHYSICS_H
#define GOTHOGRE_PHYSICS_H


namespace GothOgre
{
	//------------------------------------------------------------------------
	class CollisionShape
	{
	public:
		CollisionShape() {}
		virtual ~CollisionShape() {}
	};

	class BoxCollisionShape : public CollisionShape
	{
	public:
		BoxCollisionShape(const Vector3& _halfExtents, const MaterialPtr& _material) {}
	};

	class SphereCollisionShape : public CollisionShape
	{
	public:
		SphereCollisionShape(Real _radius, const MaterialPtr& _material) {}
	};

	class MeshCollisionShape : public CollisionShape
	{
	public:
		MeshCollisionShape(const MeshPtr& _mesh) {}
	};

	class ConvexCollisionShape : public CollisionShape
	{
	public:
		ConvexCollisionShape(const MeshPtr& _mesh) {}
	};



	//------------------------------------------------------------------------
	class CollisionObject
	{
	public:
		CollisionObject(MovableObject* _movableObject) 
			: mMovableObject(_movableObject) {}

		virtual ~CollisionObject() {}

		MovableObject* getMovableObject() const {return mMovableObject;}

		virtual CollisionShape* getCollisionShape() const = 0;
		virtual uint16 getCollisionGroup() const = 0;
		virtual uint16 getCollisionMask() const = 0;

		virtual void setResponseEnabled(bool _enable) = 0;
		virtual bool isResponseEnabled() const = 0;

	private:
		MovableObject*    mMovableObject;
	};

	class StaticCollisionObject : public CollisionObject
	{
	};

	class TriggerCollisionObject : public CollisionObject
	{
	};

	class DynamicCollisionObject : public CollisionObject
	{
	public:
		virtual void setMass(Real _mass) = 0;
		virtual Real getMass() const = 0;
		virtual void setLinearVelocity(const Vector3& _linear) = 0;
		virtual void setAngularVelocity(const Vector3& _angular) = 0;
		virtual Vector3 getLinearVelocity() const = 0;
		virtual Vector3 getAngularVelocity() const = 0;
	};




	//------------------------------------------------------------------------
	class CollisionEvent
	{
	public:
		CollisionEvent(CollisionObject* _collObj, const Vector3& _point = Vector3::ZERO, size_t _faceIndex = 0, const MaterialPtr& _material = MaterialPtr(),
			CollisionObject* _collObj2, const Vector3& _point2 = Vector3::ZERO, size_t _faceIndex2 = 0, const MaterialPtr& _material2 = MaterialPtr())
			: mCollisionObject(_collObj), mPoint(_point), mFaceIndex(_faceIndex), mMaterial(_material),
			  mCollisionObject2(_collObj2), mPoint2(_point2), mFaceIndex2(_faceIndex2), mMaterial2(_material)
		{
		}

		CollisionObject* getCollisionObject() const {return mCollisionObject;}
		const Vector3& getPoint() const {return mPoint;}
		size_t getFaceIndex() const {return mFaceIndex;}
		MaterialPtr getMaterial() const {return mMaterial;}

		CollisionObject* getCollisionObject2() const {return mCollisionObject2;}
		const Vector3& getPoint2() const {return mPoint2;}
		size_t getFaceIndex2() const {return mFaceIndex2;}
		MaterialPtr getMaterial2() const {return mMaterial2;}

	private:
		CollisionObject*  mCollisionObject;
		Vector3           mPoint;
		size_t            mFaceIndex;
		MaterialPtr       mMaterial;

		CollisionObject*  mCollisionObject2;
		Vector3           mPoint2;
		size_t            mFaceIndex2;
		MaterialPtr       mMaterial2;
	};


	class CollisionListener
	{
	public:

		virtual void intersectedByRay(const CollisionEvent& _evt) {}

		virtual void culledByFrustrum(const CollisionEvent& _evt) {}

		virtual void objectsCollided(const CollisionEvent& evt) {}

		virtual void enteredToTriggerArea(const CollisionEvent& _evt) {}
	};

	//------------------------------------------------------------------------
	class PhysicalScene
	{
	public:
		PhysicalScene() {}
		virtual ~PhysicalScene() {}

		const String& getName() const {return mName;}
		PhysicalSceneManager* getCreator() const {return mCreator;}

		virtual void setGravity(const Vector3& _g) = 0;
		virtual Vector3 getGravity() const = 0;

		virtual BoxCollisionShape*     createBoxShape(const Vector3& _halfExtents, const MaterialPtr& _material) = 0;
		virtual SphereCollisionShape*  createSphereShape(Real _radius, const MaterialPtr& _material) = 0;
		virtual CapsuleCollisionShape* createCapsuleShape(Real _radius, Real _height, const MaterialPtr& _material) = 0;
		virtual ConvexCollisionShape*  createConvexShape(const MeshPtr& _mesh) = 0;
		virtual MeshCollisionShape*    createMeshShape(const MeshPtr& _mesh) = 0;

		virtual StaticCollisionObject*  createStaticObject(MovableObject* _movable, CollisionShape* _collisionShape, uint16 _collisionGroup, uint16 _collisionMask);
		virtual DynamicCollisionObject* createDynamicObject(MovableObject* _movable, CollisionShape* _collisionShape, uint16 _collisionGroup, uint16 _collisionMask);
		virtual TriggerCollisionObject* createTriggerObject(MovableObject* _movable, CollisionShape* _collisionShape, uint16 _collisionGroup, uint16 _collisionMask);

		virtual void raycast(const Ray& _ray, uint16 _mask, CollisionListener* _listener);
		virtual void cull(const Frustrum* _frustrum, uint16 _mask, CollisionListener* _listener);

		virtual void stepSimulation(Real _seconds);
		void addCollisionListener(CollisionListener* _listener);
		void removeCollisionListener(CollisionListener* _listener);

	public:
		// Used internally by PhysicEngine
		void _setName(const String& _name);

	private:
		typedef SafeList<CollisionListener*>  CollisionListener;
		CollisionListener                     mCollisionListener;
		String                                mName;
		PhysicalSceneManager*                 mCreator;
	};



	class PhysicalSceneManager
	{
	public:
		PhysicalSceneManager();
		virtual ~PhysicalSceneManager();
		virtual const String& getName() = 0;

		PhysicalScene* createScene(const String& _name);
		void destroyScene(PhysicalScene* _scene);
		PhysicalScene* getScene(const String& _name) const;

	protected:
		virtual PhysicalScene* createSceneImpl() = 0;

	private:
		typedef map<String, PhysicScene*>::type  SceneByName;
		SceneByName  mSceneByName;
	};


	class PhysicalSceneManagerFactory
	{
	public:
		PhysicalSceneManagerFactory();
		virtual ~PhysicalSceneManagerFactory();

		virtual PhysicalSceneManager* createSceneManager();
		virtual void destroySceneManager(PhysicalSceneManager* _sm);
	};


	class PhysicalSceneManagerEnumerator : public Singleton<PhysicalSceneManagerEnumerator>
	{
	public:
		PhysicEngineManagerEnumerator();
		~PhysicEngineManagerEnumerator();

		void addFactory(PhysicalSceneManagerFactory* _engine);
		void removeFactory(PhysicalSceneManagerFactory* _engine);

		PhysicalSceneManager* createSceneManager();
		void destroySceneManager(PhysicalSceneManager* _sm);
		PhysicalSceneManager* getSceneManager();

	private:
		typedef list<PhysicalSceneManagerFactory*>::type  FactoryList;
		FactoryList            mFactoryList;
		PhysicalSceneManager*  mSceneManager;
	};



	class Physics : public Singleton<Physics>
	{
	public:

	private:
		PhysicalScene* mMainPhysicalScene;
		PhysicalScene* mSoundScene;
	};
}

#endif // GOTHOGRE_PHYSICS_H