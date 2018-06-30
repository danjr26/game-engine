#include "window_manager.h"

WindowManager::WindowManager() 
{}

void WindowManager::Add(Window* in_window) {
	windows.push_back(in_window);
}

void WindowManager::Remove(Window* in_window) {
	auto position = std::find(windows.begin(), windows.end(), in_window);
	if (position != windows.end()) {
		windows.erase(position);
	}
}

Window* WindowManager::Get(HWND in_handle) {
	for (uint i = 0; i < windows.size(); i++) {
		if (windows[i]->Get_Handle() == in_handle) {
			return windows[i];
		}
	}
	return nullptr;
}
