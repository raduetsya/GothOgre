#include "GothOgre_StdHeaders.h"
#include "GothOgre_ExactRayQuery.h"

namespace GothOgre
{
	ExactRayQuery::ExactRayQuery()
	{
		init();
	}
	//--------------------------------------------------------------------------------------
	ExactRayQuery::ExactRayQuery(Ogre::SceneManager* _sceneManager, const Ogre::Ray& _ray)
	{
		init();
		setSceneManager(_sceneManager);
		setRay(_ray);
	}
	//--------------------------------------------------------------------------------------
	ExactRayQuery::ExactRayQuery(const Ogre::Ray& _ray, Ogre::SceneManager* _sceneManager)
	{
		init();
		setSceneManager(_sceneManager);
		setRay(_ray);
	}
	//--------------------------------------------------------------------------------------
	ExactRayQuery::ExactRayQuery(Real _relx, Real _rely, Ogre::Camera* _camera)
	{
		init();
		setRay(_relx, _rely, _camera);
	}
	//--------------------------------------------------------------------------------------
	ExactRayQuery::ExactRayQuery(Real _relx, Real _rely, Ogre::Viewport* _viewport)
	{
		init();
		setRay(_relx, _rely, _viewport);
	}
	//--------------------------------------------------------------------------------------
	ExactRayQuery::ExactRayQuery(int _x, int _y, Ogre::RenderTarget* _renderTarget)
	{
		init();
		setRay(_x, _y, _renderTarget);
	}
	//--------------------------------------------------------------------------------------
	ExactRayQuery::~ExactRayQuery()
	{
		destroyRaySceneQuery();
	}
	//--------------------------------------------------------------------------------------
	void ExactRayQuery::init()
	{
		mSceneManager = nullptr;
		mRaySceneQuery = nullptr;
		mCullingMode = Ogre::CULL_CLOCKWISE;
	}
	//--------------------------------------------------------------------------------------
	void ExactRayQuery::createRaySceneQuery()
	{
		if(!mRaySceneQuery)
		{
			assert(mSceneManager);
			mRaySceneQuery = mSceneManager->createRayQuery(Ogre::Ray());
			mRaySceneQuery->setQueryTypeMask( SceneManager::ENTITY_TYPE_MASK);
			mRaySceneQuery->setSortByDistance(true);
		}
	}
	//--------------------------------------------------------------------------------------
	void ExactRayQuery::destroyRaySceneQuery()
	{
		if(mRaySceneQuery)
		{
			assert(mSceneManager);
			mSceneManager->destroyQuery(mRaySceneQuery);
			mRaySceneQuery = nullptr;
		}
	}
	//--------------------------------------------------------------------------------------
	Ogre::SceneManager* ExactRayQuery::getSceneManager() const
	{
		return mSceneManager;
	}
	//--------------------------------------------------------------------------------------
	void ExactRayQuery::setSceneManager(Ogre::SceneManager* _sceneManager)
	{
		if(mSceneManager)
		{
			if(mSceneManager != _sceneManager)
			{
				destroyRaySceneQuery();
			}
		}
		mSceneManager = _sceneManager;
		createRaySceneQuery();
	}
	//--------------------------------------------------------------------------------------
	const Ogre::Ray& ExactRayQuery::getRay() const
	{
		return mRaySceneQuery->getRay();
	}
	//--------------------------------------------------------------------------------------
	void ExactRayQuery::setRay(const Ogre::Ray& _ray)
	{
		mRaySceneQuery->setRay(_ray);
	}
	//--------------------------------------------------------------------------------------
	void ExactRayQuery::setRay(Real _relx, Real _rely, Ogre::Camera* _camera)
	{
		Ogre::Ray ray = _camera->getCameraToViewportRay(_relx, _rely);
		setSceneManager(_camera->getSceneManager());
		setRay(ray);
	}
	//--------------------------------------------------------------------------------------
	void ExactRayQuery::setRay(Real _relx, Real _rely, Ogre::Viewport* _viewport)
	{
		Ogre::Camera* camera = _viewport->getCamera();
		setRay(_relx, _rely, camera);
	}
	//--------------------------------------------------------------------------------------
	void ExactRayQuery::setRay(int _x, int _y, Ogre::RenderTarget* _renderTarget)
	{
		Real relx = Real( _x ) / Real( _renderTarget->getWidth() );
		Real rely = Real( _y ) / Real( _renderTarget->getHeight() );
		for(unsigned short i = 0; i != _renderTarget->getNumViewports(); ++i)
		{
			Ogre::Viewport* viewport = _renderTarget->getViewport(i);
			if(viewport->getLeft() < relx && relx < viewport->getLeft() + viewport->getWidth()
				&& viewport->getTop() < rely && rely < viewport->getTop() + viewport->getHeight())
			{
				setRay(relx, rely, viewport);
				break;
			}
		}
	}
	//--------------------------------------------------------------------------------------
	void ExactRayQuery::setCullingMode(Ogre::CullingMode _cullingMode)
	{
		mCullingMode = _cullingMode;
	}
	//--------------------------------------------------------------------------------------
	Ogre::CullingMode ExactRayQuery::getCullingMode() const
	{
		return mCullingMode;
	}
	//--------------------------------------------------------------------------------------
	Ogre::RaySceneQueryResult& ExactRayQuery::execute()
	{
		mRaySceneQuery->clearResults();
		Ogre::RaySceneQueryResult& rawResult = mRaySceneQuery->execute();
		
		mExactResult.clear();
		for(size_t i = 0; i != rawResult.size(); ++i)
		{
			Entity* entity = static_cast<Entity*>(rawResult[i].movable);
			if(entity)
			{
				std::pair<bool, Real> entityIntersection = 
					intersects( mRaySceneQuery->getRay(), entity, mCullingMode );

				if(entityIntersection.first)
				{
					Ogre::RaySceneQueryResultEntry entry = rawResult[i];
					entry.distance = entityIntersection.second;
					mExactResult.push_back(entry);
				}
			}
		}

		std::sort(mExactResult.begin(), mExactResult.end());
		return mExactResult;
	}
	//--------------------------------------------------------------------------------------
	const Ogre::RaySceneQueryResult& ExactRayQuery::getLastResults() const
	{
		return mExactResult;
	}
	//--------------------------------------------------------------------------------------
	Ogre::Entity* ExactRayQuery::getNearestEntity() const
	{
		if(mExactResult.empty())
			return nullptr;
		else
			return static_cast<Ogre::Entity*>(mExactResult[0].movable);
	}
	//--------------------------------------------------------------------------------------
	Ogre::Real ExactRayQuery::getNearestDistance() const
	{
		if(mExactResult.empty())
			return -1;
		else
			return mExactResult[0].distance;
	}
	//--------------------------------------------------------------------------------------
	void ExactRayQuery::clearResults()
	{
		mExactResult.clear();
	}
	//--------------------------------------------------------------------------------------
	Ogre::Ray ExactRayQuery::transformToLocalSpace( const Ogre::Ray& _ray, Ogre::SceneNode* _sceneNode )
	{
		const Ogre::Vector3& pos = _sceneNode->_getDerivedPosition();
		const Ogre::Vector3& scale = _sceneNode->_getDerivedScale();
		Ogre::Vector3 invScale = 1 / scale;
		const Ogre::Quaternion& orient = _sceneNode->_getDerivedOrientation();
		Ogre::Quaternion invOrient = orient.Inverse();

		Ogre::Ray ret;
		ret.setOrigin(invOrient * (_ray.getOrigin() - pos) * invScale);
		ret.setDirection(invOrient * (_ray.getOrigin() + _ray.getDirection() - pos) * invScale - ret.getOrigin());
		return ret;
	}
	//--------------------------------------------------------------------------------------
	std::pair<bool, Real> ExactRayQuery::intersects( const Ogre::Ray& _ray,
		Ogre::Entity* _entity, Ogre::CullingMode _cullingMode )
	{
		std::pair<bool, Real> ret;
		ret.first = false;

		Ogre::Ray transformedRay = transformToLocalSpace(_ray, _entity->getParentSceneNode());

		bool positiveSide = (_cullingMode != Ogre::CULL_ANTICLOCKWISE);
		bool negativeSide = (_cullingMode != Ogre::CULL_CLOCKWISE);

		Ogre::HardwareVertexBufferSharedPtr sharedHvb;
		const void* sharedVertexBuffer = nullptr;
		size_t sharedVertexStart, sharedVertexCount, sharedVertexSize, sharedVertexPositionOffset;
		
		Ogre::MeshPtr mesh = _entity->getMesh();
		if(!mesh.isNull() && mesh->sharedVertexData)
		{
			VertexData* sharedVertexData = mesh->sharedVertexData;
			const Ogre::VertexElement* sharedPosElem = sharedVertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
			if(sharedPosElem)
			{
				sharedHvb = sharedVertexData->vertexBufferBinding->getBuffer(sharedPosElem->getSource());
				if(!sharedHvb.isNull())
				{
					sharedVertexBuffer = sharedHvb->lock(Ogre::HardwareBuffer::HBL_READ_ONLY);
					sharedVertexStart = sharedVertexData->vertexStart;
					sharedVertexCount = sharedVertexData->vertexCount;
					sharedVertexSize = sharedHvb->getVertexSize();
					sharedVertexPositionOffset = sharedPosElem->getOffset();
				}
			}
		}
		
		if(!mesh.isNull())
		{
			for(size_t i = 0; i != _entity->getNumSubEntities(); ++i)
			{
				Ogre::SubEntity* subEntity = _entity->getSubEntity(i);
				if(!subEntity) continue;
				Ogre::SubMesh* subMesh = subEntity->getSubMesh();
				if(!subMesh) continue;
				Ogre::MaterialPtr material = subEntity->getMaterial();
				if(material.isNull() || isFullyTransparent(material)) continue;

				Ogre::HardwareIndexBufferSharedPtr hib;
				const void* indexBuffer = nullptr;
				size_t indexStart, indexCount, indexSize;

				Ogre::HardwareVertexBufferSharedPtr hvb;
				const void* vertexBuffer = nullptr;
				size_t vertexStart, vertexCount, vertexSize, vertexPositionOffset;
				bool sharedVertices = false;

				if(subMesh->indexData)
				{
					Ogre::IndexData* indexData = subMesh->indexData;
					hib = indexData->indexBuffer;
					if(!hib.isNull())
					{
						indexBuffer = hib->lock(Ogre::HardwareBuffer::HBL_READ_ONLY);
						indexStart = indexData->indexStart;
						indexCount = indexData->indexCount;
						indexSize = (hib->getType() == Ogre::HardwareIndexBuffer::IT_32BIT ? 4 : 2);
					}
				}

				if(subMesh->useSharedVertices)
				{
					hvb = sharedHvb;
					vertexBuffer = sharedVertexBuffer;
					vertexStart = sharedVertexStart;
					vertexCount = sharedVertexCount;
					vertexSize = sharedVertexSize;
					vertexPositionOffset = sharedVertexPositionOffset;
					sharedVertices = true;
				}
				else if(subMesh->vertexData)
				{
					Ogre::VertexData* vertexData = subMesh->vertexData;
					const Ogre::VertexElement* posElem = vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
					if(posElem)
					{
						hvb = vertexData->vertexBufferBinding->getBuffer(posElem->getSource());
						if(!hvb.isNull())
						{
							vertexBuffer = hvb->lock(Ogre::HardwareBuffer::HBL_READ_ONLY);
							if(vertexBuffer)
							{
								vertexStart = vertexData->vertexStart;
								vertexCount = vertexData->vertexCount;
								vertexSize = hvb->getVertexSize();
								vertexPositionOffset = posElem->getOffset();
								sharedVertices = false;
							}
						}
					}
				}

				if(vertexBuffer && indexBuffer)
				{
					std::pair<bool, Real> subMeshIntersection = 
						intersects( transformedRay, 
							indexBuffer, indexStart, indexCount, indexSize,
							sharedVertexBuffer, sharedVertexStart, sharedVertexCount, 
							sharedVertexSize, sharedVertexPositionOffset,
							positiveSide, negativeSide );

					if(subMeshIntersection.first)
					{
						if(!ret.first || subMeshIntersection.second < ret.second)
						{
							ret.first = true;
							ret.second = subMeshIntersection.second;
						}
					}
				}

				if(vertexBuffer && !sharedVertices)
					hvb->unlock();

				if(indexBuffer)
					hib->unlock();
			}
		}

		if(sharedVertexBuffer)
			sharedHvb->unlock();

		return ret;
	}
	//--------------------------------------------------------------------------------------
	std::pair<bool, Real> ExactRayQuery::intersects( const Ogre::Ray& _ray,
		const void* _indexBuffer, size_t _indexStart, size_t _indexCount, 
		size_t _indexSize,
		const void* _vertexBuffer, size_t _vertexStart, size_t _vertexCount,
		size_t _vertexSize, size_t _vertexPositionOffset,
		bool _positiveSide, bool _negativeSide)
	{
		std::pair<bool, Real> ret;
		ret.first = false;

		for(size_t j = _indexStart; j != _indexCount; j += 3)
		{
			size_t i0, i1, i2;
			if(_indexSize == 2)
			{
				i0 = ((ushort*) _indexBuffer)[j];
				i1 = ((ushort*) _indexBuffer)[j+1];
				i2 = ((ushort*) _indexBuffer)[j+2];
			}
			else
			{
				assert(_indexSize == 4);
				i0 = ((ulong*) _indexBuffer)[j];
				i1 = ((ulong*) _indexBuffer)[j+1];
				i2 = ((ulong*) _indexBuffer)[j+2];
			}

			Ogre::Vector3 v0( (Real*) ((size_t) _vertexBuffer + i0 * _vertexSize + _vertexPositionOffset) );
			Ogre::Vector3 v1( (Real*) ((size_t) _vertexBuffer + i1 * _vertexSize + _vertexPositionOffset) );
			Ogre::Vector3 v2( (Real*) ((size_t) _vertexBuffer + i2 * _vertexSize + _vertexPositionOffset) );
			std::pair<bool, Real> triangleIntersection = 
				Ogre::Math::intersects(_ray, v0, v1, v2, _positiveSide, _negativeSide);
			
			if(triangleIntersection.first)
			{
				if(!ret.first || triangleIntersection.second < ret.second)
				{
					ret.first = true;
					ret.second = triangleIntersection.second;
				}
			}
		}

		return ret;
	}
	//--------------------------------------------------------------------------------------
	bool ExactRayQuery::isFullyTransparent(const Ogre::MaterialPtr& _material)
	{
		Ogre::Technique* technique = _material->getBestTechnique();
		Ogre::Technique::PassIterator it = technique->getPassIterator();
		while(it.hasMoreElements())
		{
			Ogre::Pass* pass = it.getNext();
			if(pass->getAlphaRejectFunction() != Ogre::CMPF_ALWAYS_FAIL)
				return false;
		}
		return true;
	}

} // namespace GothOgre