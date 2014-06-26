#ifndef _3DS_FILE_LOADER_H
#define _3DS_FILE_LOADER_H


namespace GothOgre
{
	class _3dsFileLoader : public MeshLoader
	{
	public:
		_3dsFileLoader();
		ResourceLoadContext* createLoadContext();
		const String& getFileExt() const;
	};

} // namespace GothOgre

#endif // _3DS_FILE_LOADER_H