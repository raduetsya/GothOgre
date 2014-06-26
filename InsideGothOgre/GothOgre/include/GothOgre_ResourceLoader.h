#ifndef GOTHOGRE_RESOURCE_LOADER_H
#define GOTHOGRE_RESOURCE_LOADER_H

#include "GothOgre_Priority.h"
#include "GothOgre_DataStreamEx.h"


namespace GothOgre
{
	class ResourceLoaderEnumerator;


	//-----------------------------------------------------------------------
	/** Abstract class to store temporary data of a loading resource. 
	This class must not be used directly, it must be used only via
	appropriate resource manager. */
	class ResourceLoadContext
	{
	public:
		ResourceLoadContext() {}
		virtual ~ResourceLoadContext() {}

		/** Load information about a resource from a specified data stream. */
		virtual void load(const DataStreamExPtr& _srcDataStream) = 0;

		/** Skips resource data in the specified data stream, 
		without loading any data. */
		virtual void skip(const DataStreamExPtr& _stream) {}

		/** Initialise a specified resource with the loaded information.
		This function is called after the "load" function. */
		virtual void apply(Resource* _destResource) = 0;
	};


	//-----------------------------------------------------------------------
	/** Abstract class for a resource's loader.
	Derive your own class from this class to support new file format.
	This class must not be used directly, it must be used only via
	appropriate resource manager. */
	class GOTHOGRE_EXPORT ResourceLoader
	{
	public:
		/** Constructor */
		ResourceLoader(ResourceLoaderEnumerator* _enumerator, const String& _name, Priority _priority = Priority::LOWEST);

		/** Destructor */
		virtual ~ResourceLoader();

		/** Returns maximum number of contexts;
		resource manager will try to delete unused contexts
		if number of them exceed this value. 
		The default implementation returns 10. */
		virtual size_t getMaxContexts() const;

		/** Returns name of the loader. */
		const String& getName() const {return mName;}

		/** Returns true if this loader can load a specified file.
		This function can change a filename if it wants.
		@remark
		The default implementation returns true if the specified file
		exists and the extension of the specified file matchs to an extension
		returned by the getFileExt function. */
		virtual bool canLoad(String& _filename, const String& _resourceGroup) const;

		/** Creates a loading context. */
		virtual ResourceLoadContext* createLoadContext() = 0;

	protected:
		/** Returns file extension required by this loader (with leading dot). 
		Only the default implementation of the "canLoad" function calls
		this function. */
		virtual const String& getFileExt() const;

	private:
		String                     mName;
		ResourceLoaderEnumerator*  mLoaderEnumerator;
	};


	//-----------------------------------------------------------------------
	/** An internal class which is used to help to declare class ResourceManagerEx. 
	Contains a list of all resource loaders which can be used
	to load a resource of the same specific type. */
	class GOTHOGRE_EXPORT ResourceLoaderEnumerator : public ManualResourceLoader
	{
	public:
		/** Constructor */
		ResourceLoaderEnumerator();

		/** Destructor */
		~ResourceLoaderEnumerator();

		/** Returns list of all registered resource loaders. */
		const StringVector& getAllResourceLoaders() const;

		/** Whether a specified resource can be loaded. */
		bool canLoad(const String& _filename, const String& _resourceGroup) const;
		bool canLoad(Resource* _destResource) const;

		/** Loads a specified resource getting filename and resource group from the resource. */
		void load(Resource* _destResource);

		/** Loads a specified resource getting filename and resource group from the resource. */
		void load(Resource* _destResource, const String& _resourceLoaderName);

		/** Loads a specified resource with specified filename ang resource group. */
		void load(Resource* _destResource, const String& _filename, const String& _resourceGroup);

		/** Loads a specified resource with specified filename ang resource group. */
		void load(Resource* _destResource, const String& _filename, const String& _resourceGroup, const String& _resourceLoaderName);

		/** Loads a specified resource from a data stream which is already opened. */
		void load(Resource* _destResource, const DataStreamPtr& _dataStream, const String& _resourceLoaderName);

		/** Skips a resource data in a specified data stream without initialising a resource. */
		void skip(const DataStreamPtr& _dataStream, const String& _resourceLoaderName);

	private:
		friend class ResourceLoader;

		/** Called from constructor of the ResourceLoader class. */
		void addResourceLoader(ResourceLoader* _resourceLoader, Priority _priority);

		/** Called from destructor of the ResourceLoader class. */
		void removeResourceLoader(ResourceLoader* _resourceLoader);

	private:
		/** Reimplementation of Ogre::ManualResourceLoader::loadResource. */
		void loadResource(Resource* _resource);

		/** Reimplementation of Ogre::ManualResourceLoader::prepareResource. */
		void prepareResource(Resource* _resource);

	private:
		struct Extra
		{
			String                filename;
			String                group;
			DataStreamPtr         dataStream;
			String                loaderName;
			bool                  prepareBegan;
			bool                  prepareEnded;
			ResourceLoader*       loader;
			ResourceLoadContext*  context;
		};
		typedef map<Resource*, Extra>::type          ExtraByResource;
		typedef map<String, ResourceLoader*>::type   LoadersByName;
		typedef multimap<Priority, ResourceLoader*>::type LoadersByPriority;
		typedef map<ResourceLoader*, size_t>::type   NumContextsByLoader;
		typedef multimap<ResourceLoader*, ResourceLoadContext*>::type   FreeLoadContextsByLoader;

	private:
		/** Iterate loaders and find a loader which can load the specified resource. 
		This function returns nullptr if the "canLoad" function called for each of all loaders
		returns false. */
		ResourceLoader* findLoaderCanLoad(String& _filename, const String& _resourceGroup) const;
		
		/** Searchs a loader by name, throws an exception if not found. */
		ResourceLoader* getLoaderByName(const String& _loaderName) const;
		
		/** Gets extra data for specified resource. 
		The function creates extra data if it does not exist. */
		Extra& getExtra(Resource* _resource);

		/** Removes extra data (it's called after completing loading the resource). */
		void removeExtra(Resource* _resource);

		/** Creates loading context for specified loader. */
		ResourceLoadContext* createLoadContext(ResourceLoader* _loader);

		/** Marks loading context as free.
		The class don't delete loading contexts after completing loading
		and can use them again. */
		void freeLoadContext(ResourceLoadContext* _context, ResourceLoader* _loader);

		/** Returns a reference to number of contexts which have been created
		for a specified loader. */
		size_t& getNumContextsByLoader(ResourceLoader* _loader);

	private:
		OGRE_AUTO_MUTEX
		StringVector              mAllLoaderNames;
		LoadersByName             mLoadersByName;
		LoadersByPriority		  mLoadersByPriority;
		FreeLoadContextsByLoader  mFreeLoadContextsByLoader;
		ExtraByResource           mExtraByResource;
		NumContextsByLoader       mNumContextsByLoader;
	};

} // namespace GothOgre

#endif // GOTHOGRE_RESOURCE_LOADER_H