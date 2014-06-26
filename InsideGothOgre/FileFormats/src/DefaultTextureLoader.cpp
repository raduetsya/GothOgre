#include "Precompiled.h"
#include "DefaultTextureLoader.h"

namespace GothOgre
{
	//----------------------------------------------------------------------
	// DefaultTextureLoadContext
	//----------------------------------------------------------------------
	DefaultTextureLoadContext::DefaultTextureLoadContext(const String& _fileext) 
		: mImageType(_fileext.substr(1))
	{
	}
	//----------------------------------------------------------------------
	DefaultTextureLoadContext::~DefaultTextureLoadContext() 
	{
	}
	//----------------------------------------------------------------------
	void DefaultTextureLoadContext::load(const DataStreamExPtr& _srcDataStream)
	{
		DataStreamExPtr dataStream2 = _srcDataStream;
		mImage.load( dataStream2, mImageType );
	}
	//----------------------------------------------------------------------
	void DefaultTextureLoadContext::apply(Resource* _destResource)
	{
		Texture* texture = static_cast<Texture*>(_destResource);
		ConstImagePtrList imagePtrs;
		imagePtrs.push_back(&mImage);
		texture->_loadImages( imagePtrs );
	}


	//----------------------------------------------------------------------
	// DefaultTextureLoader
	//----------------------------------------------------------------------
	DefaultTextureLoader::DefaultTextureLoader(const String& _loaderName, const String& _fileext, Priority _priority)
		: TextureLoader(_loaderName, _priority), mFileExt(_fileext)
	{
	}
	//---------------------------------------------------------------------------------
	const String& DefaultTextureLoader::getFileExt() const
	{
		return mFileExt;
	}



} // namespace GothOgre