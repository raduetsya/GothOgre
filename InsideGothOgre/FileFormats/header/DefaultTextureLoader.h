#ifndef DEFAULT_TEXTURE_LOADER_H
#define DEFAULT_TEXTURE_LOADER_H


namespace GothOgre
{
	//----------------------------------------------------------------------
	class DefaultTextureLoadContext : public ResourceLoadContext
	{
	public:
		DefaultTextureLoadContext(const String& _fileext);
		~DefaultTextureLoadContext();

		void load(const DataStreamExPtr& _srcDataStream);
		void apply(Resource* _destResource);

	private:
		String mImageType;
		Image  mImage;
	};


	//----------------------------------------------------------------------
	class DefaultTextureLoader : public TextureLoader
	{
	public:
		DefaultTextureLoader(const String& _loaderName, const String& _fileext, Priority _priority = Priority::LOWEST);
		const String& getFileExt() const;

	private:
		String mFileExt;
	};


	//----------------------------------------------------------------------
#define DECLARE_TEXTURE_LOADER(_classname, _fileext, _priority) \
	class _classname##LoadContext : public DefaultTextureLoadContext \
	{ \
	public: \
		_classname##LoadContext() : DefaultTextureLoadContext(_fileext) {} \
	}; \
	\
	class _classname : public DefaultTextureLoader \
	{ \
	public: \
		_classname() : DefaultTextureLoader(#_classname, _fileext, _priority) {} \
		ResourceLoadContext* createLoadContext() {return new _classname##LoadContext;} \
	};

}

#endif // DEFAULT_TEXTURE_LOADER_H