#include "Precompiled.h"
#include "Vob.h"

//#include <intrin.h>
//#pragma intrinsic(__rdtsc)

namespace GothOgre
{
	//--------------------------------------------------------------------------
	class Vob_Name : public ZenParamString
	{
	public:
		const String& getName()
		{
			static const String s = "Name";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "Name of the vob.";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "Vob";
			return s;
		}

		String getValue(const void* _target)
		{
			const Vob* f = static_cast<const Vob*>(_target);
			return f->getName();
		}

		void setValue(void* _target, const String& _value)
		{
			Vob* f = static_cast<Vob*>(_target);
			f->setName(_value);
		}
	};
	//--------------------------------------------------------------------------
	class Vob_Type : public ZenParamString
	{
	public:
		const String& getName()
		{
			static const String s = "Type";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "Type of the vob.";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "Vob";
			return s;
		}

		String getValue(const void* _target)
		{
			const Vob* f = static_cast<const Vob*>(_target);
			return f->getWorldObjectTypeName();
		}

		bool isReadOnly()
		{
			return true;
		}
	};
	//--------------------------------------------------------------------------
	class Vob_Position : public ZenParamVector3
	{
	public:
		const String& getName()
		{
			static const String s = "Position";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "Position of the vob relative to it's parent.";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "Vob";
			return s;
		}

		Vector3 getValue(const void* _target)
		{
			const Vob* f = static_cast<const Vob*>(_target);
			return f->getPosition();
		}

		void setValue(void* _target, const Vector3& _value)
		{
			Vob* f = static_cast<Vob*>(_target);
			f->setPosition(_value);
		}
	};
	//--------------------------------------------------------------------------
	class Vob_Orientation : public ZenParamQuaternion
	{
	public:
		const String& getName()
		{
			static const String s = "Orientation";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "Orientation of the vob relative to it's parent.";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "Vob";
			return s;
		}

		Quaternion getValue(const void* _target)
		{
			const Vob* f = static_cast<const Vob*>(_target);
			return f->getOrientation();
		}

		void setValue(void* _target, const Quaternion& _value)
		{
			Vob* f = static_cast<Vob*>(_target);
			f->setOrientation(_value);
		}
	};
	//--------------------------------------------------------------------------
	class Vob_Scale : public ZenParamVector3
	{
	public:
		const String& getName()
		{
			static const String s = "Scale";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "Scale of the vob relative to it's parent.";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "Vob";
			return s;
		}

		Vector3 getValue(const void* _target)
		{
			const Vob* f = static_cast<const Vob*>(_target);
			return f->getScale();
		}

		void setValue(void* _target, const Vector3& _value)
		{
			Vob* f = static_cast<Vob*>(_target);
			f->setScale(_value);
		}
	};
	//--------------------------------------------------------------------------
	class Vob_Parent : public ZenParamString
	{
	public:
		const String& getName()
		{
			static const String s = "Parent";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "The parent of the vob.";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "Vob";
			return s;
		}

		String getValue(const void* _target)
		{
			const Vob* f = static_cast<const Vob*>(_target);
			WorldObjectPtr parent = f->getParent();
			if(parent.isNull())
				return "";
			else
				return parent->getCombinedName();
		}

		bool isReadOnly()
		{
			return true;
		}
	};
	//--------------------------------------------------------------------------
	class Vob_Visual : public ZenParamString
	{
	public:
		const String& getName()
		{
			static const String s = "Visual";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "";
			return s;
		}

		const String& getSubType()
		{
			return SubType::VISUAL_NAME;
		}

		const String& getGroup()
		{
			static const String s = "Vob";
			return s;
		}

		String getValue(const void* _target)
		{
			const Vob* f = static_cast<const Vob*>(_target);
			return f->getVisual();
		}

		void setValue(void* _target, const String& _value)
		{
			Vob* f = static_cast<Vob*>(_target);
			f->setVisual(_value);
		}
	};
	//--------------------------------------------------------------------------
	class Vob_ShowVisual : public ZenParamBool
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
			static const String s = "Vob";
			return s;
		}

		bool getValue(const void* _target)
		{
			const Vob* f = static_cast<const Vob*>(_target);
			return f->getShowVisual();
		}

		void setValue(void* _target, bool _value)
		{
			Vob* f = static_cast<Vob*>(_target);
			f->setShowVisual(_value);
		}
	};
	//--------------------------------------------------------------------------
	class Vob_ShowVisualFrozen : public ZenParamBool
	{
	public:
		const String& getName()
		{
			static const String s = "ShowVisualFrozen";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "";
			return s;
		}

		const String& getGroup()
		{
			static const String s = "Vob";
			return s;
		}

		bool getValue(const void* _target)
		{
			const Vob* f = static_cast<const Vob*>(_target);
			return f->getShowVisualFrozen();
		}

		void setValue(void* _target, bool _value)
		{
			Vob* f = static_cast<Vob*>(_target);
			f->setShowVisualFrozen(_value);
		}

		bool isInternal()
		{
			return true;
		}
	};
	//--------------------------------------------------------------------------
	class Vob_InvisibleVisual : public ZenParamString
	{
	public:
		const String& getName()
		{
			static const String s = "InvisibleVisual";
			return s;
		}

		const String& getDescription()
		{
			static const String s = "";
			return s;
		}

		const String& getSubType()
		{
			return SubType::VISUAL_NAME;
		}

		const String& getGroup()
		{
			static const String s = "Vob";
			return s;
		}

		String getValue(const void* _target)
		{
			const Vob* f = static_cast<const Vob*>(_target);
			return f->getInvisibleVisual();
		}

		void setValue(void* _target, const String& _value)
		{
			Vob* f = static_cast<Vob*>(_target);
			f->setInvisibleVisual(_value);
		}

		bool isInternal()
		{
			return true;
		}
	};
	//--------------------------------------------------------------------------
	class Vob_WorldBounds : public ZenParamAxisAlignedBox
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
			static const String s = "Vob";
			return s;
		}

		AxisAlignedBox getValue(const void* _target)
		{
			const Vob* f = static_cast<const Vob*>(_target);
			return f->getWorldBounds();
		}

		void setValue(void* _target, const AxisAlignedBox& _value)
		{
			Vob* f = static_cast<Vob*>(_target);
			f->setWorldBounds(_value);
		}

		bool isInternal()
		{
			return true;
		}
	};
	//-------------------------------------------------------------------
	Vob::Vob(ZenFactory* _creator)
		: DerivedWorldObject("BaseWorldObject"),
		mEntity(nullptr), mShowVisual(true), mShowVisualFrozen(false),
		mInvisibleEntity(nullptr)
	{
		if (createParamDictionary("Vob"))
		{
			ZenParamDictionary* dict = getParamDictionary();
			dict->addParamDef( new Vob_Name );
			dict->addParamDef( new Vob_Type );
			dict->addParamDef( new Vob_Position );
			dict->addParamDef( new Vob_Orientation );
			dict->addParamDef( new Vob_Scale );
			dict->addParamDef( new Vob_Parent );
			dict->addParamDef( new Vob_Visual );
			dict->addParamDef( new Vob_ShowVisual );
			dict->addParamDef( new Vob_ShowVisualFrozen );
			dict->addParamDef( new Vob_InvisibleVisual );
			dict->addParamDef( new Vob_WorldBounds );
		}
	}
	//-------------------------------------------------------------------
	Vob::~Vob()
	{
		destroyVisual();
		destroyInvisibleVisual();
	}
	//-------------------------------------------------------------------
	void Vob::destroyVisual()
	{
		if(mEntity)
		{
			SceneNode* sceneNode = mEntity->getParentSceneNode();
			Ogre::SceneManager* sceneManager = sceneNode->getCreator();
			sceneNode->detachObject(mEntity);
			sceneManager->destroyEntity(mEntity);
			mEntity = nullptr;
		}
	}
	//-------------------------------------------------------------------
	void Vob::updateVisual()
	{
		destroyVisual();

		if(!mVisual.empty() && mShowVisual)
		{
			MeshExPtr mesh = MeshManagerEx::getSingleton().create(mVisual);
			SceneNode* sceneNode = getSceneNode();
			SceneManagerEx* sceneMgr = (SceneManagerEx*) sceneNode->getCreator();
			mEntity = sceneMgr->createEntity(mesh, SceneQueryMask::VOB);
			sceneNode->attachObject(mEntity);
		}
	}
	//-------------------------------------------------------------------
	void Vob::setVisual(const String& _visual)
	{
		mVisual = _visual;
		updateVisual();
	}
	//-------------------------------------------------------------------
	const String& Vob::getVisual() const
	{
		return mVisual;
	}
	//-------------------------------------------------------------------
	void Vob::setShowVisual(bool _showVisual)
	{
		if(!mShowVisualFrozen)
		{
			mShowVisual = _showVisual;
			updateVisual();
		}
	}
	//-------------------------------------------------------------------
	bool Vob::getShowVisual() const
	{
		return mShowVisual;
	}
	//-------------------------------------------------------------------
	void Vob::setShowVisualFrozen(bool _freeze)
	{
		mShowVisualFrozen = _freeze;
	}
	//-------------------------------------------------------------------
	bool Vob::getShowVisualFrozen() const
	{
		return mShowVisualFrozen;
	}
	//-------------------------------------------------------------------
	void Vob::setPresetName(const String& _presetName)
	{
	}
	//-------------------------------------------------------------------
	const String& Vob::getPresetName() const
	{
		return StringUtil::BLANK;
	}
	//-------------------------------------------------------------------
	void Vob::setVisualCamAlign(VisualCamAlign::Enum _visualCamAlign)
	{
	}
	//-------------------------------------------------------------------
	VisualCamAlign::Enum Vob::getVisualCamAlign() const
	{
		return VisualCamAlign::NONE;
	}
	//-------------------------------------------------------------------
	void Vob::setCdStatic(bool _cdStatic)
	{
	}
	//-------------------------------------------------------------------
	bool Vob::getCdStatic() const
	{
		return false;
	}
	//-------------------------------------------------------------------
	void Vob::setCdDyn(bool _cdDyn)
	{
	}
	//-------------------------------------------------------------------
	bool Vob::getCdDyn() const
	{
		return false;
	}
	//-------------------------------------------------------------------
	void Vob::setStaticVob(bool _staticVob)
	{
	}
	//-------------------------------------------------------------------
	bool Vob::getStaticVob() const
	{
		return false;
	}
	//-------------------------------------------------------------------
	void Vob::setDynShadow(DynShadow::Enum _dynShadow)
	{
	}
	//-------------------------------------------------------------------
	DynShadow::Enum Vob::getDynShadow() const
	{
		return DynShadow::NONE;
	}
	//-------------------------------------------------------------------
	void Vob::setInvisibleVisual(const String& _visual)
	{
		mInvisibleVisual = _visual;
		updateInvisibleVisual();
	}
	//-------------------------------------------------------------------
	const String& Vob::getInvisibleVisual() const
	{
		return mInvisibleVisual;
	}
	//-------------------------------------------------------------------
	void Vob::updateInvisibleVisual()
	{
		destroyInvisibleVisual();
		if(!mInvisibleVisual.empty())
		{
			MeshExPtr mesh = MeshManagerEx::getSingleton().create(mInvisibleVisual);
			SceneNode* sceneNode = getSceneNode();
			SceneManagerEx* sceneMgr = (SceneManagerEx*) sceneNode->getCreator();
			mInvisibleEntity = sceneMgr->createEntity(mesh, SceneQueryMask::VOB);
			sceneNode->attachObject(mInvisibleEntity);
		}
	}
	//-------------------------------------------------------------------
	void Vob::destroyInvisibleVisual()
	{
		if(mInvisibleEntity)
		{
			SceneNode* sceneNode = mInvisibleEntity->getParentSceneNode();
			Ogre::SceneManager* sceneManager = sceneNode->getCreator();
			sceneNode->detachObject(mInvisibleEntity);
			sceneManager->destroyEntity(mInvisibleEntity);
			mInvisibleEntity = nullptr;
		}		
	}
	//-------------------------------------------------------------------
	void Vob::setWorldBounds(const AxisAlignedBox& _box)
	{
		mWorldBounds = _box;
	}
	//-------------------------------------------------------------------
	const AxisAlignedBox& Vob::getWorldBounds() const
	{
		return mWorldBounds;
	}
	//-------------------------------------------------------------------
	Vector3 Vob::exchangeYZ(const Vector3& _vec)
	{
		Vector3 vec = _vec;
		std::swap(vec[1], vec[2]);
		return vec;
	}
	//-------------------------------------------------------------------
	Matrix3 Vob::exchangeYZ(const Matrix3& _mat)
	{
		Matrix3 mat = _mat;
		std::swap(mat[0][1], mat[0][2]);
		std::swap(mat[1][0], mat[2][0]);
		std::swap(mat[1][1], mat[2][2]);
		std::swap(mat[1][2], mat[2][1]);
		return mat;
	}
	//-------------------------------------------------------------------
	AxisAlignedBox Vob::exchangeYZ(const AxisAlignedBox& _aab)
	{
		AxisAlignedBox aab;
		aab.setExtents(exchangeYZ(_aab.getMinimum()), exchangeYZ(_aab.getMaximum()));
		return aab;
	}
	//-------------------------------------------------------------------
	void Vob::readPacked(ZenArchive* _zenArchive, ushort _version)
	{
		PackedVob packedVob;
		size_t sz = _zenArchive->readRaw("dataRaw", &packedVob, sizeof(packedVob));

		if(sz == 0x53)
		{
			// Gothic 2
		}
		else if(sz == 0x4A) 
		{
			// Gothic 1 old format: flags occupy two bytes, visualAnimModeStrength
			// and vobFarClipZScale are not used
			packedVob.flags[3] = 0;
			packedVob.visualAniModeStrength = 0;
			packedVob.vobFarClipZScale = 1;
		}
		else
		{
			// Unknown format
			GOTHOGRE_EXCEPT("Vob: Unexpected dataRaw size: " << sz);
		}

		Vector3 pos( (Real) packedVob.pos[0], (Real) packedVob.pos[1], (Real) packedVob.pos[2] );
		Matrix3 rot( (Real) packedVob.rot[0], (Real) packedVob.rot[1], (Real) packedVob.rot[2] ,
					 (Real) packedVob.rot[3], (Real) packedVob.rot[4], (Real) packedVob.rot[5] ,
					 (Real) packedVob.rot[6], (Real) packedVob.rot[7], (Real) packedVob.rot[8] );
		setDerivedPosition(exchangeYZ(pos));
		setDerivedOrientation(exchangeYZ(rot));

		AxisAlignedBox bbox( (Real) packedVob.bbox3DWS[0], (Real) packedVob.bbox3DWS[1], (Real) packedVob.bbox3DWS[2], 
		                     (Real) packedVob.bbox3DWS[3], (Real) packedVob.bbox3DWS[4], (Real) packedVob.bbox3DWS[5]);
		setWorldBounds(exchangeYZ(bbox));

		PackedVobBits bits;
		memset(&bits, 0, sizeof(bits));
		memcpy(&bits, &packedVob.flags, sizeof(packedVob.flags));

		if(bits.presetName)
			setPresetName(_zenArchive->readString("presetName"));

		if(bits.vobName)
			setName(_zenArchive->readString("vobName"));

		if(bits.visual)
			setVisual(_zenArchive->readString("visual"));

		setShowVisual(bits.showVisual);
		setVisualCamAlign( (VisualCamAlign::Enum) bits.visualCamAlign);
		setCdStatic(bits.cdStatic);
		setCdDyn(bits.cdDyn);
		setStaticVob(bits.staticVob);
		setDynShadow( (DynShadow::Enum) bits.dynShadow);
	}
	//-------------------------------------------------------------------
	void Vob::readUnpacked(ZenArchive* _zenArchive, ushort _version)
	{
		setPresetName(_zenArchive->readString("presetName"));

		// Read bounding box (with the world space coordinates).
		// It's very possible the bounding box will not been used
		// because Ogre prefers to calculate bounds by itself.
		float bbox3DWS[6];
		_zenArchive->readRawFloat("bbox3DWS", bbox3DWS, sizeof(bbox3DWS));
		AxisAlignedBox bbox( (Real) bbox3DWS[0], (Real) bbox3DWS[1], (Real) bbox3DWS[2], 
		                     (Real) bbox3DWS[3], (Real) bbox3DWS[4], (Real) bbox3DWS[5]);
		setWorldBounds(exchangeYZ(bbox));

		// read transformation matrix
		Matrix3 rot = _zenArchive->readMatrix3("trafoOSToWSRot");
		Vector3 pos = _zenArchive->readVector3("trafoOSToWSPos");
		setDerivedPosition(exchangeYZ(pos));
		setDerivedOrientation(exchangeYZ(rot));

		setName(_zenArchive->readString("vobName"));
		setVisual(_zenArchive->readString("visual"));
		setShowVisual(_zenArchive->readBool("showVisual"));
		setVisualCamAlign( (VisualCamAlign::Enum) _zenArchive->readEnum("visualCamAlign") );

		setCdStatic(_zenArchive->readBool("cdStatic"));
		setCdDyn(_zenArchive->readBool("cdDyn"));
		setStaticVob(_zenArchive->readBool("staticVob"));
		setDynShadow( (DynShadow::Enum) _zenArchive->readEnum("dynShadow") );
	}
	//-------------------------------------------------------------------
	void Vob::read(ZenArchive* _zenArchive, ushort _version)
	{
		//GOTHOGRE_INFO("A " << __rdtsc());

		DerivedWorldObject::read(_zenArchive, _version);
		int pack = _zenArchive->readInt("pack");
		if(pack)
			readPacked(_zenArchive, _version);
		else
			readUnpacked(_zenArchive, _version);

		//GOTHOGRE_INFO("B " << __rdtsc());
	}
	//-------------------------------------------------------------------
	void Vob::write(ZenArchive* _zenArchive, ushort _version)
	{
		DerivedWorldObject::write(_zenArchive, _version);
	}
	//-------------------------------------------------------------------
	String Vob::getCombinedName() const
	{
		String str = getBase()->getCombinedName();
		if(!mVisual.empty())
		{
			if(!str.empty())
				str += ' ';
			str += '\"';
			str += mVisual;
			str += '\"';
		}
		return str;
	}
	//-------------------------------------------------------------------
	VobFactory::VobFactory()
		: ZenFactory("Vob")
	{
	}
	//-------------------------------------------------------------------
	const vector<String>::type& VobFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("zCVob");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& VobFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& VobFactory::getSupportedVersions() const
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
	ushort VobFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr VobFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new Vob(this);
	}

} // namespace GothOgre