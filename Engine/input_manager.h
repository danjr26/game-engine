#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "raw_input_event.h"
#include "raw_input_state.h"
#include "input_context.h"
#include "window.h"
#include <windows.h>
#include <vector>

class InputManager {
	friend LRESULT CALLBACK WindowProc(HWND i_hwnd, UINT i_message, WPARAM i_wParam, LPARAM i_lParam);

	RawInputState mRawState;
	std::vector<InputContext*> mContexts;

public:
	InputManager();
	void add(InputContext* i_context);
	void remove(InputContext* i_context);
	void update();
	void processRawEvent(const RawInputEvent& i_event);
	const RawInputState& getRawState();

private:
	static bool compareContexts(InputContext* i_context1, InputContext* i_context2);
};

#endif
