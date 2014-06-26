#ifndef MESH_AND_BSP_LOADER_H
#define MESH_AND_BSP_LOADER_H


namespace GothOgre
{
	class MeshAndBspLoader : public MeshLoader
	{
	public:
		MeshAndBspLoader();
		ResourceLoadContext* createLoadContext();
		bool canLoad(String& _filename, const String& _resourceGroup) const;
	};

} // namespace GothOgre

#endif // MESH_AND_BSP_LOADER_H