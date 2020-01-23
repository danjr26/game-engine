#ifndef DISPLAY_H
#define DISPLAY_H

#include "../../../Geometry/include/geometry.h"
#include "enum_wrappers.h"

struct DisplayMode {
	Vector2ui mDimen;
	uint mFreq;
	uint mNColorBits;
	DisplayMagnifyMode mMagnifyMode;
};

class Display {
friend class DisplayManager;
private:
	MONITORINFOEXA mMonitor;
	DISPLAY_DEVICEA mDevice;

public:
	Display(const HMONITOR& i_monitor);

	bool isPrimary() const;
	std::string getName() const;
	std::string getDescription() const;
	void getCurrentMode(DisplayMode& o_mode) const;
	void getDefaultMode(DisplayMode& o_mode) const;
	void getSupportedModes(std::vector<DisplayMode>& o_modes) const;
	void setMode(const DisplayMode& i_mode);
	void resetMode();
};

#endif