#ifndef WORLD_VIEW_WINDOW_H
#define WORLD_VIEW_WINDOW_H


namespace GUISystem
{
	ATTRIBUTE_CLASS_LAYOUT        ( WorldViewWindow, "WorldViewWindow.layout");
	ATTRIBUTE_CLASS_CONFIG_SECTION( WorldViewWindow, "WorldViewWindow");

	class WorldViewWindow : public BaseLayout
	{
	public:
		WorldViewWindow(World* _world);
		~WorldViewWindow();

		World* getWorld() const {return mWorld;}

		void setClosing(bool _closing) {mClosing = _closing;}
		bool isClosing() const {return mClosing;}

		static String generateWindowCaption(World* _world);

	private:
		void notifyPreTextureChanges(MyGUI::Canvas* _canvas);
		void requestUpdateCanvas(MyGUI::Canvas* _canvas, MyGUI::Canvas::Event _event);
		void notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name);
		void notifyWindowChangeCoord(MyGUI::Window* _sender);
		void notifyKeySetFocus(MyGUI::Widget* _sender, MyGUI::Widget* _old);

		GOTHOGRE_DECLARE_WORLD_LOADED_SAVED_ACTIVE_CHANGED(WorldViewWindow);
		GOTHOGRE_DECLARE_WORLD_OBJECT_ADDED_REMOVED_CHANGED(WorldViewWindow);

		void updateCaption();
		void updateInactiveShower();
		void updateViewportAbsCoord();

	private:
		ATTRIBUTE_FIELD_WIDGET_NAME(WorldViewWindow, mCanvas, "canvas");
		MyGUI::Canvas*      mCanvas;

		ATTRIBUTE_FIELD_WIDGET_NAME(WorldViewWindow, mPanelInactiveShower, "panelInactiveShower");
		MyGUI::Widget*      mPanelInactiveShower;

		RenderTexture*      mRenderTarget;
		World*              mWorld;
		Camera*             mCamera;
		Viewport*           mViewport;
		String              mWorldName;
		bool                mWorldChanged;
		bool                mWorldActive;
		bool                mClosing;
	};

} // namespace GUISystem

#endif // WORLD_VIEW_WINDOW_H