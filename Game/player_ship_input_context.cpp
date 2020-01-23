#include "player_ship_input_context.h"
#include "keyboard_range_identifier.h"
#include "mouse_state_identifier.h"
#include "keyboard_state_identifier.h"
#include "keyboard_action_identifier.h"
#include "player_ship.h"

player_ship::InputContext::InputContext() {
	mBindings.bindAction(Actions::dodge, KeyboardActionIdentifier(RawInputEvent::key_down, Key::space));

	mBindings.bindState(States::fire_1, MouseStateIdentifier(RawInputEvent::Type::mouse_down, MouseButton::left, InputStateChange::turn_on));
	mBindings.bindState(States::fire_1, MouseStateIdentifier(RawInputEvent::Type::mouse_up, MouseButton::left, InputStateChange::turn_off));

	mBindings.bindState(States::slow, KeyboardStateIdentifier(RawInputEvent::Type::key_down, Key::left_shift, InputStateChange::turn_on));
	mBindings.bindState(States::slow, KeyboardStateIdentifier(RawInputEvent::Type::key_up, Key::left_shift, InputStateChange::turn_off));

	mBindings.bindRange(Ranges::move_x, KeyboardRangeIdentifier(Key::a, Key::d));
	mBindings.bindRange(Ranges::move_y, KeyboardRangeIdentifier(Key::s, Key::w));
}
