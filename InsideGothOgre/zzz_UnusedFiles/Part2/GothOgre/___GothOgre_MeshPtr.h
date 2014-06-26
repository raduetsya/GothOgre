#ifndef GOTHOGRE_MESH_PTR_H
#define GOTHOGRE_MESH_PTR_H


namespace GothOgre
{
	class Mesh;

	/** A shared pointer to an object of new class ExtMesh. */
	class GOTHOGRE_EXPORT MeshPtr : public Ogre::MeshPtr
	{
	public:
		MeshPtr() : Ogre::MeshPtr() {}
		explicit MeshPtr(Ogre::Mesh* _rep) : Ogre::MeshPtr(_rep) {}
		MeshPtr(const MeshPtr& _r) : Ogre::MeshPtr(_r) {}
		MeshPtr(const Ogre::MeshPtr& _r) : Ogre::MeshPtr(_r) {}
		MeshPtr(const Ogre::ResourcePtr& _r) : Ogre::MeshPtr(_r) {}

		/// Operator =
		MeshPtr& operator =(const MeshPtr& _r)
		{
			Ogre::MeshPtr::operator =(_r);
			return *this;
		}

		MeshPtr& operator =(const Ogre::MeshPtr& _r)
		{
			Ogre::MeshPtr::operator =(_r);
			return *this;
		}

		MeshPtr& operator =(const Ogre::ResourcePtr& _r)
		{
			Ogre::MeshPtr::operator =(_r);
			return *this;
		}

		Mesh& operator*() const 
		{ 
			return (Mesh&) (Ogre::MeshPtr::operator*()); 
		}

		Mesh* operator->() const 
		{
			return (Mesh*) (Ogre::MeshPtr::operator->());
		}

		Mesh* get() const 
		{ 
			return (Mesh*) (Ogre::MeshPtr::get());
		}

		Mesh* getPointer() const
		{
			return (Mesh*) (Ogre::MeshPtr::getPointer());
		}

		// Implicit conversion to bool
		// to make valid the following code:
		// if(p) {...} where p is a shared pointer
		static void unspecified_bool(MeshPtr***)
		{
		}
		typedef void (*unspecified_bool_type)(MeshPtr***);
		operator unspecified_bool_type() const
		{
			return isNull() ? 0: unspecified_bool;
		}
	};

} // namespace GothOgre

#endif // GOTHOGRE_MESH_PTR_H