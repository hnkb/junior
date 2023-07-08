#pragma once

#define UNICODE 1
#define WIN32_LEAN_AND_MEAN 1

#define _WIN32_WINNT 0x0601  /* Windows 7 */
#include <sdkddkver.h>

#include <windows.h>
#include <windowsx.h>
#include <atlbase.h>

#include <d2d1.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
