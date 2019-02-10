#ifndef KEYBOARD_STATE_H
#define KEYBOARD_STATE_H

#include "key.h"
#include "raw_input_event.h"
#include <vector>

class KeyboardState {
private:
	std::vector<bool> mKeyStates;

public:
	KeyboardState();
	bool Is_Down(Key in_key) const;
	void Set_Key(Key in_key, bool in_value);
	void Process_Raw_Event(const RawInputEvent& in_event);
};

#endif