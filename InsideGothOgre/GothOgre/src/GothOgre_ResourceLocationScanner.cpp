#include "GothOgre_Precompiled.h"
#include "GothOgre_CommandLine.h"
#include "GothOgre_Progress.h"
#include "GothOgre_ResourceLocationScanner.h"

//-------------------------------------------------------------------------
GothOgre::ResourceLocationScanner* 
	Ogre::Singleton<GothOgre::ResourceLocationScanner>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------

namespace GothOgre
{
	ResourceLocationScanner::ResourceLocationScanner(const String& _filename)
		: ConfigSettings(_filename, ConfigFileExFlags::MULTI_SETTINGS)
	{
	}
	//---------------------------------------------------------------------
	void ResourceLocationScanner::scan(const String& _resourceGroup, bool _showProgress)
	{
		vector<String>::type vec;
		vec.push_back(_resourceGroup);
		scan(vec, _showProgress);
	}
	//---------------------------------------------------------------------
	void ResourceLocationScanner::clear(const String& _resourceGroup)
	{
		vector<String>::type vec;
		vec.push_back(_resourceGroup);
		clear(vec);
	}
	//---------------------------------------------------------------------
	void ResourceLocationScanner::rescan(const String& _resourceGroup, bool _showProgress)
	{
		vector<String>::type vec;
		vec.push_back(_resourceGroup);
		rescan(vec, _showProgress);
	}
	//---------------------------------------------------------------------
	void ResourceLocationScanner::scan(const vector<String>::type& _resourceGroups, bool _showProgress)
	{
		// Check if the scanning is needed
		bool scanNeeded = false;
		ResourceGroupManager& rgm = ResourceGroupManager::getSingleton();
		for(size_t i = 0; i != _resourceGroups.size(); ++i)
		{
			const String& resGroup = _resourceGroups[i];
			if(!rgm.resourceGroupExists(resGroup))
			{
				scanNeeded = true;
				break;
			}
		}
		if(!scanNeeded)
			return;

		// Initialise and show progress bar
		Progress& progress = Progress::getSingleton();
		if(_showProgress)
		{
			progress.setRange(0U, _resourceGroups.size());
			progress.setPosition(0);
			progress.setCaption("#{Scanning}");
			progress.begin();
		}

		// Go through all the specified resource groups
		for(size_t i = 0; i != _resourceGroups.size(); ++i)
		{
			const String& resGroup = _resourceGroups[i];
			if(!rgm.resourceGroupExists(resGroup))
			{
				GOTHOGRE_INFO(resGroup << " - Scanning resource group");

				if(_showProgress)
				{
					progress.setMessage(resGroup);
					progress.setPosition(i, true);
				}

				// Go through all sections & settings in the file
				ConfigFileEx::SectionIterator seci = getSectionIterator();
				while (seci.hasMoreElements())
				{
					String secName = seci.peekNextKey();
					ConfigFile::SettingsMultiMap* settings = seci.getNext();
					if(secName == resGroup)
					{
						ConfigFileEx::SettingsMultiMap::iterator it;
						for (it = settings->begin(); it != settings->end(); ++it)
						{
							String archType = it->first;
							String archName = it->second;
							String absPath = CommandLine::getSingleton().getAbsolutePath(archName);
							rgm.addResourceLocation(absPath, archType, resGroup, true);
						}
					}
				}
				rgm.initialiseResourceGroup(resGroup);
			}
		}

		// Hide the progress bar
		if(_showProgress)
			progress.end();
	}
	//---------------------------------------------------------------------
	void ResourceLocationScanner::clear(const vector<String>::type& _resourceGroups)
	{
		ResourceGroupManager& rgm = ResourceGroupManager::getSingleton();
		for(size_t i = 0; i != _resourceGroups.size(); ++i)
		{
			if(rgm.resourceGroupExists(_resourceGroups[i]))
				rgm.destroyResourceGroup(_resourceGroups[i]);
		}
	}
	//---------------------------------------------------------------------
	void ResourceLocationScanner::rescan(const vector<String>::type& _resourceGroups, bool _showProgress)
	{
		clear(_resourceGroups);
		scan(_resourceGroups, _showProgress);
	}
	//---------------------------------------------------------------------
	void ResourceLocationScanner::scanAll(bool _showProgress)
	{
		vector<String>::type vec = getAllResourceGroups();
		scan(vec, _showProgress);
	}
	//---------------------------------------------------------------------
	vector<String>::type ResourceLocationScanner::getAllResourceGroups() const
	{
		vector<String>::type vec;
		ConfigFileEx::SectionIterator seci = getSectionIterator();
		while (seci.hasMoreElements())
		{
			String secName = seci.peekNextKey();
			seci.getNext();
			if(!secName.empty())
				vec.push_back(secName);
		}
		return vec;
	}


} // namespace GothOgre