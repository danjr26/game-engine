#include "player_ship_input_context.h"
#include "keyboard_range_identifier.h"
#include "player_ship.h"

PlayerShipInputContext::PlayerShipInputContext(PlayerShip& in_parent) :
	InputContext(Actions::count, States::count, Ranges::count),
	mParent(in_parent) {
	
	mBindings.bindRange(Ranges::move_x, KeyboardRangeIdentifier(Key::a, Key::d));
	mBindings.bindRange(Ranges::move_y, KeyboardRangeIdentifier(Key::s, Key::w));
}
