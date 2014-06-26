#ifndef SKY_FILE_LOADER_H
#define SKY_FILE_LOADER_H


namespace GothOgre
{
	class SkyFileLoader : public SkyDefLoader
	{
	public:
		SkyFileLoader();
		ResourceLoadContext* createLoadContext();
		bool canLoad(String& _filename, const String& _resourceGroup) const;
	};


} // namespace GothOgre

#endif // SKY_FILE_LOADER_H