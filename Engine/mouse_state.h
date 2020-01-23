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
	bool isDown(MouseButton i_button);
	void setButton(MouseButton i_button, bool i_value);
	void processRawEvent(RawInputEvent i_event);
};

#endif