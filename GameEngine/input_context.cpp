#include "input_context.h"
#include "game_engine.h"
#include <algorithm>

InputContext::InputContext(uint in_nActions, uint in_nStates, uint in_nRanges) :
	mBindings(in_nActions, in_nStates, in_nRanges),
	mStates(in_nStates),
	mRanges(in_nRanges)
{}

InputEvent InputContext::Auto_Translate_Action(InputBindings::Iterator & in_iter) {
	InputEvent _event;
	_event.mContext = this;
	_event.mType = InputEvent::Type::action;
	_event.mMessage = mBindings[in_iter];
	return _event;
}

InputEvent InputContext::Auto_Translate_State(InputBindings::Iterator& in_iter, bool in_newValue) {
	InputEvent _event;
	_event.mContext = this;
	_event.mType = InputEvent::Type::state_change;
	_event.mMessage = mBindings[in_iter];
	_event.mState.oldValue = mStates[_event.mMessage];
	_event.mState.newValue = in_newValue;
	return _event;
}

InputEvent InputContext::Auto_Translate_Range(InputBindings::Iterator& in_iter, float in_newValue) {
	InputEvent _event;
	_event.mContext = this;
	_event.mType = InputEvent::Type::range_change;
	_event.mMessage = mBindings[in_iter];
	_event.mRange.oldValue = mRanges[_event.mMessage];
	_event.mRange.newValue = in_newValue;
	return _event;
}

bool InputContext::Auto_Process_Action(InputBindings::Iterator& in_iter) {
	return Distribute_Event(Auto_Translate_Action(in_iter));
}

bool InputContext::Auto_Process_State(InputBindings::Iterator& in_iter) {
	InputStateChange stateChange = mBindings.Evaluate_State(in_iter);
	bool newValue;
	switch (stateChange) {
	case InputStateChange::no_change:
		newValue = mStates[mBindings[in_iter]];
		break;
	case InputStateChange::turn_on:
		newValue = true;
		break;
	case InputStateChange::turn_off:
		newValue = false;
		break;
	case InputStateChange::toggle:
		newValue = !mStates[mBindings[in_iter]];
		break;
	}
	bool eaten = Distribute_Event(Auto_Translate_State(in_iter, newValue));
	mStates[mBindings[in_iter]] = newValue;
	return eaten;
}

bool InputContext::Auto_Process_Range(InputBindings::Iterator& in_iter) {
	float newValue = mBindings.Evaluate_Range(in_iter);
	bool eaten = Distribute_Event(Auto_Translate_Range(in_iter, newValue));
	mRanges[mBindings[in_iter]] = newValue;
	return eaten;
}

bool InputContext::Auto_Update_Actions(const RawInputEvent& in_event) {
	bool out = false;
	for (auto iter = mBindings.Iterate_Actions(in_event, GE.Input().Get_Raw_State()); iter; ++iter) {
		out = Auto_Process_Action(iter) || out;
	}
	return out;
}

bool InputContext::Auto_Update_States(const RawInputEvent& in_event) {
	bool out = false;
	for (auto iter = mBindings.Iterate_States(in_event, GE.Input().Get_Raw_State()); iter; ++iter) {
		out = Auto_Process_State(iter) || out;
	}
	return out;
}

bool InputContext::Auto_Update_Ranges(const RawInputEvent& in_event) {
	bool out = false;
	for (auto iter = mBindings.Iterate_Ranges(in_event, GE.Input().Get_Raw_State()); iter; ++iter) {
		out = Auto_Process_Range(iter) || out;
	}
	return out;
}

bool InputContext::Auto_Update(const RawInputEvent& in_event) {
	return (
		Auto_Update_Actions(in_event) || 
		Auto_Update_States(in_event) ||
		Auto_Update_Ranges(in_event)
	);
}

bool InputContext::Distribute_Event(const InputEvent& in_event) {
	for (uint i = 0; i < mListeners.size(); i++) {
		if (mListeners[i]->Post_Event(in_event)) return true;
	}
	return false;
}

bool InputContext::Process_Raw_Event(const RawInputEvent& in_event) {
	return Auto_Update(in_event);
}

bool InputContext::Get_State(uint in_index) {
	return mStates[in_index];
}

float InputContext::Get_Range(uint in_index) {
	return mRanges[in_index];
}

void InputContext::Add(InputListener* in_listener) {
	mListeners.push_back(in_listener);
	Sort();
}

void InputContext::Remove(InputListener* in_listener) {
	auto position = std::find(mListeners.begin(), mListeners.end(), in_listener);
	if (position != mListeners.end()) {
		mListeners.erase(position);
	}
	Sort();
}

void InputContext::Sort() {
	std::sort(mListeners.begin(), mListeners.end(), InputListener::Compare_Pointers);
}
