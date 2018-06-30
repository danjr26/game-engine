#ifndef KEYBOARD_RANGE_IDENTIFIER_H
#define KEYBOARD_RANGE_IDENTIFIER_H

#include "input_identifiers.h"
#include "key.h"

class KeyboardRangeIdentifier {
private:
	Key lowKey;
	Key highKey;
	float lowValue;
	float midValue;
	float highValue;
	bool acceptRepeat;

public:
	KeyboardRangeIdentifier(
		Key in_lowKey, Key in_highKey, 
		float in_lowValue = -1.0f, float in_midValue = 0.0f, 
		float in_highValue = 1.0f, bool in_acceptRepeat = false
	);
	float operator()(const RawInputEvent& in_event, const RawInputState& in_state) const;
};

#endif