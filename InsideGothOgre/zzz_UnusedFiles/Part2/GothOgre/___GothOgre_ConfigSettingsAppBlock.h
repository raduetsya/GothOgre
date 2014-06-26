#ifndef GOTHOGRE_CONFIG_SETTINGS_APP_BLOCK_H
#define GOTHOGRE_CONFIG_SETTINGS_APP_BLOCK_H

#include "GothOgre_AppBlock.h"
#include "GothOgre_GraphicSettings.h"
#include "GothOgre_EncodingSettings.h"


namespace GothOgre
{
	class ConfigSettingsAppBlock : public AppBlock
	{
	public:
		ConfigSettingsAppBlock()
			: mEncodingSettings(nullptr), mGraphicSettings(nullptr)
		{
		}

		~ConfigSettingsAppBlock()
		{
			if(mGraphicSettings)
				delete mGraphicSettings;
			
			if(mEncodingSettings)
				delete mEncodingSettings;
		}

		void initialise()
		{
			mEncodingSettings = new EncodingSettings;
			mGraphicSettings = new GraphicSettings;

			String encodingCfg = CommandLine::getSingleton().getAbsolutePath("encoding.cfg");
			mEncodingSettings->load(encodingCfg);
			String graphicCfg = CommandLine::getSingleton().getAbsolutePath("graphics.cfg");
			mGraphicSettings->load(graphicCfg);
		}

		void shutdown()
		{
			if(mGraphicSettings)
				mGraphicSettings->save();

			if(mEncodingSettings)
				mEncodingSettings->save();
		}

	private:
		GraphicSettings*            mGraphicSettings;
		EncodingSettings*           mEncodingSettings;
	};
}

#endif // GOTHOGRE_CONFIG_SETTINGS_APP_BLOCK_H