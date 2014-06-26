#ifndef GOTHOGRE_ZEN_FACTORY_H
#define GOTHOGRE_ZEN_FACTORY_H

#include "GothOgre_ZenObject.h"


namespace GothOgre
{
	class ZenArchive;
	class ZenObjectPtr;

	//------------------------------------------------------------------------
	/** Factory to create and destroy instances of ZenObject
	If you derive your class from ZenObject you should also provide
	factory to create and destroy instance of your class
	*/
	class GOTHOGRE_EXPORT ZenFactory
	{
	public:
		ZenFactory(const String& _typeName);
		virtual ~ZenFactory();

		/** Returns name of the type of an instance created by the factory */
		const String& getTypeName() const {return mTypeName;}

		/** Returns a list of supported type names.
		This list is used as alternative names for looking a factory for the type name. */
		virtual const vector<String>::type& getSupportedTypeNames() const;

		/** Returns a name of the type which is compatible with a specified zen archive.
		This function is called when an instance is saved to a specified zen archive. */
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;

		/** Returns a list of supported versions.
		This list is used to check if an instance can be loaded or not.
		This function is called when the object is loading from a zen archive. */
		virtual const vector<ushort>::type& getSupportedVersions() const;

		/** Returns a version which is compatible with a specified zen archive.
		This function is called when an instance is saved to a zen archive.	*/
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;

		/** Creates an instance. */
		virtual ZenObjectPtr createZenObject();
		
		/** Returns number of created instances by the factory */
		size_t getNumCreatedObjects() const {return mNumCreatedObjects;}

		/** Returns number of living instances by the factory.
		The number is the number of created instances decreased 
		by the number of destroyed instances. */
		size_t getNumLivingObjects() const {return mNumLivingObjects;}

	protected:
		/** Creates an instance. */
		virtual ZenObjectPtr createZenObjectImpl() = 0;

	private:
		friend class ZenObject;

		/** Called by ZenObject::ZenObject when a new instance has just been created */
		void objectCreated(ZenObject* _zenObject);

		/** Called by ZenObject::~ZenObject when an instance is going to be destroyed */
		void objectDestroyed(ZenObject* _zenObject);

	private:
		String     mTypeName;
		size_t     mNumCreatedObjects;
		size_t     mNumLivingObjects;
	};

} // GothOgre

#endif // GOTHOGRE_ZEN_FACTORY_H