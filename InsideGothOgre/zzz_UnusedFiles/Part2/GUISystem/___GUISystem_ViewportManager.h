#ifndef GUISYSTEM_VIEWPORT_MANAGER_H
#define GUISYSTEM_VIEWPORT_MANAGER_H

namespace GUISystem
{
	/** Viewport manager stores pointers to some used viewports. */
	class GUISYSTEM_EXPORT ViewportManager : public Singleton<ViewportManager>
	{
	public:
		ViewportManager();
		~ViewportManager();

		void setBackgroundViewport(Viewport* _viewport) {mBackgroundViewport = _viewport;}
		Viewport* getBackgroundViewport() const {return mBackgroundViewport;}

		void setUIViewport(Viewport* _viewport) {mUIViewport = _viewport;}
		Viewport* getUIViewport() const {return mUIViewport;}

	private:
		Viewport*       mBackgroundViewport;
		Viewport*       mUIViewport;
	};

} // namespace GUISystem


#endif // GUISYSTEM_VIEWPORT_MANAGER_H