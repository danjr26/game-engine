#include "keyboard_range_identifier.h"

KeyboardRangeIdentifier::KeyboardRangeIdentifier(Key in_lowKey, Key in_highKey,
	float in_lowValue, float in_midValue, float in_highValue, bool in_acceptRepeat) :
	lowKey(in_lowKey),
	highKey(in_highKey),
	lowValue(in_lowValue),
	midValue(in_midValue),
	highValue(in_highValue),
	acceptRepeat(in_acceptRepeat)
{}

float KeyboardRangeIdentifier::operator()(const RawInputEvent& in_event, const RawInputState& in_state) const {
	if ((in_event.keyboard.repeat) ? acceptRepeat : true) {
		if (in_event.type == RawInputEvent::Type::key_down) {
			if (in_event.keyboard.key == lowKey) {
				return lowValue;
			}
			if (in_event.keyboard.key == highKey) {
				return highValue;
			}
		}
		if (in_event.type == RawInputEvent::Type::key_up) {
			if (in_event.keyboard.key == lowKey) {
				if (in_state.keyboard.Is_Down(highKey)) {
					return highValue;
				}
				else {
					return midValue;
				}
			}
			if (in_event.keyboard.key == highKey) {
				if (in_state.keyboard.Is_Down(lowKey)) {
					return lowValue;
				}
				else {
					return midValue;
				}
			}
		}
	}
	return NAN;
}
