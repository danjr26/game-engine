#include "input_bindings.h"
#include <cmath>

InputBindings::InputBindings(uint in_nActions, uint in_nStates, uint in_nRanges) :
	actions(in_nActions),
	states(in_nStates),
	ranges(in_nRanges) {
	
	for (uint i = 0; i < in_nActions; i++) { actions[i] = nullptr; }
	for (uint i = 0; i < in_nStates; i++) { states[i] = nullptr; }
	for (uint i = 0; i < in_nRanges; i++) { ranges[i] = nullptr; }
}

uint InputBindings::Get_Number_Actions() const {
	return (uint)actions.size();
}

uint InputBindings::Get_Number_States() const {
	return (uint)states.size();
}

uint InputBindings::Get_Number_Ranges() const {
	return (uint)ranges.size();
}

void InputBindings::Bind_Action(uint in_slot, InputActionIdentifier in_action) {
	actions[in_slot] = in_action;
}

void InputBindings::Bind_State(uint in_slot, InputStateIdentifier in_state) {
	states[in_slot] = in_state;
}

void InputBindings::Bind_Range(uint in_slot, InputRangeIdentifier in_range) {
	ranges[in_slot] = in_range;
}

void InputBindings::Clear_Action(uint in_slot) {
	actions[in_slot] = nullptr;
}

void InputBindings::Clear_State(uint in_slot) {
	states[in_slot] = nullptr;
}

void InputBindings::Clear_Range(uint in_slot) {
	ranges[in_slot] = nullptr;
}

InputActionIdentifier InputBindings::Get_Bound_Action(uint in_slot) {
	return actions[in_slot];
}

InputStateIdentifier InputBindings::Get_Bound_State(uint in_slot) {
	return states[in_slot];
}

InputRangeIdentifier InputBindings::Get_Bound_Range(uint in_slot) {
	return ranges[in_slot];
}

bool InputBindings::Evaluate_Action(uint in_slot, const RawInputEvent& in_event, const RawInputState& in_state) {
	if (actions[in_slot] == nullptr) {
		return false;
	}
	else {
		return actions[in_slot](in_event, in_state);
	}
}

InputStateChange InputBindings::Evaluate_State(uint in_slot, const RawInputEvent& in_event, const RawInputState& in_state) {
	if (states[in_slot] == nullptr) {
		return InputStateChange::no_change;
	}
	else {
		return states[in_slot](in_event, in_state);
	}
}

float InputBindings::Evaluate_Range(uint in_slot, const RawInputEvent& in_event, const RawInputState& in_state) {
	if (ranges[in_slot] == nullptr) {
		return NAN;
	}
	else {
		return ranges[in_slot](in_event, in_state);
	}
}

bool InputBindings::Evaluate_Action(const Iterator& in_iter) {
	return Evaluate_Action(in_iter.mIndex, in_iter.mEvent, in_iter.mState);
}

InputStateChange InputBindings::Evaluate_State(const Iterator& in_iter) {
	return Evaluate_State(in_iter.mIndex, in_iter.mEvent, in_iter.mState);
}

float InputBindings::Evaluate_Range(const Iterator& in_iter) {
	return Evaluate_Range(in_iter.mIndex, in_iter.mEvent, in_iter.mState);
}

InputBindings::Iterator InputBindings::Iterate_Actions(const RawInputEvent& in_event, const RawInputState& in_state) {
	return Iterator(Iterator::Mode::action, *this, in_event, in_state);
}

InputBindings::Iterator InputBindings::Iterate_States(const RawInputEvent& in_event, const RawInputState& in_state) {
	return Iterator(Iterator::Mode::state, *this, in_event, in_state);
}

InputBindings::Iterator InputBindings::Iterate_Ranges(const RawInputEvent& in_event, const RawInputState& in_state) {
	return Iterator(Iterator::Mode::range, *this, in_event, in_state);
}

uint InputBindings::operator[](Iterator in_iterator) const {
	return in_iterator.mIndex;
}

InputBindings::Iterator::Iterator(Mode in_mode, InputBindings& in_parent, const RawInputEvent& in_event, const RawInputState& in_state) :
	mMode(in_mode),
	mParent(in_parent),
	mEvent(in_event),
	mState(in_state),
	mIndex(-1) {
	
	++(*this);
}

InputBindings::Iterator::operator bool() {
	switch (mMode) {
	case Mode::action:
		return mIndex < (int)mParent.Get_Number_Actions();
		break;
	case Mode::state:
		return mIndex < (int)mParent.Get_Number_States();
		break;
	case Mode::range:
		return mIndex < (int)mParent.Get_Number_Ranges();
		break;
	}
	return false;
}

InputBindings::Iterator& InputBindings::Iterator::operator++() {
	switch (mMode) {
	case Mode::action:
		do { mIndex++; } while (mIndex < (int)mParent.Get_Number_Actions() && !mParent.Evaluate_Action(mIndex, mEvent, mState));
		break;
	case Mode::state:
		do { mIndex++; } while (mIndex < (int)mParent.Get_Number_States() && mParent.Evaluate_State(mIndex, mEvent, mState) == InputStateChange::no_change);
		break;
	case Mode::range:
		do { mIndex++; } while (mIndex < (int)mParent.Get_Number_Ranges() && std::isnan(mParent.Evaluate_Range(mIndex, mEvent, mState)));
		break;
	}
	return *this;
}


