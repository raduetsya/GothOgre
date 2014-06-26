#include "Precompiled.h"
#include "SfxDatLoader.h"


namespace GothOgre
{
	//---------------------------------------------------------------------------
	void SfxDatLoader::insertSoundDef( const String& _soundDefName, const String& _soundFileName, Real _soundVolume)
	{
		SoundDefPtr soundDef = SoundDefManagerEx::getSingleton().createManual(_soundDefName);

		SoundDef::TimeProperty tp;
		tp.playingStartTime = Time(0);
		tp.playingRepeatPeriod = Time::ZERO;
		tp.fileName = _soundFileName;
		tp.resourceGroup = ResourceGroup::SOUND;
		tp.volume = _soundVolume;

		soundDef->insertTimeProperty(tp);
	}
	//---------------------------------------------------------------------------
	void SfxDatLoader::parseScript( DataStreamExPtr& stream, const String& _resourceGroup )
	{
		// Fake loading - need to be rewritten
		insertSoundDef("OW_WINDLIGHT", "wind_light.wav", 0.70f);
		insertSoundDef("OW_WINDHIGH1", "wind_high.wav", 0.70f);
		insertSoundDef("OW_BIRD8", "bird_08.wav", 0.80f);
		insertSoundDef("OW_CROW", "crow_01.wav", 0.80f);
		insertSoundDef("Wood_Day2", "wood_dayambience.wav", 0.60f);
		insertSoundDef("InsectsFrogs_Night3", "frogsandinsects2.wav", 0.20f);
		insertSoundDef("OW_WINDCAVE3", "wind_gentle_cave.wav", 0.70f);
		insertSoundDef("OW_WATERFALL2", "waterfalls_02.wav", 1.00f);
		insertSoundDef("OW_RAVEN", "raven_01.wav", 0.80f);
		insertSoundDef("TORCH_BURN", "torch_burning.wav", 1.00f);
		insertSoundDef("OW_WATERHITSSTONE", "lakeshore_stone.wav", 0.90f);
		insertSoundDef("FLIES", "flies1.wav", 0.50f);
		insertSoundDef("OW_BIRD11", "bird_11.wav", 0.80f);
		insertSoundDef("FIRE_MEDIUM", "fire_medium01.wav", 1.00f);
		insertSoundDef("OW_WATERFALL1", "waterfalls_01.wav", 1.00f);
		insertSoundDef("INSECTS_ONLY2", "insects.wav", 0.30f);
		insertSoundDef("OW_WOOD_CRACKLE", "woodcrackle_0.wav", 1.00f);
		insertSoundDef("OW_BIRD1", "bird_01.wav", 0.80f);
		insertSoundDef("WOOD_BIRDAMBIENCE1", "wood_birdambience.wav", 0.90f);
		insertSoundDef("OW_ShipWrekOrBridge", "woodcreak_shipwrek_0.wav", 1.00f);
	}
	//---------------------------------------------------------------------------
	SfxDatLoader::SfxDatLoader()
		: ScriptDatLoader("sfx.dat")
	{
	}

} // namespace GothOgre