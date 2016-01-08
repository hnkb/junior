#pragma once

#include "window.h"
#include <string>
#include <Windows.h>


namespace junior
{
	namespace _win32
	{
		class window_engine
		{
		public:
			window_engine(window* owner, const wchar_t* title);
			~window_engine();

			std::wstring get_title() const;
			void set_owner(window* new_owner);
			static window* get_owner(const HWND hWnd);

		private:
			void _create_window(const wchar_t* title);
			LRESULT _window_proc(const UINT msg, const WPARAM wParam, const WPARAM lParam);

			window* _owner;
			HWND _handle;
			static int _window_count;

		public:
			void clear(const UINT32 rgb);
			
			void draw_line(const float x1, const float y1, const float x2, const float y2, const UINT32 rgb, const float width);
			void draw_ellipse(const float x, const float y, const float rx, const float ry, const UINT32 rgb, const float width);
			void fill_ellipse(const float x, const float y, const float rx, const float ry, const UINT32 rgb);

			void write(const wchar_t* text, const float x, const float y, const UINT32 rgb);
			void write(const wchar_t* text, const UINT32 rgb);

		private:
			float _cursor_y = 10;
		};
	}
}
