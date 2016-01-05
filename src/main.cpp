
#include <Windows.h>
#include "window.h"

int __stdcall wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	window main(L"Junior test");
	main.draw_line(10, 20, 100, 200);
	main.write(L"magmag", 50, 100);

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		DispatchMessage(&msg);
		//DefWindowProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
	}

	return 0;
}
