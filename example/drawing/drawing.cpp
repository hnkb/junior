
#include <junior>
#include <Windows.h>


int __stdcall wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	junior::window main(L"Junior Drawing");

	int px = -1, py = -1;

	while (auto e = junior::wait_for_event())
	{
		if (e == junior::event_type::mouse_lbutton_down)
		{
			px = e.mouse.x;
			py = e.mouse.y;
		}

		if (e == junior::event_type::mouse_lbutton_up)
		{
			px = py = -1;
		}

		if (e == junior::event_type::mouse_move && e.mouse.is_button_pressed.left && px != -1)
		{
			e.window->draw_line(px, py, e.mouse.x, e.mouse.y);
			px = e.mouse.x;
			py = e.mouse.y;
		}

		if (e == junior::event_type::key_down && e.key.code == VK_ESCAPE)
		{
			break;
		}
	}

	return 0;
}
