#include "player_ship_input_context.h"
#include "keyboard_range_identifier.h"
#include "mouse_state_identifier.h"
#include "keyboard_state_identifier.h"
#include "player_ship.h"

player_ship::InputContext::InputContext() {
	//mBindings.bindState(States::fire_1, MouseStateIdentifier(RawInputEvent::Type::mouse_down, MouseButton::left, InputStateChange::turn_on));
	//mBindings.bindState(States::fire_1, MouseStateIdentifier(RawInputEvent::Type::mouse_up, MouseButton::left, InputStateChange::turn_off));

	mBindings.bindState(States::fire_1, KeyboardStateIdentifier(RawInputEvent::key_down, Key::space, InputStateChange::turn_on));
	mBindings.bindState(States::fire_1, KeyboardStateIdentifier(RawInputEvent::key_up, Key::space, InputStateChange::turn_off));

	mBindings.bindRange(Ranges::move_x, KeyboardRangeIdentifier(Key::a, Key::d));
	mBindings.bindRange(Ranges::move_y, KeyboardRangeIdentifier(Key::s, Key::w));
}
