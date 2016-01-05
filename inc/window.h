#pragma once

#include <string>
#include <Windows.h>


class window
{
public:
	window(const wchar_t* title);
	~window();

private:
	std::wstring _title;
	HWND _hwnd;

	LRESULT _proc(UINT msg, WPARAM wp, LPARAM lp);
	void _create();
};
