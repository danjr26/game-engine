#include "input_manager.h"
#include "game_engine.h"

InputManager::InputManager()
{}

void InputManager::Add_Before(InputContext* in_beforeWhat, InputContext* in_context) {
	if (in_beforeWhat == nullptr) {
		contexts.insert(contexts.begin(), in_context);
	}
	else {
		auto position = std::find(contexts.begin(), contexts.end(), in_beforeWhat);
		if (position == contexts.end()) {
			Log::main("error: could not find reference context");
			exit(-1);
		}
		contexts.insert(position, in_context);
	}
}

void InputManager::Add_After(InputContext* in_afterWhat, InputContext* in_context) {
	if (in_afterWhat == nullptr) {
		contexts.insert(contexts.end(), in_context);
	}
	else {
		auto position = std::find(contexts.begin(), contexts.end(), in_afterWhat);
		if (position == contexts.end()) {
			Log::main("error: could not find reference context");
			exit(-1);
		}
		contexts.insert(position + 1, in_context);
	}
}

void InputManager::Remove(InputContext* in_context) {
	auto position = std::find(contexts.begin(), contexts.end(), in_context);
	if (position != contexts.end()) {
		contexts.erase(position);
	}
}

void InputManager::Update() {
	MSG message;
	while (PeekMessageW(&message, GE.Render().mainWindow->Get_Handle(), 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
		if (message.message == WM_QUIT) {
			exit(1);
		}
	}
}

void InputManager::Process_Raw_Event(const RawInputEvent& in_event) {
	rawState.Process_Raw_Event(in_event);
	for (uint i = 0; i < contexts.size() && !contexts[i]->Process_Raw_Event(in_event); i++);
}

const RawInputState& InputManager::Get_Raw_State() {
	return rawState;
}
