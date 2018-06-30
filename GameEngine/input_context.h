#ifndef INPUT_CONTEXT_H
#define INPUT_CONTEXT_H

#include "raw_input_state.h"
#include "input_event.h"
#include "input_listener.h"
#include "input_bindings.h"


class InputContext {
protected:
	InputBindings bindings;

	std::vector<bool> states;
	std::vector<float> ranges;

	std::vector<InputListener*> listeners;

protected:
	InputContext(uint in_nActions, uint in_nStates, uint in_nRanges);

	InputEvent Auto_Translate_Action(InputBindings::Iterator& in_iter);
	InputEvent Auto_Translate_State(InputBindings::Iterator& in_iter, bool in_newValue);
	InputEvent Auto_Translate_Range(InputBindings::Iterator& in_iter, float in_newValue);

	bool Auto_Process_Action(InputBindings::Iterator& in_iter);
	bool Auto_Process_State(InputBindings::Iterator& in_iter);
	bool Auto_Process_Range(InputBindings::Iterator& in_iter);

	bool Auto_Update_Actions(const RawInputEvent& in_event);
	bool Auto_Update_States(const RawInputEvent& in_event);
	bool Auto_Update_Ranges(const RawInputEvent& in_event);

	bool Auto_Update(const RawInputEvent& in_event);

	virtual void Distribute_Event(const InputEvent& in_event);

public:
	virtual bool Process_Raw_Event(const RawInputEvent& in_event);

	bool Get_State(uint in_index);
	float Get_Range(uint in_index);

	void Add(InputListener* in_listener);
	void Remove(InputListener* in_listener);
};

#endif