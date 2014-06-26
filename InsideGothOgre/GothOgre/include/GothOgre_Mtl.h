#ifndef GOTHOGRE_MTL_H
#define GOTHOGRE_MTL_H

#include "GothOgre_ResourceEx.h"
#include "GothOgre_SubclassSharedPtr.h"
#include "GothOgre_EnumIO.h"


namespace GothOgre
{
	class SceneManagerEx;
	class MatGenParams;


	//------------------------------------------------------------------------
	/** Material's group (water, stone, and so on)
	as it is used in Spacer. */
	class GOTHOGRE_EXPORT MatGroup
	{
	public:
		enum Enum
		{
			UNDEF,
			METALL,
			STONE,
			WOOD,
			EARTH,
			WATER
		};

		MatGroup(int _value = UNDEF) : mValue(Enum(_value)) {}
		operator int() const {return mValue;}
		GOTHOGRE_DECLARE_ENUM_IO(MatGroup)
	private:
		Enum mValue;
	};



	//------------------------------------------------------------------------
	/** This class contains a material's source data, which can be used 
	to create a material later. 
	This class stores all parameters from the original Gothic's zCMaterial 
	plus extented GothOgre's information. 
	SceneManagerEx can generate a material from two instances of two classes: 
	Mtl and MatGenParams. */ 
	class GOTHOGRE_EXPORT Mtl : public ResourceEx
	{
	public:
		Mtl(ResourceManager* _creator, const String& _name, ResourceHandle _handle,
			const String& _group, bool _isManual, ManualResourceLoader* _loader);
		~Mtl();

	// Frame animation
	public:
		/** Sets up a frame animation.
		The function is used to specify the number of frames 
		per second (FPS). This makes the frame animation 
		for all specified textures, not only for diffuse texture, 
		but also for normal texture, specular texture, etc. */
		void setFPS(Real _fps);

		/** Returns FPS of the frame animation. */
		Real getFPS() const {return mFPS;}

		/** Sets the index of a displaying frame.
		This function can be used to animate textures manually
		(FPS must be set to zero in this case). */
		void setFrameIndex( size_t _frameIndex );

		/** Returns the index of a displaying frame. */
		size_t getFrameIndex() const {return mFrameIndex;}

	// UV-animation
	public:
		/** Sets up an animated scroll.
		The function is used to specify the number of loops (horizontal & vertical)
		per second.	This makes scrolling of the uv-coordinates 
		for all specified textures, not only for diffuse texture, 
		but also for normal texture, specular texture, etc. */
		void setScrollSpeed( const Vector2& _loopsPerSecond );

		/** Returns the number of loops per second of the animated scroll. */
		Vector2 getScrollSpeed() const {return mScrollSpeed;}

	// Diffuse & Ambient
	public:
		/** Sets the colour of the material.
		The alpha-component of the colour is also used to set transparency.
		The default value is black. */
		void setColour( const ColourValue& _color );

		/** Returns the colour of the material. */
		const ColourValue& getColour() const {return mColour;}

		/** Sets the name of the texture which specifies
		the colour of the material separately for each pixel. */
		void setTexture( const String& _textureName );

		/** Returns the name of the texture which specifies
		the colour of the material separately for each pixel. */
		const String& getTexture() const {return mTexture;}

		/** Sets the factor (filter) for the colour of the material. 
		The default value is (1, 1, 1, 1). */
		void setColourFactor( const ColourValue& _colourFactor );

		/** Returns the factor for the colour of the material. */
		const ColourValue& getColourFactor() const {return mColourFactor;}

	// Normal mapping (instead of bump mapping)
	public:
		/** Sets the normal map (a texture with the data for the normal mapping). */
		void setNormalTexture(const String& _normalTexture);

		/** Returns the normal map. */
		const String& getNormalTexture() const {return mNormalTexture;}

	// Specular
	public:
		/** Sets the intensity, or brightness of the specular highlight. 
		This has a range of [0-1]. */
		void setSpecularIntensity( Real _intensity );

		/** Returns the intensity of the specular highlight. */
		Real getSpecularIntensity() const {return mSpecularIntensity;}

		/** Sets the intensity of the specular highlight 
		separately for each pixel. */
		void setSpecularIntensityTexture( const String& _specularTexture );

		/** Returns the name of the texture which specifies 
		the intensity of the specular highlight separately for each pixel. */
		const String& getSpecularIntensityTexture() const {return mSpecularIntensityTexture;}

		/** Sets the factor for the intensity of the specular highlight. */
		void setSpecularIntensityFactor( Real _factor );

		/** Returns the factor for the intensity of the specular highlight. */
		Real getSpecularIntensityFactor() const {return mSpecularIntensityFactor;}

		/** Sets the colour of the specular highlight. */
		void setSpecularColour( const ColourValue& _specularColour );

		/** Returns the colour of the specular highlight. */
		const ColourValue& getSpecularColour() const {return mSpecularColour;}
		
		/** Sets the colour of the specular highlight separately for each pixel. */
		void setSpecularColourTexture( const String& _textureName );

		/** Returns the name of the texture which specifies 
		the colour of the specular highlight separately for each pixel. */
		const String& getSpecularColourTexture() const {return mSpecularColourTexture;}

		/** Sets the factor (filter) for the colour of the specular highlight. */
		void setSpecularColourFactor( const ColourValue& _factor );

		/** Returns the factor for the specular colour. */
		const ColourValue& getSpecularColourFactor() const {return mSpecularColourFactor;}

		/** Sets the shininess, affecting the size of specular highlights. */
		void setShininess( Real _shininess );

		/** Returns the shininess. */
		Real getShininess() const {return mShininess;}

		/** Sets the shininess separately for each pixel. */
		void setShininessTexture( const String& _textureName );
		
		/** Returns the name of the texture which specifies
		the shininess separately for each pixel. */
		const String& getShininessTexture() const {return mShininessTexture;}

	// Mirror
	public:
		/** Sets the amount of mirror reflection.
		This has a range of [0-1].
		Use a value of 1.0 if you need a perfect mirror; 
		or set it to 0.0 if you don't want any reflection. 
		The default value of the reflectivity is 0.0. */
		void setReflectivity( Real _reflectivity );

		/** Returns the amount of mirror reflection. */
		Real getReflectivity() const {return mReflectivity;}

		/** Sets the amount of mirror reflection separately for each pixel;
		black means no reflection, white - mirror. */
		void setReflectivityTexture( const String& _textureName );

		/** Returns the name of the texture which specifies 
		the amount of mirror reflection separately for each pixel. */
		const String& getReflectivityTexture() const {return mReflectivityTexture;}

		/** Sets the factor for the reflectivity. 
		The reflectivity is multiplied by this factor.
		The default value of the factor is 1.0 */
		void setReflectivityFactor( Real _factor );

		/** Returns the factor for the reflectivity. */
		Real getReflectivityFactor() const {return mReflectivityFactor;}

		/** Sets power of fresnel for mirror reflection. */
		void setFresnel( Real _fresnel );

		/** Returns power of fresnel for mirror reflection. */
		Real getFresnel() const {return mFresnel;}

	// Other parameters
	public:
		/** Sets the material's group. */
		void setMatGroup(MatGroup _matGroup);

		/** Returns the material's group. */
		MatGroup getMatGroup() const {return mMatGroup;}

		/** Sets the smooth angle. 
		The smooth angle may be used to calculate normals. */
		void setSmoothAngle(Degree _smoothAngle);

		/** Returns the smooth angle. */
		Degree getSmoothAngle() const {return mSmoothAngle;}

	// Utilities
	public:
		// Helper comparison operator
		struct GOTHOGRE_EXPORT LessMatGenParams 
		{ 
			bool operator()( const MatGenParams* _first, const MatGenParams* _second ) const; 
		};

		// We use ResourceHandle instead of MaterialPtr
		// because we need a weak reference to Material.
		typedef map<const MatGenParams*, ResourceHandle, LessMatGenParams>::type MaterialMap;

		/** Material iterator - can be used to iterate 
		through all generated materials from this mtl. */
		class GOTHOGRE_EXPORT MaterialIterator : public IteratorWrapper<MaterialMap, MaterialMap::const_iterator, MaterialPtr>
		{
		public:
			typedef const MatGenParams& KeyType;
			typedef MaterialPtr ValueType;

			MaterialIterator( const MaterialMap& _materialMap );
			const MatGenParams& peekNextKey() const;
			MaterialPtr peekNext() const;
			MaterialPtr getNext();
		};

		/** Gets an iterator to the list of all the materials 
		which were generated from a specified mtl. */
		MaterialIterator getGeneratedMaterialIterator( SceneManagerEx* _sceneManager );

		/** Generates a material by a specified mtl with additional parameters. */
		MaterialPtr generateMaterial( const MatGenParams& _matGenParams, SceneManagerEx* _sceneManager );

		/** Updates all materials generated from a specified mtl;
		call the function after you change an instance of the mtl class. */
		void updateMaterials();

	public:
		/** Sets any kind of user value on this object.
		@remarks
			This method allows you to associate any user value you like with 
			this MovableObject. This can be a pointer back to one of your own
			classes for instance. */
		void setUserAny( const Any& anything ) { getUserObjectBindings().setUserAny(anything); }

		/** Retrieves the custom user value associated with this object. */
		const Any& getUserAny() const { return getUserObjectBindings().getUserAny(); }

		/** Return an instance of user objects binding associated with this class.
		You can use it to associate one or more custom objects with this class instance.
		@see UserObjectBindings::setUserAny. */
		UserObjectBindings&	getUserObjectBindings() { return mUserObjectBindings; }

		/** Return an instance of user objects binding associated with this class.
		You can use it to associate one or more custom objects with this class instance.
		@see UserObjectBindings::setUserAny. */
		const UserObjectBindings& getUserObjectBindings() const { return mUserObjectBindings; }

	private:
		UserObjectBindings  mUserObjectBindings;
		Real                mFPS;
		size_t              mFrameIndex;
		Vector2             mScrollSpeed;
		ColourValue         mColour;
		String              mTexture;
		ColourValue         mColourFactor;
		String              mNormalTexture;
		Real                mSpecularIntensity;
		String              mSpecularIntensityTexture;
		Real                mSpecularIntensityFactor;
		ColourValue         mSpecularColour;
		String              mSpecularColourTexture;
		ColourValue         mSpecularColourFactor;
		Real                mShininess;
		String              mShininessTexture;
		Real                mReflectivity;
		String              mReflectivityTexture;
		Real                mReflectivityFactor;
		Real                mFresnel;
		MatGroup            mMatGroup;
		Degree              mSmoothAngle;
	};


	//-----------------------------------------------------------------------
	/** Shared pointer to Mtl. */
	typedef SubclassSharedPtr<Mtl, ResourcePtr> MtlPtr;

} // GothOgre

#endif // GOTHOGRE_MTL_H