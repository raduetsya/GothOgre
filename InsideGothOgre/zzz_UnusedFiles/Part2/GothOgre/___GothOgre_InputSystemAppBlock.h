#ifndef GOTHOGRE_INPUT_SYSTEM_APP_BLOCK_H
#define GOTHOGRE_INPUT_SYSTEM_APP_BLOCK_H

#include "GothOgre_AppBlock.h"
#include "GothOgre_InputSystem.h"


namespace GothOgre
{

	class InputSystemAppBlock : public AppBlock
	{
	public:
		InputSystemAppBlock()
			: mInputSupportRegistration(nullptr), mInputSystem(nullptr)
		{
			mInputSupportRegistration = new InputSupportRegistration;
			mInputSystem = new InputSystem;
		}

		~InputSystemAppBlock()
		{
			if(mInputSupportRegistration)
			{
				delete mInputSupportRegistration;
				mInputSupportRegistration = nullptr;
			}
			if(mInputSystem)
			{
				delete mDefaultInputManagerFactory;
				mDefaultInputManagerFactory = nullptr;
			}
		}

		void initialise()
		{
			RenderWindow* window = Ogre::Root::getSingleton().getAutoCreatedWindow();
			mInputSystem->initialise(window);
		}

		void shutdown()
		{
			mInputSystem->shutdown();
		}

	private:
		InputSupportRegistration*     mInputSupportRegistration;
		InputSystem*                  mInputSystem;
	};

}

#endif // GOTHOGRE_INPUT_SYSTEM_APP_BLOCK_H