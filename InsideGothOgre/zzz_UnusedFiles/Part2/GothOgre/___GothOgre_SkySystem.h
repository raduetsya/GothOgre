#ifndef GOTHOGRE_SKYSYSTEM_H
#define GOTHOGRE_SKYSYSTEM_H

#include "GothOgre_DayTimeListener.h"
#include "GothOgre_MaterialPtr.h"

namespace GothOgre
{
//-------------------------------------------------------------------//
//               Class for single sky state                          //
//-------------------------------------------------------------------//
	class Sky
	{
	public:
		Sky();
		~Sky();
		void		prepare(String _skyname, ColourValue _backcolour, String _tex0name);
		String		getName();
		ColourValue	getBackColour();

	private:
		// Время, с которого включается небо.
//		size_t		mStartHour;
//		size_t		mStartMin;
		// фонофый цвет неба и тумана
		ColourValue	mBackColour;
		// еще какой-то цвет )
//		ColourValue		mPolyColour;
		// цвет осадков
//		ColourValue		mRainColour;
		// текстуры слоев (если есть)
		String		mLayer0Texture;
		String		mLayer1Texture;
		// прозрачность слоев
		size_t		mLayer0Alpha;
		size_t		mLayer1Alpha;
//		Vector2		mScrollSpeed1;
//		Vector2		mScrollSpeed2;
		// Название данного типа неба
		String		mSkyName;
		// обобщающий Ogre-Материал
		MaterialPtr	mSkyMaterial;

//		FogSettings		mFogSettings;
//		list<CelestialBody*>::type	mCelestialBodies;
	};

//---------------------------------------------------------------------//
//   Class for daily sky cycles - it has number of single sky states   //
//---------------------------------------------------------------------//
	class SkyCycle
	{
	public:
		SkyCycle();
		~SkyCycle();
		void		addSky(Sky* _sky, DayTime _time);
		Sky*		changeSky(DayTime _time);
		void		getPrevNextSky(DayTime _systime, Sky **_prevSky, Sky **_nextSky);
	private:
		map<DayTime, Sky*>::type mSkyCycle;
	};

//---------------------------------------------------------------------//
//                   Main Sky system singleton Class                   //
//---------------------------------------------------------------------//
	class GOTHOGRE_EXPORT SkySystem : public Singleton<SkySystem>, 
		public AutoDayTimeListener
	{
	public:
		SkySystem();
		~SkySystem();
		void init();
		SkyCycle*	addCycle(String _namecyle);
		SkyCycle*	getCycle();
		void		setActiveCycle(SkyCycle* _skycycle);
		Sky*		addSky(SkyCycle* _skycycle, String _skyname, DayTime _startTime, ColourValue _backcolour, String _tex0name);
		void		changeSky(DayTime _time);
		void		outSky();

//		void removeSky(Sky* _sky);
//		Sky* findSky(const String& _skyName) const;

	private:
		map<String, SkyCycle*>::type mSkyList;
		Ogre::Plane		SkyPlane;
		SkyCycle*		mCurrentCycle;
		Sky*			mPrevSky;
		Sky*			mNextSky;
		bool            mInitialised;

	protected:
		void dayTimeChanged(const DayTime& _dayTime);
	};

} // namespace GothOgre

#endif // GOTHOGRE_SKYSYSTEM_H