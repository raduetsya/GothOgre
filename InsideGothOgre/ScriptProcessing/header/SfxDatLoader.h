#ifndef SFX_DAT_LOADER_H
#define SFX_DAT_LOADER_H

#include "ScriptDatLoader.h"


namespace GothOgre
{
	// Loader of the "sfx.dat" file.
	class SfxDatLoader : public ScriptDatLoader
	{
	public:
		SfxDatLoader();

	protected:
		/** Parse a script file. */
		void parseScript( DataStreamExPtr& stream, const String& _resourceGroup );

		/** Create single sound definition. */
		void insertSoundDef(const String& _soundDefName, const String& _soundFileName, Real _soundVolume);
	};

}

#endif // SFX_DAT_LOADER_H