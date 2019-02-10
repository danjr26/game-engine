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
	bool isDown(Key in_key) const;
	void setKey(Key in_key, bool in_value);
	void processRawEvent(const RawInputEvent& in_event);
};

#endif