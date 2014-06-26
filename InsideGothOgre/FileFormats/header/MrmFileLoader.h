#ifndef MRM_FILE_LOADER_H
#define MRM_FILE_LOADER_H

namespace GothOgre
{
	class MrmFileLoader : public MeshLoader
	{
	public:
		MrmFileLoader();
		ResourceLoadContext* createLoadContext();
		bool canLoad(String& _filename, const String& _resourceGroup) const;
	};

} // namespace GothOgre

#endif // MRM_FILE_LOADER_H