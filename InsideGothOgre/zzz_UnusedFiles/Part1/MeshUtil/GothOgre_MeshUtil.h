#ifndef GOTHOGRE_MESH_UTIL_H
#define GOTHOGRE_MESH_UTIL_H

namespace GothOgre
{
	struct GOTHOGRE_EXPORT MeshUtil
	{
		static void moveMesh(const MeshPtr& _mesh, const Vector3& _delta);
	};

} // namespace GothOgre

#endif // GOTHOGRE_MESH_UTIL_H