#ifndef MOUSE_STATE_H
#define MOUSE_STATE_H

#include <vector>
#include "mouse_button.h"
#include "raw_input_event.h"

class MouseState {
private:
	std::vector<bool> mButtons;

public:
	MouseState();
	bool isDown(MouseButton in_button);
	void setButton(MouseButton in_button, bool in_value);
	void processRawEvent(RawInputEvent in_event);
};

#endif