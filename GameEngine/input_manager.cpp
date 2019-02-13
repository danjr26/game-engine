#include "input_manager.h"
#include "game_engine.h"

InputManager::InputManager() :
	mContexts()
{}

void InputManager::add(InputContext* in_context) {
	mContexts.push_back(in_context);
	std::sort(mContexts.begin(), mContexts.end(), compareContexts);
}

void InputManager::remove(InputContext* in_context) {
	auto search = std::find(mContexts.begin(), mContexts.end(), in_context);
	if (search != mContexts.end()) {
		mContexts.erase(search);
	}
}

void InputManager::update() {
	std::sort(mContexts.begin(), mContexts.end(), compareContexts);
	MSG message;
	while (PeekMessageW(&message, GE.render().mMainWindow->getHandle(), 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
		if (message.message == WM_QUIT) {
			GE.quit();
		}
	}
}

void InputManager::processRawEvent(const RawInputEvent& in_event) {
	mRawState.processRawEvent(in_event);
	for (auto it = mContexts.begin(); it != mContexts.end() && !(*it)->processRawEvent(in_event); it++);
}

const RawInputState& InputManager::getRawState() {
	return mRawState;
}

bool InputManager::compareContexts(InputContext* in_context1, InputContext* in_context2) {
	return in_context1->getPriority() > in_context2->getPriority();
}
