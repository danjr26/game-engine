#ifndef INPUT_IDENTIFIERS_H
#define INPUT_IDENTIFIERS_H

#include <functional>
#include "raw_input_event.h"
#include "raw_input_state.h"

enum class InputStateChange {
	no_change,
	turn_on,
	turn_off,
	toggle
};

using InputActionIdentifier = std::function<bool(const RawInputEvent&, const RawInputState&)>;
using InputStateIdentifier = std::function<InputStateChange(const RawInputEvent&, const RawInputState&)>;
using InputRangeIdentifier = std::function<float(const RawInputEvent&, const RawInputState&)>;

#endif