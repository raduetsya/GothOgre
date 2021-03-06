#ifndef GOTHOGRE_SKY_SYSTEM_H
#define GOTHOGRE_SKY_SYSTEM_H

#include "GothOgre_SkyDef.h"
#include "GothOgre_FrameListener.h"


namespace GothOgre
{
	//---------------------------------------------------------------------------
	/** A movable object defining a sky over a bounding box. 
	An instance of this class is based on an instance of the "SkyDef" resource;
	an instance of the "SkyDef" resource defines sky's parameters. 
	So "Sky" is related to "SkyDef" nearly the same as "Entity" is related to "Mesh. */
	class GOTHOGRE_EXPORT Sky : public MovableObject
	{
	public:
        /** Constructor */
		Sky();

		/** Named constructor */
		Sky(const String& _name);

		/** Virtual destructor */
		~Sky();

        /** Returns the type name of this object. */
        const String& getMovableType() const;

		/** Sets the Sky that this Sky is based on. */
		void setSkyDef(const SkyDefPtr& _skyDef);

		/** Returns the Sky that this Sky is based on. */
		SkyDefPtr getSkyDef() const;

		/** Sets a bounding box beyond which this sky is no longer visible for any camera. */
		void setSkyBoundingBox(const AxisAlignedBox& _box, bool _localSpace = true);

		/** Returns a bounding box beyond which this sky is no longer visible. */
		const AxisAlignedBox& getSkyBoundingBox() const;

		/** Returns true if a bounding box specified for local space. */
		bool isSkyBoundingBoxInLocalSpace() const;

		/** Sets the inner part (0..1) of the bounding box. 
		This inner part is used to change sky parameters smoothly
		when a camera enters to (exits from) the sky's bounding box.
		If the camera is in the inner part of the bounding box
		then the sky has max effect.
		If the camera is outside of the bounding box,
		then the sky has no effect (i.e. it's fully transparent).
		If the camera is inside of the bounding box, but outside
		of the inner part of the bounding box
		then the sky has partial effect (i.e. partially transparent). */
		void setInnerPart(Real _innerPart);

		/** Returns the inner part (0..1) of the bounding box. */
		Real getInnerPart() const;

		/** Sets the max weight of the effects of the sky.
		This max weight is used when the camera is located 
		in the inner part of the bounding box.*/
		void setWeight(Real _weight);

		/** Returns the max weight of the effects of the sky. */
		Real getWeight() const;

		/** Sets the priority of the sky object. 
		The priority is used to select a sky to show when
		two (or more) sky objects have intersecting bounding boxes. 
		The default priority is 0. */
		void setPriority(int _priority);

		/** Returns the priority of the sky object. */
		int getPriority() const;

	protected:
		// Skies are not rendered in the normal way, so we return 0
		Real getBoundingRadius() const {return 0;}
		const AxisAlignedBox& getBoundingBox() const {return AxisAlignedBox::BOX_NULL;}
		bool isVisible() const {return false;}
		void _updateRenderQueue(RenderQueue* _queue) {}
		void visitRenderables(Renderable::Visitor* _visitor, bool _debugRenderables = false) {}

	private:
		void init();

	private:
		SkyDefPtr            mSkyDef;
		AxisAlignedBox       mSkyBoundingBox;
		bool                 mSkyBoundingBoxInLocalSpace;
		Real                 mInnerPart;
		Real                 mWeight;
		int                  mPriority;
	};



	//---------------------------------------------------------------------------
	/** The main singleton class to manage sky objects. */
	class GOTHOGRE_EXPORT SkySystem : public Singleton<SkySystem>, 
		public MovableObjectFactory
	{
	public:
		SkySystem();
		~SkySystem();

		void initialise();
		void shutdown();

		/** Get the type of the object to be created. */
		const String& getType() const;
		static const String FACTORY_TYPE_NAME;

		/** Creates a sky object (a sky over some bounding box)
		- with an autogenerated name. */
		Sky* createSky(SceneManager* _sceneManager);

		/** Destroys a sky object. */
		void destroySky(Sky* _sky);

		typedef list<String>::type VisibleSkyList;
		typedef ConstVectorIterator<VisibleSkyList> VisibleSkyIterator;

		/** Returns the list of skies which are visible just now 
		for a specified camera. */
		VisibleSkyIterator getVisibleSkyIterator(Camera* _camera) const;

		/** Returns true if a specified sky is visible just now 
		for a specified camera. */
		bool isVisible(Camera* _camera, const String& _skyName) const;

	public:
		/** Adds a sky to the list of skies which are visible just now.
		The function is called internally by a scene manager's implementation. */
		void _addToVisibleList(Camera* _camera, const String& _skyName);

	private:
		MovableObject* createInstanceImpl(const String& _name, const NameValuePairList* _params);
		void destroyInstance(MovableObject* _obj);

		GOTHOGRE_DECLARE_FRAME_ENDED(SkySystem);
		void deleteUnusedInCurrentFrame();
		void resetUsedInCurrentFrame();

	private:
		struct VisibleSkies
		{
			VisibleSkyList  list;
			bool            usedInCurrentFrame;
		};
		typedef map<Camera*, VisibleSkies>::type   VisibleSkiesMap;

		VisibleSkiesMap     mVisibleSkiesMap;
	};

} // namespace GothOgre

#endif // GOTHOGRE_SKY_SYSTEM_H