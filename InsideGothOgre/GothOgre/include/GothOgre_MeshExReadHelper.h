#ifndef GOTHOGRE_MESH_EX_READ_HELPER_H
#define GOTHOGRE_MESH_EX_READ_HELPER_H

#include "GothOgre_MeshEx.h"
#include "GothOgre_MeshReadHelper.h"


namespace GothOgre
{
	//----------------------------------------------------------------------
	/** Version of the MeshExReadHelper class for MeshEx. */
	class GOTHOGRE_EXPORT MeshExReadHelper : public MeshReadHelper
	{
	public:
		MeshExReadHelper();
		MeshExReadHelper(const MeshExPtr& _msh);
		~MeshExReadHelper();

		/** Sets the mesh to retrieve information from. */
		void setMesh(const MeshExPtr& _msh);

		/** Returns the mesh. */
		MeshExPtr getMesh() const;

		/** Returns the material of a submesh. */
		const MtlPtr& getMtl(size_t _subMeshIndex) const;
		const MatGenParams& getMatGenParams(size_t _subMeshIndex) const;
	};

} // namespace GothOgre

#endif // GOTHOGRE_MESH_EX_READ_HELPER_H