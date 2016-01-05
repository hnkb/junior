#pragma once

#include <string>
#include <Windows.h>


class window
{
public:
	window(const wchar_t* title);
	~window();

	void draw_line(const int x1, const int y1, const int x2, const int y2);
	void write(const wchar_t* text, const int x, const int y);

private:
	std::wstring _title;
	HWND _hwnd;

	LRESULT _proc(UINT msg, WPARAM wp, LPARAM lp);
	void _create();
};
