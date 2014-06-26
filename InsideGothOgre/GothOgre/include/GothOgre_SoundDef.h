#ifndef GOTHOGRE_SOUND_DEF_H
#define GOTHOGRE_SOUND_DEF_H

#include "GothOgre_ResourceEx.h"
#include "GothOgre_SubclassSharedPtr.h"
#include "GothOgre_Time.h"


namespace GothOgre
{
	/** Sound's definition. */
	class GOTHOGRE_EXPORT SoundDef : public ResourceEx
	{
	public:
		/** Sound's properties at a time. */
		struct TimeProperty
		{
			/** World time to start playing of the sound. */
			Time    playingStartTime;

			/** Period to play the sound again after it. */
			Time    playingRepeatPeriod;

			/** Name of the file with sound data. */
			String  fileName;

			/** Resource group's name. */
			String  resourceGroup;

			/** Volume of the sound, [0..1]. */
			Real    volume;

			TimeProperty() :
				playingStartTime( Time::ZERO ),
				playingRepeatPeriod( Time::INFINITY ),
				volume( 1 ) {}
		};

		/** Returns the number of the time properties. */
		size_t getNumTimeProperties() const;

		/** Returns a time property by an index. */
		const TimeProperty& getTimeProperty(size_t _index) const;

		typedef vector<TimeProperty>::type TimeProperties;
		typedef ConstVectorIterator<TimeProperties> TimePropertyIterator;

		/** Returns an iterator to the internal list of the time properties. */
		TimePropertyIterator getTimePropertyIterator() const;

		/** Inserts a time property into the internal list. */
		void insertTimeProperty(const TimeProperty& _timeProperty);

		/** Removes a time property from the internal list. */
		void removeTimeProperty(size_t _index);

		/** Clears the internal list of the time properties. */
		void removeAllTimeProperties();

	public:
		SoundDef(ResourceManager* _creator, const String& _name, ResourceHandle _handle,
			const String& _group, bool _isManual, ManualResourceLoader* _loader);
		~SoundDef();

	private:
		TimeProperties  mTimeProperties;
	};

	//---------------------------------------------------------------------------
	/** Shared pointer to SoundDef. */
	typedef SubclassSharedPtr<SoundDef, ResourcePtr> SoundDefPtr;


} // namespace GothOgre

#endif // GOTHOGRE_SOUND_DEF_H