#include "GUISystem_Precompiled.h"
#include "GUISystem_MyGUITextureUtil.h"

namespace GUISystem
{
	//--------------------------------------------------------------------------
	TexturePtr MyGUITextureUtil::textureFromMyGUI(MyGUI::ITexture* _myGuiTex)
	{
		if(!_myGuiTex)
			return TexturePtr();

		MyGUI::OgreTexture* ogrePlatformTex 
			= static_cast<MyGUI::OgreTexture*>(_myGuiTex);

		TexturePtr texture = ogrePlatformTex->getOgreTexture();
		return texture;
	}

}