#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "raw_input_event.h"
#include "raw_input_state.h"
#include "input_context.h"
#include "window.h"
#include <windows.h>

class InputManager {
	friend LRESULT CALLBACK WindowProc(HWND in_hwnd, UINT in_message, WPARAM in_wParam, LPARAM in_lParam);

	RawInputState mRawState;
	std::vector<InputContext*> mContexts;

public:
	InputManager();
	void addBefore(InputContext* in_beforeWhat, InputContext* in_context);
	void addAfter(InputContext* in_afterWhat, InputContext* in_context);
	void remove(InputContext* in_context);
	void update();
	void processRawEvent(const RawInputEvent& in_event);
	const RawInputState& getRawState();
};

#endif
