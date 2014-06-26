#ifndef GOTHOGRE_RESOURCE_LOCATION_SCANNER_H
#define GOTHOGRE_RESOURCE_LOCATION_SCANNER_H

#include "GothOgre_ConfigSettings.h"

namespace GothOgre
{
	class GOTHOGRE_EXPORT ResourceLocationScanner : public Singleton<ResourceLocationScanner>, public ConfigSettings
	{
	public:
		ResourceLocationScanner(const String& _filename);

	public:
		void scan(const vector<String>::type& _resourceGroups, bool _showProgress = true);
		void rescan(const vector<String>::type& _resourceGroups, bool _showProgress = true);
		void clear(const vector<String>::type& _resourceGroups);

		void scan(const String& _resourceGroup, bool _showProgress = true);
		void rescan(const String& _resourceGroup, bool _showProgress = true);
		void clear(const String& _resourceGroup);

		void scanAll(bool _showProgress = true);

	private:
		vector<String>::type getAllResourceGroups() const;
	};

} // namespace GothOgre

#endif // GOTHOGRE_RESOURCE_LOCATION_SCANNER_H