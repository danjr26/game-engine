#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "raw_input_event.h"
#include "raw_input_state.h"
#include "input_context.h"
#include "window.h"
#include <Windows.h>

class InputManager {
	friend LRESULT CALLBACK WindowProc(HWND in_hwnd, UINT in_message, WPARAM in_wParam, LPARAM in_lParam);

	RawInputState rawState;
	std::vector<InputContext*> contexts;

public:
	InputManager();
	void Add_Before(InputContext* in_beforeWhat, InputContext* in_context);
	void Add_After(InputContext* in_afterWhat, InputContext* in_context);
	void Remove(InputContext* in_context);
	void Update();
	void Process_Raw_Event(const RawInputEvent& in_event);
	const RawInputState& Get_Raw_State();
};

#endif
