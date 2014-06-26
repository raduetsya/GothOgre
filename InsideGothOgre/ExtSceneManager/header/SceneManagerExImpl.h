#ifndef SCENE_MANAGER_EX_IMPL_H
#define SCENE_MANAGER_EX_IMPL_H

#include "NewSceneNode.h"
#include "NewLight.h"


namespace GothOgre
{
	class MaterialGenerator;
	class PointInformator;
	class MovableObjectInformator;
	class EntityCreator;
	class EntityCreatorForLargeMesh;
	class SkyCalculator;
	class SkyVisualizer;


	//-------------------------------------------------------------------------
	/** Implementation of the "SceneManagerEx" abstract class*/
	class SceneManagerExImpl : public SceneManagerEx, 
		public BoundingSphereTreeListener
	{
	// Main functions
	public:
		SceneManagerExImpl(const String& name);
		~SceneManagerExImpl();
		const String& getTypeName() const;

		/** Empties the entire scene, inluding all SceneNodes, Entities, Lights, BillboardSets etc. 
		Overriden function of SceneManager. */
		void clearScene();


	// Rendering
	protected:
		/** Prompts the class to send its contents to the renderer. 
		Overriden function of SceneManager. */
		void _renderScene( Camera* _camera, Viewport* _vp, bool _includeOverlays );

		// Processing just before SceneManager::_renderScene
		void preRender(Viewport* _viewport);

		// Processing just after SceneManager::_renderScene
		void postRender(Viewport* _viewport);


	// Scene nodes
	public:
		/** Get a reference to bounding sphere tree with scene nodes.
		This reference is used by scene nodes to tell the tree
		when a scene node is moved. */
		SceneNodeBST& _getSceneNodeBST();

		/** Sets the distance to the far clipping plane. 
		This function is used by SkyVisualizer to send calculated value
		to the scene manager. */
		void _setFarClipDistance( Real _farClipDistance );

	protected:
		/** Updates the scene graph ie the tree of SceneNode instances managed by this class. */
		void _updateSceneGraph( Camera* _camera);

		/** Parses the scene to find visible objects to render. */
		void _findVisibleObjects(Camera* _camera, VisibleObjectsBoundsInfo* _visibleBounds, bool _onlyShadowCasters);

		/** Creates an instance of a SceneNode. */
		SceneNode* createSceneNodeImpl();

		/** Creates an instance of a SceneNode with a given name. */
		SceneNode* createSceneNodeImpl(const String& _name);


	// Entities
	public:
		/** Creates an entity (instance of a discrete mesh). */
		Entity* createEntity(const MeshExPtr& _mesh, uint32 _queryMask);

		/** Destroys an entity. */
		void destroyEntity(Entity* _entity);

		/** Creates a list of entities for a large mesh.
		The function uses splitting of the specified mesh into several meshes.
		The function can be used for static meshes only. */
		EntityList createEntityListForLargeMesh(const MeshExPtr& _mesh, uint32 _queryMask);

		/** Destroys a list of entities, created by 
		the "createEntityListForLarge" function. */
		void destroyEntityList(EntityList& _entList);		

		/** Returns list of sub-sub-entities with a specified parent subentity. */
		SubSubEntityList& getSubSubEntityList(SubEntity* _subEnt);

	// Lights:
	protected:
		/** Creates a light with a generated name. */
		Light* createLight();

		/** Locates a list of lights which could be affecting the frustum. */
		void findLightsAffectingFrustum(const Camera* _camera);
		
		/** Get a reference to bounding sphere tree with lights.
		This reference is used by lights to tell the tree
		when a light is moved. */
		LightBST& _getLightBST();


	// Material generating
	public:
		/** Generates a material by a specified mtl with additional parameters. */
		MaterialPtr generateMaterial(const MtlPtr& _mtl, const MatGenParams& _matGenParams);

		/** Generates a material by a specified mtl with additional parameters. */
		MaterialPtr generateMaterial(const MatGenInfo& _matGenInfo);

		/** Gets an iterator to the list of all the materials 
		which were generated from a specified mtl. */
		MaterialIterator getGeneratedMaterialIterator(const MtlPtr& _mtl);

		/** Updates all materials generated from a specified mtl;
		call the function after you change an instance of the mtl class. */
		void updateMaterials(const MtlPtr& _mtl);

		/** Updates a single material generated from some mtl;
		call the function if you wish to regenerate a material with new generation parameters. */
		void updateMaterial(const MaterialPtr& _material, const MatGenParams& _matGenParams);

		/** Returns an information used to generated a material. */
		const MatGenInfo& getMatGenInfo(const MaterialPtr& _material);

		/** Returns a pointer to the material generator 
		(this pointer is used internally). */
		MaterialGenerator* _getMaterialGenerator();


	// Ray queries
	public:
		// Base ray query
		RaySceneQueryExPtr createRayQueryEx();

		// Extended ray queries
		SceneUnderwaterInfo getUnderwaterInfo(const Vector3& _point);
		SceneUnderwaterInfo getUnderwaterInfo(MovableObject* _object);
		SceneSectorInfo     getSectorInfo(const Vector3& _point);
		SceneSectorInfo     getSectorInfo(MovableObject* _object);
		SceneFloorInfo      getFloorInfo(const Vector3& _point);
		SceneFloorInfo      getFloorInfo(MovableObject* _object);

	// Skies
	protected:
		/** Queues the sky scene nodes added by the addSceneNodeToSky function. */
		void _queueSkiesForRendering( Camera* _camera );

	// Private members:
	private:
		struct FindVisibleObjects
		{
			RenderQueue*              renderQueue;
			VisibleObjectsBoundsInfo* visibleBounds;
			Camera*                   camera;
			bool                      onlyShadowCasters;
		};

		// Overriden function of BoundingSphereTreeListener
		void enumerateFrustumIntersects(SceneNodeBST::ElementHandle _handle, Real _distance, void* _param);

		GOTHOGRE_DECLARE_GRAPHIC_SETTINGS_CHANGED( SceneManagerExImpl );

	private:
		// bounding sphere trees: scene nodes
		SceneNodeBST                mSceneNodeBST;

		// bounding sphere trees: lights
		LightBST                       mLightBST;

		// far clipping distance in cantimeters
		Real                        mFarClipDistance;

		// values from graphics.cfg
		SceneNodeBST::RealPairList         mDistanceMinRadiusPairList;
		bool                        mNoHardwareFarClipping;
		PolygonMode                 mPolygonMode;
		
		// utility classes
		MaterialGenerator*          mMaterialGenerator;
		PointInformator*            mPointInformator;
		MovableObjectInformator*    mMovableObjectInformator;
		EntityCreator*              mEntityCreator;
		EntityCreatorForLargeMesh*  mEntityCreatorForLargeMesh;
		SkyCalculator*              mSkyCalculator;
		//SkyVisualizer*              mSkyVisualizer;
	};

}

#endif // SCENE_MANAGER_EX_Impl_H