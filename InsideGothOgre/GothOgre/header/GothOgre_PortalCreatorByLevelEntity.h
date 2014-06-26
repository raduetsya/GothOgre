#ifndef GOTHOGRE_PORTAL_CREATOR_BY_LEVEL_ENTITY_H
#define GOTHOGRE_PORTAL_CREATOR_BY_LEVEL_ENTITY_H

#include "GothOgre_PortalSystem.h"
#include "GothOgre_Mtl.h"
#include "GothOgre_MeshExReadHelper.h"


namespace GothOgre
{
	//-----------------------------------------------------------------------
	/** Internal class can create portals via PortalSystem by level entity. */
	class PortalCreatorByLevelEntity
	{
	public:
		PortalCreatorByLevelEntity();
		~PortalCreatorByLevelEntity();

		typedef PortalSystem::PortalList PortalList;

		/** Creates portals by information retrieved from a level mesh. */
		const PortalList& execute(Entity* _entity);

	private:
		/** Creates portal infos and portal infos' groups for the entire level mesh. */
		void createPortalInfos();

		/** Creates portal infos and portal infos' groups for the selected submesh. */
		void createPortalInfosBySubMesh();

		/** Joins some portal infos, when portals use common vertices and have close normals. */
		void joinPortalInfos();

		/** Searchs opposite portals. */
		void findOppositePortalInfos();

		/** Creates portals by portal infos. */
		void createPortals();

	private:
		// Information for future creating single portal.
		struct PortalInfo
		{
			Vector3                center;
			Vector3                normal;
			Real                   area;
			vector<Vector3>::type  vertices;
			Portal*                portal;
			bool                   deleted;
		};

		typedef list<PortalInfo>::type PortalInfos;

		// Information for future creating some portals with the same materials.
		struct PortalInfoGroup
		{
			MaterialPtr  material;
			MtlPtr       mtl;
			String       firstSector;
			String       secondSector;
			PortalInfos  portalInfos;
		};

		typedef list<PortalInfoGroup>::type PortalInfoGroups;

	private:
		SceneManagerEx*                 mSceneManager;
		Entity*                         mEntity;
		MeshExPtr                       mMesh;
		MeshExReadHelper                mMeshReadHelper;
		size_t                          mSubEntityIndex;
		SubEntity*                      mSubEntity;
		SubMesh*                        mSubMesh;
		PortalInfoGroups                mPortalInfoGroups;
		PortalList                      mPortals;
	};

} // namespace GothOgre

#endif // GOTHOGRE_PORTAL_CREATOR_BY_LEVEL_ENTITY_H