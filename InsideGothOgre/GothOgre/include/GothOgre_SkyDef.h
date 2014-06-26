#ifndef GOTHOGRE_SKY_DEF_H
#define GOTHOGRE_SKY_DEF_H

#include "GothOgre_ResourceEx.h"
#include "GothOgre_ResourceGroup.h"
#include "GothOgre_SubclassSharedPtr.h"
#include "GothOgre_Time.h"
#include "GothOgre_Mtl.h"
#include "GothOgre_MatGenParams.h"
#include "GothOgre_SoundDef.h"


namespace GothOgre
{

	//------------------------------------------------------------------------
	/** This class is designed to describe parameters of a sky. */
	class GOTHOGRE_EXPORT SkyDef : public ResourceEx
	{
	public:
		/** Modifier of a value: multiplies a value by a multiplier, 
		then increases the result by an addend. */
		template<typename T>
		class BaseModifier
		{
		public:
			BaseModifier(const BaseModifier& _src) : mMultiplier(_src.mMultiplier), mAddend(_src.mAddend) {}
			BaseModifier(T _multiplier, T _addend) : mMultiplier(_multiplier), mAddend(_addend) {}

			T getMultiplier() const {return mMultiplier;}
			void setMultiplier(T _multiplier) {mMultiplier = _multiplier;}

			T getAddend() const {return mAddend;}
			void setAddend(T _addend) {mAddend = _addend;}

			template<typename VAL>
			VAL transform(VAL _x) const {return mMultiplier * _x + mAddend;}

			void operator +=(const BaseModifier& _rhs) 
			{
				mAddend = mAddend + _rhs.mAddend; 
				mMultiplier = mMultiplier + _rhs.mMultiplier;
			}
			
			void operator *=(const BaseModifier& _rhs) 
			{
				mAddend = mAddend + mMultiplier * _rhs.mAddend; 
				mMultiplier = mMultiplier * _rhs.mMultiplier;
			}
			
			void operator *=(Real _coef) 
			{
				mAddend = T(mAddend * _coef); 
				mMultiplier = T(mMultiplier * _coef);
			}

			typedef T ValueType;

		private:
			T  mMultiplier;
			T  mAddend;
		};

		template<typename T>
		class Modifier;

		template<>
		class GOTHOGRE_EXPORT Modifier<Real> : public BaseModifier<Real>
		{
		public:
			Modifier<Real>(const BaseModifier<Real>& _src) : BaseModifier<Real>(_src) {}
			Modifier<Real>(Real _multiplier, Real _addend) : BaseModifier<Real>(_multiplier, _addend) {}
			Modifier<Real>() : BaseModifier<Real>(0, 0) {}

			Modifier<Real> operator +(const Modifier<Real>& _rhs) const {Modifier<Real> result = *this; result += _rhs; return result;}
			Modifier<Real> operator *(const Modifier<Real>& _rhs) const {Modifier<Real> result = *this; result *= _rhs; return result;}
			Modifier<Real> operator *(Real _coef) const {Modifier<Real> result = *this; result *= _coef; return result;}
			
			static const Modifier<Real> ZERO;
			static const Modifier<Real> IDENTITY;
		};

		template<>
		class GOTHOGRE_EXPORT Modifier<Degree> : public BaseModifier<Degree>
		{
		public:
			Modifier<Degree>(const BaseModifier<Degree>& _src) : BaseModifier<Degree>(_src) {}
			Modifier<Degree>(Degree _multiplier, Degree _addend) : BaseModifier<Degree>(_multiplier, _addend) {}
			Modifier<Degree>() : BaseModifier<Degree>( Degree(0), Degree(0)) {}

			Modifier<Degree> operator +(const Modifier<Degree>& _rhs) const {Modifier<Degree> result = *this; result += _rhs; return result;}
			Modifier<Degree> operator *(const Modifier<Degree>& _rhs) const {Modifier<Degree> result = *this; result *= _rhs; return result;}
			Modifier<Degree> operator *(Real _coef) const {Modifier<Degree> result = *this; result *= _coef; return result;}

			static const Modifier<Degree> ZERO;
			static const Modifier<Degree> IDENTITY;
		};

		template<>
		class GOTHOGRE_EXPORT Modifier<size_t> : public BaseModifier<size_t>
		{
		public:
			Modifier<size_t>(const BaseModifier<size_t>& _src) : BaseModifier<size_t>(_src) {}
			Modifier<size_t>(size_t _multiplier, size_t _addend) : BaseModifier<size_t>(_multiplier, _addend) {}
			Modifier<size_t>() : BaseModifier<size_t>( size_t(0), size_t(0)) {}

			Modifier<size_t> operator +(const Modifier<size_t>& _rhs) const {Modifier<size_t> result = *this; result += _rhs; return result;}
			Modifier<size_t> operator *(const Modifier<size_t>& _rhs) const {Modifier<size_t> result = *this; result *= _rhs; return result;}
			Modifier<size_t> operator *(Real _coef) const {Modifier<size_t> result = *this; result *= _coef; return result;}

			static const Modifier<size_t> ZERO;
			static const Modifier<size_t> IDENTITY;
		};

		template<>
		class GOTHOGRE_EXPORT Modifier<ColourValue> : public BaseModifier<ColourValue>
		{
		public:
			Modifier<ColourValue>(const BaseModifier<ColourValue>& _src) : BaseModifier<ColourValue>(_src) {}
			Modifier<ColourValue>(const ColourValue& _multiplier, const ColourValue& _addend) : BaseModifier<ColourValue>(_multiplier, _addend) {}
			Modifier<ColourValue>() : BaseModifier<ColourValue>(ColourValue(0, 0, 0, 0), ColourValue(0, 0, 0, 0)) {}

			Modifier<ColourValue> operator +(const Modifier<ColourValue>& _rhs) const {Modifier<ColourValue> result = *this; result += _rhs; return result;}
			Modifier<ColourValue> operator *(const Modifier<ColourValue>& _rhs) const {Modifier<ColourValue> result = *this; result *= _rhs; return result;}
			Modifier<ColourValue> operator *(Real _coef) const {Modifier<ColourValue> result = *this; result *= _coef; return result;}

			static const Modifier<ColourValue> ZERO;
			static const Modifier<ColourValue> IDENTITY;
		};

		template<>
		class GOTHOGRE_EXPORT Modifier<Vector2> : public BaseModifier<Vector2>
		{
		public:
			Modifier<Vector2>(const BaseModifier<Vector2>& _src) : BaseModifier<Vector2>(_src) {}
			Modifier<Vector2>(const Vector2& _multiplier, const Vector2& _addend) : BaseModifier<Vector2>(_multiplier, _addend) {}
			Modifier<Vector2>() : BaseModifier<Vector2>(Vector2::ZERO, Vector2::ZERO) {}

			Modifier<Vector2> operator +(const Modifier<Vector2>& _rhs) const {Modifier<Vector2> result = *this; result += _rhs; return result;}
			Modifier<Vector2> operator *(const Modifier<Vector2>& _rhs) const {Modifier<Vector2> result = *this; result *= _rhs; return result;}
			Modifier<Vector2> operator *(Real _coef) const {Modifier<Vector2> result = *this; result *= _coef; return result;}

			static const Modifier<Vector2> ZERO;
			static const Modifier<Vector2> IDENTITY;
		};

		template<>
		class GOTHOGRE_EXPORT Modifier<Vector3> : public BaseModifier<Vector3>
		{
		public:
			Modifier<Vector3>(const BaseModifier<Vector3>& _src) : BaseModifier<Vector3>(_src) {}
			Modifier<Vector3>(const Vector3& _multiplier, const Vector3& _addend) : BaseModifier<Vector3>(_multiplier, _addend) {}
			Modifier<Vector3>() : BaseModifier<Vector3>(Vector3::ZERO, Vector3::ZERO) {}

			Modifier<Vector3> operator +(const Modifier<Vector3>& _rhs) const {Modifier<Vector3> result = *this; result += _rhs; return result;}
			Modifier<Vector3> operator *(const Modifier<Vector3>& _rhs) const {Modifier<Vector3> result = *this; result *= _rhs; return result;}
			Modifier<Vector3> operator *(Real _coef) const {Modifier<Vector3> result = *this; result *= _coef; return result;}

			static const Modifier<Vector3> ZERO;
			static const Modifier<Vector3> IDENTITY;
		};

		typedef Modifier<Real>        RealModifier;
		typedef Modifier<size_t>      SizeTModifier;
		typedef Modifier<Degree>      DegreeModifier;
		typedef Modifier<ColourValue> ColourValueModifier;
		typedef Modifier<Vector3>     Vector3Modifier;
		typedef Modifier<Vector2>     Vector2Modifier;

	public:
		/** Backgroung of a viewport. */
		struct GOTHOGRE_EXPORT Background
		{
			/** The name of the background setting. */
			String       name;

			/** The colour of the background of the viewport. */
			ColourValue  colour;
		};

		/** Is there a background in the SkyDef? */
		bool hasBackground() const;

		/** Returns a reference to the background. */
		const Background& getBackground() const;

		/** Inserts a background to the SkyDef;
		if there is a background already, it will be replaced with the specified one. */
		void insertBackground(const Background& _src);

		/** Removes a background from the SkyDef. */
		void removeBackground();

	public:
		/** Background's controller. */
		struct GOTHOGRE_EXPORT BackgroundController
		{
			/** Key of the background's controller. */
			struct GOTHOGRE_EXPORT Key
			{
				Real                 value;
				Real                 period;
				ColourValueModifier  colourModifier;

				Key() : 
					value( 0 ),
					period( Math::POS_INFINITY ), 
					colourModifier( ColourValueModifier::IDENTITY ) {}
			};

			typedef vector<Key>::type Keys;

			/** Keys of the controller. */
			Keys    keys;

			/** The name of the controlled background. */
			String  backgroundName;

			/** The name of the controller's function. */
			String  functionName;
		};

		/** Returns the number of the background controllers in the SkyDef. */
		size_t getNumBackgroundControllers() const;

		/** Returns a background controller by index. */
		const BackgroundController& getBackgroundController(size_t _index) const;

		typedef vector<BackgroundController>::type BackgroundControllers;
		typedef ConstVectorIterator<BackgroundControllers> BackgroundControllerIterator;

		/** Returns an iterator to the list of all the background controllers. */
		BackgroundControllerIterator getBackgroundControllerIterator() const;

		/** Inserts a background controller with a specified name to the SkyDef,
		the name must be unique. */
		void insertBackgroundController(const BackgroundController& _src);

		/** Removes a background controller by index from the SkyDef. */
		void removeBackgroundController(size_t _index);

		/** Removes all backgrounds from the SkyDef. */
		void removeAllBackgroundControllers();

	public:
		/** Z-fog. */
		struct GOTHOGRE_EXPORT Fog
		{
			/** The name of the fog*/
			String       name;

			/** The colour of the fog. */
			ColourValue  colour;

			/** The distance (in cm) at which linear fog starts to encroach. */
			Real         nearDistance;
			
			/** The distance (in cm) at which linear fog becomes completely opaque. */
			Real         farDistance;
		};

		/** Is there a fog in the SkyDef? */
		bool hasFog() const;

		/** Returns a reference to the fog. */
		const Fog& getFog() const;

		/** Inserts a fog to the SkyDef;
		if there is a fog already, it will be replaced with the specified one. */
		void insertFog(const Fog& _src);

		/** Removes a fog from the SkyDef. */
		void removeFog();

	public:
		/** Fog's controller. */
		struct GOTHOGRE_EXPORT FogController
		{
			/** Key of the background's controller. */
			struct GOTHOGRE_EXPORT Key
			{
				Real                 value;
				Real                 period;
				ColourValueModifier  colourModifier;
				RealModifier         nearDistanceModifier;
				RealModifier         farDistanceModifier;

				Key() :
					value( 0 ),
					period( Math::POS_INFINITY ),
					colourModifier( ColourValueModifier::IDENTITY ),
					nearDistanceModifier (RealModifier::IDENTITY ),
					farDistanceModifier( RealModifier::IDENTITY ) {}
			};

			typedef vector<Key>::type Keys;

			/** Name of the controlled fog. */
			String  fogName;

			/** Name of the controller function. */
			String  functionName;

			/** Keys of the controller. */
			Keys    keys;
		};

		/** Returns the number of the fog controllers in the SkyDef. */
		size_t getNumFogControllers() const;

		/** Returns a fog controller by index. */
		const FogController& getFogController(size_t _index) const;

		typedef vector<FogController>::type FogControllers;
		typedef ConstVectorIterator<FogControllers> FogControllerIterator;

		/** Returns an iterator to the list of all the fog controllers. */
		FogControllerIterator getFogControllerIterator() const;

		/** Inserts a fog controller with a specified name to the SkyDef,
		the name must be unique. */
		void insertFogController(const FogController& _src);

		/** Removes a fog controller by index from the SkyDef. */
		void removeFogController(size_t _index);

		/** Removes all fog controllers from the SkyDef. */
		void removeAllFogControllers();

	public:
		/** Far clipping plane. */
		struct GOTHOGRE_EXPORT FarClippingPlane
		{
			/** The name of the fog*/
			String       name;

			/** The distance (in cm) from camera to the far clipping plane. */
			Real         distance;
		};

		/** Is there a far clipping plane's in the SkyDef? */
		bool hasFarClippingPlane() const;

		/** Returns a reference to the far clipping plane's. */
		const FarClippingPlane& getFarClippingPlane() const;

		/** Inserts a far clipping plane's to the SkyDef;
		if there is a far clipping plane's already, it will be replaced with the specified one. */
		void insertFarClippingPlane(const FarClippingPlane& _src);

		/** Removes a far clipping plane's from the SkyDef. */
		void removeFarClippingPlane();

	public:
		/** FarClippingPlane's controller. */
		struct GOTHOGRE_EXPORT FarClippingPlaneController
		{
			/** Key of the background's controller. */
			struct GOTHOGRE_EXPORT Key
			{
				Real                 value;
				Real                 period;
				RealModifier         distanceModifier;

				Key() :
					value( 0 ),
					period( Math::POS_INFINITY ),
					distanceModifier( RealModifier::IDENTITY ) {}
			};

			typedef vector<Key>::type Keys;

			/** Name of the controlled far clipping plane. */
			String  farClippingPlaneName;

			/** Name of the controller function. */
			String  functionName;

			/** Keys of the controller. */
			Keys    keys;
		};

		/** Returns the number of the far clipping plane's controllers in the SkyDef. */
		size_t getNumFarClippingPlaneControllers() const;

		/** Returns a far clipping plane's controller by index. */
		const FarClippingPlaneController& getFarClippingPlaneController(size_t _index) const;

		typedef vector<FarClippingPlaneController>::type FarClippingPlaneControllers;
		typedef ConstVectorIterator<FarClippingPlaneControllers> FarClippingPlaneControllerIterator;

		/** Returns an iterator to the list of all the far clipping plane's controllers. */
		FarClippingPlaneControllerIterator getFarClippingPlaneControllerIterator() const;

		/** Inserts a far clipping plane's controller with a specified name to the SkyDef,
		the name must be unique. */
		void insertFarClippingPlaneController(const FarClippingPlaneController& _src);

		/** Removes a far clipping plane's controller by index from the SkyDef. */
		void removeFarClippingPlaneController(size_t _index);

		/** Removes all far clipping plane's controllers from the SkyDef. */
		void removeAllFarClippingPlaneControllers();

	public:
		/** Atmosphere is the air excluding clouds. */
		struct GOTHOGRE_EXPORT Atmosphere
		{
			String       name;
			Real         radius;
			Degree       minAltitude;
			int          layer;
			ColourValue  colour;
			ColourValue  ambientLight;
			ColourValue  ambientLightFilter;
			ColourValue  diffuseLightFilter;
			ColourValue  specularLightFilter;

			Atmosphere() :
				colour( ColourValue(0, 0, 1, 1) ),
				ambientLight( ColourValue(0, 0, 0, 0) ),
				ambientLightFilter( ColourValue(1, 1, 1, 1) ),
				diffuseLightFilter( ColourValue(1, 1, 1, 1) ),
				specularLightFilter( ColourValue(1, 1, 1, 1) ) {}
		};

		/** Returns the number of the atmospheres in the SkyDef. */
		size_t getNumAtmospheres() const;

		/** Returns a atmosphere by index. */
		const Atmosphere& getAtmosphere(size_t _index) const;

		/** Returns a atmosphere by index. */
		const Atmosphere& getAtmosphere(const String& _name) const;

		typedef vector<Atmosphere>::type Atmospheres;
		typedef ConstVectorIterator<Atmospheres> AtmosphereIterator;

		/** Returns an iterator to the list of all the atmospheres. */
		AtmosphereIterator getAtmosphereIterator() const;

		/** Does the atmosphere exist. */
		bool hasAtmosphere(const String& _name) const;

		/** Returns index of a atmosphere, or -1 if not found */
		size_t findAtmosphereIndex(const String& _name) const;

		/** Returns index of a atmosphere, throw an exception if not found */
		size_t getAtmosphereIndex(const String& _name) const;

		/** Inserts a atmosphere with a specified name to the SkyDef,
		the name must be unique. */
		void insertAtmosphere(const Atmosphere& _src);

		/** Removes a atmosphere by index from the SkyDef. */
		void removeAtmosphere(size_t _index);

		/** Removes all atmospheres from the SkyDef. */
		void removeAllAtmospheres();

	public:
		/** Atmosphere's controller. */
		struct GOTHOGRE_EXPORT AtmosphereController
		{
			typedef std::pair<Degree, ColourValueModifier>         AngleColourModifierPair;
			typedef vector<AngleColourModifierPair>::type          AngleColourModifierPairs;
			typedef ConstVectorIterator<AngleColourModifierPairs>  AngleColourModifierPairIterator;

			struct GOTHOGRE_EXPORT Key
			{
				Real                      value;
				Real                      period;
				ColourValueModifier       ambientLightModifier;
				ColourValueModifier       ambientLightFilterModifier;
				ColourValueModifier       diffuseLightFilterModifier;
				ColourValueModifier       specularLightFilterModifier;
				ColourValueModifier       colourModifier;
				AngleColourModifierPairs  angleColourModifierPairs;

				Key() :
					value( 0 ),
					period( Math::POS_INFINITY ),
					ambientLightModifier( ColourValueModifier::IDENTITY ),
					ambientLightFilterModifier( ColourValueModifier::IDENTITY ),
					diffuseLightFilterModifier( ColourValueModifier::IDENTITY ),
					specularLightFilterModifier( ColourValueModifier::IDENTITY ),
					colourModifier( ColourValueModifier::IDENTITY ) {}
			};

			typedef vector<Key>::type Keys;

			String  atmosphereName;
			String  functionName;
			Keys    keys;
		};

		/** Returns the number of the atmosphere controllers in the SkyDef. */
		size_t getNumAtmosphereControllers() const;

		/** Returns a atmosphere controller by index. */
		const AtmosphereController& getAtmosphereController(size_t _index) const;

		typedef vector<AtmosphereController>::type AtmosphereControllers;
		typedef ConstVectorIterator<AtmosphereControllers> AtmosphereControllerIterator;

		/** Returns an iterator to the list of all the atmosphere controllers. */
		AtmosphereControllerIterator getAtmosphereControllerIterator() const;

		/** Inserts a atmosphere controller with a specified name to the SkyDef,
		the name must be unique. */
		void insertAtmosphereController(const AtmosphereController& _src);

		/** Removes a atmosphere controller by index from the SkyDef. */
		void removeAtmosphereController(size_t _index);

		/** Removes all atmosphere controllers from the SkyDef. */
		void removeAllAtmosphereControllers();

	public:
		/** Body is a celestial body: sun, moon, etc. */
		struct GOTHOGRE_EXPORT Body
		{
			String       name;
			MeshPtr      mesh;
			String       mtlName;
			int          layer;
			Degree       altitude;
			Degree       azimuth;
			Real         distance;
			Vector3      position;
			Vector3      eulerAnglesDeg;
			Vector3      scale;
			ColourValue  ambientLight;
			ColourValue  diffuseLight;
			ColourValue  specularLight;

			Body() :
				altitude( 0 ),
				azimuth( 0 ),
				distance( 1000 ),
				position( Vector3::ZERO ),
				eulerAnglesDeg( Vector3::ZERO ),
				scale( Vector3::UNIT_SCALE ),
				ambientLight( ColourValue(0, 0, 0, 0) ),
				diffuseLight( ColourValue(0, 0, 0, 0) ),
				specularLight( ColourValue(0, 0, 0, 0) ) {}
		};

		/** Returns the number of the bodys in the SkyDef. */
		size_t getNumBodies() const;

		/** Returns a body by index. */
		const Body& getBody(size_t _index) const;

		/** Returns a body by index. */
		const Body& getBody(const String& _name) const;

		typedef vector<Body>::type Bodies;
		typedef ConstVectorIterator<Bodies> BodyIterator;

		/** Returns an iterator to the list of all the bodies. */
		BodyIterator getBodyIterator() const;

		/** Does the body exist. */
		bool hasBody(const String& _name) const;

		/** Returns index of a body, or -1 if not found */
		size_t findBodyIndex(const String& _name) const;

		/** Returns index of a body, throw an exception if not found */
		size_t getBodyIndex(const String& _name) const;

		/** Inserts a body with a specified name to the SkyDef,
		the name must be unique. */
		void insertBody(const Body& _src);

		/** Removes a body by index from the SkyDef. */
		void removeBody(size_t _index);

		/** Removes all bodies from the SkyDef. */
		void removeAllBodies();

	public:
		/** Body's controller. */
		struct GOTHOGRE_EXPORT BodyController
		{
			struct GOTHOGRE_EXPORT Key
			{
				Real                   value;
				Real                   period;
				DegreeModifier         altitudeModifier;
				DegreeModifier         azimuthModifier;
				RealModifier           distanceModifier;
				Vector3Modifier        positionModifier;
				Vector3Modifier        eulerAnglesDegModifier;
				Vector3Modifier        scaleModifier;
				ColourValueModifier    ambientLightModifier;
				ColourValueModifier    diffuseLightModifier;
				ColourValueModifier    specularLightModifier;

				Key() :
					value( 0 ),
					period( Math::POS_INFINITY ),
					altitudeModifier( DegreeModifier::IDENTITY ),
					azimuthModifier( DegreeModifier::IDENTITY ),
					distanceModifier( RealModifier::IDENTITY ),
					positionModifier( Vector3Modifier::IDENTITY ),
					eulerAnglesDegModifier( Vector3Modifier::IDENTITY ),
					scaleModifier( Vector3Modifier::IDENTITY ),
					ambientLightModifier( ColourValueModifier::IDENTITY ),
					diffuseLightModifier( ColourValueModifier::IDENTITY ),
					specularLightModifier( ColourValueModifier::IDENTITY ) {}
			};

			typedef vector<Key>::type Keys;

			String  bodyName;
			String  functionName;
			Keys    keys;
		};

		/** Returns the number of the body controllers in the SkyDef. */
		size_t getNumBodyControllers() const;

		/** Returns a body controller by index. */
		const BodyController& getBodyController(size_t _index) const;

		typedef vector<BodyController>::type BodyControllers;
		typedef ConstVectorIterator<BodyControllers> BodyControllerIterator;

		/** Returns an iterator to the list of all the body controllers. */
		BodyControllerIterator getBodyControllerIterator() const;

		/** Inserts a body controller with a specified name to the SkyDef,
		the name must be unique. */
		void insertBodyController(const BodyController& _src);

		/** Removes a body controller by index from the SkyDef. */
		void removeBodyController(size_t _index);

		/** Removes all body controllers from the SkyDef. */
		void removeAllBodyControllers();

	public:
		/** CloudLayer can be any mesh linked to the camera's current position. */
		struct GOTHOGRE_EXPORT CloudLayer
		{
			String       name;
			MeshPtr      mesh;
			String       mtlName;
			int          layer;
			Vector3      position;
			Vector3      eulerAnglesDeg;
			Vector3      scale;
			ColourValue  ambientLight;
			ColourValue  diffuseLight;
			ColourValue  specularLight;
			ColourValue  ambientLightFilter;
			ColourValue  diffuseLightFilter;
			ColourValue  specularLightFilter;

			CloudLayer() :
				position(      Vector3::ZERO ),
				eulerAnglesDeg( Vector3::ZERO ),
				scale(         Vector3::UNIT_SCALE ),
				ambientLight(  ColourValue(0, 0, 0, 0) ),
				diffuseLight(  ColourValue(0, 0, 0, 0) ),
				specularLight( ColourValue(0, 0, 0, 0) ),
				ambientLightFilter(  ColourValue(1, 1, 1, 1) ),
				diffuseLightFilter(  ColourValue(1, 1, 1, 1) ),
				specularLightFilter( ColourValue(1, 1, 1, 1) ) {}
		};

		/** Returns the number of the cloud layers in the SkyDef. */
		size_t getNumCloudLayers() const;

		/** Returns a cloud layer by index. */
		const CloudLayer& getCloudLayer(size_t _index) const;

		typedef vector<CloudLayer>::type CloudLayers;
		typedef ConstVectorIterator<CloudLayers> CloudLayerIterator;

		/** Returns an iterator to the list of all the cloud layers. */
		CloudLayerIterator getCloudLayerIterator() const;

		/** Returns a cloud layer by index. */
		const CloudLayer& getCloudLayer(const String& _name) const;

		/** Does the cloud layer exist. */
		bool hasCloudLayer(const String& _name) const;

		/** Returns index of a cloud layer, or -1 if not found */
		size_t findCloudLayerIndex(const String& _name) const;

		/** Returns index of a cloud layer, throw an exception if not found */
		size_t getCloudLayerIndex(const String& _name) const;

		/** Inserts a cloud layer with a specified name to the SkyDef,
		the name must be unique. */
		void insertCloudLayer(const CloudLayer& _src);

		/** Removes a cloud layer by index from the SkyDef. */
		void removeCloudLayer(size_t _index);

		/** Removes all cloud layers from the SkyDef. */
		void removeAllCloudLayers();

	public:
		/** CloudLayer's controller. */
		struct GOTHOGRE_EXPORT CloudLayerController
		{
			struct GOTHOGRE_EXPORT Key
			{
				Real                   value;
				Real                   period;
				Vector3Modifier        positionModifier;
				Vector3Modifier        eulerAnglesDegModifier;
				Vector3Modifier        scaleModifier;
				ColourValueModifier    ambientLightModifier;
				ColourValueModifier    diffuseLightModifier;
				ColourValueModifier    specularLightModifier;
				ColourValueModifier    ambientLightFilterModifier;
				ColourValueModifier    diffuseLightFilterModifier;
				ColourValueModifier    specularLightFilterModifier;

				Key() :
					value( 0 ),
					period( Math::POS_INFINITY ),
					positionModifier( Vector3Modifier::IDENTITY ),
					eulerAnglesDegModifier( Vector3Modifier::IDENTITY ),
					scaleModifier( Vector3Modifier::IDENTITY ),
					ambientLightModifier( ColourValueModifier::IDENTITY ),
					diffuseLightModifier( ColourValueModifier::IDENTITY ),
					specularLightModifier( ColourValueModifier::IDENTITY ),
					ambientLightFilterModifier( ColourValueModifier::IDENTITY ),
					diffuseLightFilterModifier( ColourValueModifier::IDENTITY ),
					specularLightFilterModifier( ColourValueModifier::IDENTITY ) {}
			};

			typedef vector<Key>::type Keys;

			String  cloudLayerName;
			String  functionName;
			Keys    keys;
		};

		/** Returns the number of the cloud layer controllers in the SkyDef. */
		size_t getNumCloudLayerControllers() const;

		/** Returns a cloud layer controller by index. */
		const CloudLayerController& getCloudLayerController(size_t _index) const;

		typedef vector<CloudLayerController>::type CloudLayerControllers;
		typedef ConstVectorIterator<CloudLayerControllers> CloudLayerControllerIterator;

		/** Returns an iterator to the list of all the cloud layer controllers. */
		CloudLayerControllerIterator getCloudLayerControllerIterator() const;

		/** Inserts a cloud layer controller with a specified name to the SkyDef,
		the name must be unique. */
		void insertCloudLayerController(const CloudLayerController& _src);

		/** Removes a cloud layer controller by index from the SkyDef. */
		void removeCloudLayerController(size_t _index);

		/** Removes all cloud layer controllers from the SkyDef. */
		void removeAllCloudLayerControllers();

	public:
		/** Material's controller. */
		struct GOTHOGRE_EXPORT MaterialController
		{
			struct GOTHOGRE_EXPORT Key
			{
				Real                   value;
				Real                   period;
				SizeTModifier		   frameIndexModifier;
				Vector2Modifier        scrollSpeedModifier;
				ColourValueModifier	   colourFactorModifier;
				RealModifier           specularIntensityFactorModifier;
				ColourValueModifier	   specularColourFactorModifier;
				RealModifier           reflectivityFactorModifier;

				Key() :
					value( 0 ),
					period( Math::POS_INFINITY ),
					frameIndexModifier( SizeTModifier::IDENTITY ),
					scrollSpeedModifier( Vector2Modifier::IDENTITY ),
					colourFactorModifier( ColourValueModifier::IDENTITY ),
					specularIntensityFactorModifier( RealModifier::IDENTITY ),
					specularColourFactorModifier( ColourValueModifier::IDENTITY ),
					reflectivityFactorModifier( RealModifier::IDENTITY ) {}
			};

			typedef vector<Key>::type Keys;

			String  mtlName;
			String  functionName;
			Keys    keys;
		};

		/** Returns the number of the material controllers in the SkyDef. */
		size_t getNumMaterialControllers() const;

		/** Returns a material controller by index. */
		const MaterialController& getMaterialController(size_t _index) const;

		typedef vector<MaterialController>::type MaterialControllers;
		typedef ConstVectorIterator<MaterialControllers> MaterialControllerIterator;

		/** Returns an iterator to the list of all the material controllers
		which were defined in the sky. */
		MaterialControllerIterator getMaterialControllerIterator() const;

		/** Inserts a material controller with a specified name to the SkyDef,
		the name must be unique. */
		void insertMaterialController(const MaterialController& _src);

		/** Removes a material controller by index from the SkyDef. */
		void removeMaterialController(size_t _index);

		/** Removes all material controllers from the SkyDef. */
		void removeAllMaterialControllers();

	public:
		/** Sound is a sound source linked to the skies. */
		struct GOTHOGRE_EXPORT Sound
		{
			String       name;
			SoundDefPtr  soundDef;
			bool         looping;
			Vector3      position;
			Real         maxDistance;
			Real         volumePerc;

			Sound() :
				looping( false ),
				position(      Vector3::ZERO ),
				maxDistance( 1 ),
				volumePerc( 1 ) {}
		};

		/** Returns the number of the sounds in the SkyDef. */
		size_t getNumSounds() const;

		/** Returns a sound by index. */
		const Sound& getSound(size_t _index) const;

		typedef vector<Sound>::type Sounds;
		typedef ConstVectorIterator<Sounds> SoundIterator;

		/** Returns an iterator to the list of all the sounds 
		which were defined in the sky. */
		SoundIterator getSoundIterator() const;

		/** Returns a sound by index. */
		const Sound& getSound(const String& _name) const;

		/** Does the sound exist. */
		bool hasSound(const String& _name) const;

		/** Returns index of a sound, or -1 if not found */
		size_t findSoundIndex(const String& _name) const;

		/** Returns index of a sound, throw an exception if not found */
		size_t getSoundIndex(const String& _name) const;

		/** Inserts a sound with a specified name to the SkyDef,
		the name must be unique. */
		void insertSound(const Sound& _src);

		/** Removes a sound by index from the SkyDef. */
		void removeSound(size_t _index);

		/** Removes all sounds from the SkyDef. */
		void removeAllSounds();

	public:
		/** Sound's controller. */
		struct GOTHOGRE_EXPORT SoundController
		{
			struct GOTHOGRE_EXPORT Key
			{
				Real                   value;
				Real                   period;
				Vector3Modifier        positionModifier;
				RealModifier           maxDistanceModifier;
				RealModifier           volumePercModifier;

				Key() :
					value( 0 ),
					period( Math::POS_INFINITY ),
					positionModifier( Vector3Modifier::IDENTITY ),
					maxDistanceModifier( RealModifier::IDENTITY ),
					volumePercModifier( RealModifier::IDENTITY ) {}
			};

			typedef vector<Key>::type Keys;

			String  soundName;
			String  functionName;
			Keys    keys;
		};

		/** Returns the number of the sound controllers in the SkyDef. */
		size_t getNumSoundControllers() const;

		/** Returns a sound controller by index. */
		const SoundController& getSoundController(size_t _index) const;

		typedef vector<SoundController>::type SoundControllers;
		typedef ConstVectorIterator<SoundControllers> SoundControllerIterator;

		/** Returns an iterator to the list of all the sound controllers
		which were defined in the sky. */
		SoundControllerIterator getSoundControllerIterator() const;

		/** Inserts a sound controller with a specified name to the SkyDef,
		the name must be unique. */
		void insertSoundController(const SoundController& _src);

		/** Removes a sound controller by index from the SkyDef. */
		void removeSoundController(size_t _index);

		/** Removes all sound controllers from the SkyDef. */
		void removeAllSoundControllers();

	public:
		/** Overlay is another sky which must be rendered after this sky. */
		struct GOTHOGRE_EXPORT Overlay
		{
			String       name;
			String       skyName;
			Real         weight;

			Overlay() :
				weight( 1 ) {}
		};

		/** Returns the number of the overlays in the SkyDef. */
		size_t getNumOverlays() const;

		/** Returns a overlay by index. */
		const Overlay& getOverlay(size_t _index) const;

		typedef vector<Overlay>::type Overlays;
		typedef ConstVectorIterator<Overlays> OverlayIterator;

		/** Returns an iterator to the list of all the overlays applied to this sky. */
		OverlayIterator getOverlayIterator() const;

		/** Returns a overlay by index. */
		const Overlay& getOverlay(const String& _name) const;

		/** Does the overlay exist. */
		bool hasOverlay(const String& _name) const;

		/** Returns index of a overlay, or -1 if not found */
		size_t findOverlayIndex(const String& _name) const;

		/** Returns index of a overlay, throw an exception if not found */
		size_t getOverlayIndex(const String& _name) const;

		/** Inserts a overlay with a specified name to the SkyDef,
		the name must be unique. */
		void insertOverlay(const Overlay& _src);

		/** Removes a overlay by index from the SkyDef. */
		void removeOverlay(size_t _index);

		/** Removes all overlays from the SkyDef. */
		void removeAllOverlays();

	public:
		/** Overlay's controller. */
		struct GOTHOGRE_EXPORT OverlayController
		{
			struct GOTHOGRE_EXPORT Key
			{
				Real                   value;
				Real                   period;
				RealModifier           weightModifier;

				Key() :
					value( 0 ),
					period( Math::POS_INFINITY ),
					weightModifier( RealModifier::IDENTITY ) {}
			};

			typedef vector<Key>::type Keys;

			String  overlayName;
			String  functionName;
			Keys    keys;
		};

		/** Returns the number of the overlay controllers in the SkyDef. */
		size_t getNumOverlayControllers() const;

		/** Returns a overlay controller by index. */
		const OverlayController& getOverlayController(size_t _index) const;

		typedef vector<OverlayController>::type OverlayControllers;
		typedef ConstVectorIterator<OverlayControllers> OverlayControllerIterator;

		/** Returns an iterator to the list of all the overlay controllers. */
		OverlayControllerIterator getOverlayControllerIterator() const;

		/** Inserts a overlay controller with a specified name to the SkyDef,
		the name must be unique. */
		void insertOverlayController(const OverlayController& _src);

		/** Removes a overlay controller by index from the SkyDef. */
		void removeOverlayController(size_t _index);

		/** Removes all overlay controllers from the SkyDef. */
		void removeAllOverlayControllers();

	public:
		/** A function can calculate a value in any time by some algorithm. */
		struct GOTHOGRE_EXPORT Function
		{
		public:
			/** Name of the function. */
			String           name;

			enum Type
			{
				CONSTANT,       // uses: value
				LINEAR,         // uses: initValue, rate, period
				QUADRATIC,      // uses: initValue, initRate, rate2, period 
				SIN,            // uses: initPhase, minValue, maxValue, period
				RANDOM_VALUE,   // uses: minValue, maxValue, step
				RANDOM_RATE,    // uses: initValue, minValue, maxValue, minRate, maxRate, step
				RANDOM_RATE2,   // uses: initValue, minValue, maxValue, initRate, minRate, maxRate, minRate2, maxRate2, step, bounce
				RANDOM_MANUAL,  // uses: initValue, minRate, maxRate, states
			};

			/** Type of the function. */
			Type            type;

			enum Argument
			{
				WORLD_TIME,
				WORLD_TIME_REL,   // uses: skyName
				SYSTEM_TIME,
				SYSTEM_TIME_REL,  // uses: skyName
				ALTITUDE_OF_BODY, // uses: bodyName
				UNDERWATER_DEPTH,
				INDOOR_DEPTH
			};

			/** Type of the argument the function depends on. */
			Argument         argument;

			/** Name of the body. */
			String           bodyName;

			/** Name of the sky. */
			String           skyName;

			/** Value of the function. */
			Real             value;

			/** Initial value of the function. */
			Real             initValue;

			/** Minimum value of the function. */
			Real             minValue;

			/** Maximum value of the function. */
			Real             maxValue;

			/** Rate of increase of the function. */
			Real             rate;

			/** Initial rate of increase of the function. */
			Real             initRate;

			/** Minimum rate of increase of the function. */
			Real             minRate;

			/** Maximum rate of increase of the function. */
			Real             maxRate;
			
			/** Rate of increase of rate of increase of the function. */
			Real             rate2;

			/** Initial rate of increase of rate of increase of the function. */
			Real             initRate2;

			/** Minimum rate of increase of rate of increase of the function. */
			Real             minRate2;

			/** Maximum rate of increase of rate of increase of the function. */
			Real             maxRate2;

			/** Bounce coefficient. */
			Real             bounce;
			
			/** Period of the function. */
			Real             period;

			/** Initial phase of the function. */
			Real             initPhase;

			/** Time of transition to a neightbouring stationary state. */
			Real             step;

			/** Description of a stationary state. */
			struct State
			{
				Real   value;
				Real   step;
				Real   stayChance;
				Real   upChance;
				Real   downChance;

				State() : 
					stayChance(0), upChance(0), downChance(0), step(0), value(0) {}
			};

			typedef vector<State>::type States;

			/** Stationary states. */
			States states;

			Function() :
				value(0),
				initValue(0),
				minValue(Math::NEG_INFINITY),
				maxValue(Math::POS_INFINITY),
				rate(0),
				initRate(0),
				minRate(Math::NEG_INFINITY),
				maxRate(Math::POS_INFINITY),
				initRate2(0),
				minRate2(Math::NEG_INFINITY),
				maxRate2(Math::POS_INFINITY),
				bounce( 1 ),
				period( Math::POS_INFINITY ),
				initPhase( 0 ),
				step( 0 ) {}
		};

		/** Returns the number of the functions in the SkyDef. */
		size_t getNumFunctions() const;

		/** Returns a function by index. */
		const Function& getFunction(size_t _index) const;

		typedef vector<Function>::type Functions;
		typedef ConstVectorIterator<Functions> FunctionIterator;

		/** Returns an iterator to the list of all the functions. */
		FunctionIterator getFunctionIterator() const;

		/** Returns a function by index. */
		const Function& getFunction(const String& _name) const;

		/** Does the function exist. */
		bool hasFunction(const String& _name) const;

		/** Returns index of a function, or -1 if not found */
		size_t findFunctionIndex(const String& _name) const;

		/** Returns index of a function, throw an exception if not found */
		size_t getFunctionIndex(const String& _name) const;

		/** Inserts a function with a specified name to the SkyDef,
		the name must be unique. */
		void insertFunction(const Function& _src);

		/** Removes a function by index from the SkyDef. */
		void removeFunction(size_t _index);

		/** Removes all functions from the SkyDef. */
		void removeAllFunctions();

	public:
		SkyDef(ResourceManager* _creator, const String& _name, ResourceHandle _handle,
			const String& _group, bool _isManual, ManualResourceLoader* _loader);
		~SkyDef();

	private:
		Fog*                   mFog;
		FogControllers         mFogControllers;
		FarClippingPlane*      mFarClippingPlane;
		FarClippingPlaneControllers mFarClippingPlaneControllers;
		Background*            mBackground;
		BackgroundControllers  mBackgroundControllers;
		Atmospheres            mAtmospheres;
		AtmosphereControllers  mAtmosphereControllers;
		Bodies                 mBodies;
		BodyControllers        mBodyControllers;
		CloudLayers            mCloudLayers;
		CloudLayerControllers  mCloudLayerControllers;
		MaterialControllers    mMaterialControllers;
		Sounds                 mSounds;
		SoundControllers       mSoundControllers;
		Overlays               mOverlays;
		OverlayControllers     mOverlayControllers;
		Functions              mFunctions;
	};


	//-----------------------------------------------------------------------
	/** Shared pointer to SkyDef. */
	typedef SubclassSharedPtr<SkyDef, ResourcePtr> SkyDefPtr;


} // namespace GothOgre

#endif // GOTHOGRE_SKY_DEF_H