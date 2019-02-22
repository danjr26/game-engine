#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <vector>
#include "window.h"

class WindowManager {
private:
	std::vector<Window*> mWindows;

public:
	WindowManager();
	~WindowManager();

	void add(Window* in_window);
	void remove(Window* in_window);
	Window* get(HWND in_handle);
};

#endif