#include "input_manager.h"
#include "game_engine.h"

InputManager::InputManager()
{}

void InputManager::addBefore(InputContext* in_beforeWhat, InputContext* in_context) {
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

void InputManager::addAfter(InputContext* in_afterWhat, InputContext* in_context) {
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

void InputManager::remove(InputContext* in_context) {
	auto position = std::find(mContexts.begin(), mContexts.end(), in_context);
	if (position != mContexts.end()) {
		mContexts.erase(position);
	}
}

void InputManager::update() {
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
	for (uint i = 0; i < mContexts.size() && !mContexts[i]->processRawEvent(in_event); i++);
}

const RawInputState& InputManager::getRawState() {
	return mRawState;
}
