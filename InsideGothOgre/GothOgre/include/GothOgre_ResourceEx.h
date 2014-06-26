#ifndef GOTHOGRE_RESOURCE_EX_H
#define GOTHOGRE_RESOURCE_EX_H


namespace GothOgre
{
	/** Base class for resources with a additional feature making possible 
	to change resource's name.
	(The standard Ogre's "Resource" class don't provide such feature). */
	class GOTHOGRE_EXPORT ResourceEx : public Resource
	{
	public:
		/** Change the name of the resource. */
		void setName(const String& _name);

		ResourceEx(ResourceManager* _creator, const String& _name, ResourceHandle _handle,
			const String& _group, bool _isManual, ManualResourceLoader* _loader);

		~ResourceEx();

	protected:
		void loadImpl() {}
		void unloadImpl() {}
		size_t calculateSize() const {return 1;}
	};

} // namespace GothOgre

#endif // GOTHOGRE_RESOURCE_EX_H