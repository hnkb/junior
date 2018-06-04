
#include "window_engine.h"

using namespace junior;
using namespace junior::_win32;


int window_engine::_window_count = 0;


window_engine::window_engine(window* owner, const wchar_t* title, const UINT32 background_color)
	: _owner(owner), _d2d_factory(nullptr), _dwrite_factory(nullptr), _screen_target(nullptr), _canvas_target(nullptr), _text_format(nullptr), _default_brush(nullptr),
	  _background_color(D2D1::ColorF(background_color)), _text(), _text_margin(20)
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
	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.hInstance = GetModuleHandleW(nullptr);
	wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wcex.lpszClassName = L"junior_window";

	wcex.lpfnWndProc = [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
		auto target = (window_engine*)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
		return target ? target->_window_proc(msg, wp, lp) : DefWindowProcW(hwnd, msg, wp, lp);
	};

	RegisterClassExW(&wcex);

	_handle = CreateWindowExW(0, wcex.lpszClassName, title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, wcex.hInstance, nullptr);

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
		if (_screen_target)
		{
			_resize_canvas(D2D1::SizeF((float)LOWORD(lParam), (float)HIWORD(lParam)));
			_screen_target->Resize(D2D1::SizeU(LOWORD(lParam), HIWORD(lParam)));
			InvalidateRect(_handle, nullptr, FALSE);
		}
		return 0;

	case WM_DISPLAYCHANGE:
		InvalidateRect(_handle, nullptr, FALSE);
		return 0;

	case WM_PAINT:
		_update_screen();
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
	// TODO: use vector and GetWindowTextLength
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

	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_d2d_factory);

	if (SUCCEEDED(hr)) hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(_dwrite_factory), reinterpret_cast<IUnknown**>(&_dwrite_factory));
	
	if (SUCCEEDED(hr)) hr = _dwrite_factory->CreateTextFormat(L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 24.0f, L"", &_text_format);

	return hr;
}

HRESULT window_engine::_create_device_resources()
{
	if (_canvas_target) return S_FALSE;
	if (!_handle || !_d2d_factory) return E_NOT_VALID_STATE;

	RECT rc;
	GetClientRect(_handle, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	HRESULT hr = _d2d_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(_handle, size), &_screen_target);

	if (SUCCEEDED(hr)) hr = _screen_target->CreateCompatibleRenderTarget(&_canvas_target);
	if (SUCCEEDED(hr))
	{
		_canvas_target->BeginDraw();
		_canvas_target->Clear(_background_color);
		hr = _canvas_target->EndDraw();
	}

	if (SUCCEEDED(hr)) hr = _canvas_target->CreateSolidColorBrush(D2D1::ColorF(0), &_default_brush);
	_queued_drawing = false;

	return hr;
}

void window_engine::_discard_device_resources()
{
	_screen_target = nullptr;
	_canvas_target = nullptr;
	_default_brush = nullptr;
	_queued_drawing = false;
}

HRESULT window_engine::_update_screen()
{
	HRESULT hr = _create_device_resources();
	if (FAILED(hr)) return E_NOT_VALID_STATE;

	RECT rc;
	GetUpdateRect(_handle, &rc, FALSE);

	CComPtr<ID2D1Bitmap> canvas;
	hr = _canvas_target->GetBitmap(&canvas);
	if (SUCCEEDED(hr))
	{
		auto rect = D2D1::RectF((float)rc.left, (float)rc.top, (float)rc.right, (float)rc.bottom);


		CComPtr<IDWriteTextLayout> layout;

		if (_text_format && _text.length())
		{
			auto screen_size = _screen_target->GetSize();

			bool is_arabic = _text[0] && ((_text[0] >= 0x0600 && _text[0] <= 0x06ff) || (_text[0] >= 0x08a0 && _text[0] <= 0x08ff) || (_text[0] >= 0xfb50 && _text[0] <= 0xfdff) || (_text[0] >= 0xfe70 && _text[0] <= 0xfeff));
			_text_format->SetReadingDirection(is_arabic ? DWRITE_READING_DIRECTION_RIGHT_TO_LEFT : DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

			if (SUCCEEDED(_dwrite_factory->CreateTextLayout(_text.c_str(), (UINT32)_text.length(), _text_format, screen_size.width - 2 * _text_margin, screen_size.height - 2 * _text_margin, &layout)))
			{
				//DWRITE_TEXT_METRICS metrics;
				//layout->GetMetrics(&metrics);
				//if (metrics.height > (screen_size.height - 2 * margin)) MessageBox(_handle, L"Scrollbars needed!", 0, 0);

				// TODO: here, we must merge output of GetUpdateRect() with rect around layout of each paragraph that needs re-drawing,
				//       because DrawTextLayout() doesn't allow drawing only a portion of layout (instead of whole-layout)
				// For now, invalidate the whole screen when we need to draw text.
				rect = D2D1::RectF(0, 0, screen_size.width, screen_size.height);
			}
		}


		_screen_target->BeginDraw();
		//_screen_target->FillRectangle(rect, _get_brush(_background_color)); // TODO: clear background, not needed when canvas is guaranteed to be opaque
		_screen_target->DrawBitmap(canvas, rect, 1, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, rect);
		if (layout) _screen_target->DrawTextLayout(D2D1::Point2F(_text_margin, _text_margin), layout, _get_brush(0x404040));
		hr = _screen_target->EndDraw();
	}

	if (SUCCEEDED(hr))
	{
		ValidateRect(_handle, &rc);
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		_discard_device_resources();
		hr = S_OK;
	}

	return hr;
}

HRESULT window_engine::_resize_canvas(D2D1_SIZE_F screen_size)
{
	if (!_screen_target || !_canvas_target) return E_NOT_VALID_STATE;

	auto canvas_size = _canvas_target->GetSize();
	if (canvas_size.width >= screen_size.width && canvas_size.height >= screen_size.height) return S_FALSE;
	auto new_size = D2D1::SizeF(max(canvas_size.width, screen_size.width), max(canvas_size.height, screen_size.height));

	bool middle_of_queue = _queued_drawing;
	if (middle_of_queue) end_draw();

	CComPtr<ID2D1BitmapRenderTarget> new_target;
	HRESULT hr = _screen_target->CreateCompatibleRenderTarget(new_size, &new_target);
	if (SUCCEEDED(hr))
	{
		new_target->BeginDraw();
		new_target->Clear(_background_color);
		hr = new_target->EndDraw();
	}

	CComPtr<ID2D1Bitmap> current_canvas;
	CComPtr<ID2D1Bitmap> new_canvas;
	if (SUCCEEDED(hr)) hr = _canvas_target->GetBitmap(&current_canvas);
	if (SUCCEEDED(hr)) hr = new_target->GetBitmap(&new_canvas);
	if (SUCCEEDED(hr)) hr = new_canvas->CopyFromBitmap(nullptr, current_canvas, nullptr);

	if (SUCCEEDED(hr))
	{
		_canvas_target = new_target;

		if (middle_of_queue) hr = begin_draw() ? S_OK : E_FAIL;
	}

	return hr;
}


CComPtr<ID2D1SolidColorBrush> window_engine::_get_brush(const D2D1_COLOR_F& color)
{
	if (!_default_brush) return nullptr;

	_default_brush->SetColor(color);
	return _default_brush;
}

CComPtr<ID2D1SolidColorBrush> window_engine::_get_brush(const UINT32 rgb)
{
	return _get_brush(D2D1::ColorF(rgb, 1.f));
}

bool window_engine::begin_draw()
{
	if (_queued_drawing) return false; // TODO: warn user about begin/end draw mismatch

	HRESULT hr = _create_device_resources();
	if (SUCCEEDED(hr))
	{
		_canvas_target->BeginDraw();
		_queued_drawing = true;
	}
	return SUCCEEDED(hr);
}

bool window_engine::end_draw()
{
	if (!_canvas_target) return false;
	if (!_queued_drawing) return false; // TODO: warn user about begin/end draw mismatch

	HRESULT hr = _canvas_target->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		_discard_device_resources();
		hr = S_OK;
	}

	if (SUCCEEDED(hr))
	{
		InvalidateRect(_handle, nullptr, FALSE);
	}

	_queued_drawing = false;

	return SUCCEEDED(hr);
}


void window_engine::clear(const UINT32 rgb)
{
	// TODO: implement clear()
}

void window_engine::draw_line(const float x1, const float y1, const float x2, const float y2, const UINT32 rgb, const float width)
{
	_draw([&]() {
		_canvas_target->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), _get_brush(rgb), width);
	});
}

void window_engine::draw_ellipse(const float x, const float y, const float rx, const float ry, const UINT32 rgb, const float width)
{
	_draw([&]() {
		_canvas_target->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), rx, ry), _get_brush(rgb), width);
	});
}

void window_engine::fill_ellipse(const float x, const float y, const float rx, const float ry, const UINT32 rgb)
{
	_draw([&]() {
		_canvas_target->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), rx, ry), _get_brush(rgb));
	});
}

void window_engine::write(const wchar_t* text, const float x, const float y, const UINT32 rgb)
{
	_draw([&]() {
		if (_text_format)
		{
			auto target_size = _canvas_target->GetSize();
			auto layout_rect = D2D1::RectF(x, y, target_size.width - 10, target_size.height);

			_text_format->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

			_canvas_target->DrawTextW(text, lstrlen(text), _text_format, layout_rect, _get_brush(rgb));
		}
	});
}

void window_engine::write(const wchar_t* text, const UINT32 rgb)
{
	_text.append(text);
	InvalidateRect(_handle, nullptr, FALSE);
	// TODO: only invalidate portion around changed or added paragraphs
}
