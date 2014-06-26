#ifndef MATERIAL_GENERATOR_IMPL_H
#define MATERIAL_GENERATOR_IMPL_H

#include "MaterialGenerator.h"


namespace GothOgre
{
	//-------------------------------------------------------------------------
	class MaterialGeneratorImpl : public MaterialGenerator
	{
	public:
		MaterialGeneratorImpl(SceneManagerExImpl* _sm);
		~MaterialGeneratorImpl();

	protected:
		void setupMaterial( Material& _rMaterial, const MtlPtr& _mtl, 
			const MatGenParams& _matGenParams );

	private:
		enum SettingNumberOption
		{
			EXACTLY,
			OR_GREATER
		};
		static void setNumTechniques( Material& _rMaterial, unsigned short _numTechniques, SettingNumberOption _opt = EXACTLY);
		static void setNumPasses( Technique* _technique, unsigned short _numPasses, SettingNumberOption _opt = EXACTLY );
		static void setNumTextureUnitStates( Pass* _pass, unsigned short _numTextureUnitStates, SettingNumberOption _opt = EXACTLY);
		static StringVector findAnimatedTextureNames(const String& _texname);
		static String getRealTextureName(const String& _filename);
	};

} // namespace GothOgre

#endif // MATERIAL_GENERATOR_IMPL_H