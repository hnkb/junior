
#include "event.h"
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
			auto wnd = (window*)GetWindowLongPtrW(msg.hwnd, GWLP_USERDATA);

			int type = 0;
			switch (msg.message)
			{
			case WM_QUIT:
				type = EVENT_QUIT;
				break;
			case WM_KEYDOWN:
				type = EVENT_KEYDOWN;
				break;
			case WM_LBUTTONDOWN:
				type = EVENT_LBUTTONDOWN;
				break;
			case WM_MOUSEMOVE:
				type = EVENT_MOUSEMOVE;
				break;
			}

			return event(type, wnd, mouse_event(_mouse_event_from_msg(msg)), keyboard_event(_keyboard_event_from_msg(msg)));
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

		return event(EVENT_QUIT);
	}
}
