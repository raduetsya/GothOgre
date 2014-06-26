#ifndef GOTHOGRE_ZEN_OBJECT_H
#define GOTHOGRE_ZEN_OBJECT_H

#include "GothOgre_ZenStringInterface.h"
#include "GothOgre_ZenFactory.h"

namespace GothOgre
{
	class ZenArchive;
	class ZenFactory;

	//------------------------------------------------------------------------
	/** Instances of the class can be written to ZenArchive or read from ZenArchive.
	This is a base class, derive your own classes from it.
	*/
	class GOTHOGRE_EXPORT ZenObject : public ZenStringInterface
	{
	protected:
		friend class ZenObjectPtr;

		/** Constructor, only to be called by the creator factory. */
		ZenObject();
		
	public:
		/** Destructor, only to be called by the shared pointer's "destroy" function. */
		virtual ZenObject::~ZenObject();

		/** Gets a pointer to a factory created this object */
		ZenFactory* getCreator() const {return mCreator;}

		/** Returns name of the type of an instance created by the factory */
		const String& getTypeName() const;

		/** Returns a list of supported type names.
		This list is used as alternative names for looking a factory for the type name. */
		const vector<String>::type& getSupportedTypeNames() const;

		/** Returns a name of the type which is compatible with a specified zen archive.
		This function is called when an instance is saved to a specified zen archive. */
		const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;

		/** Returns a list of supported versions.
		This list is used to check if an instance can be loaded or not.
		This function is called when the object is loading from a zen archive. */
		const vector<ushort>::type& getSupportedVersions() const;

		/** Returns a version which is compatible with a specified zen archive.
		This function is called when an instance is saved to a zen archive.	*/
		ushort getCompatibleVersion(ZenArchive* _zenArchive) const;

		/** Loads the object from a specified zen archive */
		virtual void read(ZenArchive* _zenArchive, ushort _version) {}
		
		/** Saves the object to a specified zen archive */
		virtual void write(ZenArchive* _zenArchive, ushort _version) {}

	private:
		friend class ZenFactory;
		void _init(ZenFactory* _creator);

	private:
		ZenFactory*	  mCreator;
		unsigned int  mUseCount;
	};



	//------------------------------------------------------------------------
	/** Shared pointer to an instance of ZenObject. */
	class GOTHOGRE_EXPORT ZenObjectPtr
	{
	public:
		/** Constructor, does not initialise the SharedPtr */
		ZenObjectPtr() 
		{
			mZenObject = nullptr;
		}
		
		/** Constructor. */
		ZenObjectPtr(ZenObject* _p)
		{
			mZenObject = nullptr;
			bind(_p);
		}

		ZenObjectPtr(const ZenObjectPtr& _ptr)
		{
			mZenObject = nullptr;
			bind(_ptr.mZenObject);
		}
			
		ZenObjectPtr& operator =(const ZenObjectPtr& _ptr)
		{
			bind(_ptr.mZenObject);
			return *this;
		}

		~ZenObjectPtr()
		{
			release();
		}

		ZenObject& operator*() const
		{
			assert(mZenObject);
			return *mZenObject;
		}

		ZenObject* operator->() const
		{
			assert(mZenObject);
			return mZenObject;
		}

		ZenObject* get() const
		{
			return mZenObject;
		}

		void bind(ZenObject* _p);

		bool unique() const
		{
			return useCount() == 1;
		}

		unsigned int useCount () const;

		ZenObject* getPointer() const
		{
			return get();
		}
		
		bool isNull(void) const
		{
			return !mZenObject;
		}

		void setNull(void)
		{
			release();
			mZenObject = nullptr;
		}

		friend bool operator ==(const ZenObjectPtr& _ptr1, const ZenObjectPtr& _ptr2)
		{
			return _ptr1.get() == _ptr2.get();
		}

		friend bool operator !=(const ZenObjectPtr& _ptr1, const ZenObjectPtr& _ptr2)
		{
			return _ptr1.get() != _ptr2.get();
		}

		friend bool operator <(const ZenObjectPtr& _ptr1, const ZenObjectPtr& _ptr2)
		{
			return _ptr1.get() < _ptr2.get();
		}

		// Implicit conversion to bool
		// to make valid the following code:
		// if(p) {...} where p is a shared pointer
		static void unspecified_bool(ZenObjectPtr***)
		{
		}

		typedef void (*unspecified_bool_type)(ZenObjectPtr***);

		operator unspecified_bool_type() const
		{
			return isNull() ? 0: unspecified_bool;
		}

	private:
		void release();
		void destroy();

	private:
		ZenObject* mZenObject;
	};

} // namespace GothOgre

#endif // GOTHOGRE_ZEN_OBJECT_H