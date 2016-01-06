
#include <junior>
#include <Windows.h>


int __stdcall wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	junior::window main(L"Junior Drawing");

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
