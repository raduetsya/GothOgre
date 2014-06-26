#include "Precompiled.h"
#include "ZenMtl.h"

namespace GothOgre
{
	//--------------------------------------------------------------------------
	class ZenMtl_Name : public ZenParamString
	{
	public:
		const String& getName()
		{
			static const String s = "Name";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "Material";
			return s;
		}

		String getValue(const void* _target)
		{
			const ZenMtl* f = static_cast<const ZenMtl*>(_target);
			return f->getName();
		}

		void setValue(void* _target, const String& _value)
		{
			ZenMtl* f = static_cast<ZenMtl*>(_target);
			f->setName(_value);
		}
	};
	//--------------------------------------------------------------------------
	class ZenMtl_SectorName : public ZenParamString
	{
	public:
		const String& getName()
		{
			static const String s = "SectorName";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "Material";
			return s;
		}

		String getValue(const void* _target)
		{
			const ZenMtl* f = static_cast<const ZenMtl*>(_target);
			return f->getSectorName();
		}

		void setValue(void* _target, const String& _value)
		{
			ZenMtl* f = static_cast<ZenMtl*>(_target);
			f->setSectorName(_value);
		}

		bool isInternal()
		{
			return true;
		}
	};




	//-------------------------------------------------------------------
	// ZenMtl
	//-------------------------------------------------------------------
	ZenMtl::ZenMtl()
	{
		if (createParamDictionary("ZenMtl"))
		{
			ZenParamDictionary* dict = getParamDictionary();
			dict->addParamDef( new ZenMtl_Name );
			dict->addParamDef( new ZenMtl_SectorName );
		}
	}
	//-------------------------------------------------------------------
	ZenMtl::~ZenMtl()
	{
	}
	//-------------------------------------------------------------------
	void ZenMtl::setName(const String& _name)
	{
		mMtl = MtlManagerEx::getSingleton().findByName(_name);
	}
	//-------------------------------------------------------------------
	const String& ZenMtl::getName() const
	{
		return mMtl.isNull() ? mMtl->getName() : StringUtil::BLANK;
	}
	//-------------------------------------------------------------------
	void ZenMtl::setSectorName(const String& _name)
	{
		mSectorName = _name;
	}
	//-------------------------------------------------------------------
	const String& ZenMtl::getSectorName() const
	{
		return mSectorName;
	}
	//-------------------------------------------------------------------
	enum
	{
		GOTHIC1_VERSION  = 0x4400,
		GOTHIC2_VERSION  = 0x9C03,
		GOTHOGRE_VERSION = 0xC350
	};
	//-------------------------------------------------------------------
	void ZenMtl::read(ZenArchive* _zenArchive, ushort _version)
	{
		// Gothic I
		String fullName = _zenArchive->readString("name");
		String mtlName = fullName;
		if(MtlNameUtil::isSectorFloorName(mtlName))
		{
			std::pair<String, String> strPair = MtlNameUtil::splitSectorFloorName(mtlName);
			mSectorName = strPair.first;
			mtlName = strPair.second;
		}
		else
		{
			mSectorName.clear();
		}

		mMtl = MtlManagerEx::getSingleton().findByName(mtlName);
		if(!mMtl.isNull())
			return;

		mMtl = MtlManagerEx::getSingleton().createManual(mtlName);

		int matGroup = _zenArchive->readEnum("matGroup");
		mMtl->setMatGroup( matGroup );
		
		ColourValue colour = _zenArchive->readColourValue("color", ColourValue::Black);
		mMtl->setColour( colour);
		
		Degree smoothAngle = (Degree) _zenArchive->readReal("smoothAngle", 60);
		mMtl->setSmoothAngle( smoothAngle );

		String texture = _zenArchive->readString("texture", "");
		mMtl->setTexture( texture );

		String texScale = _zenArchive->readString("texScale");
		
		Real texAniFPS = _zenArchive->readReal("texAniFPS", 0);
		mMtl->setFPS( texAniFPS );

		int texAniMapMode = _zenArchive->readEnum("texAniMapMode");
		
		Vector2 texAniMapDir = StrConv::fromString<Vector2>( _zenArchive->readString("texAniMapDir") );
		mMtl->setScrollSpeed( texAniMapDir );

		bool noCollDet = _zenArchive->readBool("noCollDet");
		bool noLightMap = _zenArchive->readBool("noLightMap");
		bool lodDontCollapse = _zenArchive->readBool("lodDontCollapse");
		String detailObject = _zenArchive->readString("detailObject");

		if( _version == GOTHIC2_VERSION || _version == GOTHOGRE_VERSION )
		{
			// Gothic II
			Real detailObjectScale = _zenArchive->readReal("detailObjectScale");
			bool forceOccluder = _zenArchive->readBool("forceOccluder");
			bool environmentalMapping = _zenArchive->readBool("environmentalMapping");
			Real environmentalMappingStrength = _zenArchive->readReal("environmentalMappingStrength");
			int waveMode = _zenArchive->readEnum("waveMode");
			int waveSpeed = _zenArchive->readEnum("waveSpeed");
			Real waveMaxAmplitude = _zenArchive->readReal("waveMaxAmplitude");
			Real waveGridSize = _zenArchive->readReal("waveGridSize");
			bool ignoreSunLight = _zenArchive->readBool("ignoreSunLight");
			int alphaFunc = _zenArchive->readEnum("alphaFunc");
		}

		if( _version == GOTHOGRE_VERSION )
		{
			// GothOgre
			size_t frame = _zenArchive->readInt("frame", 0);
			mMtl->setFrameIndex( frame );

			ColourValue colorFactor = _zenArchive->readColourValue( "colorFactor", ColourValue(1, 1, 1, 1) );
			mMtl->setColourFactor( colorFactor );

			Real specularIntensity = _zenArchive->readReal( "specularIntensity", 0.0f );
			mMtl->setSpecularIntensity( specularIntensity );

			String specularIntensityTexture = _zenArchive->readString( "specularIntensityTexture" );
			mMtl->setSpecularIntensityTexture( specularIntensityTexture );

			Real specularIntensityFactor = _zenArchive->readReal( "specularIntensityFactor", 1.0f );
			mMtl->setSpecularIntensityFactor( specularIntensityFactor );

			ColourValue specularColor = _zenArchive->readColourValue( "specularColor", ColourValue(1, 1, 1, 1) );
			mMtl->setSpecularColour( specularColor );

			String specularColorTexture = _zenArchive->readString( "specularColorTexture" );
			mMtl->setSpecularColourTexture( specularColorTexture );

			ColourValue specularColorFactor = _zenArchive->readColourValue( "specularColorFactor", ColourValue(1, 1, 1, 1) );
			mMtl->setSpecularColourFactor( specularColorFactor );

			Real shininess = _zenArchive->readReal( "shininess", 5.0f );
			mMtl->setShininess( shininess );

			String shininessTexture = _zenArchive->readString( "shininessTexture" );
			mMtl->setShininessTexture( shininessTexture );

			Real reflectivity = _zenArchive->readReal( "reflectivity", 0.0f );
			mMtl->setReflectivity( reflectivity );

			String reflectivityTexture = _zenArchive->readString( "reflectivityTexture" );
			mMtl->setReflectivityTexture( reflectivityTexture );

			Real reflectivityFactor = _zenArchive->readReal( "reflectivityFactor", 1.0f );
			mMtl->setReflectivityFactor( reflectivityFactor );

			Real fresnel = _zenArchive->readReal( "fresnel", 0.0f );
			mMtl->setFresnel( fresnel );
		}
	}
	//-------------------------------------------------------------------
	void ZenMtl::write(ZenArchive* _zenArchive, ushort _version)
	{
	}
	//-------------------------------------------------------------------
	ZenMtlFactory::ZenMtlFactory()
		: ZenFactory("ZenMtl")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& ZenMtlFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCMaterial");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& ZenMtlFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& ZenMtlFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back( GOTHIC1_VERSION  );
			sVersions.push_back( GOTHIC2_VERSION  );
			sVersions.push_back( GOTHOGRE_VERSION );
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort ZenMtlFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr ZenMtlFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new ZenMtl;
	}

} // namespace GothOgre