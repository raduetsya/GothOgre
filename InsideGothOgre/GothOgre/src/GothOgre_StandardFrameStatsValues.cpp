#include "GothOgre_Precompiled.h"
#include "GothOgre_StrConv.h"
#include "GothOgre_OgreTypeIO.h"
#include "GothOgre_StdIO.h"
#include "GothOgre_SceneManagerEx.h"
#include "GothOgre_WorldTime.h"
#include "GothOgre_World.h"
#include "GothOgre_WorldManager.h"
#include "GothOgre_SoundSystem.h"
#include "GothOgre_StandardFrameStatsValues.h"

namespace GothOgre
{
	//-------------------------------------------------------------------------
	// Implementations of standard frame statistics' values
	//-------------------------------------------------------------------------
	class FPSFrameStatsValue : public FrameStatsValue
	{
	public:
		FPSFrameStatsValue()
			: FrameStatsValue("FPS", true)
		{
		}

		String getValue()
		{
			RenderTarget* target;
			World* world = WorldManager::getSingleton().getActiveWorld();
			if(world && world->getViewport())
			{
				target = world->getViewport()->getTarget();
			}
			else
			{
				target = Root::getSingleton().getAutoCreatedWindow();;
			}
			const RenderTarget::FrameStats& stats = target->getStatistics();
			return StrConv::toString( (int) stats.lastFPS );
		}
	};
	//-------------------------------------------------------------------------
	class BatchesFrameStatsValue : public FrameStatsValue
	{
	public:
		BatchesFrameStatsValue()
			: FrameStatsValue("Batches", true)
		{
		}

		String getValue()
		{
			RenderTarget* target;
			World* world = WorldManager::getSingleton().getActiveWorld();
			if(world && world->getViewport())
			{
				target = world->getViewport()->getTarget();
			}
			else
			{
				target = Root::getSingleton().getAutoCreatedWindow();;
			}
			const RenderTarget::FrameStats& stats = target->getStatistics();
			return StrConv::toString( stats.batchCount );
		}
	};
	//-------------------------------------------------------------------------
	class TrianglesFrameStatsValue : public FrameStatsValue
	{
	public:
		TrianglesFrameStatsValue()
			: FrameStatsValue("Triangles", true)
		{
		}

		String getValue()
		{
			RenderTarget* target;
			World* world = WorldManager::getSingleton().getActiveWorld();
			if(world && world->getViewport())
			{
				target = world->getViewport()->getTarget();
			}
			else
			{
				target = Root::getSingleton().getAutoCreatedWindow();;
			}
			const RenderTarget::FrameStats& stats = target->getStatistics();
			return StrConv::toString( stats.triangleCount );
		}
	};
	//-------------------------------------------------------------------------
	class CameraPositionFrameStatsValue : public FrameStatsValue
	{
	public:
		CameraPositionFrameStatsValue()
			: FrameStatsValue("CameraPosition", true)
		{
		}

		String getValue()
		{
			World* world = WorldManager::getSingleton().getActiveWorld();
			if(!world)
				return StringUtil::BLANK;

			Camera* camera = world->getCamera();
			const Vector3& pos = camera->getDerivedPosition();
			return StrConv::toString( pos );
		}
	};
	//-------------------------------------------------------------------------
	class CameraOrientationFrameStatsValue : public FrameStatsValue
	{
	public:
		CameraOrientationFrameStatsValue()
			: FrameStatsValue("CameraOrientation", true)
		{
		}

		String getValue()
		{
			World* world = WorldManager::getSingleton().getActiveWorld();
			if(!world)
				return StringUtil::BLANK;

			Camera* camera = world->getCamera();
			const Quaternion& q = camera->getDerivedOrientation();
			return StrConv::toString( q );
		}
	};
	//-------------------------------------------------------------------------
	class SectorNameFrameStatsValue : public FrameStatsValue
	{
	public:
		SectorNameFrameStatsValue()
			: FrameStatsValue("SectorName", true)
		{
		}

		String getValue()
		{
			World* world = WorldManager::getSingleton().getActiveWorld();
			if(!world)
				return StringUtil::BLANK;

			Camera* camera = world->getCamera();
			SceneManagerEx* sm = world->getSceneManager();
			const SceneSectorInfo& sectorInfo = sm->getSectorInfo(camera);
			String sectorName = sectorInfo.sectorName;
			if(sectorName.empty())
			{
				const SceneUnderwaterInfo& underwaterInfo = sm->getUnderwaterInfo(camera);
				if(!underwaterInfo.waterMaterial.isNull())
				{
					sectorName = "UNDERWATER";
				}
			}
			return sectorName;
		}
	};
	//-------------------------------------------------------------------------
	class WorldTimeFrameStatsValue : public FrameStatsValue
	{
	public:
		WorldTimeFrameStatsValue()
			: FrameStatsValue("WorldTime", true)
		{
		}

		String getValue()
		{
			Time time = WorldTime::getSingleton().getCurrentTime();
			StrStream ss;
			ss.precision(0);
			ss << time;
			return ss.str();
		}
	};
	//-------------------------------------------------------------------------
	template<bool _3D, int _index>
	class CurrentlyPlayingSoundFrameStatsValue : public FrameStatsValue
	{
	public:
		CurrentlyPlayingSoundFrameStatsValue()
			: FrameStatsValue( 
				String("Sound") + (_3D ? ("3D(" + StrConv::toString(_index) + ")"): "2D"), 
				false)
		{
		}

		String getValue()
		{
			SoundSystem::PlayingSoundIterator it = SoundSystem::getSingleton().getPlayingSoundIterator(_3D);
			int i = _index;
			while(it.hasMoreElements())
			{
				const String& filename = it.getNext();
				if((i--) == 0)
					return filename;
			}
			return StringUtil::BLANK;
		}
	};



	
	//-------------------------------------------------------------------------
	// StandardFrameStatsValues - manager of all standard frame statistics' values
	// listened above
	//-------------------------------------------------------------------------
	StandardFrameStatsValues::StandardFrameStatsValues()
	{
		mList.push_back(new FPSFrameStatsValue);
		mList.push_back(new BatchesFrameStatsValue);
		mList.push_back(new TrianglesFrameStatsValue);
		mList.push_back(new CameraPositionFrameStatsValue);
		mList.push_back(new CameraOrientationFrameStatsValue);
		mList.push_back(new SectorNameFrameStatsValue);
		mList.push_back(new WorldTimeFrameStatsValue());
		mList.push_back(new CurrentlyPlayingSoundFrameStatsValue<false, 0>());
		mList.push_back(new CurrentlyPlayingSoundFrameStatsValue<true, 0>());
		mList.push_back(new CurrentlyPlayingSoundFrameStatsValue<true, 1>());
		mList.push_back(new CurrentlyPlayingSoundFrameStatsValue<true, 2>());
		mList.push_back(new CurrentlyPlayingSoundFrameStatsValue<true, 3>());
		mList.push_back(new CurrentlyPlayingSoundFrameStatsValue<true, 4>());
		mList.push_back(new CurrentlyPlayingSoundFrameStatsValue<true, 5>());
		mList.push_back(new CurrentlyPlayingSoundFrameStatsValue<true, 6>());
		mList.push_back(new CurrentlyPlayingSoundFrameStatsValue<true, 7>());
	}
	//-------------------------------------------------------------------------
	StandardFrameStatsValues::~StandardFrameStatsValues()
	{
		for(List::reverse_iterator it = mList.rbegin(); it != mList.rend(); ++it)
		{
			FrameStatsValue* p = *it;
			delete p;
		}
	}

}
