#include "boolean_or_action_identifier.h"

BooleanOrActionIdentifier::BooleanOrActionIdentifier(const InputActionIdentifier& in_identifier1, const InputActionIdentifier& in_identifier2) :
	identifier1(in_identifier1),
	identifier2(in_identifier2)
{}

bool BooleanOrActionIdentifier::operator()(const RawInputEvent& in_event, const RawInputState& in_state) const {
	return identifier1(in_event, in_state) || identifier2(in_event, in_state);
}
