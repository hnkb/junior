
#include <junior.h>
#include <Windows.h>

using namespace junior;


int __stdcall wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	window a(L"window 1"), b(L"window 2");
	window c = a;

	while (auto e = wait_for_event())
	{
		if (e == event_type::mouse_lbutton_down)
			e.window->write(L"click!");

		if (e == event_type::window_destroy)
		{
			if (e.window == &a)
				b.write(L"window 1 closed!");
			else if (e.window == &b)
				a.write(L"window 2 closed!");
		}
	}
	
	return 0;
}
