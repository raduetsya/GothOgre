#ifndef GOTHOGRE_FRAME_EVENT_UTILITIES_H
#define GOTHOGRE_FRAME_EVENT_UTILITIES_H


namespace GothOgre
{
	class GOTHOGRE_EXPORT FrameEventUtilities
	{
	public:
		static void addFrameListener( FrameListener* _listener );
		static void removeFrameListener( FrameListener* _listener );
		static size_t getNumFrameListeners();
	};

} // namespace GothOgre


#define GOTHOGRE_ADD_FRAME_LISTENER(frameListener) \
	do { \
		GOTHOGRE_LISTENER_LOG(Info, "Ogre: Registering an FrameListener"); \
		FrameEventUtilities::addFrameListener(frameListener); \
	} while(false)

#define GOTHOGRE_REMOVE_FRAME_LISTENER(frameListener) \
	do { \
		GOTHOGRE_LISTENER_LOG(Info, "Ogre: Unregistering an FrameListener"); \
		FrameEventUtilities::removeFrameListener(frameListener); \
	} while(false)


#endif // GOTHOGRE_FRAME_EVENT_UTILITIES_H