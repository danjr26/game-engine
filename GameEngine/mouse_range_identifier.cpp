#include "mouse_range_identifier.h"
#include "game_engine.h"

MouseRangeIdentifier::MouseRangeIdentifier(MouseAxis in_axis, float in_lowValue, float in_highValue) :
	mAxis(in_axis),
	mLowValue(in_lowValue),
	mHighValue(in_highValue)
{}

float MouseRangeIdentifier::operator()(const RawInputEvent& in_event, const RawInputState& in_state) const {
	Window* window = GE.Windows().Get(in_event.mWindowHandle);
	Vector2i windowDimensions = window->Get_Dimensions();
	return (in_event.mType == RawInputEvent::Type::mouse_move) ? 
		Lerp<float>(mLowValue, mHighValue, Inv_Lerp<float>(0.0f, (float)windowDimensions.Get((uint)mAxis), (float)in_event.mMouse.mPosition.Get((uint)mAxis))) : NAN;
}
