
#include <Windows.h>
#include "window.h"

int __stdcall wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	window main(L"Junior test");

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		DispatchMessage(&msg);
		//DefWindowProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
	}

	return 0;
}
