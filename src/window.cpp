
#include "window.h"


window::window(const wchar_t* title) : _hwnd(nullptr), _title(title)
{
	_create();
}

window::~window()
{
	DestroyWindow(_hwnd);
}


void window::_create()
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


	_hwnd = CreateWindowExW(0, wcex.lpszClassName, _title.c_str(), WS_OVERLAPPEDWINDOW,
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
