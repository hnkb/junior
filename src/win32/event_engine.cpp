
#include "event_engine.h"
#include <map>
#include "window_engine.h"

using namespace junior;
using namespace junior::_win32;


junior::mouse_event event_engine::_mouse_event_from_msg(const MSG& msg)
{
	mouse_event m(msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST);

	if (m)
	{
		m.x = GET_X_LPARAM(msg.lParam);
		m.y = GET_Y_LPARAM(msg.lParam);

		m.is_button_pressed.left = (msg.wParam & MK_LBUTTON) != 0;
		m.is_button_pressed.middle = (msg.wParam & MK_MBUTTON) != 0;
		m.is_button_pressed.right = (msg.wParam & MK_RBUTTON) != 0;
		m.is_button_pressed.control = (msg.wParam & MK_CONTROL) != 0;
		m.is_button_pressed.shift = (msg.wParam & MK_SHIFT) != 0;

		m.wheel_delta = GET_WHEEL_DELTA_WPARAM(msg.wParam);
	}

	return m;
}

keyboard_event event_engine::_keyboard_event_from_msg(const MSG& msg)
{
	keyboard_event k(msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST);
	if (k) k.code = (int)msg.wParam;
	return k;
}

event event_engine::_event_from_msg(const MSG& msg)
{
	if (msg.message == WM_USER + 1)
		return event(event_type::window_destroy, (window*)msg.lParam, mouse_event(), keyboard_event());

	static std::map<UINT, event_type> mapped_events({
		{ WM_QUIT, event_type::quit },
		{ WM_KEYDOWN, event_type::key_down }, { WM_KEYUP, event_type::key_up },
		{ WM_MOUSEMOVE, event_type::mouse_move }, { WM_MOUSEWHEEL, event_type::mouse_wheel },
		{ WM_LBUTTONDOWN, event_type::mouse_lbutton_down }, { WM_LBUTTONUP, event_type::mouse_lbutton_up },
		{ WM_RBUTTONDOWN, event_type::mouse_rbutton_down }, { WM_RBUTTONUP, event_type::mouse_rbutton_up },
		{ WM_MBUTTONDOWN, event_type::mouse_mbutton_down }, { WM_MBUTTONUP, event_type::mouse_mbutton_up },
	});

	return event(mapped_events.count(msg.message) ? mapped_events.at(msg.message) : event_type::invalid,
		window_engine::get_owner(msg.hwnd), mouse_event(_mouse_event_from_msg(msg)), keyboard_event(_keyboard_event_from_msg(msg)));
}

event event_engine::wait_for_event()
{
	MSG msg;

	while (GetMessageW(&msg, nullptr, 0, 0))
	{
		DispatchMessageW(&msg);

		auto e = _event_from_msg(msg);
		if (e) return e;
	}

	return event(event_type::quit);
}
