#ifndef MUSIC_DAT_LOADER_H
#define MUSIC_DAT_LOADER_H

#include "ScriptDatLoader.h"


namespace GothOgre
{
	// Loader of the "music.dat" file.
	class MusicDatLoader : public ScriptDatLoader
	{
	public:
		MusicDatLoader();

	protected:
		/** Parse a script file. */
		void parseScript( DataStreamExPtr& stream, const String& _resourceGroup );

		/** Create single sound definition. */
		void insertSoundDef(const String& _soundDefName, const String& _dayTimeSound, const String& _nightTimeSound);
	};

}

#endif // MUSIC_DAT_LOADER_H