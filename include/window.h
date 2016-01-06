#pragma once


namespace junior
{
	class window
	{
	public:
		window(const wchar_t* title);
		window(const window& other);
		window(window&& other);
		window& operator=(const window& other);
		window& operator=(window&& other);
		~window();

		void draw_line(const int x1, const int y1, const int x2, const int y2);
		void draw_circle(const int x, const int y, const int radius);
		void write(const wchar_t* text, const int x, const int y);
		void write(const wchar_t* text);

	private:
		long* _wndproc(const unsigned int msg, const int* wParam, const long* lParam);
		void _create(const wchar_t* title);
		static int _count;

		void* _handle;
		int _cursor_y = 10;
	};
}
