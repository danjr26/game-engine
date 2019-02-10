#include "input_manager.h"
#include "game_engine.h"

InputManager::InputManager()
{}

void InputManager::Add_Before(InputContext* in_beforeWhat, InputContext* in_context) {
	if (in_beforeWhat == nullptr) {
		mContexts.insert(mContexts.begin(), in_context);
	}
	else {
		auto position = std::find(mContexts.begin(), mContexts.end(), in_beforeWhat);
		if (position == mContexts.end()) {
			throw InvalidArgumentException("could not find reference input context for insertion");
		}
		mContexts.insert(position, in_context);
	}
}

void InputManager::Add_After(InputContext* in_afterWhat, InputContext* in_context) {
	if (in_afterWhat == nullptr) {
		mContexts.insert(mContexts.end(), in_context);
	}
	else {
		auto position = std::find(mContexts.begin(), mContexts.end(), in_afterWhat);
		if (position == mContexts.end()) {
			throw InvalidArgumentException("could not find reference input context for insertion");
		}
		mContexts.insert(position + 1, in_context);
	}
}

void InputManager::Remove(InputContext* in_context) {
	auto position = std::find(mContexts.begin(), mContexts.end(), in_context);
	if (position != mContexts.end()) {
		mContexts.erase(position);
	}
}

void InputManager::Update() {
	MSG message;
	while (PeekMessageW(&message, GE.Render().mMainWindow->Get_Handle(), 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
		if (message.message == WM_QUIT) {
			GE.Exit();
		}
	}
}

void InputManager::Process_Raw_Event(const RawInputEvent& in_event) {
	mRawState.Process_Raw_Event(in_event);
	for (uint i = 0; i < mContexts.size() && !mContexts[i]->Process_Raw_Event(in_event); i++);
}

const RawInputState& InputManager::Get_Raw_State() {
	return mRawState;
}
