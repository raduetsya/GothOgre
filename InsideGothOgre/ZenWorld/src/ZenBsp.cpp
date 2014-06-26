#include "Precompiled.h"
#include "ZenBsp.h"


namespace GothOgre
{
	//--------------------------------------------------------------------------
	class ZenBsp_Name : public ZenParamString
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
			static const String s = "ZenBsp";
			return s;
		}

		String getValue(const void* _target)
		{
			const ZenBsp* f = static_cast<const ZenBsp*>(_target);
			return f->getName();
		}

		void setValue(void* _target, const String& _value)
		{
			ZenBsp* f = static_cast<ZenBsp*>(_target);
			f->setName(_value);
		}
	};
	//--------------------------------------------------------------------------
	class ZenBsp_Type : public ZenParamString
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
			static const String s = "ZenBsp";
			return s;
		}

		String getValue(const void* _target)
		{
			const ZenBsp* f = static_cast<const ZenBsp*>(_target);
			return f->getWorldObjectTypeName();
		}

		bool isReadOnly()
		{
			return true;
		}
	};
	//--------------------------------------------------------------------------
	class ZenBsp_Position : public ZenParamVector3
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
			static const String s = "ZenBsp";
			return s;
		}

		Vector3 getValue(const void* _target)
		{
			const ZenBsp* f = static_cast<const ZenBsp*>(_target);
			return f->getPosition();
		}

		void setValue(void* _target, const Vector3& _value)
		{
			ZenBsp* f = static_cast<ZenBsp*>(_target);
			f->setPosition(_value);
		}
	};
	//--------------------------------------------------------------------------
	class ZenBsp_Orientation : public ZenParamQuaternion
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
			static const String s = "ZenBsp";
			return s;
		}

		Quaternion getValue(const void* _target)
		{
			const ZenBsp* f = static_cast<const ZenBsp*>(_target);
			return f->getOrientation();
		}

		void setValue(void* _target, const Quaternion& _value)
		{
			ZenBsp* f = static_cast<ZenBsp*>(_target);
			f->setOrientation(_value);
		}
	};
	//--------------------------------------------------------------------------
	class ZenBsp_Scale : public ZenParamVector3
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
			static const String s = "ZenBsp";
			return s;
		}

		Vector3 getValue(const void* _target)
		{
			const ZenBsp* f = static_cast<const ZenBsp*>(_target);
			return f->getScale();
		}

		void setValue(void* _target, const Vector3& _value)
		{
			ZenBsp* f = static_cast<ZenBsp*>(_target);
			f->setScale(_value);
		}
	};
	//---------------------------------------------------------------------------
	// ZenBsp
	//---------------------------------------------------------------------------
	ZenBsp::ZenBsp()
		: DerivedWorldObject("BaseWorldObject")
	{
		if (createParamDictionary("ZenBsp"))
		{
			ZenParamDictionary* dict = getParamDictionary();
			dict->addParamDef( new ZenBsp_Name );
			dict->addParamDef( new ZenBsp_Type );
			dict->addParamDef( new ZenBsp_Position );
			dict->addParamDef( new ZenBsp_Orientation );
			dict->addParamDef( new ZenBsp_Scale );
		}
	}
	//---------------------------------------------------------------------------
	ZenBsp::~ZenBsp()
	{
		destroyVisual();
	}
	//---------------------------------------------------------------------------
	void ZenBsp::read(ZenArchive* _zenArchive, ushort _version)
	{
		DataStreamPtr dataStream = _zenArchive->getDataStream();
		
		// Read mesh
		String bspName = getWorld()->getShortName();
		setName(bspName);

		mBspMesh = MeshManagerEx::getSingleton().findByName(bspName);
		if(mBspMesh.isNull())
		{
			mBspMesh = MeshManagerEx::getSingleton().create(bspName);
			MeshManagerEx::getSingleton().load( mBspMesh.get(), dataStream, "MeshAndBspLoader" );
		}
		else
		{
			MeshManagerEx::getSingleton().skip( dataStream, "MeshAndBspLoader" );
		}

		updateVisual();
	}
	//-------------------------------------------------------------------
	void ZenBsp::write(ZenArchive* _zenArchive, ushort _version)
	{

	}
	//---------------------------------------------------------------------------
	void ZenBsp::updateVisual()
	{
		destroyVisual();
		
		SceneNode* sceneNode = getSceneNode();
		SceneManagerEx* sm = (SceneManagerEx*) (sceneNode->getCreator());
		mEntityList = sm->createEntityListForLargeMesh(mBspMesh, SceneQueryMask::LEVEL_BSP);
		
		for(SceneManagerEx::EntityList::iterator it = mEntityList.begin(); 
			it != mEntityList.end(); ++it)
		{
			Entity* entity = *it;
			SceneNode* child = sceneNode->createChildSceneNode();
			child->attachObject(entity);

		/*	PortalSystem::PortalList portals = PortalSystem::getSingleton().createPortalsByLevelEntity(entity);
			for(PortalSystem::PortalList::iterator it2 = portals.begin(); 
				it2 != portals.end(); ++it2)
			{
				Portal* portal = *it2;
				child->attachObject(portal);
				mPortalList.push_back(portal);
			}*/
		}
	}
	//---------------------------------------------------------------------------
	void ZenBsp::destroyVisual()
	{
		SceneNode* sceneNode = getSceneNode();
		SceneManagerEx* sm = (SceneManagerEx*) (sceneNode->getCreator());
		sm->destroyEntityList(mEntityList);
		PortalSystem::getSingleton().destroyPortals(mPortalList);
	}



	//---------------------------------------------------------------------------
	// ZenBspFactory
	//---------------------------------------------------------------------------
	ZenBspFactory::ZenBspFactory()
		: ZenFactory("ZenBsp")
	{
	}
	//---------------------------------------------------------------------------
	const vector<String>::type& ZenBspFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		return sTypeNames;
	}
	//---------------------------------------------------------------------------
	const String& ZenBspFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//---------------------------------------------------------------------------
	const vector<ushort>::type& ZenBspFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		return sVersions;
	}
	//---------------------------------------------------------------------------
	ushort ZenBspFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return 0;
	}
	//---------------------------------------------------------------------------
	ZenObjectPtr ZenBspFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new ZenBsp;
	}

} // namespace GothOgre