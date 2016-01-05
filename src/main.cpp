
#include <Windows.h>
#include "junior"


int __stdcall wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	junior::window main(L"Junior test");

	while (auto e = junior::wait_for_event())
	{
		if (e == EVENT_MOUSEMOVE && e.mouse.is_button_pressed.left)
		{
			e.target->draw_circle(e.mouse.x, e.mouse.y, 2);
		}

		if (e == EVENT_KEYDOWN && e.key.code == VK_ESCAPE)
		{
			break;
		}
	}

	return 0;
}
