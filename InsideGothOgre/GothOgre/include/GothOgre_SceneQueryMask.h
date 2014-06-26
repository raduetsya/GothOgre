#ifndef GOTHOGRE_SCENE_QUERY_MASK_H
#define GOTHOGRE_SCENE_QUERY_MASK_H


namespace GothOgre
{
	struct GOTHOGRE_EXPORT SceneQueryMask
	{
		enum 
		{
			// Objects which are invisible in the game mode,
			// however they can be shown in the editor mode.
			INVISIBLE_LIGHT     = 0x00000001,
			INVISIBLE_SOUND     = 0x00000002,
			INVISIBLE_CAMERA    = 0x00000004,
			INVISIBLE_WAYPOINT  = 0x00000008,
			INVISIBLE_WAYNET    = 0x00000010,
			INVISIBLE_SKY       = 0x00000020,
			INVISIBLE_IN_GAME   = 0x0000FFFF,

			/// Vob's visual
			VOB                 = 0x00010000,

			/// Compiled world's visual
			LEVEL_BSP           = 0x00020000,

			/// Sky (celestial sphere, celestial bodies, clouds, and weather effects).
			SKY                 = 0x00040000,
			
			/// Active tool's drawing: a crosshair, a pivot for moving objects, etc.,
			/// These objects are drawn but they normally doesn't participate in the collision detections.
			ACTIVE_TOOL_DRAWING = 0x80000000
		};
	};

} // namespace GothOgre

#endif // GOTHOGRE_SCENE_QUERY_MASK_H