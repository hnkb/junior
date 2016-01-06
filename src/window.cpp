
#include "window.h"
#include <algorithm>
#include <Windows.h>


namespace junior
{
	window::window(const wchar_t* title) : _handle(nullptr)
	{
		_create(title);
	}

	window::window(const window& other) : _handle(nullptr)
	{
		*this = other;
	}

	window::window(window&& other) : _handle(nullptr)
	{
		*this = std::move(other);
	}

	window& window::operator=(const window& other)
	{
		if (this != &other)
		{
			wchar_t buffer[1024];
			GetWindowTextW((HWND)other._handle, buffer, 1023);
			_create(buffer);
		}
		return *this;
	}

	window& window::operator=(window&& other)
	{
		if (this != &other)
		{
			_handle = other._handle;
			other._handle = nullptr;

			if (_handle) SetWindowLongPtrW((HWND)_handle, GWLP_USERDATA, (LONG_PTR)this);
		}
		return *this;
	}

	window::~window()
	{
		if (_handle) DestroyWindow((HWND)_handle);
	}


	int window::_count = 0;

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
			return target ? (LPARAM)target->_wndproc(msg, (int*)wp, (long*)lp) : DefWindowProcW(hwnd, msg, wp, lp);
		};

		RegisterClassExW(&wcex);


		_handle = CreateWindowExW(0, wcex.lpszClassName, title, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, GetModuleHandleW(nullptr), nullptr);


		if (_handle)
		{
			_count++;
			SetWindowLongPtrW((HWND)_handle, GWLP_USERDATA, (LONG_PTR)this);
			ShowWindow((HWND)_handle, SW_SHOWDEFAULT);
			UpdateWindow((HWND)_handle);
		}
	}

	long* window::_wndproc(const unsigned int msg, const int* wParam, const long* lParam)
	{
		switch (msg)
		{
		case WM_DESTROY:
			PostMessageW(nullptr, WM_USER + 1, 0, (LPARAM)this);
			if (!--_count) PostAppMessageW(nullptr, WM_QUIT, 0, 0);// PostQuitMessage(0);
			return 0;
		}
		return (long*)DefWindowProcW((HWND)_handle, msg, (WPARAM)wParam, (LPARAM)lParam);
	}


	void window::draw_line(const int x1, const int y1, const int x2, const int y2)
	{
		if (!_handle) return;
		auto hdc = GetDC((HWND)_handle);
		MoveToEx(hdc, x1, y1, nullptr);
		LineTo(hdc, x2, y2);
	}

	void window::draw_circle(const int x, const int y, const int radius)
	{
		if (!_handle) return;
		auto hdc = GetDC((HWND)_handle);
		Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
	}

	void window::write(const wchar_t* text, const int x, const int y)
	{
		if (!_handle) return;
		auto hdc = GetDC((HWND)_handle);
		TextOutW(hdc, x, y, text, lstrlenW(text));
	}

	void window::write(const wchar_t* text)
	{
		if (!_handle) return;
		auto hdc = GetDC((HWND)_handle);
		TextOutW(hdc, 10, _cursor_y += 20, text, lstrlenW(text));
	}
}
