#include "Precompiled.h"
#include "MusicDatLoader.h"


namespace GothOgre
{
	//---------------------------------------------------------------------------
	void MusicDatLoader::insertSoundDef( const String& _soundDefName, const String& _daySoundFileName, const String& _nightSoundFileName)
	{
		SoundDefPtr soundDef = SoundDefManagerEx::getSingleton().createManual(_soundDefName);
		
		SoundDef::TimeProperty dayTP;
		dayTP.playingStartTime = Time(0, 6, 30, 0); // 6:30
		dayTP.playingRepeatPeriod = Time::ONE_DAY;
		dayTP.fileName = _daySoundFileName;
		dayTP.resourceGroup = ResourceGroup::MUSIC;
		dayTP.volume = 1;

		SoundDef::TimeProperty nightTP;
		nightTP.playingStartTime = Time(0, 18, 30, 0); // 18:30
		nightTP.playingRepeatPeriod = Time::ONE_DAY;
		nightTP.fileName = _nightSoundFileName;
		nightTP.resourceGroup = ResourceGroup::MUSIC;
		nightTP.volume = 1;

		soundDef->insertTimeProperty(dayTP);
		soundDef->insertTimeProperty(nightTP);
	}
	//---------------------------------------------------------------------------
	void MusicDatLoader::parseScript( DataStreamExPtr& stream, const String& _resourceGroup )
	{
		// Fake loading - need to be rewritten
		insertSoundDef("MUSIC_XAR", "xt_daystd.sgt", "xt_ngtstd.sgt");
		insertSoundDef("MUSIC_KHO", "kh_daystd.sgt", "kh_ngtstd.sgt");
		insertSoundDef("MUSIC_DEF", "nw_daystd.sgt", "nw_ngtstd.sgt");
		insertSoundDef("MUSIC_MOO", "mo_daystd.sgt", "mo_ngtstd.sgt");
		insertSoundDef("MUSIC_MOI", "mi_daystd.sgt", "mi_ngtstd.sgt");
	}
	//---------------------------------------------------------------------------
	MusicDatLoader::MusicDatLoader()
		: ScriptDatLoader("music.dat")
	{
	}

} // namespace GothOgre