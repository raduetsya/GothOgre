#include "Precompiled.h"
#include "ZoneMusicDefault.h"

namespace GothOgre
{
	//-------------------------------------------------------------------
	// Parameters
	//-------------------------------------------------------------------
	class ZoneMusicDefault_WorldBounds : public ZenParamAxisAlignedBox // overrides ZoneMusic_WorldBounds to make it internal
	{
	public:
		const String& getName()
		{
			static const String s = "WorldBounds";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "ZoneMusic";
			return s;
		}

		AxisAlignedBox getValue(const void* _target)
		{
			const ZoneMusicDefault* f = static_cast<const ZoneMusicDefault*>(_target);
			return f->getBase()->getParameter<AxisAlignedBox>("WorldBounds");
		}

		void setValue(void* _target, const AxisAlignedBox& _value)
		{
			ZoneMusicDefault* f = static_cast<ZoneMusicDefault*>(_target);
			f->getBase()->setParameter<AxisAlignedBox>("WorldBounds", _value);
		}

		bool isInternal()
		{
			return true;
		}
	};




	//-------------------------------------------------------------------
	// ZoneMusicDefault
	//-------------------------------------------------------------------
	ZoneMusicDefault::ZoneMusicDefault(ZenFactory* _creator)
		: DerivedWorldObject("ZoneMusic")
	{
		if (createParamDictionary("ZoneMusicDefault"))
		{
			ZenParamDictionary* dict = getParamDictionary();
			dict->addParamDef( new ZoneMusicDefault_WorldBounds );
		}
	}
	//-------------------------------------------------------------------
	ZoneMusicDefault::~ZoneMusicDefault()
	{
	}
	//-------------------------------------------------------------------
	void ZoneMusicDefault::read(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::read(_zenArchive, _version);
		setParameter<AxisAlignedBox>("WorldBounds", AxisAlignedBox::BOX_INFINITE);
	}
	//-------------------------------------------------------------------
	void ZoneMusicDefault::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}




	//-------------------------------------------------------------------
	// ZoneMusicDefaultFactory
	//-------------------------------------------------------------------
	ZoneMusicDefaultFactory::ZoneMusicDefaultFactory()
		: ZenFactory("ZoneMusicDefault")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& ZoneMusicDefaultFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("oCZoneMusicDefault:oCZoneMusic:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& ZoneMusicDefaultFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& ZoneMusicDefaultFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0x0000);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort ZoneMusicDefaultFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr ZoneMusicDefaultFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new ZoneMusicDefault(this);
	}

} // namespace GothOgre