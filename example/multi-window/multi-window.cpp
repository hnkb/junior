
#include <junior.h>
#include <Windows.h>

using namespace junior;


int __stdcall wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	window a(L"window 1"), b(L"window 2");
	window c = a;

	c.write(L"This was a copy of window 1.\nIt will still respond to click events like the other windows (because the event handling code does not filter which window was the source of the event), but when you close this window, no message will be displayed on the other two windows, sorry!\nclick!\nclick!");
	c.write(L"سلام بر شما دوستان\nدر اینجا میخواهیم یک پاراگراف متن فارسی (Persian) بنویسیم ببینیم که آیا دوست عزیزمان DirectWrite با کمک دوست عزیزش Direct2D توانایی تحریر و نمایش صحیح آن را دارد و یا خیر. البته در حال حاضر به جای تحلیل کل متن فقط کاراکتر اول آن چک می‌شود؛ شاید هم همین بهترین کار باشد؟ و من الله توفیق،\nهانی!");

	static int x = 600, y = 400;

	while (auto e = wait_for_event())
	{
		if (e == event_type::mouse_lbutton_down)
			e.window->begin_draw().write(L"click!\n").fill_circle(x -= 30, y -= 20, 4).end_draw();

		if (e == event_type::window_destroy)
		{
			if (e.window == &a)
				b.write(L"\nwindow 1 closed!\n\n");
			else if (e.window == &b)
				a.write(L"\nwindow 2 closed!\n\n");
		}
	}
	
	return 0;
}
