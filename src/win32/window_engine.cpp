
#include "window_engine.h"

using namespace junior;
using namespace junior::_win32;


int window_engine::_window_count = 0;


window_engine::window_engine(window* owner, const wchar_t* title, const UINT32 background_color)
	: _owner(owner), _d2d_factory(nullptr), _render_target(nullptr), _background_color(D2D1::ColorF(background_color))
{
	_create_device_independent_resources();
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
		_create_device_resources();
		ShowWindow(_handle, SW_SHOWDEFAULT);
		UpdateWindow(_handle);
	}
}

LRESULT window_engine::_window_proc(const UINT msg, const WPARAM wParam, const WPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
		if (_render_target)
		{
			_render_target->Resize(D2D1::SizeU(LOWORD(lParam), HIWORD(lParam)));
			InvalidateRect(_handle, nullptr, FALSE);
		}
		return 0;

	case WM_DISPLAYCHANGE:
		InvalidateRect(_handle, nullptr, FALSE);
		return 0;

	case WM_PAINT:
		if (_render_target)
		{
			_render_target->BeginDraw();
			_render_target->Clear(_background_color);
			_render_target->EndDraw();
			ValidateRect(_handle, nullptr);
		}
		return 0;

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


HRESULT window_engine::_create_device_independent_resources()
{
	if (CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED) == S_FALSE)
		CoUninitialize();

	return D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_d2d_factory);
}

HRESULT window_engine::_create_device_resources()
{
	if (_render_target) return S_FALSE;
	if (!_handle || !_d2d_factory) return E_NOT_VALID_STATE;

	RECT rc;
	GetClientRect(_handle, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	return _d2d_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(_handle, size), &_render_target);
}

void window_engine::_discard_device_resources()
{
	_render_target = nullptr;
}


bool window_engine::begin_draw()
{
	HRESULT hr = _create_device_resources();
	if (SUCCEEDED(hr)) _render_target->BeginDraw();
	return SUCCEEDED(hr);
}

bool window_engine::end_draw()
{
	HRESULT hr = _render_target->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		_discard_device_resources();
		hr = S_OK;
	}

	return SUCCEEDED(hr);
}


void window_engine::clear(const UINT32 rgb)
{
	// TODO: implement clear()
}

void window_engine::draw_line(const float x1, const float y1, const float x2, const float y2, const UINT32 rgb, const float width)
{
	if (_render_target)
	{
		CComPtr<ID2D1SolidColorBrush> brush = nullptr;
		_render_target->CreateSolidColorBrush(D2D1::ColorF(rgb), &brush);
		_render_target->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), brush, width);
	}
}

void window_engine::draw_ellipse(const float x, const float y, const float rx, const float ry, const UINT32 rgb, const float width)
{
	if (_render_target)
	{
		CComPtr<ID2D1SolidColorBrush> brush = nullptr;
		_render_target->CreateSolidColorBrush(D2D1::ColorF(rgb), &brush);
		_render_target->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), rx, ry), brush, width);
	}
}

void window_engine::fill_ellipse(const float x, const float y, const float rx, const float ry, const UINT32 rgb)
{
	if (_render_target)
	{
		CComPtr<ID2D1SolidColorBrush> brush = nullptr;
		_render_target->CreateSolidColorBrush(D2D1::ColorF(rgb), &brush);
		_render_target->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), rx, ry), brush);
	}
}

void window_engine::write(const wchar_t* text, const float x, const float y, const UINT32 rgb)
{
	//if (!_handle) return;
	//auto hdc = GetDC(_handle);
	//TextOutW(hdc, x, y, text, lstrlenW(text));

	fill_ellipse(x, y, 3, 3, rgb);
}

void window_engine::write(const wchar_t* text, const UINT32 rgb)
{
	write(text, 10, _cursor_y += 20, rgb);
}
