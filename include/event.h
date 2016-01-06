#pragma once

#include "window.h"


namespace junior
{
	enum class event_type : int
	{
		invalid,
		quit,
		key_down, key_up,
		mouse_move, mouse_wheel,
		mouse_lbutton_down, mouse_lbutton_up,
		mouse_rbutton_down, mouse_rbutton_up,
		mouse_mbutton_down, mouse_mbutton_up,
	};

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
		int wheel_delta;

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
		event() : type(event_type::invalid) {}
		event(const event_type event_type) : type(event_type) {}
		event(const event_type event_type, window* event_window, const mouse_event& event_mouse, const keyboard_event& event_key)
			: type(event_type), window(event_window), mouse(event_mouse), key(event_key) {}

		inline operator bool() const { return type != event_type::invalid && type != event_type::quit; }
		inline bool operator==(const event_type val) const { return type == val; }

		const event_type type;
		window* window;
		const mouse_event mouse;
		const keyboard_event key;
	};


	event wait_for_event();
}
