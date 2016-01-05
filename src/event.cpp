
#include "event.h"

using namespace junior;


event::event(MSG msg) : _action(0), mouse(msg), key(msg), target((window*)GetWindowLongPtrW(msg.hwnd, GWLP_USERDATA))
{
	switch (msg.message)
	{
	case WM_QUIT:
		_action = EVENT_QUIT;
		break;
	case WM_KEYDOWN:
		_action = EVENT_KEYDOWN;
		break;
	case WM_LBUTTONDOWN:
		_action = EVENT_LBUTTONDOWN;
		break;
	case WM_MOUSEMOVE:
		_action = EVENT_MOUSEMOVE;
		break;
	}
}

event junior::wait_for_event()
{
	MSG msg;

	while (GetMessageW(&msg, nullptr, 0, 0))
	{
		DispatchMessageW(&msg);

		event e(msg);
		if (e) return e;
	}

	return event(EVENT_QUIT);
}
