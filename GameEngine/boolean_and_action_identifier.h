#ifndef BOOLEAN_AND_ACTION_IDENTIFIER_H
#define BOOLEAN_AND_ACTION_IDENTIFIER_H

#include "input_identifiers.h"
#include "definitions.h"

template<uint n>
struct BooleanAndActionIdentifier {
	InputActionIdentifier identifiers[n];

	template<typename... Args, typename = std::enable_if_t<sizeof...(Args) == n>>
	BooleanAndActionIdentifier(Args&... in_members) :
		identifiers(in_members...) {}

	bool operator()(const RawInputEvent& in_event, const RawInputState& in_state) const {
		for (uint i = 0; i < n; i++) {
			if (!identifiers[i](in_event, in_state)) {
				return false;
			}
		}
		return true;
	}
};

#endif