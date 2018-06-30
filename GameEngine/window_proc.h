#ifndef WINDOW_PROC_H
#define WINDOW_PROC_H

#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND in_hwnd, UINT in_message, WPARAM in_wParam, LPARAM in_lParam);

#endif