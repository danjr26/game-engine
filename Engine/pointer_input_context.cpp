#include "pointer_input_context.h"
#include "game_engine.h"
#include "mouse_range_identifier.h"

PointerInputContext::PointerInputContext() {
	
	mBindings.bindRange(Range::fraction_x, MouseRangeIdentifier(MouseRangeIdentifier::MouseAxis::x));
	mBindings.bindRange(Range::fraction_y, MouseRangeIdentifier(MouseRangeIdentifier::MouseAxis::y));
}

bool PointerInputContext::processRawEvent(const RawInputEvent& i_event) {
	Camera* mainCamera = GE.cameras().get(CameraManager::ID::main);
	Projectiond mainProjection = mainCamera->getProjection();
	Vector2d mainOffset = Vector2d(mainProjection.mMaxima - mainProjection.mMinima);
	Window* window = GE.windows().get(i_event.mWindowHandle);
	Vector2d windowDim = window->getDimensions();

	InputEvent _event;
	_event.mContext = this;
	_event.mType = InputEvent::Type::range_change;

	_event.mMessage = Range::mai_x;
	_event.mRange.oldValue = mRanges[Range::mai_x];
	mRanges[Range::mai_x] = mRanges[Range::fraction_x] * mainOffset.x();
	_event.mRange.newValue = mRanges[Range::mai_x];
	distributeEvent(_event);

	_event.mMessage = Range::mai_y;
	_event.mRange.oldValue = mRanges[Range::mai_y];
	mRanges[Range::mai_y] = mRanges[Range::fraction_y] * -mainOffset.y();
	_event.mRange.newValue = mRanges[Range::mai_y];
	distributeEvent(_event);

	return autoUpdate(i_event);
}