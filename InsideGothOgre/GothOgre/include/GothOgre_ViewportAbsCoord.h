#ifndef GOTHOGRE_VIEWPORT_ABS_COORD_H
#define GOTHOGRE_VIEWPORT_ABS_COORD_H


namespace GothOgre
{
	/** This class represents absolute coordinates of a viewport, i.e.
	coordinates of a viewport relative to the render window's top-left corner. 
	These coordinates is always relative to the render window's top-left corner
	even when the viewport uses rendering to another render target.	*/
	class ViewportAbsCoord
	{
	public:
		ViewportAbsCoord() {}
		ViewportAbsCoord(int _left, int _top, int _width, int _height) : mLeft(_left), mTop(_top), mWidth(_width), mHeight(_height) {}

		void setLeft(int _left) {mLeft = _left;}
		void setTop(int _top) {mTop = _top;}
		void setWidth(int _width) {mWidth = _width;}
		void setHeight(int _height) {mHeight = _height;}

		int getLeft() const {return mLeft;}
		int getTop() const {return mTop;}
		int getWidth() const {return mWidth;}
		int getHeight() const {return mHeight;}

	private:
		int mLeft;
		int mTop;
		int mWidth;
		int mHeight;
	};


} // namespace GothOgre

#endif // GOTHOGRE_VIEWPORT_ABS_COORD_H


