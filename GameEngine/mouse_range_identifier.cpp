#include "mouse_range_identifier.h"
#include "game_engine.h"

MouseRangeIdentifier::MouseRangeIdentifier(MouseAxis in_axis, float in_lowValue, float in_highValue) :
	axis(in_axis),
	lowValue(in_lowValue),
	highValue(in_highValue)
{}

float MouseRangeIdentifier::operator()(const RawInputEvent& in_event, const RawInputState& in_state) const {
	Window* window = GE.Windows().Get(in_event.windowHandle);
	Vector2i windowDimensions = window->Get_Dimensions();
	return (in_event.type == RawInputEvent::Type::mouse_move) ? 
		Lerp<float>(lowValue, highValue, Inv_Lerp<float>(0.0f, (float)windowDimensions.Get((uint)axis), (float)in_event.mouse.position.Get((uint)axis))) : NAN;
}
