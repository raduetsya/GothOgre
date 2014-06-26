#ifndef VOB_H
#define VOB_H

namespace GothOgre
{
	/***/
	struct VisualCamAlign
	{
		enum Enum
		{
			NONE,
			YAW,
			FULL
		};
	};

	/***/
	struct DynShadow
	{
		enum Enum
		{
			NONE,
			BLOB
		};
	};

	/***/
	class Vob : public DerivedWorldObject
	{
	public:
		Vob(ZenFactory* _creator);
		virtual ~Vob();

		String getCombinedName() const;

		void setVisual(const String& _visual);
		const String& getVisual() const;

		void setShowVisual(bool _showVisual);
		bool getShowVisual() const;

		void setShowVisualFrozen(bool _freeze);
		bool getShowVisualFrozen() const;

		void setAlignVisual(bool _alignVisual);
		bool getAlignVisual() const;

		void setVisualCamAlign(VisualCamAlign::Enum _visualCamAlign);
		VisualCamAlign::Enum getVisualCamAlign() const;
		void setPresetName(const String& _presetName);
		const String& getPresetName() const;
		void setCdStatic(bool _cdStatic);
		bool getCdStatic() const;
		void setCdDyn(bool _cdDyn);
		bool getCdDyn() const;
		void setStaticVob(bool _staticVob);
		bool getStaticVob() const;
		void setDynShadow(DynShadow::Enum _dynShadow);
		DynShadow::Enum getDynShadow() const;

		void setInvisibleVisual(const String& _visual);
		const String& getInvisibleVisual() const;

		void setWorldBounds(const AxisAlignedBox& _box);
		const AxisAlignedBox& getWorldBounds() const;

		virtual void read(ZenArchive* _zenArchive, ushort _version);
		virtual void write(ZenArchive* _zenArchive, ushort _version);

	private:
		#pragma pack(push, 1)

		struct PackedVobBits
		{
			ulong showVisual:1;		// 0x000001
			ulong visualCamAlign:2;	// 0x000002-0x000006
			ulong cdStatic:1;		// 0x000008
			ulong cdDyn:1;			// 0x000010
			ulong staticVob:1;		// 0x000020
			ulong dynShadow:2;		// 0x000040-0x000080
			ulong presetName:1;		// 0x000100
			ulong vobName:1;		// 0x000200
			ulong visual:1;			// 0x000400
			ulong unknown1:5;		// 0x000800-0x008000
			ulong visualAniMode:2;	// 0x010000-0x020000
			ulong unknown2:5;		// 0x040000-0x400000
			ulong isAmbient: 1;		// 0x800000
		};

		struct PackedVob
		{
			float			bbox3DWS[6];
			float			pos[3];
			float			rot[9];
			char			flags[3];
			float			visualAniModeStrength;
			float			vobFarClipZScale;
		};

		#pragma pack(pop)

	private:
		void updateVisual();
		void destroyVisual();
		void updateInvisibleVisual();
		void destroyInvisibleVisual();
		void readPacked(ZenArchive* _zenArchive, ushort _version);
		void readUnpacked(ZenArchive* _zenArchive, ushort _version);
		Vector3 exchangeYZ(const Vector3& _vec);
		Matrix3 exchangeYZ(const Matrix3& _vec);
		AxisAlignedBox exchangeYZ(const AxisAlignedBox& _aab);

	private:
		String          mVisual;
		bool      	    mShowVisual;
		bool            mShowVisualFrozen;
		Entity*         mEntity;
		AxisAlignedBox  mWorldBounds;
		String          mInvisibleVisual;
		Entity*         mInvisibleEntity;
	};

	/***/
	class VobFactory : public ZenFactory
	{
	public:
		VobFactory();
		virtual const vector<String>::type& getSupportedTypeNames() const;
		virtual const String& getCompatibleTypeName(ZenArchive* _zenArchive) const;
		virtual const vector<ushort>::type& getSupportedVersions() const;
		virtual ushort getCompatibleVersion(ZenArchive* _zenArchive) const;
		virtual ZenObjectPtr createZenObjectImpl();
	};

} // namespace GothOgre

#endif // VOB_H
