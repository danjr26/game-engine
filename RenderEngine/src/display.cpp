#include "..\include\internal\display.h"

void transfer(const DEVMODEA& i_source, DisplayMode& o_target);
void transfer(const DisplayMode& i_source, DEVMODEA& o_target);

Display::Display(const HMONITOR& i_monitor) :
mMonitor{},
mDevice{} {
	mMonitor.cbSize = sizeof(mMonitor);
	mDevice.cb = sizeof(mDevice);
	if (!GetMonitorInfoA(i_monitor, &mMonitor)) fail();
	if (!EnumDisplayDevicesA(mMonitor.szDevice, 0, &mDevice, 0)) fail();
}

bool Display::isPrimary() const {
	return mDevice.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE;
}

std::string Display::getName() const {
	return mDevice.DeviceName;
}

std::string Display::getDescription() const {
	return mDevice.DeviceString;
}

void Display::getCurrentMode(DisplayMode& o_mode) const {
	DEVMODEA mode = {};
	mode.dmSize = sizeof(mode);
	EnumDisplaySettingsA(mDevice.DeviceName, ENUM_CURRENT_SETTINGS, &mode);
	transfer(mode, o_mode);
}

void Display::getDefaultMode(DisplayMode& o_mode) const {
	DEVMODEA mode = {};
	mode.dmSize = sizeof(mode);
	EnumDisplaySettingsA(mDevice.DeviceName, ENUM_REGISTRY_SETTINGS, &mode);
	transfer(mode, o_mode);
}

void Display::getSupportedModes(std::vector<DisplayMode>& o_modes) const {
	DEVMODEA mode = {};
	mode.dmSize = sizeof(mode);
	for (uint i = 0; EnumDisplaySettingsA(mDevice.DeviceName, i, &mode); i++) {
		DisplayMode outMode;
		transfer(mode, outMode);
		o_modes.push_back(outMode);
		ZeroMemory(&mode, sizeof(mode));
	}
}

void Display::setMode(const DisplayMode& i_mode) {
	DEVMODEA mode = {};
	mode.dmSize = sizeof(mode);
	transfer(i_mode, mode);
	ChangeDisplaySettingsExA(mDevice.DeviceName, &mode, nullptr, CDS_FULLSCREEN, nullptr);
}

void Display::resetMode() {
	ChangeDisplaySettingsExA(mDevice.DeviceName, nullptr, nullptr, CDS_FULLSCREEN, nullptr);
}

void transfer(const DEVMODEA& i_source, DisplayMode& o_target) {
	if (i_source.dmFields & DM_BITSPERPEL) o_target.mNColorBits = i_source.dmBitsPerPel;
	if (i_source.dmFields & DM_PELSWIDTH) o_target.mDimen.x() = i_source.dmPelsWidth;
	if (i_source.dmFields & DM_PELSHEIGHT) o_target.mDimen.y() = i_source.dmPelsHeight;
	if (i_source.dmFields & DM_DISPLAYFREQUENCY) o_target.mFreq = i_source.dmDisplayFrequency;
	if (i_source.dmFields & DM_DISPLAYFIXEDOUTPUT) o_target.mMagnifyMode = DisplayMagnifyMode::fromWin(i_source.dmDisplayFixedOutput);
}

void transfer(const DisplayMode& i_source, DEVMODEA& o_target) { 
	if (i_source.mNColorBits) {
		o_target.dmBitsPerPel = i_source.mNColorBits;
		o_target.dmFields |= DM_BITSPERPEL;
	}
	if (i_source.mDimen.x()) {
		o_target.dmPelsWidth = i_source.mDimen.x();
		o_target.dmFields |= DM_PELSWIDTH;
	}
	if (i_source.mDimen.y()) {
		o_target.dmPelsHeight = i_source.mDimen.y();
		o_target.dmFields |= DM_PELSHEIGHT;
	}
	if (i_source.mFreq) {
		o_target.dmDisplayFrequency = i_source.mFreq;
		o_target.dmFields |= DM_DISPLAYFREQUENCY;
	}
	if (i_source.mMagnifyMode) {
		o_target.dmDisplayFixedOutput = i_source.mMagnifyMode.toWin();
		o_target.dmFields |= DM_DISPLAYFIXEDOUTPUT;
	}
}
