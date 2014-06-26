#include "GothOgre_Precompiled.h"
#include "GothOgre_ZenStringInterface.h"

namespace GothOgre
{
	//----------------------------------------------------------------------------
	// ZenParamDictionary
	//----------------------------------------------------------------------------
	ZenParamDictionary::ZenParamDictionary()
	{
		mUseCount = 0;
	}
	//----------------------------------------------------------------------------
	ZenParamDictionary::~ZenParamDictionary()
	{
		for(ZenParamDefMap::iterator it = mParamDefMap.begin(); it != mParamDefMap.end(); ++it)
		{
			ZenParamDef* paramDef = it->second;
			delete paramDef;
		}
	}
	//----------------------------------------------------------------------------
	ZenParamNameList ZenParamDictionary::getParamNameList() const
	{
		return mParamNameList;
	}
	//----------------------------------------------------------------------------
	void ZenParamDictionary::addParamDef(ZenParamDef* _paramDef)
	{
		ZenParamDefMap::const_iterator it = mParamDefMap.find(_paramDef->getName());
		GOTHOGRE_ASSERT(it == mParamDefMap.end(), "Parameter '" << _paramDef->getName() << "' already exists in the ParamDictionary.");

		mParamNameList.push_back(_paramDef->getName());
		mParamDefMap.insert(std::make_pair(_paramDef->getName(), _paramDef));
	}
	//----------------------------------------------------------------------------
	ZenParamDef* ZenParamDictionary::findParamDef(const String& _name) const
	{
		ZenParamDefMap::const_iterator it = mParamDefMap.find(_name);
		if (it != mParamDefMap.end())
		{
			return it->second;
		}
		else
		{
			return nullptr;
		}
	}




	//----------------------------------------------------------------------------
	OGRE_STATIC_MUTEX_INSTANCE( ZenStringInterface::msDictionaryMutex )
    ZenDictionaryMap ZenStringInterface::msDictionaryMap;
	//----------------------------------------------------------------------------
	ZenStringInterface::ZenStringInterface()
	{
		mParamDictionary = nullptr;
	}
	//----------------------------------------------------------------------------
	ZenStringInterface::~ZenStringInterface() 
	{
		//*
		destroyParamDictionary();
		//*/
	}
	//----------------------------------------------------------------------------
	ZenParamDictionary* ZenStringInterface::getParamDictionary()
	{
		return mParamDictionary;
	}
	//----------------------------------------------------------------------------
	const ZenParamDictionary* ZenStringInterface::getParamDictionary() const
	{
		return mParamDictionary;
	}
	//----------------------------------------------------------------------------
	bool ZenStringInterface::createParamDictionary(const String& _className)
	{
		OGRE_LOCK_MUTEX( msDictionaryMutex )
		ZenDictionaryMap::iterator it = msDictionaryMap.find(_className);
		if(it == msDictionaryMap.end())
		{
			it = msDictionaryMap.insert( std::make_pair( _className, new ZenParamDictionary ) ).first;
			mParamDictionary = it->second;
			mParamDictionary->mIterator = it;
		}
		else
		{
			mParamDictionary = it->second;
		}
		return (++mParamDictionary->mUseCount) == 1;
	}
	//----------------------------------------------------------------------------
	void ZenStringInterface::destroyParamDictionary()
	{
		if(mParamDictionary)
		{
			if(!--mParamDictionary->mUseCount)
			{
				msDictionaryMap.erase(mParamDictionary->mIterator);
				delete mParamDictionary;
			}
			mParamDictionary = nullptr;
		}
	}
	//----------------------------------------------------------------------------
	ZenParamNameList ZenStringInterface::getParamNameList() const
	{
		const ZenParamDictionary* dict = getParamDictionary();
		if(dict)
		{
			return dict->getParamNameList();
		}
		return ZenParamNameList();
	}
	//----------------------------------------------------------------------------
	ZenParamDef* ZenStringInterface::findParamDef(const String& _name, void** _targetPtr)
	{
		const ZenParamDictionary* dict = getParamDictionary();
		if(dict)
		{
			*_targetPtr = this;
			return dict->findParamDef(_name);
		}
		*_targetPtr = nullptr;
		return nullptr;
	}
	//----------------------------------------------------------------------------
	ZenParamDef* ZenStringInterface::findParamDef(const String& _name, const void** _targetPtr) const
	{
		return const_cast<ZenStringInterface*>(this)->findParamDef(_name, const_cast<void**>(_targetPtr));
	}
	//----------------------------------------------------------------------------
	ZenParamDef* ZenStringInterface::findParamDef(const String& _name) const
	{
		const void* targetPtr;
		return findParamDef(_name, &targetPtr);
	}
	//----------------------------------------------------------------------------
	ZenParamDef* ZenStringInterface::getParamDef(const String& _name, void** _targetPtr)
	{
		ZenParamDef* paramDef = findParamDef(_name, _targetPtr);
		if(!paramDef)
		{
			GOTHOGRE_EXCEPT("Parameter '" << _name << "' not found.");
		}
		return paramDef;
	}
	//----------------------------------------------------------------------------
	ZenParamDef* ZenStringInterface::getParamDef(const String& _name, const void** _targetPtr) const
	{
		return const_cast<ZenStringInterface*>(this)->getParamDef(_name, const_cast<void**>(_targetPtr));
	}
	//----------------------------------------------------------------------------
	ZenParamDef* ZenStringInterface::getParamDef(const String& _name) const
	{
		const void* targetPtr;
		return getParamDef(_name, &targetPtr);
	}

} // namespace GothOgre