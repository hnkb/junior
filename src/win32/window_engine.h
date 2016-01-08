#pragma once

#include "window.h"
#include <string>
#include <Windows.h>
#include <atlbase.h>
#include <d2d1.h>


namespace junior
{
	namespace _win32
	{
		class window_engine
		{
		public:
			window_engine(window* owner, const wchar_t* title, const UINT32 background_color = 0xffffff);
			~window_engine();

			std::wstring get_title() const;
			void set_owner(window* new_owner);
			static window* get_owner(const HWND hWnd);

		private:
			void _create_window(const wchar_t* title);
			LRESULT _window_proc(const UINT msg, const WPARAM wParam, const WPARAM lParam);

			HRESULT _create_device_independent_resources();
			HRESULT _create_device_resources();
			void _discard_device_resources();

			window* _owner;
			HWND _handle;
			static int _window_count;

			CComPtr<ID2D1HwndRenderTarget> _render_target;
			CComPtr<ID2D1Factory> _d2d_factory;

		public:
			bool begin_draw();
			bool end_draw();

			void clear(const UINT32 rgb);
			
			void draw_line(const float x1, const float y1, const float x2, const float y2, const UINT32 rgb, const float width);
			void draw_ellipse(const float x, const float y, const float rx, const float ry, const UINT32 rgb, const float width);
			void fill_ellipse(const float x, const float y, const float rx, const float ry, const UINT32 rgb);

			void write(const wchar_t* text, const float x, const float y, const UINT32 rgb);
			void write(const wchar_t* text, const UINT32 rgb);

		private:
			D2D1_COLOR_F _background_color;
			float _cursor_y = 10;
		};
	}
}
