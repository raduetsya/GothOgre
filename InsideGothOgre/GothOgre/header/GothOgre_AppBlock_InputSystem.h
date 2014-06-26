#ifndef GOTHOGRE_APP_BLOCK_INPUT_SYSTEM_H
#define GOTHOGRE_APP_BLOCK_INPUT_SYSTEM_H

#include "GothOgre_AppBlock.h
#include "GothOgre_InputSystem.h"
#include "GothOgre_InputManagerEnumerator.h"
#include "GothOgre_DefaultInputManager.h"


namespace GothOgre
{

	class InputSystemAppBlock : public AppBlock
	{
	public:
		InputSystemAppBlock()
			: mInputSystem(nullptr), mInputManagerEnumerator(nullptr),
			  mDefaultInputManagerFactory(nullptr), mInputManager(nullptr)
		{
			mInputSystem = new InputSystem;
			mInputManagerEnumerator = new InputManagerEnumerator;
			mDefaultInputManagerFactory = new DefaultInputManagerFactory;			
		}

		~InputSystemAppBlock()
		{
			if(mDefaultInputManagerFactory)
			{
				delete mDefaultInputManagerFactory;
				mDefaultInputManagerFactory = nullptr;
			}
			if(mInputManagerEnumerator)
			{
				delete mInputManagerEnumerator;
				mInputManagerEnumerator = nullptr;
			}
			if(mInputSystem)
			{
				delete mInputSystem;
				mInputSystem = nullptr;
			}
		}

		void initialise()
		{
			RenderWindow* window = Ogre::Root::getSingleton().getAutoCreatedWindow();
			mInputManager = mInputManagerEnumerator->createInputManager(window);
		}

		void shutdown()
		{
			mInputManagerEnumerator->destroyInputManager(mInputManager);
			mInputManager = nullptr;
		}

	private:
		InputManagerEnumerator*       mInputManagerEnumerator;
		DefaultInputManagerFactory*   mDefaultInputManagerFactory;
		InputSystem*                  mInputSystem;
		InputManager*                 mInputManager;
	};

}

#endif // GOTHOGRE_APP_BLOCK_INPUT_SYSTEM_H