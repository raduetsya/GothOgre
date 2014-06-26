#include "GothOgre_Precompiled.h"
#include "GothOgre_PortalCreatorByLevelEntity.h"
#include "GothOgre_MtlNameUtil.h"


namespace GothOgre
{
	//---------------------------------------------------------------------------
	PortalCreatorByLevelEntity::PortalCreatorByLevelEntity()
	{
	}
	//---------------------------------------------------------------------------
	PortalCreatorByLevelEntity::~PortalCreatorByLevelEntity()
	{
	}
	//---------------------------------------------------------------------------
	const PortalCreatorByLevelEntity::PortalList& 
		PortalCreatorByLevelEntity::execute(Entity* _levelEntity)
	{
		mEntity = _levelEntity;
		mMesh = mEntity->getMesh();
		mSceneManager = (SceneManagerEx*) (mEntity->_getManager());
		
		mPortalInfoGroups.clear();
		createPortalInfos();
		joinPortalInfos();
		mPortals.clear();
		createPortals();
		return mPortals;
	}
	//---------------------------------------------------------------------------
	void PortalCreatorByLevelEntity::createPortalInfos()
	{
		// The MeshExReadHelpers will help to get vertices and faces from the mesh.
		mMeshReadHelper.setMesh(mMesh);

		for(mSubEntityIndex = 0; mSubEntityIndex != mMesh->getNumSubMeshes(); ++mSubEntityIndex)
		{
			mSubEntity = mEntity->getSubEntity(mSubEntityIndex);
			mSubMesh = mMesh->getSubMesh(mSubEntityIndex);
			createPortalInfosBySubMesh(); // process submesh
		}
	}
	//---------------------------------------------------------------------------
	void PortalCreatorByLevelEntity::createPortalInfosBySubMesh()
	{
		// Get the submesh's material
		MaterialPtr material = mSubEntity->getMaterial();
		MtlPtr mtl = mMesh->getSubMesh(mSubEntityIndex)->getMtl();
		if(material.isNull() || mtl.isNull())
			return;

		// If it's not a portal, return
		const String& mtlName = mtl->getName();
		if(!MtlNameUtil::isPortalName(mtlName))
			return;

		// Search a portal infos' group, if it exists...
		PortalInfoGroups::iterator pigIterator; // pig = Portal Info Group
		for(pigIterator = mPortalInfoGroups.begin(); 
			pigIterator != mPortalInfoGroups.end(); ++pigIterator)
		{
			PortalInfoGroup& pig = *pigIterator;
			if(pig.mtl == mtl)
				break;
		}

		// and create it, if it doesn't exist.
		if(pigIterator == mPortalInfoGroups.end())
		{
			mPortalInfoGroups.push_back(PortalInfoGroup());
			pigIterator = mPortalInfoGroups.end();
			--pigIterator;
			PortalInfoGroup& pig = *pigIterator;
			pig.mtl = mtl;
			pig.material = material;
			
			std::pair<String, String> sectors = MtlNameUtil::splitPortalName(mtlName);
			pig.firstSector = sectors.first;
			pig.secondSector = sectors.second;

		}
		PortalInfoGroup& pig = *pigIterator;

		// Process each face
		size_t numFaces = mMeshReadHelper.getNumFaces(mSubEntityIndex);
		PortalInfos& portalInfos = pig.portalInfos;

		for(size_t k = 0; k != numFaces; ++k)
		{
			// get coordinates of the face's vertices
			MeshReadHelper::Face face = mMeshReadHelper.getFace(mSubEntityIndex, k);
			Vector3 v0 = mMeshReadHelper.getVertex(face.i[0]);
			Vector3 v1 = mMeshReadHelper.getVertex(face.i[1]);
			Vector3 v2 = mMeshReadHelper.getVertex(face.i[2]);
			
			// calculate geometric parameters
			Vector3 crossp = (v1 - v0).crossProduct(v2 - v1);
			Real crossp_length = crossp.length();
			if(crossp_length == 0)
				continue;

			Vector3 center = (v0 + v1 + v2) / 3;
			Vector3 normal = crossp / crossp_length;
			Real area = crossp_length / 2;

			// add new portal info
			portalInfos.push_back(PortalInfo());
			PortalInfo& portalInfo = portalInfos.back();
			portalInfo.vertices.reserve(3);
			portalInfo.vertices.push_back(v0);
			portalInfo.vertices.push_back(v1);
			portalInfo.vertices.push_back(v2);
			portalInfo.normal = normal;
			portalInfo.center = center;
			portalInfo.area = area;
			portalInfo.portal = nullptr;
			portalInfo.deleted = false;
		}
	}
	//---------------------------------------------------------------------------
	void PortalCreatorByLevelEntity::joinPortalInfos()
	{
		// Search portal infos which can be joined 
		// (i.e. the portal infos which use intersecting sets of vertices 
		// and have about the same normal).
		for(PortalInfoGroups::iterator pigIterator = mPortalInfoGroups.begin(); 
			pigIterator != mPortalInfoGroups.end(); ++pigIterator)
		{
			PortalInfoGroup& pig = *pigIterator;
			PortalInfos& portalInfos = pig.portalInfos;
			
			for(PortalInfos::iterator it1 = portalInfos.begin();
				it1 != portalInfos.end(); ++it1)
			{
				PortalInfo& pi1 = *it1;
				if(pi1.deleted) 
					continue;

				// pi1 - the first checking portal info
				PortalInfos::iterator it2 = it1; ++it2;
				for(; it2 != portalInfos.end(); ++it2)
				{
					PortalInfo& pi2 = *it2;
					if(pi2.deleted) 
						continue;

					// pi2 - the second checking portal info
					for(size_t k1 = 0; k1 != pi1.vertices.size(); ++k1)
					{
						const Vector3& vk1 = pi1.vertices[k1];

						// search near vertices
						bool nearVerts[3] = {false, false, false};
						size_t numNearVerts = 0;
						for(size_t k2 = 0; k2 != 3; ++k2)
						{
							const Vector3& vk2 = pi2.vertices[k2];
							if(vk1.positionCloses(vk2))
							{
								nearVerts[k2] = true;
								++numNearVerts;
							}
						}

						// Join portal infos if at least two of three vertices 
						// have near coordinates
						if(numNearVerts >= 2)
						{
							if(numNearVerts == 2)
							{
								for(size_t k2 = 0; k2 != 3; ++k2)
									if(!nearVerts[k2])
									{
										pi1.vertices.push_back(pi2.vertices[k2]);
										break;
									}

								pi1.center = pi1.center * pi1.area + pi2.center * pi2.area;
								pi1.area += pi2.area;
								pi1.normal += pi2.normal;
								pi1.normal.normalise();
							}

							// pi2 must be skipped from now, because the portal info 
							// has been joined to another portal info (pi1)
							pi2.deleted = true; 
						}
					}
				}
			}

			// Remove the "deleted" portal infos from the list
			PortalInfos::iterator it = portalInfos.begin();
			while(it != portalInfos.end())
			{
				PortalInfos::iterator itErase = it++;
				PortalInfo& pi = *itErase;
				if(pi.deleted)
					portalInfos.erase(itErase);
			}
		}
	}
	//---------------------------------------------------------------------------
	void PortalCreatorByLevelEntity::createPortals()
	{
		// Search total number of portals which will be created.
		// (We need the number to reserve place in the result vector).
		size_t numPortals = 0;
		PortalInfoGroups::iterator pigIterator;
		for(pigIterator = mPortalInfoGroups.begin(); 
			pigIterator != mPortalInfoGroups.end(); ++pigIterator)
		{
			PortalInfoGroup& pig = *pigIterator;
			PortalInfos& portalInfos = pig.portalInfos;
			numPortals += portalInfos.size();
		}
	
		mPortals.reserve(numPortals);

		// Create portals by portal infos
		for(pigIterator = mPortalInfoGroups.begin(); 
			pigIterator != mPortalInfoGroups.end(); ++pigIterator)
		{
			PortalInfoGroup& pig = *pigIterator;
			PortalInfos& portalInfos = pig.portalInfos;

			for(PortalInfos::iterator it = portalInfos.begin();
				it != portalInfos.end(); ++it)
			{
				PortalInfo& pi = *it;
				
				Portal* portal = PortalSystem::getSingleton().createPortal(mSceneManager);
				portal->setNormal(pi.normal);
				portal->setArea(pi.area);
				portal->setCenter(pi.center);
				portal->setVertices(pi.vertices);
				portal->setMaterial(pig.material);

				pi.portal = portal;
				mPortals.push_back(portal);
			}
		}
	}

} // namespace GothOgre