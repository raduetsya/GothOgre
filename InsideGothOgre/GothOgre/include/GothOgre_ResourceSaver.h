#ifndef GOTHOGRE_RESOURCE_SAVER_H
#define GOTHOGRE_RESOURCE_SAVER_H

#include "GothOgre_Priority.h"
#include "GothOgre_DataStreamEx.h"


namespace GothOgre
{
	class ResourceSaverEnumerator;

	//-----------------------------------------------------------------------
	/** Abstract class to store temporary data of a saving resource. 
	This class must not be used directly, it must be used only via
	appropriate resource manager. */
	class ResourceSaveContext
	{
	public:
		ResourceSaveContext() {}
		virtual ~ResourceSaveContext() {}

		virtual void perceive(const Resource* _srcResource) = 0;
		virtual void save(const DataStreamExPtr& _destDataStream) = 0;
	};


	//-----------------------------------------------------------------------
	/** Abstract class for a resource's saver.
	Derive your own class from this class to support new file format.
	This class must not be used directly, it must be used only via
	appropriate resource manager. */
	class GOTHOGRE_EXPORT ResourceSaver
	{
	public:
		/** Constructor */
		ResourceSaver(ResourceSaverEnumerator* _enumerator, const String& _name, Priority _priority = Priority::LOWEST);

		/** Destructor */
		virtual ~ResourceSaver();
		
		/** Returns the name of the saver. */
		const String& getName() const {return mName;}

		/** Returns true if this saver can save a resource to a specified file.
		This function can change a filename if it wants.
		@remark
		The default implementation returns true if the extension 
		of the specified file matchs to an extension
		returned by the getFileExt function. */
		virtual bool canSave(String& _filename, const String& _resourceGroup) const;

		/** Creates a saving context. */
		virtual ResourceSaveContext* createSaveContext() = 0;

	protected:
		/** Returns file extension required by this loader (with leading dot). 
		Only the default implementation of the "canLoad" function calls
		this function. */
		virtual const String& getFileExt() const;

	private:
		String                    mName;
		ResourceSaverEnumerator*  mSaverEnumerator;
	};


	//-----------------------------------------------------------------------
	/** An internal class which is used to help to declare class ResourceManagerEx. 
	Contains a list of all resource savers which can be used
	to save a resource of the same specific type. */
	class GOTHOGRE_EXPORT ResourceSaverEnumerator
	{
	public:
		/** Constructor */
		ResourceSaverEnumerator();

		/** Destructor */
		~ResourceSaverEnumerator();

		/** Returns list of all registered resource savers. */
		const StringVector& getAllResourceSavers() const;

		/** Whether a specified resource can be saved. */
		bool canSave(const String& _filename, const String& _resourceGroup) const;

		/** Saves a specified resource to a specified filename. */
		void save(const Resource* _srcResource, const String& _filename, const String& _resourceGroup);

		/** Saves a specified resource to a specified filename. */
		void save(const Resource* _srcResource, const String& _filename, const String& _resourceGroup, const String& _resourceSaverName);

		/** Saves a specified resource to a data stream which is already opened for writing. */
		void save(const Resource* _srcResource, const DataStreamPtr& _dataStream, const String& _resourceSaverName);

	private:
		friend class ResourceSaver;

		/** Called from constructor of the ResourceSaver class. */
		void addResourceSaver(ResourceSaver* _resourceSaver, Priority _priority);

		/** Called from destructor of the ResourceSaver class. */
		void removeResourceSaver(ResourceSaver* _resourceSaver);

	private:
		/** Main procedure. */
		void saveResource(const Resource* _resource, const DataStreamPtr& _dataStream, ResourceSaver* _saver);

	private:
		typedef map<String, ResourceSaver*>::type   SaversByName;
		typedef multimap<Priority, ResourceSaver*>::type SaversByPriority;
		typedef map<ResourceSaver*, ResourceSaveContext*>::type   FreeSaveContextsBySaver;

	private:
		/** Iterate savers and find a saver which can save the specified resource. 
		This function returns nullptr if the "canSave" function called for each of all savers
		returns false. */
		ResourceSaver* findSaverCanSave(String& _filename, const String& _resourceGroup) const;
		
		/** Searchs a saver by name, throws an exception if not found. */
		ResourceSaver* getSaverByName(const String& _loaderName) const;

		/** Creates saving context for specified saver. */
		ResourceSaveContext* createSaveContext(ResourceSaver* _loader);

		/** Marks saving context as free.
		The class don't delete saving contexts after completing saving
		and can use them again. */
		void freeSaveContext(ResourceSaveContext* _context, ResourceSaver* _saver);

	private:
		OGRE_AUTO_MUTEX
		StringVector             mAllSaverNames;
		SaversByName             mSaversByName;
		SaversByPriority		 mSaversByPriority;
		FreeSaveContextsBySaver  mFreeSaveContextsBySaver;
	};

} // namespace GothOgre

#endif // GOTHOGRE_RESOURCE_SAVER_H