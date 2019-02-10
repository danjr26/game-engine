#ifndef BOOLEAN_OR_ACTION_IDENTIFIER_H
#define BOOLEAN_OR_ACTION_IDENTIFIER_H

#include "input_identifiers.h"
#include "definitions.h"

template<uint n>
struct BooleanOrActionIdentifier {
	InputActionIdentifier mIdentifiers[n];

	template<typename... Args, typename = std::enable_if_t<sizeof...(Args) == n>>
	BooleanOrActionIdentifier(Args&... in_members) :
		mIdentifiers(in_members...)
	{}

	bool operator()(const RawInputEvent& in_event, const RawInputState& in_state) const {
		for (uint i = 0; i < n; i++) {
			if (mIdentifiers[i](in_event, in_state)) {
				return true;
			}
		}
		return false;
	}
};

#endif