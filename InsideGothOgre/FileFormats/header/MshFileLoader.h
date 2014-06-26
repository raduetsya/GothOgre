#ifndef MSH_FILE_LOADER_H
#define MSH_FILE_LOADER_H


namespace GothOgre
{
	class MshFileLoader : public MeshLoader
	{
	public:
		MshFileLoader();
		ResourceLoadContext* createLoadContext();
		bool canLoad(String& _filename, const String& _resourceGroup) const;
	};

} // namespace GothOgre

#endif // MSH_FILE_LOADER_H