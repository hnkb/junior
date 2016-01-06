#pragma once

#include "window.h"


namespace junior
{
#define EVENT_QUIT 1
#define EVENT_KEYDOWN 2
#define EVENT_LBUTTONDOWN 3
#define EVENT_MOUSEMOVE 4


	struct mouse_event
	{
	public:
		mouse_event() : _valid(false) {}
		mouse_event(bool valid) : _valid(valid) {}
		inline operator bool() const { return _valid; }

		int x;
		int y;
		struct button_pressed
		{
			bool left;
			bool middle;
			bool right;
			bool control;
			bool shift;
		} is_button_pressed;

	private:
		bool _valid;
	};

	struct keyboard_event
	{
	public:
		keyboard_event() : _valid(false) {}
		keyboard_event(bool valid) : _valid(valid) {}
		
		int code;

	private:
		bool _valid;
	};


	struct event
	{
	public:
		event() : type(0) {}
		event(const int event_type) : type(event_type) {}
		event(const int event_type, window* event_window, const mouse_event& event_mouse, const keyboard_event& event_key)
			: type(event_type), window(event_window), mouse(event_mouse), key(event_key) {}

		inline operator bool() const { return type && type != EVENT_QUIT; }
		inline bool operator==(const int val) const { return type == val; }

		const int type;
		window* window;
		const mouse_event mouse;
		const keyboard_event key;
	};


	event wait_for_event();
}
