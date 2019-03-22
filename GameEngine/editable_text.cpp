#include "editable_text.h"
#include "game_engine.h"

EditableText::EditableText(TextInputContext* in_context, FontFaceRasterSet* in_rasterSet) :
	mInput(in_context),
	mText("text", in_rasterSet),
	mCursorSprite(AxisAlignedRectangled(
		AxisAlignedRectangled::fromExtrema(
			Vector2d(0, 0), 
			Vector2d(mCursorWidth, mText.getCharHeight())
		)
	), nullptr, ColorRGBAf(1, 1, 1, 1)),
	mCursorBlinkStepper(0.5),
	mCursorPosition(0)
{
	mText.getTransform().setParent(&mTransform);
	mText.getDepthTransform().setParent(&mDepthTransform);
	mCursorSprite.getTransform().setParent(&mText.getScrollTransform());
	mCursorSprite.getDepthTransform().setParent(&mText.getDepthTransform());
	setCursorPosition(0);

	//text.setScrollPosition(Vector2d(0, 50));
}

Transform2d& EditableText::getTransform() {
	return mTransform;
}

DepthTransform2d& EditableText::getDepthTransform() {
	return mDepthTransform; 
}

void EditableText::setCursorPosition(uint in_cursorPos) {
	mCursorPosition = GEUtil::min<uint>(in_cursorPos, (uint)mText.getText().size());
	updateCursorSprite();
	mText.setScrollPosition(mText.getScrollToIncludeChar(mCursorPosition));
}

void EditableText::incrementCursor() {
	setCursorPosition(mCursorPosition + 1);
}

void EditableText::decrementCursor() {
	if (mCursorPosition > 0) {
		setCursorPosition(mCursorPosition - 1);
	}
}

void EditableText::lowerCursor() {
	Vector2f newPos = mText.getLocalCharPosition(mCursorPosition);
	newPos[1] += mText.getNewlineHeight() * 1.5f;
	setCursorPosition(mText.getClosestCharIndex(newPos));
}

void EditableText::raiseCursor() {
	Vector2f newPos = mText.getLocalCharPosition(mCursorPosition);
	newPos[1] -= mText.getNewlineHeight() * 0.5f;
	setCursorPosition(mText.getClosestCharIndex(newPos));
}

void EditableText::update(double in_dt) {
	InputEvent _event;
	while (mInput.getNumberEvents()) {
		_event = mInput.popEvent();
		if (_event.mType == InputEvent::Type::action) {
			switch (_event.mMessage) {
			case TextInputContext::Action::backspace:
				if (mCursorPosition > 0) {
					mText.remove(mCursorPosition - 1, 1);
					decrementCursor();
				}
				break;
			case TextInputContext::Action::_delete:
				if (mCursorPosition < mText.getText().size()) {
					mText.remove(mCursorPosition, 1);
				}
				break;
			case TextInputContext::Action::newline:
				mText.insert("\n", mCursorPosition);
				incrementCursor();
				break;
			case TextInputContext::Action::cursor_left:
				decrementCursor();
				break;
			case TextInputContext::Action::cursor_right:
				incrementCursor();
				break;
			case TextInputContext::Action::cursor_up:
				raiseCursor();
				break;
			case TextInputContext::Action::cursor_down:
				lowerCursor();
				break;
			case TextInputContext::Action::cursor_begin:
				setCursorPosition(0);
				break;
			case TextInputContext::Action::cursor_end:
				setCursorPosition((uint)mText.getText().size());
				break;
			default:
				mText.insert((char)(_event.mMessage + ' '), mCursorPosition);
				incrementCursor();
				break;
			}
		}
	}

	if (mCursorBlinkStepper.stepTotal(in_dt)) {
		if (mCursorSprite.isEnabled()) {
			mCursorSprite.disable();
		}
		else {
			mCursorSprite.enable();
		}
	}
}

void EditableText::updateCursorSprite() {
	Vector2f position = mText.getLocalCharPosition(mCursorPosition);

	if (position.x() > 0.0) {
		position[0] -= (float)(mCursorSprite.getTransform().getLocalScale().x() / 2.0);
	}

	mCursorSprite.getTransform().setLocalPosition(position);
}

double EditableText::z() const {
	return mDepthTransform.getWorldDepth();
}

bool EditableText::shouldCull() const {
	return false;
}

void EditableText::render() {
	Vector2f corner = Vector2f(Vector2d(mTransform.getLocalPosition()));
	Vector2f dimensions = mText.getContainerDimensions();
	corner[1] = GE.render().mMainWindow->getDimensions().y() - corner.y() - dimensions.y();

	glScissor((int)corner.x(), (int)corner.y(), (int)dimensions.x(), (int)dimensions.y());
	glEnable(GL_SCISSOR_TEST);

	mText.render();
	if (mCursorSprite.isEnabled()) {
		mCursorSprite.render();
	}
	glDisable(GL_SCISSOR_TEST);
}
