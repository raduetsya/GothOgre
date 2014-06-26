#ifndef GOTHOGRE_MAT_GEN_PARAMS_H
#define GOTHOGRE_MAT_GEN_PARAMS_H

#include "GothOgre_Mtl.h"


namespace GothOgre
{
	//------------------------------------------------------------------------
	/** Parameters which are used by an object of class SceneManagerEx 
	when generating Ogre::Material by Mtl. */
	class GOTHOGRE_EXPORT MatGenParams
	{
	public:
		MatGenParams();

	// Frame animation
	public:
		/** Sets the index of a displaying frame.
		This function can be used to animate textures manually
		(FPS must be set to zero in this case). */
		void setFrameIndex( size_t _frameIndex );

		/** Returns the index of a displaying frame. */
		size_t getFrameIndex() const {return mFrameIndex;}

		/** Returns true if the index of a displaying frame is defined. */
		bool isFrameIndexDefined() const {return mFrameIndexDefined;}

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

		/** Returns true if the scroll speed is defined. */
		bool isScrollSpeedDefined() const {return mFrameIndexDefined;}

	// Diffuse & Ambient
	public:
		/** Sets the factor (filter) for the colour of the material. 
		The default value is (1, 1, 1, 1). */
		void setColourFactor( const ColourValue& _colourFactor );

		/** Returns the factor for the colour of the material. */
		const ColourValue& getColourFactor() const {return mColourFactor;}

		/** Returns true if the factor for the colour is defined. */
		bool isColourFactorDefined() const {return mColourFactorDefined;}

	// Specular
	public:
		/** Sets the factor for the intensity of the specular highlight. */
		void setSpecularIntensityFactor( Real _factor );

		/** Returns the factor for the intensity of the specular highlight. */
		Real getSpecularIntensityFactor() const {return mSpecularIntensityFactor;}

		/** Returns true if the factor for the intensity of the specular highlight is defined. */
		bool isSpecularIntensityFactorDefined() const {return mSpecularIntensityFactorDefined;}

		/** Sets the factor (filter) for the colour of the specular highlight. */
		void setSpecularColourFactor( const ColourValue& _factor );

		/** Returns the factor for the specular colour. */
		const ColourValue& getSpecularColourFactor() const {return mSpecularColourFactor;}

		/** Returns true if the factor for the specular colour is defined. */
		bool isSpecularColourFactorDefined() const {return mSpecularColourFactorDefined;}

	// Mirror
	public:
		/** Sets the factor for the reflectivity. 
		The reflectivity is multiplied by this factor.
		The default value of the factor is 1.0 */
		void setReflectivityFactor( Real _factor );

		/** Returns the factor for the reflectivity. */
		Real getReflectivityFactor() const {return mReflectivityFactor;}

		/** Returns true if the factor for the reflectivity is defined. */
		bool isReflectivityFactorDefined() const {return mReflectivityFactorDefined;}

	public:
		void setSectorName(const String& _sectorName);
		const String& getSectorName() const {return mSectorName;}
		bool isSectorNameDefined() const {return mSectorNameDefined;}

		void setMorph(bool _morph);
		bool isMorph() const {return mMorph;}

		void setSkin(bool _skin);
		bool isSkin() const {return mSkin;}

		void setSelected(bool _selected);
		bool isSelected() const {return mIsSelected;}

		void setParentSelected(bool _parentSelected);
		bool isParentSelected() const {return mIsParentSelected;}

	// Operations
	public:
		// This compare operator is needed to allow using the class as a map's key
		bool operator <( const MatGenParams& _rhs ) const;
		
	private:
		// Compare two values (as strcmp)
		static int compare( const String& _first, const String& _second );
		static int compare( const ColourValue& _first, const ColourValue& _second );
		static int compare( const Vector2& _first, const Vector2& _second );
		static int compare( Real _first, Real _second );
		static int compare( size_t _first, size_t _second );

	private:
		union
		{
			struct
			{
				bool       mFrameIndexDefined:1;
				bool       mScrollSpeedDefined:1;
				bool       mColourFactorDefined:1;
				bool       mSpecularIntensityFactorDefined:1;
				bool       mSpecularColourFactorDefined:1;
				bool       mReflectivityFactorDefined:1;
				bool       mSectorNameDefined:1;
				bool       mMorph:1;
				bool       mSkin:1;
				bool       mIsSelected:1;
				bool       mIsParentSelected:1;
			};
			uint32         mFlags;
		};
		size_t             mFrameIndex;
		Vector2            mScrollSpeed;
		ColourValue        mColourFactor;
		Real               mSpecularIntensityFactor;
		ColourValue        mSpecularColourFactor;
		Real               mReflectivityFactor;
		String             mSectorName;
	};


	struct MatGenInfo
	{
		MtlPtr           mtl;
		MatGenParams     matGenParams;
		SceneManagerEx*  sceneManager;
	};


} // namespace GothOgre

#endif // GOTHOGRE_MAT_GEN_PARAMS_H