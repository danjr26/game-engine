#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H

#include "vector.h"
#include "definitions.h"

class InputContext;

struct InputEvent {
	InputContext* mContext;
	enum class Type {
		action,
		state_change,
		range_change
	} mType;
	uint mMessage;
	union {
		struct {
			float oldValue;
			float newValue;
		} mRange;
		struct {
			bool oldValue;
			bool newValue;
		} mState;
	};
};

#endif