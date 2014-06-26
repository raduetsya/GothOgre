#ifndef GOTHOGRE_SOUND_PTR_H
#define GOTHOGRE_SOUND_PTR_H


namespace GothOgre
{
	class Sound;


	/** Specialisation of SharedPtr to allow SharedPtr to be assigned to SoundPtr 
	@note Has to be a subclass since we need operator=. */
	class GOTHOGRE_EXPORT SoundPtr : public ResourcePtr
	{
	public:
		SoundPtr() : ResourcePtr() {}
		explicit SoundPtr(Sound* rep);
		SoundPtr(const SoundPtr& r) : ResourcePtr(r) {} 
		SoundPtr(const ResourcePtr& r) : ResourcePtr(r) {}

		inline Sound& operator*() const { return (Sound&) (ResourcePtr::operator *()); }
		inline Sound* operator->() const { return (Sound*) (ResourcePtr::operator ->()); }
		inline Sound* get() const { return (Sound*) (ResourcePtr::get()); }
		inline Sound* getPointer() const { return (Sound*) (ResourcePtr::getPointer()); }

		// Implicit conversion to bool
		// to make valid the following code:
		// if(p) {...} where p is a shared pointer
		static void unspecified_bool(SoundPtr***)
		{
		}
		typedef void (*unspecified_bool_type)(SoundPtr***);
		operator unspecified_bool_type() const
		{
			return isNull() ? 0: unspecified_bool;
		}
	};

} // namespace GothOgre

#endif // GOTHOGRE_SOUND_PTR_H