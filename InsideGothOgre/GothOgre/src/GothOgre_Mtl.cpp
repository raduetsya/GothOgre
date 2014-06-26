#include "GothOgre_Precompiled.h"
#include "GothOgre_Mtl.h"
#include "GothOgre_MtlManagerEx.h"
#include "GothOgre_SceneManagerEx.h"
#include "GothOgre_SceneManagerExEnumerator.h"


namespace GothOgre
{
	//------------------------------------------------------------------------
	// MatGroup
	//------------------------------------------------------------------------
	GOTHOGRE_IMPLEMENT_ENUM_IO(MatGroup)
	GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT(UNDEF)
	GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT(STONE)
	GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT(WOOD)
	GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT(EARTH)
	GOTHOGRE_IMPLEMENT_ENUM_IO_CONSTANT(WATER)
	GOTHOGRE_IMPLEMENT_ENUM_IO_END


	//------------------------------------------------------------------------
	// Mtl
	//------------------------------------------------------------------------
	Mtl::Mtl(ResourceManager* _creator, const String& _name, ResourceHandle _handle,
			const String& _group, bool _isManual, ManualResourceLoader* _loader)
	: ResourceEx(_creator, _name, _handle, _group, _isManual, _loader)
	{
		mFPS = 0;
		mFrameIndex = 0;
		mScrollSpeed = Vector2::ZERO;
		mColour = ColourValue::Black;
		mColourFactor = ColourValue(1, 1, 1, 1);
		mSpecularIntensity = 0;
		mSpecularIntensityFactor = 1;
		mSpecularColour = ColourValue::White;
		mSpecularColourFactor = ColourValue(1, 1, 1, 1);
		mShininess = 5;
		mReflectivity = 0;
		mReflectivityFactor = 1;
		mFresnel = 0;

		mMatGroup = MatGroup::UNDEF;
		mSmoothAngle = 60;
	}
	//------------------------------------------------------------------------
	Mtl::~Mtl()
	{
	}
	//------------------------------------------------------------------------
	void Mtl::setFPS( Real _fps )
	{
		mFPS = _fps;
		++mStateCount;
	}
	//------------------------------------------------------------------------
	void Mtl::setFrameIndex( size_t _frameIndex )
	{
		mFrameIndex = _frameIndex;
		++mStateCount;
	}
	//------------------------------------------------------------------------
	void Mtl::setScrollSpeed( const Vector2& _loopsPerSecond )
	{
		mScrollSpeed = _loopsPerSecond;
		++mStateCount;
	}
	//------------------------------------------------------------------------
	void Mtl::setColour( const ColourValue& _colour )
	{
		mColour = _colour;
		++mStateCount;
	}
	//------------------------------------------------------------------------
	void Mtl::setTexture( const String& _textureName )
	{
		mTexture = _textureName;
		++mStateCount;
	}
	//------------------------------------------------------------------------
	void Mtl::setColourFactor( const ColourValue& _factor )
	{
		mColourFactor = _factor;
		++mStateCount;
	}
	//------------------------------------------------------------------------
	void Mtl::setNormalTexture( const String& _textureName )
	{
		mNormalTexture = _textureName;
		++mStateCount;
	}
	//------------------------------------------------------------------------
	void Mtl::setSpecularIntensity( Real _intensity )
	{
		mSpecularIntensity = _intensity;
		++mStateCount;
	}
	//------------------------------------------------------------------------
	void Mtl::setSpecularIntensityTexture( const String& _textureName )
	{
		mSpecularIntensityTexture = _textureName;
		++mStateCount;
	}
	//------------------------------------------------------------------------
	void Mtl::setSpecularIntensityFactor( Real _factor )
	{
		mSpecularIntensityFactor = _factor;
		++mStateCount;
	}
	//------------------------------------------------------------------------
	void Mtl::setSpecularColour( const ColourValue& _colour )
	{
		mSpecularColour = _colour;
		++mStateCount;
	}
	//------------------------------------------------------------------------
	void Mtl::setSpecularColourTexture( const String& _textureName )
	{
		mSpecularColourTexture = _textureName;
		++mStateCount;
	}
	//------------------------------------------------------------------------
	void Mtl::setSpecularColourFactor( const ColourValue& _factor )
	{
		mSpecularColourFactor = _factor;
		++mStateCount;
	}
	//------------------------------------------------------------------------
	void Mtl::setShininess( Real _shininess )
	{
		mShininess = _shininess;
		++mStateCount;
	}
	//------------------------------------------------------------------------
	void Mtl::setShininessTexture( const String& _textureName )
	{
		mShininessTexture = _textureName;
		++mStateCount;
	}
	//------------------------------------------------------------------------
	void Mtl::setReflectivity( Real _reflectivity )
	{
		mReflectivity = _reflectivity;
		++mStateCount;
	}
	//------------------------------------------------------------------------
	void Mtl::setReflectivityTexture( const String& _textureName )
	{
		mReflectivityTexture = _textureName;
		++mStateCount;
	}
	//------------------------------------------------------------------------
	void Mtl::setReflectivityFactor( Real _factor )
	{
		mReflectivityFactor = _factor;
		++mStateCount;
	}
	//------------------------------------------------------------------------
	void Mtl::setFresnel( Real _fresnel )
	{
		mFresnel = _fresnel;
		++mStateCount;
	}
	//------------------------------------------------------------------------
	void Mtl::setMatGroup(MatGroup _matGroup)
	{
		mMatGroup = _matGroup;
		++mStateCount;
	}
	//------------------------------------------------------------------------
	void Mtl::setSmoothAngle(Degree _smoothAngle)
	{
		mSmoothAngle = _smoothAngle;
		++mStateCount;
	}
	//------------------------------------------------------------------------
	MaterialPtr Mtl::generateMaterial(const MatGenParams& _matGenParams, SceneManagerEx* _sceneManager)
	{
		MtlPtr mtl = MtlManager::getSingleton().getByHandle(getHandle());
		return _sceneManager->generateMaterial(mtl, _matGenParams);
	}
	//------------------------------------------------------------------------
	void Mtl::updateMaterials()
	{
		MtlPtr mtl = MtlManager::getSingleton().getByHandle(getHandle());
		SceneManagerExEnumerator::SceneManagerIterator it = SceneManagerExEnumerator::getSingleton().getSceneManagerIterator();
		while(it.hasMoreElements())
		{
			SceneManagerEx* sceneManager = it.getNext();
			sceneManager->updateMaterials(mtl);
		}
	}
	//------------------------------------------------------------------------
	Mtl::MaterialIterator Mtl::getGeneratedMaterialIterator(SceneManagerEx* _sceneManager)
	{
		MtlPtr mtl = MtlManager::getSingleton().getByHandle(getHandle());
		return _sceneManager->getGeneratedMaterialIterator(mtl);
	}
	//------------------------------------------------------------------------
	bool Mtl::LessMatGenParams::operator()( const MatGenParams* _first, const MatGenParams* _second ) const
	{
		return *_first < *_second;
	}
	//------------------------------------------------------------------------
	Mtl::MaterialIterator::MaterialIterator( const MaterialMap& _materialMap )
		: IteratorWrapper<MaterialMap, MaterialMap::const_iterator, MaterialPtr>
			( _materialMap.begin(), _materialMap.end() ) 
	{
	}
	//------------------------------------------------------------------------
	const MatGenParams& Mtl::MaterialIterator::peekNextKey() const 
	{ 
		return *(mCurrent->first); 
	}
	//------------------------------------------------------------------------
	MaterialPtr Mtl::MaterialIterator::peekNext() const 
	{ 
		ResourceHandle handle = mCurrent->second;
		return MaterialManager::getSingleton().getByHandle( handle );
	}
	//------------------------------------------------------------------------
	MaterialPtr Mtl::MaterialIterator::getNext() 
	{ 
		ResourceHandle handle = (mCurrent++)->second;
		return MaterialManager::getSingleton().getByHandle( handle );
	}

} // namespace GothOgre