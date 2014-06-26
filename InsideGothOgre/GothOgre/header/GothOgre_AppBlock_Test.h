#ifndef GOTHOGRE_APP_BLOCK_TEST_H
#define GOTHOGRE_APP_BLOCK_TEST_H

#include "GothOgre_AppBlock.h


namespace GothOgre
{
	class TestAppBlock : public AppBlock
	{
	public:
		void initialise()
		{
		// test-1
		/*	mResourceLocationScanner->scanAll();
			ZenArchive za;
			//za.readHeader("NewWorld_Part_Xardas_01_Uncompiled.zen", World::GROUP_NAME);
			//za.readHeader("NewWorld_Part_Xardas_01.zen", World::GROUP_NAME);
			za.readHeader("OrcTempel.zen", World::GROUP_NAME);
			//za.readHeader("NewWorld_Part_Farm_01.zen", World::GROUP_NAME);
			//za.readHeader("NewWorld_Uncompiled.zen", World::GROUP_NAME);
			//za.readHeader("NewWorld.zen", World::GROUP_NAME);
			//za.readHeader("World.zen", World::GROUP_NAME);
			za.readObject();
			za.readFooter();
		*/
			// test-2
		/*	mResourceLocationScanner->scanAll();
			SceneNode* sceneNode = World::getSingleton().getSceneManager()->getRootSceneNode()->createChildSceneNode();
			MeshPtr mesh = MeshManager::getSingleton().create("treasure.3ds");
			Entity* entity = World::getSingleton().getSceneManager()->createEntity("A", mesh->getName());
			entity->setMeshLodBias(200*200);
			entity->setCastShadows(true);
			sceneNode->attachObject(entity);
			sceneNode->showBoundingBox(true);
		*/
			// test-3 temporary skies
		/*	SkyCycle *G1Cycle = mSkySystem->addCycle("Gothic_1");
			mSkySystem->addSky(G1Cycle, "Sky_0", 0, 10, ColourValue(0.4705, 0.549, 0.70588), "SKYDAY_LAYER1_A0.TGA"); //0, 0
			mSkySystem->addSky(G1Cycle, "Sky_1", 0, 20, ColourValue(0.4705, 0.549, 0.70588), "SKYDAY_LAYER1_A0.TGA"); //6, 0
			mSkySystem->addSky(G1Cycle, "Sky_2", 0, 30, ColourValue(0.70588, 0.2941, 0.23529), "SKYDAY_LAYER1_A0.TGA"); //7, 12
			mSkySystem->addSky(G1Cycle, "Sky_3", 0, 40, ColourValue(0.0784, 0.0784, 0.23529), "SKYNIGHT_LAYER0_A0.TGA"); //8, 24
			mSkySystem->addSky(G1Cycle, "Sky_4", 0, 50, ColourValue(0.0196, 0.0196, 0.23529), "SKYNIGHT_LAYER0_A0.TGA"); //12, 0
			mSkySystem->addSky(G1Cycle, "Sky_5", 0, 60, ColourValue(0.0196, 0.0196, 0.23529), "SKYDAY_LAYER0_A0.TGA"); //15, 36
			mSkySystem->addSky(G1Cycle, "Sky_6", 0, 70, ColourValue(0.3137, 0.23529, 0.41176), "SKYDAY_LAYER0_A0.TGA"); //16, 48
			mSkySystem->addSky(G1Cycle, "Sky_7", 0, 80, ColourValue(0.4705, 0.549, 0.70588), "SKYDAY_LAYER1_A0.TGA"); //18, 0

			mSkySystem->setActiveCycle(G1Cycle);
		*/
			
			// test-4 lights and shadows
			//Ogre::SceneManager* mgr = World::getSingleton().getSceneManager();
			//Ogre::Light* light = mgr->createLight();			
		}
	};
}

#endif // GOTHOGRE_APP_BLOCK_TEST_H
