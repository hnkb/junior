
#include "junior-simple.h"
#include <vector>
#include <Windows.h>


namespace junior
{
	std::vector<window> windows;

	window& create_window(const wchar_t* title)
	{
		ShowWindow(GetConsoleWindow(), SW_HIDE);
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

	void fill_circle(const int x, const int y, const int radius)
	{
		main_window().fill_circle(x, y, radius);
	}

	void write(const wchar_t* text, const int x, const int y)
	{
		main_window().write(text, x, y);
	}

	void write(const wchar_t* text)
	{
		main_window().write(text);
	}
}
