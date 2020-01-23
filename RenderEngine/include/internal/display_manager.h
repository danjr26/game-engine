#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include "display.h"

class Window;

class DisplayManager {
private:
	std::vector<Display> mDisplays;

public:
	DisplayManager();

	Display* getPrimaryDisplay();
	void getDisplays(std::vector<Display*>& i_displays);
};

#endif