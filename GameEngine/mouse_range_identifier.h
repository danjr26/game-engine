#ifndef MOUSE_RANGE_IDENTIFIER_H
#define MOUSE_RANGE_IDENTIFIER_H

#include "input_identifiers.h"
#include "mouse_button.h"

class MouseRangeIdentifier {
public:
	enum class MouseAxis {
		x = 0,
		y = 1
	};

private:
	MouseAxis axis;
	float lowValue;
	float highValue;

public:
	MouseRangeIdentifier(MouseAxis in_axis, float in_lowValue = 0.0f, float in_highValue = 1.0f);
	float operator()(const RawInputEvent& in_event, const RawInputState& in_state) const;
};

#endif