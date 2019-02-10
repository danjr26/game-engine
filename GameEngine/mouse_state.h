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
	bool Is_Down(MouseButton in_button);
	void Set_Button(MouseButton in_button, bool in_value);
	void Process_Raw_Event(RawInputEvent in_event);
};

#endif