#include "GothOgre_Precompiled.h"
#include "GothOgre_MeshEx.h"
#include "GothOgre_AnyLite.h"


namespace GothOgre
{
	//---------------------------------------------------------------------------	
	MeshEx::MeshEx(ResourceManager* _creator, const String& _name, ResourceHandle _handle,
		const String& _group, bool _isManual, ManualResourceLoader* _loader)
	: Mesh(_creator, _name, _handle, _group, _isManual, _loader)
	{
	}
	//---------------------------------------------------------------------------	
	MeshEx::~MeshEx()
	{
	}
	//---------------------------------------------------------------------------	
	const SubMeshEx::Addon& SubMeshEx::getAddon() const
	{
		const UserObjectBindings& uob = this->getUserObjectBindings();
		const Addon* addon = uob.getUserAny().getPtr<Addon>();
		if(!addon)
		{
			static const Addon sEmpty;
			return sEmpty;
		}
		return *addon;
	}
	//---------------------------------------------------------------------------	
	SubMeshEx::Addon& SubMeshEx::getAddon()
	{
		UserObjectBindings& uob = this->getUserObjectBindings();
		Addon* addon = uob.getUserAny().getPtr<Addon>();
		if(!addon)
		{
			uob.setUserAny( (AnyLite) Addon() );
			addon = uob.getUserAny().getPtr<Addon>();
		}
		return *addon;
	}

} // namespace GothOgre