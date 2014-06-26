#ifndef GOTHOGRE_TEST_APP_BLOCK_H
#define GOTHOGRE_TEST_APP_BLOCK_H

#include "GothOgre_AppBlock.h"
#include "GothOgre_ZenArchive.h"
#include "GothOgre_ResourceGroup.h"
#include "GothOgre_ViewportManager.h"

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

			SceneManager* mgr = World::getSingleton().getSceneManager();
			Ogre::Viewport* vp = ViewportManager::getSingleton().getMainViewport();
	
			// set background and some fog
			vp->setBackgroundColour(ColourValue(0.0f, 0.0f, 0.0f));
			mgr->setFog(Ogre::FOG_NONE);

			// set shadow properties
			mgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);
			//mgr->setShadowTextureSelfShadow(true);
			mgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));
			mgr->setShadowTextureSize(1024);
			mgr->setShadowTextureCount(1);

			// use a small amount of ambient lighting
			mgr->setAmbientLight(ColourValue(0.1f, 0.1f, 0.1f));

			// add a bright light above the scene
			Ogre::Light* light = mgr->createLight();			
			light->setType(Light::LT_POINT);
			light->setPosition(30200, -21600, 6200);
			light->setSpecularColour(ColourValue::White);
		}

		void postinitialise()
		{
			ResourceLocationScanner::getSingleton().scanAll();
			ZenArchive za;
			za.readHeader("NewWorld_Part_Xardas_01_Uncompiled.zen", ResourceGroup::WORLDS);
			za.readObject();
			za.readFooter();
		}
	};
}

#endif // GOTHOGRE_TEST_APP_BLOCK_H
