#include "player_ship_input_context.h"
#include "keyboard_range_identifier.h"
#include "mouse_state_identifier.h"
#include "player_ship.h"

PlayerShipInputContext::PlayerShipInputContext() :
	InputContext(Actions::count, States::count, Ranges::count) {
	
	//mBindings.bindState(States::fire_1, MouseStateIdentifier(RawInputEvent::Type::mouse_down, MouseButton::left, InputStateChange::turn_on));

	mBindings.bindRange(Ranges::move_x, KeyboardRangeIdentifier(Key::a, Key::d));
	mBindings.bindRange(Ranges::move_y, KeyboardRangeIdentifier(Key::s, Key::w));
}
