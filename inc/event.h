#pragma once

#include "window.h"
#include <Windows.h>


namespace junior
{
#define EVENT_QUIT 1
#define EVENT_KEYDOWN 2
#define EVENT_LBUTTONDOWN 3
#define EVENT_MOUSEMOVE 4

	struct mouse_event
	{
	public:
		int x;
		int y;
		struct button_pressed
		{
			bool left;
			button_pressed() : left(false) {}
			button_pressed(WPARAM wParam) : left(wParam & MK_LBUTTON) {}
		} is_button_pressed;

		mouse_event() :x(0), y(0) {}
		mouse_event(MSG msg) : x(LOWORD(msg.lParam)), y(HIWORD(msg.lParam)), is_button_pressed(msg.wParam) {}
	};

	struct keyboard_event
	{
	public:
		int code;

		keyboard_event() : code(0) {}
		keyboard_event(MSG msg) : code((int)msg.wParam) {}
	};

	struct event
	{
	public:
		event() : _action(0) {}
		event(int action) : _action(action) {}
		event(MSG msg);

		operator bool() const { return _action && _action != EVENT_QUIT; }
		bool operator==(const int val) const { return _action == val; }

		window* target;
		const mouse_event mouse;
		const keyboard_event key;

	private:
		int _action;
	};

	event wait_for_event();
}
