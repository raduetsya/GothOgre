#include "GothOgre_Precompiled.h"
#include "GothOgre_MaterialUtil.h"
#include "GothOgre_MaterialGenerator.h"


namespace GothOgre
{
	//------------------------------------------------------------------------
	static const String USER_MTL = "Mtl";
	static const String USER_MAT_GEN_PARAMS = "MatGenParams";
	static NameGenerator s_MaterialNameGenerator("Auto");
	//--------------------------------------------------------------------------
	// Without the following declaration we cannot pack MtlPtr to Ogre::Any.
	template<class _Elem, class _Traits> 
	inline std::basic_ostream<_Elem, _Traits>& operator<<(
		std::basic_ostream<_Elem, _Traits>& _ostrm, const MtlPtr& _val)
	{
		return _ostrm;
	}

	template<class _Elem, class _Traits> 
	inline std::basic_ostream<_Elem, _Traits>& operator<<(
		std::basic_ostream<_Elem, _Traits>& _ostrm, const MatGenParams& _val)
	{
		return _ostrm;
	}
	//------------------------------------------------------------------------
	MaterialPtr MaterialUtil::generateMaterial(const MtlPtr& _mtl, const MatGenParams& _matGenParams)
	{
		MaterialPtr material;
		Mtl::MaterialHandles::iterator it = _mtl->mMaterialHandles.find(_matGenParams);
		if(it != _mtl->mMaterialHandles.end())
		{
			// Material was generated already, just return it
			// (Materials can be shared.)
			ResourceHandle handle = it->second;
			material = MaterialManager::getSingleton().getByHandle(handle);
		}
		else
		{
			// Generate a new material
			String materialName = s_MaterialNameGenerator.generate();
			material = MaterialManager::getSingleton().create(materialName, StringUtil::BLANK);
			
			// We need a material generator.
			MaterialGenerator* generator = MaterialGeneratorRegistration::getSingleton().getGenerator();

			// Initalise the material with the specified params.
			generator->_updateMaterial(material, _mtl, _matGenParams);

			// Store the smart pointer to the mtl and the material generation params
			// in the material's "user any" member.
			if(!material->getNumTechniques())
			{
				MaterialManager::getSingleton().remove( (ResourcePtr) material);
				return MaterialPtr();
			}
			Technique* technique = material->getTechnique(0);
			UserObjectBindings& uob = technique->getUserObjectBindings();
			uob.setUserAny( USER_MTL, (Any) _mtl );
			uob.setUserAny( USER_MAT_GEN_PARAMS, (Any) _matGenParams );

			// Store the handle to the new material in the mtl.
			ResourceHandle handle = material->getHandle();
			_mtl->mMaterialHandles.insert(std::make_pair(_matGenParams, handle));
		}
		return material;
	}
	//------------------------------------------------------------------------
	void MaterialUtil::updateAllMaterials(const MtlPtr& _mtl)
	{
		// We need a material generator.
		MaterialGenerator* generator = MaterialGeneratorRegistration::getSingleton().getGenerator();

		// Update all materials generated based on the specified mtl.
		for(Mtl::MaterialHandles::iterator it = _mtl->mMaterialHandles.begin();
			it != _mtl->mMaterialHandles.end(); ++it)
		{
			const MatGenParams& matGenParams = it->first;
			ResourceHandle handle = it->second;
			MaterialPtr material = MaterialManager::getSingleton().getByHandle(handle);
			generator->_updateMaterial(material, _mtl, matGenParams);
		}
	}
	//------------------------------------------------------------------------
	MtlPtr MaterialUtil::getMtl(const MaterialPtr& _material)
	{
		if(!_material->getNumTechniques())
			return MtlPtr();
		
		Technique* technique = _material->getTechnique(0);
		UserObjectBindings& uob = technique->getUserObjectBindings();
		Any mtlAny = uob.getUserAny( USER_MTL );
		
		if(mtlAny.isEmpty())
			return MtlPtr();

		MtlPtr mtl = *any_cast<MtlPtr>(&mtlAny);
		return mtl;		
	}
	//------------------------------------------------------------------------
	MtlPtr MaterialUtil::getMtl(const MaterialPtr& _material, MatGenParams& _rMatGenParams)
	{
		if(!_material->getNumTechniques())
			return MtlPtr();
		
		Technique* technique = _material->getTechnique(0);
		UserObjectBindings& uob = technique->getUserObjectBindings();
		Any mtlAny = uob.getUserAny( USER_MTL );
		Any matGenParamsAny = uob.getUserAny( USER_MAT_GEN_PARAMS );
		
		if(mtlAny.isEmpty() || matGenParamsAny.isEmpty())
			return MtlPtr();

		MtlPtr mtl = *any_cast<MtlPtr>(&mtlAny);
		_rMatGenParams = *any_cast<MatGenParams>(&matGenParamsAny);
		return mtl;	
	}
	//------------------------------------------------------------------------
	MtlPtr MaterialUtil::getMtl(const MaterialPtr& _material, MatGenParams* _rMatGenParams)
	{
		return getMtl(_material, *_rMatGenParams);
	}
	//------------------------------------------------------------------------
	MatGenParams MaterialUtil::getMatGenParams(const MaterialPtr& _material)
	{
		if(!_material->getNumTechniques())
			return MatGenParams();
		
		Technique* technique = _material->getTechnique(0);
		UserObjectBindings& uob = technique->getUserObjectBindings();
		Any matGenParamsAny = uob.getUserAny( USER_MAT_GEN_PARAMS );
		
		if(matGenParamsAny.isEmpty())
			return MatGenParams();

		return *any_cast<MatGenParams>(&matGenParamsAny);
	}

} // namespace GothOgre