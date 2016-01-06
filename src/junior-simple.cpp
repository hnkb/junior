
#include "junior-simple.h"
#include <vector>


namespace junior
{
	std::vector<window> windows;

	window& create_window(const wchar_t* title)
	{
		windows.push_back(window(title));
		return windows.back();
	}

	inline window& main_window()
	{
		return windows.front();
	}

	void draw_line(const int x1, const int y1, const int x2, const int y2)
	{
		main_window().draw_line(x1, y1, x2, y2);
	}

	void draw_circle(const int x, const int y, const int radius)
	{
		main_window().draw_circle(x, y, radius);
	}

	void write(const wchar_t* text, const int x, const int y)
	{
		main_window().write(text, x, y);
	}
}
