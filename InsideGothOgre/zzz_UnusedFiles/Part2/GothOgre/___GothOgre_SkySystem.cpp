#include "GothOgre_Precompiled.h"
#include "GothOgre_SkySystem.h"
#include "GothOgre_DayTimeSystem.h"
#include "GothOgre_World.h"
#include "GothOgre_Material.h"

//-------------------------------------------------------------------------
GothOgre::SkySystem* Ogre::Singleton<GothOgre::SkySystem>::ms_Singleton = nullptr;
//-------------------------------------------------------------------------

namespace GothOgre
{
	//-------------------------------------------------------------------------------------------------
	Sky::Sky()
	{
	}
	//-------------------------------------------------------------------------------------------------
	Sky::~Sky()
	{
		if(Ogre::MaterialManager::getSingleton().resourceExists(mSkyName))
			Ogre::MaterialManager::getSingleton().remove(mSkyName);
	}
	//-------------------------------------------------------------------------------------------------
	void Sky::prepare(String _skyname, ColourValue _backcolour, String _tex0name)
	{
		mSkyName = _skyname;
		mBackColour = _backcolour;
		mLayer0Texture = _tex0name;
		mSkyMaterial = Ogre::MaterialManager::getSingleton().create(
			_skyname,	ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		Pass * pass = mSkyMaterial->getTechnique(0)->getPass(0);
		pass->setDepthWriteEnabled(false);
		pass->setLightingEnabled(false);

		Ogre::TextureUnitState *texState = pass->createTextureUnitState();

		texState->setTextureName(mLayer0Texture);
		texState->setScrollAnimation(0.002, 0.002);
		texState->setTextureScale(0.03, 0.03);
		texState->setColourOperationEx(Ogre::LBX_ADD_SIGNED, Ogre::LBS_MANUAL,
			Ogre::LBS_TEXTURE, mBackColour);

//		texState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
//		texState->setColourOperation(Ogre::LBO_REPLACE);
//		texState->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL,
//			Ogre::LBS_CURRENT, ColourValue(0.4,0.4,0.4));

	}
	//-------------------------------------------------------------------------------------------------
	ColourValue Sky::getBackColour()
	{
		return mBackColour;
	}
	//-------------------------------------------------------------------------------------------------
	String Sky::getName()
	{
		return mSkyName;
	}
	//-------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------
	SkyCycle::SkyCycle()
	{

	}
	//-------------------------------------------------------------------------------------------------
	SkyCycle::~SkyCycle()
	{
		for(map<DayTime, Sky*>::type::iterator itsky = mSkyCycle.begin();
			itsky != mSkyCycle.end(); ++itsky)
		{
			delete itsky->second;
		}
		mSkyCycle.clear();
	}
	//-------------------------------------------------------------------------------------------------
	void SkyCycle::addSky(Sky* _sky, DayTime _time)
	{
		mSkyCycle.insert(std::make_pair(_time, _sky));
	}
	//-------------------------------------------------------------------------------------------------
	Sky* SkyCycle::changeSky(DayTime _time)
	{
		for(map<DayTime, Sky*>::type::iterator itsky = mSkyCycle.begin();
			itsky != mSkyCycle.end(); ++itsky)
		{
			if (_time == itsky->first)
			{
				return itsky->second;
			}
		}
		return nullptr;
	}
	//-------------------------------------------------------------------------------------------------
	void SkyCycle::getPrevNextSky(DayTime _systime, Sky **_prevsky, Sky **_nextsky)
	{
		*_prevsky = nullptr;
		*_nextsky = nullptr;
		for(map<DayTime, Sky*>::type::iterator itsky = mSkyCycle.begin();
			itsky != mSkyCycle.end(); ++itsky)
		{
			if (_systime >=itsky->first)
				*_prevsky = itsky->second;
			if (_systime < itsky->first)
				*_nextsky = itsky->second;
		}
	}
	//-------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------
	SkySystem::SkySystem()
	{
		mInitialised = false;
		SkyPlane = Ogre::Plane(0, 0, -1, 10000);
		mCurrentCycle = nullptr;
	}
	//-------------------------------------------------------------------------------------------------
	SkySystem::~SkySystem()
	{
		for(map<String, SkyCycle*>::type::iterator itcycle = mSkyList.begin();
			itcycle != mSkyList.end(); ++itcycle)
		{
			delete itcycle->second;
		}
	}
	//-------------------------------------------------------------------------------------------------
	SkyCycle* SkySystem::addCycle(String _namecycle)
	{
		SkyCycle *tempCycle = new SkyCycle;
		mSkyList.insert(std::make_pair(_namecycle, tempCycle));
		return tempCycle;
	}
	//-------------------------------------------------------------------------------------------------
	SkyCycle* SkySystem::getCycle()
	{
		return mCurrentCycle;
	}
	//-------------------------------------------------------------------------------------------------
	void SkySystem::setActiveCycle(SkyCycle* _skycycle)
	{
//		size_t sysTime = GameTime::getSingleton().getTimeHour()*3600 + 
//			GameTime::getSingleton().getTimeMin()*60 + GameTime::getSingleton().getTimeSec();
		DayTime sysTime = DayTimeSystem::getSingleton().getCurrentDayTime();
		sysTime.setDays(0);
		Sky *prevSky;
		Sky *nextSky;
		_skycycle->getPrevNextSky(sysTime, &prevSky, &nextSky);

		mCurrentCycle = _skycycle;
	}
	//-------------------------------------------------------------------------------------------------
	void SkySystem::changeSky(DayTime _time)
	{
		SkyCycle *workCycle = getCycle();
		Sky *newSky = workCycle->changeSky(_time);
		if (newSky != nullptr)
		{
			String newMaterialName = newSky->getName();
			World::getSingleton().getSceneManager()->setSkyPlane(true, SkyPlane, newMaterialName, 10000, 1);
			ColourValue fadeColour = newSky->getBackColour();
			World::getSingleton().getSceneManager()->setFog(Ogre::FOG_LINEAR, fadeColour, 0, 15000, 30000);
			World::getSingleton().getCamera()->getViewport()->setBackgroundColour(fadeColour);
		}
	}
	//-------------------------------------------------------------------------------------------------
	Sky* SkySystem::addSky(SkyCycle* _skycycle, String _skyname, DayTime _startTime, ColourValue _backcolour, String _tex0name)
	{
		Sky *tempsky = new Sky;
		_skycycle->addSky(tempsky, _startTime);
		tempsky->prepare(_skyname, _backcolour, _tex0name);
		return tempsky;
	}
	//-------------------------------------------------------------------------------------------------
	void SkySystem::outSky()
	{
	}
	//-------------------------------------------------------------------------------------------------
	void SkySystem::dayTimeChanged(const DayTime& _dayTime)
	{
		if(!mInitialised)
		{
			mInitialised = true;
			init();
		}

		DayTime dayTime = _dayTime;
		dayTime.setDays(0);
		changeSky(dayTime);
		outSky();
	}
	//-------------------------------------------------------------------------------------------------
	void SkySystem::init()
	{
		SkyCycle *defaultCycle = addCycle("DefaultCycle");
		addSky(defaultCycle, "DefaultSky", DayTime(0, 8, 00, 00), ColourValue(0.4, 0.4, 1.0), "SkyDay_NewWorld_Layer0_A0.tga");
		setActiveCycle(defaultCycle);
	}

} // namespace GothOgre