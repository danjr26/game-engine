#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <vector>
#include "window.h"

class WindowManager {
private:
	std::vector<Window*> mWindows;

public:
	WindowManager();
	void Add(Window* in_window);
	void Remove(Window* in_window);
	Window* Get(HWND in_handle);
};

#endif