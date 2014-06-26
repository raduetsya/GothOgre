#include "Precompiled.h"
#include "ZenFileDialog.h"

namespace GUISystem
{
	ZenFileDialog::ZenFileDialog()
	{
		setResourceGroup(ResourceGroup::WORLDS);
		
		Filter filter;
		filter.setDescription("#{ZenWorlds}");
		filter.addPattern("*.zen");
		filter.setDefaultExt(".zen");
		addFilter(filter);

		initialiseByAttributes(this);
	}

} // namespace GUISystem