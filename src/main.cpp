
#include "junior"
#include <Windows.h>


int __stdcall wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	junior::window main(L"Junior test");

	while (auto e = junior::wait_for_event())
	{
		if (e == junior::event_type::mouse_move && e.mouse.is_button_pressed.left)
		{
			e.window->draw_circle(e.mouse.x, e.mouse.y, 2);
		}

		if (e == junior::event_type::key_down && e.key.code == VK_ESCAPE)
		{
			break;
		}
	}

	return 0;
}

//#include "junior-simple.h"
//
//
//int __stdcall wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
//{
//	create_window(L"Junior simple test");
//
//	write(L"Hello Junior!", 10, 10);
//	draw_circle(500, 300, 40);
//
//	wait_for_event();
//
//	return 0;
//}
