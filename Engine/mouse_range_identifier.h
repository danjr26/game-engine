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
	MouseAxis mAxis;
	float mLowValue;
	float mHighValue;

public:
	MouseRangeIdentifier(MouseAxis i_axis, float i_lowValue = 0.0f, float i_highValue = 1.0f);
	float operator()(const RawInputEvent& i_event, const RawInputState& i_state) const;
};

#endif