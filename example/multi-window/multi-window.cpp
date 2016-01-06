
#include <junior>
#include <Windows.h>

using namespace junior;


int __stdcall wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	window a(L"window 1"), b(L"window 2");

	while (auto e = wait_for_event())
	{
		if (e == event_type::mouse_lbutton_down)
			e.window->write(L"click!");

		if (e == event_type::window_destroy)
		{
			MessageBox(0, L"Window closed!", 0, 0);
			if (e.window == &a)
				b.write(L"window 1 closed!");
			else
				a.write(L"window 2 closed!");
		}
	}
	
	return 0;
}
