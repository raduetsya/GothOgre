	void App::shutdownOgre()
	{
			...
	
			// Check number of alive listeners.
			size_t numFrameListeners = FrameEventUtilities::getNumFrameListeners();
			size_t numWindowEventListeners = WindowEventUtilities::getNumWindowEventListeners();
			if(numFrameListeners || numWindowEventListeners)
			{
				GOTHOGRE_LOG(Warning, "Ogre: Not all listeners were unregistered!");
				
				if(numFrameListeners)
					GOTHOGRE_LOG(Warning, "Ogre: Number of lost FrameListeners = " << numFrameListeners);

				if(numWindowEventListeners)
					GOTHOGRE_LOG(Warning, "Ogre: Number of lost WindowEventListeners = " << numWindowEventListeners);
			}
			
			...
	}