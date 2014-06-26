#include "Precompiled.h"
#include "VobLevelCompo.h"

namespace GothOgre
{
	//--------------------------------------------------------------------------
	class VobLevelCompo_ShowVisual : public ZenParamBool // overrides Vob_ShowVisual to make it internal
	{
	public:
		const String& getName()
		{
			static const String s = "ShowVisual";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "VobLevelCompo";
			return s;
		}

		bool getValue(const void* _target)
		{
			const VobLevelCompo* f = static_cast<const VobLevelCompo*>(_target);
			return f->getBase()->getParameter<bool>("ShowVisual");
		}

		void setValue(void* _target, bool _value)
		{
			VobLevelCompo* f = static_cast<VobLevelCompo*>(_target);
			f->getBase()->setParameter<bool>("ShowVisual", _value);
		}

		bool isInternal()
		{
			return true;
		}
	};
	//--------------------------------------------------------------------------
	class VobLevelCompo_ZenBsp : public ZenParamString
	{
	public:
		const String& getName()
		{
			static const String s = "ZenBsp";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "VobLevelCompo";
			return s;
		}

		String getValue(const void* _target)
		{
			const VobLevelCompo* f = static_cast<const VobLevelCompo*>(_target);
			return f->getZenBspName();
		}

		bool isReadOnly()
		{
			return true;
		}
	};
	//-------------------------------------------------------------------
	VobLevelCompo::VobLevelCompo(ZenFactory* _creator)
		: DerivedWorldObject("Vob")
	{
		if (createParamDictionary("VobLevelCompo"))
		{
			ZenParamDictionary* dict = getParamDictionary();
			dict->addParamDef( new VobLevelCompo_ShowVisual );
			dict->addParamDef( new VobLevelCompo_ZenBsp);
		}
	}
	//-------------------------------------------------------------------
	VobLevelCompo::~VobLevelCompo()
	{
	}
	//-------------------------------------------------------------------
	void VobLevelCompo::setZenBspName(const String& _zenBspName)
	{
		if(_zenBspName.empty())
			mZenBsp.setNull();
		else
			mZenBsp = getWorld()->findWorldObject(_zenBspName, "ZenBsp");

		setParameter("ShowVisual", mZenBsp.isNull());
	}
	//-------------------------------------------------------------------
	const String& VobLevelCompo::getZenBspName() const
	{
		return mZenBsp.isNull() ? StringUtil::BLANK : mZenBsp->getName();
	}
	//-------------------------------------------------------------------
	bool VobLevelCompo::isCompiled() const
	{
		return !mZenBsp.isNull();
	}
	//-------------------------------------------------------------------
	void VobLevelCompo::read(ZenArchive* _zenArchive, ushort _version)
	{
		setParameter("ShowVisual", false);
		setParameter("ShowVisualFrozen", true);

		DerivedWorldObject::read(_zenArchive, _version);

		setParameter("ShowVisualFrozen", false);

		String worldName = getWorld()->getShortName();
		setZenBspName(worldName);
	}
	//-------------------------------------------------------------------
	void VobLevelCompo::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	VobLevelCompoFactory::VobLevelCompoFactory()
		: ZenFactory("VobLevelCompo")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& VobLevelCompoFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCVobLevelCompo:zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& VobLevelCompoFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& VobLevelCompoFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0xCC00);
			sVersions.push_back(0x3001);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort VobLevelCompoFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr VobLevelCompoFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new VobLevelCompo(this);
	}

} // namespace GothOgre