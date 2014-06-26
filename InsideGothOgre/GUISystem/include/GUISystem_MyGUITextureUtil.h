#ifndef GUISYSTEM_MYGUI_TEXTURE_UTIL_H
#define GUISYSTEM_MYGUI_TEXTURE_UTIL_H

namespace GUISystem
{
	struct GUISYSTEM_EXPORT MyGUITextureUtil
	{
		static TexturePtr textureFromMyGUI(MyGUI::ITexture* _texture);
	};

} // namespace GUISystem

#endif // GUISYSTEM_MYGUI_TEXTURE_UTIL_H
