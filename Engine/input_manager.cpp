#include "input_manager.h"
#include "game_engine.h"

InputManager::InputManager() :
	mContexts()
{}

void InputManager::add(InputContext* i_context) {
	mContexts.push_back(i_context);
	std::sort(mContexts.begin(), mContexts.end(), compareContexts);
}

void InputManager::remove(InputContext* i_context) {
	auto search = std::find(mContexts.begin(), mContexts.end(), i_context);
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

void InputManager::processRawEvent(const RawInputEvent& i_event) {
	mRawState.processRawEvent(i_event);
	for (auto it = mContexts.begin(); it != mContexts.end() && !(*it)->processRawEvent(i_event); it++);
}

const RawInputState& InputManager::getRawState() {
	return mRawState;
}

bool InputManager::compareContexts(InputContext* i_context1, InputContext* i_context2) {
	return i_context1->getPriority() > i_context2->getPriority();
}
