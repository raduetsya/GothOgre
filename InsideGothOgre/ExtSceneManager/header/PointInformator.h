#ifndef POINT_INFORMATOR_H
#define POINT_INFORMATOR_H


namespace GothOgre
{
	class SceneManagerExImpl;

	//-------------------------------------------------------------------------
	class PointInformator
	{
	public:
		PointInformator(SceneManagerExImpl* _sm);
		~PointInformator();

		/** Calculates information about a point in 3D space. */
		SceneUnderwaterInfo  getUnderwaterInfo(const Vector3& _point);
		SceneSectorInfo      getSectorInfo(const Vector3& _point);
		SceneFloorInfo       getFloorInfo(const Vector3& _point);

	private:
		class UnderwaterListener : public RaySceneQueryExListener
		{
		public:
			UnderwaterListener(SceneManagerExImpl* _sm);
			bool checkSubEntity(SubEntity* _subEnt);
		private:
			SceneManagerExImpl* mSceneManager;
		};

		class SectorListener : public RaySceneQueryExListener
		{
		public:
			SectorListener(SceneManagerExImpl* _sm);
			bool checkSubEntity(SubEntity* _subEnt);
		private:
			SceneManagerExImpl* mSceneManager;
		};

	private:
		SceneManagerExImpl* mSceneManager;
	};

} // namespace GothOgre

#endif // POINT_INFORMATOR_H