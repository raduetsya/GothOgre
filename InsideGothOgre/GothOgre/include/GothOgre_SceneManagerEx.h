#ifndef GOTHOGRE_SCENE_MANAGER_EX_H
#define GOTHOGRE_SCENE_MANAGER_EX_H

#include "GothOgre_Mtl.h"
#include "GothOgre_MatGenParams.h"
#include "GothOgre_MeshEx.h"
#include "GothOgre_RaySceneQueryEx.h"
#include "GothOgre_ScenePositionInfo.h"
#include "GothOgre_ViewportAbsCoord.h"


namespace GothOgre
{
	//-------------------------------------------------------------------------
	/** Part of subentity.
	A single subentity can contain many such parts.
	These subsubentities are not used in the rendering process,
	however they can be used in the raycasting algrorithm. */
	struct SubSubEntity
	{
		SubEntity*      parent;
		AxisAlignedBox  bounds;
		size_t          indexStart;
		size_t          indexCount;
	};
	typedef vector<SubSubEntity>::type             SubSubEntityList;



	//-----------------------------------------------------------------------
	/** Extended version of Ogre::SceneManager.
	It supports extended scene query and the new "Mtl" material class. */
	class GOTHOGRE_EXPORT SceneManagerEx : public SceneManager
	{
	public:
		SceneManagerEx(const String& _instanceName);
		~SceneManagerEx();

	// Entities:
	public:
		/** Creates an entity (instance of a discrete mesh). */
		virtual Entity* createEntity(const MeshExPtr& _mesh, uint32 _queryMask) = 0;

		/** Destroys an entity. */
		virtual void destroyEntity(Entity* _entity) = 0;

		typedef vector<Entity*>::type EntityList;

		/** Creates a list of entities for a large mesh.
		The function uses splitting of the specified mesh into several meshes.
		The function can be used for static meshes only. */
		virtual EntityList createEntityListForLargeMesh(const MeshExPtr& _mesh, uint32 _queryMask) = 0;

		/** Destroys a list of entities, created by 
		the "createEntityListForLargeMesh" function,
		clears the list. */
		virtual void destroyEntityList(EntityList& _entList) = 0;

		/** Returns list of sub-sub-entities with a specified parent subentity. */
		virtual SubSubEntityList& getSubSubEntityList(SubEntity* _subEnt) = 0;

	// Ray queries:
	public:
		/** Creates ray query. GothOgre provides own version of the "RaySceneQuery" class. */
		virtual RaySceneQueryExPtr createRayQueryEx() = 0;
		RaySceneQueryExPtr createRayQueryEx(const Ray& _ray, uint32 _queryMask = 0xFFFFFFFF);
		RaySceneQueryExPtr createRayQueryEx(Real _relx, Real _rely, Camera* _camera, uint32 _queryMask = 0xFFFFFFFF);
		RaySceneQueryExPtr createRayQueryEx(int _x, int _y, const ViewportAbsCoord& _vpAbsCoord, Camera* _camera, uint32 _queryMask = 0xFFFFFFFF);

		/** Information about a point under water. */
		virtual SceneUnderwaterInfo getUnderwaterInfo(const Vector3& _point) = 0;
		virtual SceneUnderwaterInfo getUnderwaterInfo(MovableObject* _object) = 0;

		/** Sector information about a point. */
		virtual SceneSectorInfo getSectorInfo(const Vector3& _point) = 0;
		virtual SceneSectorInfo getSectorInfo(MovableObject* _object) = 0;

		/** Information about a point relative the floor. */
		virtual SceneFloorInfo getFloorInfo(const Vector3& _point) = 0;
		virtual SceneFloorInfo getFloorInfo(MovableObject* _object) = 0;

	public:
		/** Generates a material by a specified mtl with additional parameters. */
		virtual MaterialPtr generateMaterial(const MtlPtr& _mtl, const MatGenParams& _matGenParams) = 0;

		/** Generates a material by a specified mtl with additional parameters. */
		virtual MaterialPtr generateMaterial(const MatGenInfo& _matGenInfo) = 0;

		typedef Mtl::MaterialIterator  MaterialIterator;

		/** Gets an iterator to the list of all the materials 
		which were generated from a specified mtl. */
		virtual MaterialIterator getGeneratedMaterialIterator(const MtlPtr& _mtl) = 0;

		/** Updates all materials generated from a specified mtl;
		call the function after you change an instance of the mtl class. */
		virtual void updateMaterials(const MtlPtr& _mtl) = 0;

		/** Updates a single material generated from some mtl;
		call the function if you wish to regenerate a material with new generation parameters. */
		virtual void updateMaterial(const MaterialPtr& _material, const MatGenParams& _matGenParams) = 0;

		/** Returns an information used to generated a material. */
		virtual const MatGenInfo& getMatGenInfo(const MaterialPtr& _material) = 0;

	// Directions (up, north, west):
	public:
		/** Sets the east direction for the scene.
		It's used for calculating the skies.
		The default value is Vector3(1, 0, 0). */
		void setEastDirection(const Vector3& _eastDirection) {mEastDirection = _eastDirection;}

		/** Returns the east direction. */
		const Vector3& getEastDirection() const {return mEastDirection;}

		/** Sets the north direction for the scene.
		It's used for calculating the skies.
		The default value is Vector3(0, 1, 0). */
		void setNorthDirection(const Vector3& _northDirection) {mNorthDirection = _northDirection;}

		/** Returns the north direction for the scene. */
		const Vector3& getNorthDirection() const {return mNorthDirection;}

		/** Sets the upward direction for the scene 
		It's used for searching floor, and setup the gravity).
		The default value is Vector3(0, 0, 1). */
		void setUpDirection(const Vector3& _upDirection) {mUpDirection = _upDirection;}

		/** Returns the upward direction for the scene. */
		const Vector3& getUpDirection() const {return mUpDirection;}

	private:
		Vector3  mUpDirection;
		Vector3  mNorthDirection;
		Vector3  mEastDirection;
	};


	/** Type of scene managers derived from the SceneManagerEx class. */
	enum SceneTypeEx
	{
		ST_GOTHOGRE = 128
	};
}

#endif // GOTHOGRE_SCENE_MANAGER_EX_H
