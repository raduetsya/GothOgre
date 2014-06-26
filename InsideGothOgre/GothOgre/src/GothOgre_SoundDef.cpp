#include "GothOgre_Precompiled.h"
#include "GothOgre_SoundDef.h"


namespace GothOgre
{
	//---------------------------------------------------------------------------	
	size_t SoundDef::getNumTimeProperties() const
	{
		return mTimeProperties.size();
	}
	//---------------------------------------------------------------------------	
	const SoundDef::TimeProperty& SoundDef::getTimeProperty(size_t _index) const
	{
		GOTHOGRE_ASSERT(_index < mTimeProperties.size(), "Index " << _index << " is out of range");
		return mTimeProperties[_index];
	}
	//---------------------------------------------------------------------------	
	SoundDef::TimePropertyIterator SoundDef::getTimePropertyIterator() const
	{
		return (TimePropertyIterator) mTimeProperties;
	}
	//---------------------------------------------------------------------------	
	void SoundDef::insertTimeProperty(const TimeProperty& _timeProperty)
	{
		mTimeProperties.push_back( _timeProperty );
		++mStateCount;
	}
	//---------------------------------------------------------------------------	
	void SoundDef::removeTimeProperty(size_t _index)
	{
		mTimeProperties.erase( mTimeProperties.begin() + _index );
		++mStateCount;
	}
	//---------------------------------------------------------------------------	
	void SoundDef::removeAllTimeProperties()
	{
		if(!mTimeProperties.empty())
		{
			mTimeProperties.clear();
			++mStateCount;
		}
	}
	//---------------------------------------------------------------------------	
	SoundDef::SoundDef(ResourceManager* _creator, const String& _name, ResourceHandle _handle,
		const String& _group, bool _isManual, ManualResourceLoader* _loader)
	: ResourceEx(_creator, _name, _handle, _group, _isManual, _loader)
	{
	}
	//---------------------------------------------------------------------------	
	SoundDef::~SoundDef()
	{
		removeAllTimeProperties();
	}

} // namespace GothOgre