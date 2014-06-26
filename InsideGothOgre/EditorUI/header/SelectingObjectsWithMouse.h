#ifndef SELECTING_OBJECTS_WITH_MOUSE_H
#define SELECTING_OBJECTS_WITH_MOUSE_H


namespace GUISystem
{
	class SelectingObjectsWithMouse
	{
	public:
		SelectingObjectsWithMouse();
		~SelectingObjectsWithMouse();
	
	private:
		GOTHOGRE_DECLARE_MOUSE_PRESSED( SelectingObjectsWithMouse );
	};
	
} // namespace GUISystem
	
#endif // SELECTING_OBJECTS_WITH_MOUSE_H