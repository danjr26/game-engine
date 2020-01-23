#include "keyboard_text_input_context.h"
#include "keyboard_action_identifier.h"

KeyboardTextInputContext::KeyboardTextInputContext() {
	for (char c = ' '; c <= '~'; c++) {
		mBindings.bindAction(Action::character + c - ' ', KeyboardActionIdentifier(RawInputEvent::character, c, true));
	}
	mBindings.bindAction(Action::backspace, KeyboardActionIdentifier(RawInputEvent::key_down, Key::backspace, true));
	mBindings.bindAction(Action::_delete, KeyboardActionIdentifier(RawInputEvent::key_down, Key::_delete, true));
	mBindings.bindAction(Action::newline, KeyboardActionIdentifier(RawInputEvent::key_down, Key::enter, true));
	mBindings.bindAction(Action::cursor_left, KeyboardActionIdentifier(RawInputEvent::key_down, Key::left_arrow, true));
	mBindings.bindAction(Action::cursor_right, KeyboardActionIdentifier(RawInputEvent::key_down, Key::right_arrow, true));
	mBindings.bindAction(Action::cursor_up, KeyboardActionIdentifier(RawInputEvent::key_down, Key::up_arrow, true));
	mBindings.bindAction(Action::cursor_down, KeyboardActionIdentifier(RawInputEvent::key_down, Key::down_arrow, true));
	mBindings.bindAction(Action::cursor_begin, KeyboardActionIdentifier(RawInputEvent::key_down, Key::home, true));
	mBindings.bindAction(Action::cursor_end, KeyboardActionIdentifier(RawInputEvent::key_down, Key::end, true));
}

bool KeyboardTextInputContext::processRawEvent(const RawInputEvent& i_event) {
	return autoUpdate(i_event);
}
