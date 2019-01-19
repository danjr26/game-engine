#include "keyboard_text_input_context.h"
#include "keyboard_action_identifier.h"

KeyboardTextInputContext::KeyboardTextInputContext() {
	for (char c = ' '; c <= '~'; c++) {
		bindings.Bind_Action(Action::character + c - ' ', KeyboardActionIdentifier(RawInputEvent::character, c, true));
	}
	bindings.Bind_Action(Action::backspace, KeyboardActionIdentifier(RawInputEvent::key_down, Key::backspace, true));
	bindings.Bind_Action(Action::_delete, KeyboardActionIdentifier(RawInputEvent::key_down, Key::_delete, true));
	bindings.Bind_Action(Action::newline, KeyboardActionIdentifier(RawInputEvent::key_down, Key::enter, true));
	bindings.Bind_Action(Action::cursor_left, KeyboardActionIdentifier(RawInputEvent::key_down, Key::left_arrow, true));
	bindings.Bind_Action(Action::cursor_right, KeyboardActionIdentifier(RawInputEvent::key_down, Key::right_arrow, true));
	bindings.Bind_Action(Action::cursor_up, KeyboardActionIdentifier(RawInputEvent::key_down, Key::up_arrow, true));
	bindings.Bind_Action(Action::cursor_down, KeyboardActionIdentifier(RawInputEvent::key_down, Key::down_arrow, true));
	bindings.Bind_Action(Action::cursor_begin, KeyboardActionIdentifier(RawInputEvent::key_down, Key::home, true));
	bindings.Bind_Action(Action::cursor_end, KeyboardActionIdentifier(RawInputEvent::key_down, Key::end, true));
}

bool KeyboardTextInputContext::Process_Raw_Event(const RawInputEvent& in_event) {
	return Auto_Update(in_event);
}