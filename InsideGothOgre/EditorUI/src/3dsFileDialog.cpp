#include "Precompiled.h"
#include "3dsFileDialog.h"

namespace GUISystem
{
	_3dsFileDialog::_3dsFileDialog()
	{
		setResourceGroup(ResourceGroup::MESHES);
		
		Filter filter;
		filter.setDescription("#{3dsMeshes}");
		filter.addPattern("*.3ds");
		filter.setDefaultExt(".3ds");
		addFilter(filter);

		initialiseByAttributes(this);
	}

} // namespace GUISystem