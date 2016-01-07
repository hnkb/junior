
#include "window_engine.h"


using namespace junior;
using namespace junior::_win32;


int window_engine::_window_count = 0;


window_engine::window_engine(window* owner, const wchar_t* title) : _owner(owner)
{
	_create_window(title);
}

window_engine::~window_engine()
{
	if (_handle) DestroyWindow(_handle);
}


void window_engine::_create_window(const wchar_t* title)
{
	WNDCLASSEXW wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hInstance = GetModuleHandleW(nullptr);
	wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wcex.lpszClassName = L"junior_window";

	wcex.lpfnWndProc = [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
		auto target = (window_engine*)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
		return target ? target->_window_proc(msg, wp, lp) : DefWindowProcW(hwnd, msg, wp, lp);
	};

	RegisterClassExW(&wcex);

	_handle = CreateWindowExW(0, wcex.lpszClassName, title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, GetModuleHandleW(nullptr), nullptr);

	if (_handle)
	{
		_window_count++;
		SetWindowLongPtrW(_handle, GWLP_USERDATA, (LONG_PTR)this);
		ShowWindow(_handle, SW_SHOWDEFAULT);
		UpdateWindow(_handle);
	}
}

LRESULT window_engine::_window_proc(const UINT msg, const WPARAM wParam, const WPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostMessageW(nullptr, WM_USER + 1, 0, (LPARAM)_owner);
		if (!--_window_count) PostQuitMessage(0);
		return 0;
	}
	return DefWindowProcW(_handle, msg, wParam, lParam);
}


std::wstring window_engine::get_title() const
{
	wchar_t buffer[1024];
	GetWindowTextW(_handle, buffer, 1024);
	return std::wstring(buffer);
}

void window_engine::set_owner(window* new_owner)
{
	_owner = new_owner;
}

window* window_engine::get_owner(const HWND hWnd)
{
	auto target = (window_engine*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
	return target ? target->_owner : nullptr;
}
