#include "mouse_range_identifier.h"
#include "game_engine.h"

MouseRangeIdentifier::MouseRangeIdentifier(MouseAxis i_axis, float i_lowValue, float i_highValue) :
	mAxis(i_axis),
	mLowValue(i_lowValue),
	mHighValue(i_highValue)
{}

float MouseRangeIdentifier::operator()(const RawInputEvent& i_event, const RawInputState& i_state) const {
	Window* window = GE.windows().get(i_event.mWindowHandle);
	Vector2i windowDimensions = window->getDimensions();
	if (i_event.mType == RawInputEvent::Type::mouse_move) {
		return geutil::lerp<float>(mLowValue, mHighValue, (float)i_event.mMouse.mPosition.get((uint)mAxis), 0.0f, (float)windowDimensions.get((uint)mAxis));
	}
	else {
		return NAN;
	}
}
