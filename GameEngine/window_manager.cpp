#include "window_manager.h"

WindowManager::WindowManager() 
{}

void WindowManager::Add(Window* in_window) {
	mWindows.push_back(in_window);
}

void WindowManager::Remove(Window* in_window) {
	auto position = std::find(mWindows.begin(), mWindows.end(), in_window);
	if (position != mWindows.end()) {
		mWindows.erase(position);
	}
}

Window* WindowManager::Get(HWND in_handle) {
	for (uint i = 0; i < mWindows.size(); i++) {
		if (mWindows[i]->Get_Handle() == in_handle) {
			return mWindows[i];
		}
	}
	return nullptr;
}
