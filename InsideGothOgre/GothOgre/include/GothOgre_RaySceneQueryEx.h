#ifndef GOTHOGRE_RAY_SCENE_QUERY_EX_H
#define GOTHOGRE_RAY_SCENE_QUERY_EX_H

#include "GothOgre_Mtl.h"


namespace GothOgre
{
	class SceneManagerEx;


	//------------------------------------------------------------------------
	/** Extended version of Ogre::RaySceneQueryResultEntry.
	This provides more information about ray intersection. */
	struct RaySceneQueryExResultEntry
    {
        /// Distance along the ray.
        Real           distance;

        /// The movable, or NULL.
        MovableObject* movable;

		/// Index of the subentity which was intersected by the ray.
		size_t	       subEntityIndex;

		/// Material of the subentity which was intersected by the ray.
		MaterialPtr    material;

		/// Index of the triangular face which was intersected by the ray.
		size_t	       faceIndex;

		/// Normal to the face in the world space (not in the mesh local space).
		Vector3        normal;

        /// Comparison operator for sorting
        bool operator < (const RaySceneQueryExResultEntry& rhs) const
        {
            return this->distance < rhs.distance;
        }
	};

	typedef vector<RaySceneQueryExResultEntry>::type  RaySceneQueryExResult;



	//------------------------------------------------------------------------
	/// Listener for RaySceneQueryEx
	class GOTHOGRE_EXPORT RaySceneQueryExListener
	{
	public:
		/** Check a scene node before main processing: 
		if the function returns false, the scene node will not be processed. */
		virtual bool checkSceneNode(SceneNode* _sceneNode) {return true;}

		/** Check a movable before main processing: 
		if the function returns false, the movable will not be processed. */
		virtual bool checkMovable(MovableObject* _movable) {return true;}

		/** Check a subentity before main processing: 
		if the function returns false, the subentity will not be processed. */
		virtual bool checkSubEntity(SubEntity* _subEntity) {return true;}

		/** Check a result entry before adding it to the result list: 
		if the function returns false, the entry will not be added. */
		virtual bool checkResultEntry(const RaySceneQueryExResultEntry& _entry) {return true;}
	};


	//------------------------------------------------------------------------
	/// Extended version of Ogre::RaySceneQuery.
	/// Provides more information about intersection.
	class GOTHOGRE_EXPORT RaySceneQueryEx : public SceneQuery
	{
	public:
		RaySceneQueryEx(SceneManagerEx* _sceneManager);
		virtual ~RaySceneQueryEx();

		/// Sets the ray which is to be used for this query. 
		void setRay(const Ray& _ray);

		/// Returns the ray which is to be used for this query.
		const Ray& getRay() const;

		/// Sets the maximum number of results returned from the query (only relevant if 
		/// results are being sorted). The default value is 0x7FFF.
		/// The results are sorted.
        void setMaxResults(ushort _maxResults);

		/// Gets the maximum number of results returned from the query (only relevant if 
        /// results are being sorted).
        ushort getMaxResults() const;

		/// Sets if multiple intersections of the same movable with the ray 
		/// are allowed or not.
		void setMultipleIntersections(bool _multiple);

		/// Returns true if multiple intersections of the same movable 
		/// with the ray are allowed.
		bool getMultipleIntersections() const;

		/// Executes the query, returning the results back in one list. 
		RaySceneQueryExResult& execute(RaySceneQueryExListener* _listener = nullptr);

		/// Gets the results of the last query that was run using this object, 
		/// provided the query was executed using the collection-returning version of execute. 
		RaySceneQueryExResult& getLastResults();

		/// Clears the results of the last query execution. 
		void clearResults();

	protected:
		virtual void executeImpl() = 0;

	protected:
        Ray                      mRay;
        ushort                   mMaxResults;
		bool                     mMultipleIntersections;
		RaySceneQueryExResult    mResult;
		RaySceneQueryExListener* mListener;

		friend class RaySceneQueryExPtr;
	};



	//------------------------------------------------------------------------
	// Shared pointer to RaySceneQueryEx.
	// We cannot simply use 
	// "typedef SharedPtr<RaySceneQueryEx> RaySceneQueryExPtr"
	// because we need to override the destroy function.
	class GOTHOGRE_EXPORT RaySceneQueryExPtr : public SharedPtr<RaySceneQueryEx>
	{
	public:
		RaySceneQueryExPtr()
			: SharedPtr<RaySceneQueryEx>() {}
		
		RaySceneQueryExPtr(RaySceneQueryEx* _rep)
			: SharedPtr<RaySceneQueryEx>(_rep) {}

		RaySceneQueryExPtr(const RaySceneQueryExPtr& _ptr)
			: SharedPtr<RaySceneQueryEx>(_ptr) {}

		~RaySceneQueryExPtr() {release();}

	protected:
		void destroy();
	};

}

#endif // GOTHOGRE_RAY_SCENE_QUERY_EX_H
