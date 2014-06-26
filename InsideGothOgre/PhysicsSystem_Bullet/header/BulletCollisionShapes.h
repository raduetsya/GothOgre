



	class BulletShapeHolder
	{
	public:
		BulletShapeHolder() 
		{
			mBtShape = nullptr;
		}

		~BulletShapeHolder()
		{
			if(mBtShape)
				delete mBtShape;
		}

		btCollisionShape* getBulletShape() const 
		{
			return mBtShape;
		}

	protected:
		btCollisionShape* mBtShape;
	};



	class BulletBoxCollisionShape : public BoxCollisionShape, public BulletShapeHolder
	{
	public:
		BulletBoxCollisionShape(const Vector3& _halfExtents)
		{
			mBtShape = new btBoxShape( BulletConverter::toBullet(_halfExtents) );
		}
	};


	class SphereCollisionShape : public CollisionShape, public BulletShapeHolder
	{
	public:
		SphereCollisionShape(Real _radius)
		{
			mBtShape = new btSphereShape( BulletConverter::toBullet(_radius));
		}
	};


	class MeshCollisionShape : public CollisionShape
	{
	public:
		MeshCollisionShape(const MeshPtr& _mesh) 
		{
			mSMI = new btTriangleIndexVertexArray;



			mBtShapePtr
		}

		btCollisionShape* getBulletShape() const
		{
			return &mBtShape;
		}

	protected:
		btStridingMeshInterface* mSMI;
	};

	class ConvexCollisionShape : public CollisionShape
	{
	public:
		ConvexCollisionShape(const MeshPtr& _mesh) {}
	};