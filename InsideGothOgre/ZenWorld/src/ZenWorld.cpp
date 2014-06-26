#include "Precompiled.h"
#include "ZenWorld.h"

namespace GothOgre
{
	ZenWorld::ZenWorld()
	{
	}
	//-------------------------------------------------------------------
	ZenWorld::~ZenWorld()
	{
	}
	//-------------------------------------------------------------------
	void ZenWorld::read(ZenArchive* _zenArchive, ushort _version)
	{
		Progress& progress = Progress::getSingleton();
		progress.begin();
		progress.setRange(0, 100);

		ResourceLocationScanner::getSingleton().scanAll();

		bool zenbspFound = false;

		ZenArchive::Chunk* chunk = nullptr;
		while(chunk = _zenArchive->readChunkBegin())
		{
			if(chunk->getChunkName() == "MeshAndBsp")
			{
				Progress::Operation* op = progress.beginChildOperation(0, 25);
				WorldObjectPtr zenbsp = ZenManager::getSingleton().createZenObject("ZenBsp");
				zenbsp->read(_zenArchive, 0);
				getRootWorldObject()->addChild(zenbsp);
				progress.endChildOperation(op);
				zenbspFound = true;
			}
			else if(chunk->getChunkName() == "VobTree")
			{
				Progress::Operation* op = progress.beginChildOperation( (zenbspFound ? 25 : 0), 100);
				readVobTree(_zenArchive);
				progress.endChildOperation(op);
			}
			else if(chunk->getChunkName() == "WayNet")
			{
				// TODO
			}
			_zenArchive->readChunkEnd(chunk);
		}

		progress.end();
	}
	//-------------------------------------------------------------------
	void ZenWorld::readVobTree(ZenArchive* _zenArchive)
	{
		Progress& progress = Progress::getSingleton();
		progress.setRange(_zenArchive->getDataStream()->tellg(), _zenArchive->getDataStream()->size());
		mChildID = 0;
		_readVobTree(_zenArchive, getRootWorldObject());
	}
	//-------------------------------------------------------------------
	void ZenWorld::_readVobTree(ZenArchive* _zenArchive, const WorldObjectPtr& _root)
	{
		Progress& progress = Progress::getSingleton();
		progress.setPosition(_zenArchive->getDataStream()->tellg());

		StrStream ss;
		ss << "childs" << mChildID++;
		int childs = _zenArchive->readInt(ss.str());
		for(int i = 0; i != childs; ++i)
		{
			WorldObjectPtr wobj;
			if(!_root.isNull())
			{
				wobj = _zenArchive->readObject();
				if(!wobj.isNull())
				{
					_root->addChild(wobj, true);
				}
			}
			_readVobTree(_zenArchive, wobj);
		}
	}
	//-------------------------------------------------------------------
	void ZenWorld::write(ZenArchive* _zenArchive, ushort _version)
	{


	}
	//-------------------------------------------------------------------
	ZenWorldFactory::ZenWorldFactory()
		: ZenFactory("ZenWorld")
	{
		WorldManager::getSingleton().setWorldTypeName(getTypeName());
	}
	//-------------------------------------------------------------------
	const vector<String>::type& ZenWorldFactory::getSupportedTypeNames() const
	{
		static vector<String>::type sTypeNames;
		if(sTypeNames.empty())
		{
			sTypeNames.push_back("oCWorld:zCWorld");
		}
		return sTypeNames;
	}
	//-------------------------------------------------------------------
	const String& ZenWorldFactory::getCompatibleTypeName(ZenArchive* _zenArchive) const
	{
		return getSupportedTypeNames()[0];
	}
	//-------------------------------------------------------------------
	const vector<ushort>::type& ZenWorldFactory::getSupportedVersions() const
	{
		static vector<ushort>::type sVersions;
		if(sVersions.empty())
		{
			sVersions.push_back(0xFC01);
		}
		return sVersions;
	}
	//-------------------------------------------------------------------
	ushort ZenWorldFactory::getCompatibleVersion(ZenArchive* _zenArchive) const
	{
		return getSupportedVersions()[0];
	}
	//-------------------------------------------------------------------
	ZenObjectPtr ZenWorldFactory::createZenObject()
	{
		// Only one world can exist at the same time
		if(WorldManager::getSingleton()._getCreateMode() == WorldManager::CREATE_NEW_WORLD)
			return ZenFactory::createZenObject();
		else
			return WorldManager::getSingleton().getActiveWorld();
	}
	//-------------------------------------------------------------------
	ZenObjectPtr ZenWorldFactory::createZenObjectImpl()
	{
		return (ZenObjectPtr) new ZenWorld;
	}

} // namespace GothOgre