#include "mouse_range_identifier.h"
#include "game_engine.h"

MouseRangeIdentifier::MouseRangeIdentifier(MouseAxis in_axis, float in_lowValue, float in_highValue) :
	mAxis(in_axis),
	mLowValue(in_lowValue),
	mHighValue(in_highValue)
{}

float MouseRangeIdentifier::operator()(const RawInputEvent& in_event, const RawInputState& in_state) const {
	Window* window = GE.windows().get(in_event.mWindowHandle);
	Vector2i windowDimensions = window->getDimensions();
	return (in_event.mType == RawInputEvent::Type::mouse_move) ? 
		GEUtil::lerp<float>(mLowValue, mHighValue, GEUtil::invLerp<float>(0.0f, (float)windowDimensions.get((uint)mAxis), (float)in_event.mMouse.mPosition.get((uint)mAxis))) : NAN;
}
