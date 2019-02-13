#include "pointer_input_context.h"
#include "game_engine.h"
#include "mouse_range_identifier.h"

PointerInputContext::PointerInputContext() :
	InputContext(Action::count, State::count, Range::count) {
	
	mBindings.bindRange(Range::fraction_x, MouseRangeIdentifier(MouseRangeIdentifier::MouseAxis::x));
	mBindings.bindRange(Range::fraction_y, MouseRangeIdentifier(MouseRangeIdentifier::MouseAxis::y));
}

bool PointerInputContext::processRawEvent(const RawInputEvent& in_event) {
	Camera* mainCamera = GE.cameras().get(CameraManager::ID::main);
	Projectiond mainProjection = mainCamera->getProjection();
	Vector2d mainOffset = Vector2d(mainProjection.mMaxima - mainProjection.mMinima);
	Window* window = GE.windows().get(in_event.mWindowHandle);
	Vector2d windowDim = window->getDimensions();

	InputEvent _event;
	_event.mContext = this;
	_event.mType = InputEvent::Type::range_change;

	_event.mMessage = Range::main_x;
	_event.mRange.oldValue = mRanges[Range::main_x];
	mRanges[Range::main_x] = mRanges[Range::fraction_x] * mainOffset.x() + mainProjection.mMinima.x();
	_event.mRange.newValue = mRanges[Range::main_x];
	distributeEvent(_event);

	_event.mMessage = Range::main_y;
	_event.mRange.oldValue = mRanges[Range::main_y];
	mRanges[Range::main_y] = -mRanges[Range::fraction_y] * mainOffset.y() + mainProjection.mMaxima.y();
	_event.mRange.newValue = mRanges[Range::main_y];
	distributeEvent(_event);

	return autoUpdate(in_event);
}