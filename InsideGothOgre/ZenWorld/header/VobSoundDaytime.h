#ifndef VOB_SOUND_DAYTIME_H
#define VOB_SOUND_DAYTIME_H

namespace GothOgre
{
	/***/
	class VobSoundDaytime : public DerivedWorldObject
	{
	public:
		VobSoundDaytime(ZenFactory* _creator);
		~VobSoundDaytime();

		void setStartTime(const Time& _dayTime);
		const Time& getStartTime() const;

		void setEndTime(const Time& _dayTime);
		const Time& getEndTime() const;

		void setSoundName(const String& _name);
		const String& getSoundName() const;

		void setSoundName2(const String& _name);
		const String& getSoundName2() const;

		void read(ZenArchive* _zenArchive, ushort _version);
		void write(ZenArchive* _zenArchive, ushort _version);

	private:
		GOTHOGRE_DECLARE_WORLD_TIME_CAME_EX(startTimeCame, registerStartTimeListener, unregisterStartTimeListener, VobSoundDaytime);
		GOTHOGRE_DECLARE_WORLD_TIME_CAME_EX(endTimeCame,   registerEndTimeListener,   unregisterEndTimeListener,   VobSoundDaytime);

		void registerStartTimeListener();
		void registerEndTimeListener();

	private:
		Time    mStartTime;
		Time    mEndTime;
		String  mSoundName;
		String  mSoundName2;
	};

	/***/
	class VobSoundDaytimeFactory : public ZenFactory
	{
	public:
		VobSoundDaytimeFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // VOB_SOUND_DAYTIME_H
