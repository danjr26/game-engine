#ifndef WINDOWS_ERROR_H
#define WINDOWS_ERROR_H

#include <string>
#include <functional>
#include <windows.h>
#undef min
#undef max

using WindowsProcFunc = std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>;
std::string getWindowsErrorMessage();

#endif