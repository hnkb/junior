
#include "window.h"

using namespace junior;


window::window(const wchar_t* title) : _hwnd(nullptr)
{
	_create(title);
}

window::~window()
{
	DestroyWindow(_hwnd);
}


void window::_create(const wchar_t* title)
{
	WNDCLASSEXW wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hInstance = GetModuleHandleW(nullptr);
	wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = L"junior_window";

	wcex.lpfnWndProc = [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
		auto target = (window*)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
		return target ? target->_proc(msg, wp, lp) : DefWindowProcW(hwnd, msg, wp, lp);
	};

	RegisterClassExW(&wcex);


	_hwnd = CreateWindowExW(0, wcex.lpszClassName, title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, GetModuleHandleW(nullptr), nullptr);


	if (_hwnd)
	{
		SetWindowLongPtrW(_hwnd, GWLP_USERDATA, (LONG_PTR)this);
		ShowWindow(_hwnd, SW_SHOWDEFAULT);
		UpdateWindow(_hwnd);
	}
}

LRESULT window::_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProcW(_hwnd, msg, wp, lp);
}


void window::draw_line(const int x1, const int y1, const int x2, const int y2)
{
	auto hdc = GetDC(_hwnd);
	MoveToEx(hdc, x1, y1, nullptr);
	LineTo(hdc, x2, y2);
}

void window::write(const wchar_t* text, const int x, const int y)
{
	auto hdc = GetDC(_hwnd);
	TextOutW(hdc, x, y, text, lstrlenW(text));
}
