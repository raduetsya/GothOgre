#include "GothOgre_Precompiled.h"
#include "GothOgre_MaterialGenerator.h"
#include "GothOgre_CodepageConverter.h"

//-------------------------------------------------------------------------
GothOgre::MaterialGeneratorRegistration* 
	Ogre::Singleton<GothOgre::MaterialGeneratorRegistration>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------



namespace GothOgre
{
	//-------------------------------------------------------------------------
	// MaterialGenerator
	//-------------------------------------------------------------------------
	MaterialGenerator::MaterialGenerator()
	{
		MaterialGeneratorRegistration::getSingleton().registerGenerator(this);
	}
	//-------------------------------------------------------------------------
	MaterialGenerator::~MaterialGenerator()
	{
		MaterialGeneratorRegistration::getSingleton().unregisterGenerator(this);
	}




	//-----------------------------------------------------------------------
	// MaterialGeneratorRegistration
	//-----------------------------------------------------------------------
	MaterialGeneratorRegistration::MaterialGeneratorRegistration()
	{
		mGenerator = nullptr;
	}
	//-----------------------------------------------------------------------
	MaterialGeneratorRegistration::~MaterialGeneratorRegistration()
	{
		if(mGenerator)
			GOTHOGRE_ERROR("Material generator is not unregistered before program exit.");
	}
	//-----------------------------------------------------------------------
	void MaterialGeneratorRegistration::registerGenerator(MaterialGenerator* _generator)
	{
		if(mGenerator && mGenerator != _generator)
			GOTHOGRE_ERROR("Material generator is already registered.");
		mGenerator = _generator;
	}
	//-----------------------------------------------------------------------
	void MaterialGeneratorRegistration::unregisterGenerator(MaterialGenerator* _generator)
	{
		if(mGenerator == _generator)
			mGenerator = nullptr;
	}
	//-----------------------------------------------------------------------
	MaterialGenerator* MaterialGeneratorRegistration::getGenerator() const
	{
		GOTHOGRE_ASSERT(mGenerator, "Material generator is not registered.");
		return mGenerator;
	}

} // namespace GothOgre