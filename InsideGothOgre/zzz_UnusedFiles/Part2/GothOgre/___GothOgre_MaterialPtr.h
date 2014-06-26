#ifndef GOTHOGRE_MATERIAL_PTR_H
#define GOTHOGRE_MATERIAL_PTR_H


namespace GothOgre
{
	class Material;

	/** A shared pointer to an object of new class ExtMaterial. */
	class GOTHOGRE_EXPORT MaterialPtr : public Ogre::MaterialPtr
	{
	public:
		MaterialPtr() : Ogre::MaterialPtr() {}
		explicit MaterialPtr(Ogre::Material* _rep) : Ogre::MaterialPtr(_rep) {}
		MaterialPtr(const MaterialPtr& _r) : Ogre::MaterialPtr(_r) {}
		MaterialPtr(const Ogre::MaterialPtr& _r) : Ogre::MaterialPtr(_r) {}
		MaterialPtr(const Ogre::ResourcePtr& _r) : Ogre::MaterialPtr(_r) {}

		/// Operator =
		MaterialPtr& operator =(const MaterialPtr& _r)
		{
			Ogre::MaterialPtr::operator =(_r);
			return *this;
		}

		MaterialPtr& operator =(const Ogre::MaterialPtr& _r)
		{
			Ogre::MaterialPtr::operator =(_r);
			return *this;
		}

		MaterialPtr& operator =(const Ogre::ResourcePtr& _r)
		{
			Ogre::MaterialPtr::operator =(_r);
			return *this;
		}

		Material& operator*() const 
		{ 
			return (Material&) (Ogre::MaterialPtr::operator*()); 
		}

		Material* operator->() const 
		{
			return (Material*) (Ogre::MaterialPtr::operator->());
		}

		Material* get() const 
		{ 
			return (Material*) (Ogre::MaterialPtr::get());
		}

		Material* getPointer() const
		{
			return (Material*) (Ogre::MaterialPtr::getPointer());
		}

		// Implicit conversion to bool
		// to make valid the following code:
		// if(p) {...} where p is a shared pointer
		static void unspecified_bool(MaterialPtr***)
		{
		}
		typedef void (*unspecified_bool_type)(MaterialPtr***);
		operator unspecified_bool_type() const
		{
			return isNull() ? 0: unspecified_bool;
		}
	};

} // namespace GothOgre

#endif // GOTHOGRE_MATERIAL_PTR_H