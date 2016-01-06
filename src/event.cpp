
#include "event.h"
#include <map>
#include <Windows.h>


namespace junior
{
	namespace _internal
	{
		mouse_event _mouse_event_from_msg(MSG msg)
		{
			mouse_event m(msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST);

			m.x = LOWORD(msg.lParam);
			m.y = HIWORD(msg.lParam);

			m.is_button_pressed.left = (msg.wParam & MK_LBUTTON) != 0;
			m.is_button_pressed.middle = (msg.wParam & MK_MBUTTON) != 0;
			m.is_button_pressed.right = (msg.wParam & MK_RBUTTON) != 0;
			m.is_button_pressed.control = (msg.wParam & MK_CONTROL) != 0;
			m.is_button_pressed.shift = (msg.wParam & MK_SHIFT) != 0;

			return m;
		}

		keyboard_event _keyboard_event_from_msg(MSG msg)
		{
			keyboard_event k(msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST);
			k.code = (int)msg.wParam;
			return k;
		}

		event _event_from_msg(MSG msg)
		{
			static std::map<UINT, event_type> mapped_events({
				{ WM_QUIT, event_type::quit },
				{ WM_KEYDOWN, event_type::key_down }, { WM_KEYUP, event_type::key_up },
				{ WM_MOUSEMOVE, event_type::mouse_move },
				{ WM_LBUTTONDOWN, event_type::mouse_lbutton_down },
			});

			return event(mapped_events.count(msg.message) ? mapped_events.at(msg.message) : event_type::invalid,
				(window*)GetWindowLongPtrW(msg.hwnd, GWLP_USERDATA),
				mouse_event(_mouse_event_from_msg(msg)), keyboard_event(_keyboard_event_from_msg(msg)));
		}
	}


	event junior::wait_for_event()
	{
		MSG msg;

		while (GetMessageW(&msg, nullptr, 0, 0))
		{
			DispatchMessageW(&msg);

			auto e = _internal::_event_from_msg(msg);
			if (e) return e;
		}

		return event(event_type::quit);
	}
}
