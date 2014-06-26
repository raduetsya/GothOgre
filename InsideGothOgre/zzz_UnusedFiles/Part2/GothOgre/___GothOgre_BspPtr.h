#ifndef GOTHOGRE_BSP_PTR_H
#define GOTHOGRE_BSP_PTR_H


namespace GothOgre
{
	class Bsp;


	/** Specialisation of SharedPtr to allow SharedPtr to be assigned to BspPtr 
	@note Has to be a subclass since we need operator=. */
	class GOTHOGRE_EXPORT BspPtr : public ResourcePtr
	{
	public:
		BspPtr() : ResourcePtr() {}
		explicit BspPtr(Bsp* rep);
		BspPtr(const BspPtr& r) : ResourcePtr(r) {} 
		BspPtr(const ResourcePtr& r) : ResourcePtr(r) {}

		inline Bsp& operator*() const { return (Bsp&) (ResourcePtr::operator *()); }
		inline Bsp* operator->() const { return (Bsp*) (ResourcePtr::operator ->()); }
		inline Bsp* get() const { return (Bsp*) (ResourcePtr::get()); }
		inline Bsp* getPointer() const { return (Bsp*) (ResourcePtr::getPointer()); }

		// Implicit conversion to bool
		// to make valid the following code:
		// if(p) {...} where p is a shared pointer
		static void unspecified_bool(BspPtr***)
		{
		}
		typedef void (*unspecified_bool_type)(BspPtr***);
		operator unspecified_bool_type() const
		{
			return isNull() ? 0: unspecified_bool;
		}
	};

} // namespace GothOgre

#endif // GOTHOGRE_BSP_PTR_H