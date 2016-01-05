#pragma once

#include <Windows.h>


namespace junior
{
	class window
	{
	public:
		window(const wchar_t* title);
		~window();

		void draw_line(const int x1, const int y1, const int x2, const int y2);
		void write(const wchar_t* text, const int x, const int y);

	private:
		HWND _hwnd;

		LRESULT _proc(UINT msg, WPARAM wp, LPARAM lp);
		void _create(const wchar_t* title);
	};
}
