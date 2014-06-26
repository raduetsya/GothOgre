#ifndef GOTHOGRE_MATERIAL_GENERATOR_H
#define GOTHOGRE_MATERIAL_GENERATOR_H

#include "GothOgre_Mtl.h"


namespace GothOgre
{
	//-----------------------------------------------------------------------
	// Auxiliary class.
	// This class is designed to implement material generation support.
	// Don't use it directly until you create a new implementation of material generation support.
	class GOTHOGRE_EXPORT MaterialGenerator
	{
	public:
		MaterialGenerator();
		~MaterialGenerator();

		/** Initialises a material with using of information 
		from a specified mtl and additional parameters specified. */
		virtual void _updateMaterial(const MaterialPtr& _material, 
			const MtlPtr& _mtl, const MatGenParams& _matGenParams) = 0;
	};


	//-----------------------------------------------------------------------
	// Auxiliary class.
	// This class is designed to implement material generation support.
	// Don't use it directly until you create a new implementation of material generation support.
	class GOTHOGRE_EXPORT MaterialGeneratorRegistration : public Singleton<MaterialGeneratorRegistration>
	{
	public:
		MaterialGeneratorRegistration();
		~MaterialGeneratorRegistration();

		// Returns a pointer to the registered implementation of material generator
		MaterialGenerator* getGenerator() const;

	private:
		friend class MaterialGenerator;
		void registerGenerator(MaterialGenerator* _generator);
		void unregisterGenerator(MaterialGenerator* _generator);

	private:
		MaterialGenerator* mGenerator;
	};

} // namespace GothOgre

#endif // GOTHOGRE_MATERIAL_GENERATOR_H