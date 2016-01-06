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

			HDC get_dc() const { return GetDC(_handle); }

		private:
			LRESULT _window_proc(const UINT msg, const WPARAM wParam, const WPARAM lParam);

			window* _owner;
			HWND _handle;

			static int _window_count;
		};
	}
}
