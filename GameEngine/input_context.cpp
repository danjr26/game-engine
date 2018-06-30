#include "input_context.h"
#include "game_engine.h"

InputContext::InputContext(uint in_nActions, uint in_nStates, uint in_nRanges) :
	bindings(in_nActions, in_nStates, in_nRanges),
	states(in_nStates),
	ranges(in_nRanges)
{}

InputEvent InputContext::Auto_Translate_Action(InputBindings::Iterator & in_iter) {
	InputEvent _event;
	_event.context = this;
	_event.type = InputEvent::Type::action;
	_event.message = bindings[in_iter];
	return _event;
}

InputEvent InputContext::Auto_Translate_State(InputBindings::Iterator& in_iter, bool in_newValue) {
	InputEvent _event;
	_event.context = this;
	_event.type = InputEvent::Type::state_change;
	_event.message = bindings[in_iter];
	_event.state.oldValue = states[_event.message];
	_event.state.newValue = in_newValue;
	return _event;
}

InputEvent InputContext::Auto_Translate_Range(InputBindings::Iterator & in_iter, float in_newValue) {
	InputEvent _event;
	_event.context = this;
	_event.type = InputEvent::Type::range_change;
	_event.message = bindings[in_iter];
	_event.range.oldValue = ranges[_event.message];
	_event.range.newValue = in_newValue;
	return _event;
}

bool InputContext::Auto_Process_Action(InputBindings::Iterator& in_iter) {
	Distribute_Event(Auto_Translate_Action(in_iter));
	return true;
}

bool InputContext::Auto_Process_State(InputBindings::Iterator& in_iter) {
	InputStateChange stateChange = bindings.Evaluate_State(in_iter);
	bool newValue;
	switch (stateChange) {
	case InputStateChange::no_change:
		newValue = states[bindings[in_iter]];
		break;
	case InputStateChange::turn_on:
		newValue = true;
		break;
	case InputStateChange::turn_off:
		newValue = false;
		break;
	case InputStateChange::toggle:
		newValue = !states[bindings[in_iter]];
		break;
	}
	Distribute_Event(Auto_Translate_State(in_iter, newValue));
	states[bindings[in_iter]] = newValue;
	return true;
}

bool InputContext::Auto_Process_Range(InputBindings::Iterator& in_iter) {
	float newValue = bindings.Evaluate_Range(in_iter);
	Distribute_Event(Auto_Translate_Range(in_iter, newValue));
	ranges[bindings[in_iter]] = newValue;
	return true;
}

bool InputContext::Auto_Update_Actions(const RawInputEvent& in_event) {
	bool out = false;
	for (auto iter = bindings.Iterate_Actions(in_event, GE.Input().Get_Raw_State()); iter; ++iter) {
		out = Auto_Process_Action(iter) || out;
	}
	return out;
}

bool InputContext::Auto_Update_States(const RawInputEvent & in_event) {
	bool out = false;
	for (auto iter = bindings.Iterate_States(in_event, GE.Input().Get_Raw_State()); iter; ++iter) {
		out = Auto_Process_State(iter) || out;
	}
	return out;
}

bool InputContext::Auto_Update_Ranges(const RawInputEvent & in_event) {
	bool out = false;
	for (auto iter = bindings.Iterate_Ranges(in_event, GE.Input().Get_Raw_State()); iter; ++iter) {
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

void InputContext::Distribute_Event(const InputEvent& in_event) {
	for (uint i = 0; i < listeners.size(); i++) {
		listeners[i]->Post_Event(in_event);
	}
}

bool InputContext::Process_Raw_Event(const RawInputEvent& in_event) {
	return Auto_Update(in_event);
}

bool InputContext::Get_State(uint in_index) {
	return states[in_index];
}

float InputContext::Get_Range(uint in_index) {
	return ranges[in_index];
}

void InputContext::Add(InputListener* in_listener) {
	listeners.push_back(in_listener);
}

void InputContext::Remove(InputListener* in_listener) {
	auto position = std::find(listeners.begin(), listeners.end(), in_listener);
	if (position != listeners.end()) {
		listeners.erase(position);
	}
}