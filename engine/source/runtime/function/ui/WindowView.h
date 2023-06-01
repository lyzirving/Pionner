#ifndef __WINDOW_VIEW_H__
#define __WINDOW_VIEW_H__

namespace Pionner
{
	class WindowView
	{
	public:
		WindowView() {};
		virtual ~WindowView() = default;

		virtual void draw() = 0;
	};
}

#endif