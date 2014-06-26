#ifndef GOTHOGRE_ENTITY_VIS_EX_H
#define GOTHOGRE_ENTITY_VIS_EX_H

namespace GothOgre
{
	class GOTHOGRE_EXPORT EntityVisEx : public Ogre::Entity
	{
	public:
		/** Sets the distance at which the object is no longer rendered.
		@param dist Distance beyond which the object will not be rendered 
			(the default is 0, which means objects are always rendered).
		*/
		void setRenderingDistance(Real _dist, Real _distCoef = 0.0f);

		void setMeshLodBias(Real _factor, Real _factorCoef, ushort _maxDetailIndex = 0, ushort _minDetailIndex = 99);

		void setMeshLodBias(Real _factor, ushort _maxDetailIndex = 0, ushort _minDetailIndex = 99)
		{
			setMeshLodBias(_factor, 0.0f, _maxDetailIndex, _minDetailIndex);
		}
		
	protected:
		/** Private constructor (instances cannot be created directly).
		*/
		EntityVisEx();
		/** Private constructor - specify name (the usual constructor used).
		*/
		EntityVisEx( const String& name, const MeshPtr& mesh);		

		/** Internal method to notify the object of the camera to be used for the next rendering operation.
            @remarks
                Certain objects may want to do specific processing based on the camera position. This method notifies
                them in case they wish to do this.
        */
        virtual void _notifyCurrentCamera(Camera* cam);

	protected:
		Real mDist;
		Real mDistCoef;
		Real mFactor;
		Real mFactorCoef;
	};




	/** Factory object for creating Entity instances */
	class GOTHOGRE_EXPORT EntityVisExFactory : public MovableObjectFactory
	{
	protected:
		MovableObject* createInstanceImpl( const String& name, const NameValuePairList* params);
	public:
		EntityVisExFactory() {}
		~EntityVisExFactory() {}

		static String FACTORY_TYPE_NAME;

		const String& getType(void) const;
		void destroyInstance( MovableObject* obj);

	};

} // namespace GothOgre

#endif // GOTHOGRE_ENTITY_VIS_EX_H