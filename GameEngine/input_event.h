#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H

#include "vector.h"
#include "definitions.h"

class InputContext;

struct InputEvent {
	InputContext* context;
	enum class Type {
		action,
		state_change,
		range_change
	} type;
	uint message;
	union {
		struct {
			float oldValue;
			float newValue;
		} range;
		struct {
			bool oldValue;
			bool newValue;
		} state;
	};
};

#endif