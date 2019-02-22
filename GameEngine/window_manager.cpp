#include "window_manager.h"

WindowManager::WindowManager() 
{}

WindowManager::~WindowManager() {
	for (auto it = mWindows.begin(); it != mWindows.end(); it++) {
		delete *it;
	}
}

void WindowManager::add(Window* in_window) {
	mWindows.push_back(in_window);
}

void WindowManager::remove(Window* in_window) {
	auto position = std::find(mWindows.begin(), mWindows.end(), in_window);
	if (position != mWindows.end()) {
		mWindows.erase(position);
	}
}

Window* WindowManager::get(HWND in_handle) {
	for (uint i = 0; i < mWindows.size(); i++) {
		if (mWindows[i]->getHandle() == in_handle) {
			return mWindows[i];
		}
	}
	return nullptr;
}
