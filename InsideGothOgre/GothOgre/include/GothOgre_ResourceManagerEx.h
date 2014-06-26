#ifndef GOTHOGRE_RESOURCE_MANAGER_EX_H
#define GOTHOGRE_RESOURCE_MANAGER_EX_H

#include "GothOgre_ResourceLoader.h"
#include "GothOgre_ResourceSaver.h"


namespace GothOgre
{
	class GOTHOGRE_EXPORT ResourceManagerEx : 
		public ResourceLoaderEnumerator, public ResourceSaverEnumerator
	{
	public:
		ResourceManagerEx();
		virtual ~ResourceManagerEx();

		/** Returns Ogre resource manager which is used internally 
		by this resource manager. */
		virtual Ogre::ResourceManager* getResourceManager() const = 0;

		/** Returns the default resource group for this resource manager. */
		virtual const String& getDefaultResourceGroup() const;

		/** Returns true if this resource manager is case-sensitive.
		It affects the following functions: findByName, create, createManual, remove. */
		virtual bool isCaseSensitive() const;
		
		/** Find a resource by name.
		This function return the null pointer if not found. */
		ResourcePtr findByName(const String& _name, const String& _groupName);

		/** Find a resource by name, using the default resource group.
		This function return the null pointer if not found. */
		ResourcePtr findByName(const String& _name);

		/** Creates a resource with intention to load it from file later.	
		If a resource with the specified name is loaded already	
		then this function just returns a pointer to it. */
		ResourcePtr create(const String& _name, const String& _groupName);

		/** Creates a resource with intention to load it from file later, 
		using the default resource group.	
		If a resource with the specified name is loaded already	
		then this function just returns a pointer to it. */
		ResourcePtr create(const String& _name);

		/** Creates a resource with intention to initialise it manually
		by setting its properties, not by loading a file. 
		It's an alternative way to create resource.
		If a resource with the specified name exists already
		then it will be destroyed and recreated. */		
		ResourcePtr createManual(const String& _name, const String& _groupName);

		/** Creates a resource with intention to initialise it manually
		by setting its properties, not by loading a file. 
		It's an alternative way to create resource.
		If a resource with the specified name exists already
		then it will be destroyed and recreated. */		
		ResourcePtr createManual(const String& _name);

		/** Removes a resource by name.
		If a resource with the specified name does not exist,
		then this function does nothing. */
		void remove(const String& _name);

		/** Removes a resource by Ogre's resource handle.
		If a resource with the specified name does not exist,
		then this function does nothing. */
		void remove(ResourceHandle _handle);
		
		/** Removes a resource and sets a specified shared pointer to null. */
		template<typename T>
		void remove(SharedPtr<T>& _resPtr);

	public:
		/** Whether a specified resource can be loaded. */
		bool canLoad(const String& _filename) const;
		bool canLoad(const String& _filename, const String& _resourceGroup) const;
		bool canLoad(Resource* _destResource) const;

		/** Whether a specified resource can be saved. */
		bool canSave(const String& _filename) const;
		bool canSave(const String& _filename, const String& _resourceGroup) const;

	protected:
		// Convertor to upper-case
		class UppercasedName
		{
		public:
			UppercasedName(const String& _name, bool _dontConvertToUpperCase);
			operator const String&() const {return *mResult;}

		private:
			String         mTemp;
			const String*  mResult;
		};

		// Converts a specified name to upper-case if the resource manager is case-insensitive
		UppercasedName uppercasedName(const String& _name) {return UppercasedName(_name, isCaseSensitive());}
	};
	//----------------------------------------------------------------------------
	template<typename T>
	void ResourceManagerEx::remove(SharedPtr<T>& _resPtr)
	{
		ResourceHandle handle = _resPtr->getHandle();
		_resPtr.setNull();
		remove(handle);
	}

} // namespace GothOgre

#endif // GOTHOGRE_RESOURCE_MANAGER_EX_H