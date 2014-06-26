#ifndef GOTHOGRE_TEXTURE_PTR_H
#define GOTHOGRE_TEXTURE_PTR_H


namespace GothOgre
{
	/** A shared pointer to an object of new class ExtMesh. */
	class GOTHOGRE_EXPORT TexturePtr : public Ogre::TexturePtr
	{
	public:
		TexturePtr() : Ogre::TexturePtr() {}
		explicit TexturePtr(Ogre::Texture* _rep) : Ogre::TexturePtr(_rep) {}
		TexturePtr(const TexturePtr& _r) : Ogre::TexturePtr(_r) {}
		TexturePtr(const Ogre::TexturePtr& _r) : Ogre::TexturePtr(_r) {}
		TexturePtr(const Ogre::ResourcePtr& _r) : Ogre::TexturePtr(_r) {}

		/// Operator =
		TexturePtr& operator =(const TexturePtr& _r)
		{
			Ogre::TexturePtr::operator =(_r);
			return *this;
		}

		TexturePtr& operator =(const Ogre::TexturePtr& _r)
		{
			Ogre::TexturePtr::operator =(_r);
			return *this;
		}

		TexturePtr& operator =(const Ogre::ResourcePtr& _r)
		{
			Ogre::TexturePtr::operator =(_r);
			return *this;
		}

		// Implicit conversion to bool
		// to make valid the following code:
		// if(p) {...} where p is a shared pointer
		static void unspecified_bool(TexturePtr***)
		{
		}
		typedef void (*unspecified_bool_type)(TexturePtr***);
		operator unspecified_bool_type() const
		{
			return isNull() ? 0: unspecified_bool;
		}
	};

} // namespace GothOgre

#endif // GOTHOGRE_TEXTURE_PTR_H