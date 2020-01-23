#ifndef KEYBOARD_RANGE_IDENTIFIER_H
#define KEYBOARD_RANGE_IDENTIFIER_H

#include "input_identifiers.h"
#include "key.h"

class KeyboardRangeIdentifier {
private:
	Key mLowKey;
	Key mHighKey;
	float mLowValue;
	float mMidValue;
	float mHighValue;
	bool mAcceptRepeat;

public:
	KeyboardRangeIdentifier(
		Key i_lowKey, Key i_highKey, 
		float i_lowValue = -1.0f, float i_midValue = 0.0f, 
		float i_highValue = 1.0f, bool i_acceptRepeat = false
	);
	float operator()(const RawInputEvent& i_event, const RawInputState& i_state) const;
};

#endif