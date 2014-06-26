#include "GothOgre_Precompiled.h"
#include "GothOgre_AppImpl.h"

#include "GothOgre_ZenArchive.h"
#include "GothOgre_ResourceGroup.h"
#include "GothOgre_SceneQueryMask.h"


namespace GothOgre
{
	//----------------------------------------------------------------------
	void AppImpl::test()
	{
		// Test #1
	//	WorldManager::getSingleton().loadWorld("NewWorld_Part_Xardas_01.zen", ResourceGroup::WORLDS);

	/*
	//	Profiler::getSingleton().setEnabled(true);
	
		Progress& progress = Progress::getSingleton();
		progress.begin();
		ResourceLocationScanner::getSingleton().scanAll();
	
		ZenArchive za;
		//za.readHeader("NewWorld_Part_Xardas_01.zen", ResourceGroup::WORLDS);
		//za.readHeader("NewWorld_Part_City_01.zen", ResourceGroup::WORLDS);
		za.readHeader("NewWorld.zen", ResourceGroup::WORLDS);
		//za.readHeader("OrcTempel.zen", ResourceGroup::WORLDS);
		//za.readHeader("NEWWORLD_UNCOMPILED.zen", ResourceGroup::WORLDS);
		za.readObject();
		za.readFooter();

		progress.end();	
	*/
	
		// Test #2
	/*	ResourceLocationScanner::getSingleton().scanAll();
		SceneNode* sceneNode = World::getSingleton().getSceneManager()->getRootSceneNode()->createChildSceneNode();
		MeshPtr mesh = MeshManagerEx::getSingleton().create("treasure.3ds");
		Entity* entity = World::getSingleton().getSceneManager()->createEntity(mesh, SceneQueryMask::VOB);
		sceneNode->attachObject(entity);
		sceneNode->showBoundingBox(true);
	
	*/
	}
}