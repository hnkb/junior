
#include "window.h"
#include <algorithm>

using namespace junior;


window::window(const wchar_t* title) : _hwnd(nullptr)
{
	_create(title);
}

window::window(const window& other) : _hwnd(nullptr)
{
	*this = other;
}

window::window(window&& other) : _hwnd(nullptr)
{
	*this = std::move(other);
}

window& window::operator=(const window& other)
{
	if (this != &other)
	{
		wchar_t buffer[1024];
		GetWindowTextW(other._hwnd, buffer, 1023);
		_create(buffer);
	}
	return *this;
}

window& window::operator=(window&& other)
{
	if (this != &other)
	{
		_hwnd = other._hwnd;
		other._hwnd = nullptr;

		if (_hwnd) SetWindowLongPtrW(_hwnd, GWLP_USERDATA, (LONG_PTR)this);
	}
	return *this;
}

window::~window()
{
	if (_hwnd) DestroyWindow(_hwnd);
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
	if (!_hwnd) return;
	auto hdc = GetDC(_hwnd);
	MoveToEx(hdc, x1, y1, nullptr);
	LineTo(hdc, x2, y2);
}

void window::draw_circle(const int x, const int y, const int radius)
{
	if (!_hwnd) return;
	auto hdc = GetDC(_hwnd);
	Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
}

void window::write(const wchar_t* text, const int x, const int y)
{
	if (!_hwnd) return;
	auto hdc = GetDC(_hwnd);
	TextOutW(hdc, x, y, text, lstrlenW(text));
}
