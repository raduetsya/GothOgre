#include "GothOgre_Precompiled.h"
#include "GothOgre_MeshExReadHelper.h"


namespace GothOgre
{
	//-----------------------------------------------------------------------
	// MeshReadHelper
	//-----------------------------------------------------------------------
	MeshExReadHelper::MeshExReadHelper()
		: MeshReadHelper()
	{
	}
	//-----------------------------------------------------------------------
	MeshExReadHelper::MeshExReadHelper(const MeshExPtr& _mesh)
		: MeshReadHelper(_mesh)
	{
	}
	//-----------------------------------------------------------------------
	MeshExReadHelper::~MeshExReadHelper()
	{
	}
	//-----------------------------------------------------------------------
	void MeshExReadHelper::setMesh(const MeshExPtr& _mesh)
	{
		MeshReadHelper::setMesh(_mesh);
	}
	//-----------------------------------------------------------------------
	MeshExPtr MeshExReadHelper::getMesh() const
	{
		return MeshReadHelper::getMesh();
	}
	//-----------------------------------------------------------------------
	const MtlPtr& MeshExReadHelper::getMtl(size_t _subMeshIndex) const
	{
		MeshExPtr meshEx = MeshReadHelper::getMesh();
		return meshEx->getSubMesh(_subMeshIndex)->getMtl();
	}
	//-----------------------------------------------------------------------
	const MatGenParams& MeshExReadHelper::getMatGenParams(size_t _subMeshIndex) const
	{
		MeshExPtr meshEx = MeshReadHelper::getMesh();
		return meshEx->getSubMesh(_subMeshIndex)->getMatGenParams();
	}

} // namespace GothOgre