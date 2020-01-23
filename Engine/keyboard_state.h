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
	bool isDown(Key i_key) const;
	void setKey(Key i_key, bool i_value);
	void processRawEvent(const RawInputEvent& i_event);
};

#endif