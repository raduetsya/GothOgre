#ifndef GOTHOGRE_APP_BLOCK_CONFIGURATION_H
#define GOTHOGRE_APP_BLOCK_CONFIGURATION_H

#include "GothOgre_AppBlock.h
#include "GothOgre_GraphicSettings.h"
#include "GothOgre_EncodingSettings.h"

namespace GothOgre
{
	class ConfigurationAppBlock : public AppBlock
	{
	public:
		ConfigurationAppBlock()
			: mEncodingSettings(nullptr), mGraphicsSettings(nullptr)
		{
			mEncodingSettings = new EncodingSettings;
			mGraphicsSettings = new GraphicsSettings;

			String encodingCfg = CommandLine::getSingleton().getAbsolutePath("encoding.cfg");
			mEncodingSettings->load(encodingCfg);
			String graphicCfg = CommandLine::getSingleton().getAbsolutePath("graphics.cfg");
			mGraphicSettings->load(graphicCfg);
		}

		~ConfigurationAppBlock()
		{
			if(mGraphicSettings)
				mGraphicSettings->save();

			if(mEncodingSettings)
				mEncodingSettings->save();

			if(mGraphicSettings)
				delete mGraphicSettings;
			
			if(mEncodingSettings)
				delete mEncodingSettings;
		}

	private:
		GraphicSettings*            mGraphicSettings;
		EncodingSettings*           mEncodingSettings;
	};
}

#endif // GOTHOGRE_APP_BLOCK_CONFIGURATION_H