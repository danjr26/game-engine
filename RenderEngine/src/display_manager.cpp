#include "../include/internal/display_manager.h"

BOOL enumMonitorsCallback(HMONITOR i_monitor, HDC i_dc, LPRECT i_rect, LPARAM i_arg) {
	std::vector<Display>* ptr = (std::vector<Display>*)i_arg;
	ptr->push_back(Display(i_monitor));
	return true;
}

DisplayManager::DisplayManager() :
mDisplays() {
	EnumDisplayMonitors(NULL, NULL, enumMonitorsCallback, (LPARAM)&this->mDisplays);
}

Display* DisplayManager::getPrimaryDisplay() {
	for (auto it = mDisplays.begin(); it != mDisplays.end(); it++) {
		if (it->isPrimary()) return &*it;
	}
	return nullptr;
}

void DisplayManager::getDisplays(std::vector<Display*>& i_displays) {
	for (auto it = mDisplays.begin(); it != mDisplays.end(); it++) {
		i_displays.push_back(&*it);
	}
}
