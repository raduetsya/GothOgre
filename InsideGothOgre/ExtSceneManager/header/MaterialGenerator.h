#ifndef MATERIAL_GENERATOR_H
#define MATERIAL_GENERATOR_H


namespace GothOgre
{
	class SceneManagerExImpl;


	//-------------------------------------------------------------------------
	class MaterialGenerator
	{
	public:
		MaterialGenerator(SceneManagerExImpl* _sm);
		virtual ~MaterialGenerator();

		void increaseNumberOfCreatedMaterials();
		void increaseNumberOfDestroyedMaterials();

		/** Generates a material by a specified mtl with additional parameters. */
		MaterialPtr generateMaterial(const MtlPtr& _mtl, const MatGenParams& _matGenParams);

		/** Generates a material by a specified mtl with additional parameters. */
		MaterialPtr generateMaterial(const MatGenInfo& _matGenInfo);

		typedef Mtl::MaterialMap       MaterialMap;
		typedef Mtl::MaterialIterator  MaterialIterator;

		/** Gets an iterator to the list of all the materials 
		which were generated from a specified mtl. */
		MaterialIterator getGeneratedMaterialIterator(const MtlPtr& _mtl);

		/** Updates all materials generated from a specified mtl;
		call the function after you change an instance of the mtl class. */
		void updateMaterials(const MtlPtr& _mtl);

		/** Updates a single material generated from some mtl;
		call the function if you wish to regenerate a material with new generation parameters. */
		void updateMaterial(const MaterialPtr& _material, const MatGenParams& _matGenParams);

		/** Returns an information used to generated a material. */
		const MatGenInfo& getMatGenInfo(const MaterialPtr& _material);

	protected:
		virtual void setupMaterial( Material& _rMaterial, 
			const MtlPtr& _mtl, const MatGenParams& _matGenParams ) = 0;

	private:
		typedef map<SceneManagerExImpl*, MaterialMap>::type  MaterialMapBySceneManager;

		class MatGenInfoHolder : public MatGenInfo
		{
		public:
			MatGenInfoHolder() {mInitialised = false;}
			~MatGenInfoHolder() {removeReferenceFromMtl();}
			void setInitialised() {mInitialised = true;}

		private:
			void removeReferenceFromMtl();

		private:
			bool  mInitialised;
		};

	private:
		MaterialMap& getMaterialMap( const MtlPtr& _mtl );
		static MatGenInfoHolder& getMatGenInfoHolder( const MaterialPtr& _material );
		static MaterialMap& getMaterialMap( const MtlPtr& _mtl, SceneManagerExImpl* _sceneManager );
		static MaterialPtr generateMaterial( const MtlPtr& _mtl, const MatGenParams& _matGenParams, SceneManagerExImpl* _sceneManager );

	private:
		SceneManagerExImpl*   mSceneManager;
		size_t                mNumCreatedMaterials;
		size_t                mNumDestroyedMaterials;
		static NameGenerator  ms_NameGenerator;
	};

} // namespace GothOgre

#endif // MATERIAL_GENERATOR_H