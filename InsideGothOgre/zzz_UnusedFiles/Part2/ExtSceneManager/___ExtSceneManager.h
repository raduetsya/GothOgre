#ifndef SCENE_MANAGER_EX_BASE_H
#define SCENE_MANAGER_EX_BASE_H

namespace GothOgre
{
	class BspPart;
	class NonBspPart;
	class ExtSceneManagerFrameStats;

	class ExtSceneManager : public SceneManager
	{
	public:
		ExtSceneManager(const String& name);
		~ExtSceneManager();
		const String& getTypeName(void) const;

	public:
		// Creates an instance of a SceneNode. 
		SceneNode* createSceneNodeImpl();

		// Creates an instance of a SceneNode with a given name. 
		SceneNode* createSceneNodeImpl(const String& _name);

		/** The same as (getNonBspPartMaxSize() / 2). */
		const Vector3& getNonBspPartMaxHalfSize() const {return mNonBspPartMaxHalfSize;}

		/** Returns the name of the sector where a movable object now.
		The sector is a room bounded by portals. */
		const String& getSector(MovableObject* _obj);
		
		/** Returns true if a movable object is underwater now. */
		bool isUnderwater(MovableObject* _obj);

	public:
		virtual void _findVisibleObjects(Camera* _camera,  
			VisibleObjectsBoundsInfo* _visibleBounds, bool _onlyShadowCasters);

	protected:
		void bspAdded(const BspPtr& _bsp);
		void bspRemoved(const BspPtr& _bsp);
		void bspRenderingDistanceChanged();
		void nonBspPartMaxSizeChanged();
		RaySceneQuery* createRayQueryImpl();

	private:
		friend class FastSceneNode;

		// Список данных по BSP-деревьям, используемый для их рендеринга
		list<BspPart>::type			mBspParts;

		list<NonBspPart>::type		mNonBspParts;

		Vector3						mNonBspPartMaxHalfSize;
		
		size_t						mCheckVisibilityCount;

		ExtSceneManagerFrameStats*  mFrameStats;
	};
		
} // namespace GothOgre

#endif // SCENE_MANAGER_EX_BASE_H
