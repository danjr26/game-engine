#include "editable_text.h"
#include "game_engine.h"

EditableText::EditableText(TextInputContext* in_context, FontFaceRasterSet* in_rasterSet) :
	mInput(in_context),
	mText("text", in_rasterSet),
	mCursorSprite(AxisAlignedRectangled(
		AxisAlignedRectangled::From_Extrema(
			Vector2d(0, 0), 
			Vector2d(mCursorWidth, mText.Get_Char_Height())
		)
	), nullptr, ColorRGBAf(1, 1, 1, 1)),
	mCursorBlinkStepper(0.5),
	mCursorPosition(0)
{
	mText.Get_Transform().Set_Parent(&mTransform);
	mText.Get_Depth_Transform().Set_Parent(&mDepthTransform);
	mCursorSprite.Get_Transform().Set_Parent(&mText.Get_Scroll_Transform());
	mCursorSprite.Get_Depth_Transform().Set_Parent(&mText.Get_Depth_Transform());
	Set_Cursor_Position(0);

	//text.Set_Scroll_Position(Vector2d(0, 50));
}

void EditableText::Set_Cursor_Position(uint in_cursorPos) {
	mCursorPosition = Min<uint>(in_cursorPos, (uint)mText.Get_Text().size());
	Update_Cursor_Sprite();
	mText.Set_Scroll_Position(mText.Get_Scroll_To_Include_Char(mCursorPosition));
}

void EditableText::Increment_Cursor() {
	Set_Cursor_Position(mCursorPosition + 1);
}

void EditableText::Decrement_Cursor() {
	if (mCursorPosition > 0) {
		Set_Cursor_Position(mCursorPosition - 1);
	}
}

void EditableText::Lower_Cursor() {
	Vector2f newPos = mText.Get_Local_Char_Position(mCursorPosition);
	newPos[1] += mText.Get_Newline_Height() * 1.5f;
	Set_Cursor_Position(mText.Get_Closest_Char_Index(newPos));
}

void EditableText::Raise_Cursor() {
	Vector2f newPos = mText.Get_Local_Char_Position(mCursorPosition);
	newPos[1] -= mText.Get_Newline_Height() * 0.5f;
	Set_Cursor_Position(mText.Get_Closest_Char_Index(newPos));
}

void EditableText::Update(double in_dt) {
	InputEvent _event;
	while (mInput.Get_Number_Events()) {
		_event = mInput.Pop_Event();
		if (_event.mType == InputEvent::Type::action) {
			switch (_event.mMessage) {
			case TextInputContext::Action::backspace:
				if (mCursorPosition > 0) {
					mText.Delete(mCursorPosition - 1, 1);
					Decrement_Cursor();
				}
				break;
			case TextInputContext::Action::_delete:
				if (mCursorPosition < mText.Get_Text().size()) {
					mText.Delete(mCursorPosition, 1);
				}
				break;
			case TextInputContext::Action::newline:
				mText.Insert("\n", mCursorPosition);
				Increment_Cursor();
				break;
			case TextInputContext::Action::cursor_left:
				Decrement_Cursor();
				break;
			case TextInputContext::Action::cursor_right:
				Increment_Cursor();
				break;
			case TextInputContext::Action::cursor_up:
				Raise_Cursor();
				break;
			case TextInputContext::Action::cursor_down:
				Lower_Cursor();
				break;
			case TextInputContext::Action::cursor_begin:
				Set_Cursor_Position(0);
				break;
			case TextInputContext::Action::cursor_end:
				Set_Cursor_Position((uint)mText.Get_Text().size());
				break;
			default:
				mText.Insert((char)(_event.mMessage + ' '), mCursorPosition);
				Increment_Cursor();
				break;
			}
		}
	}

	if (mCursorBlinkStepper.Step_Total(in_dt)) {
		if (mCursorSprite.Is_Enabled()) {
			mCursorSprite.Disable();
		}
		else {
			mCursorSprite.Enable();
		}
	}
}

void EditableText::Update_Cursor_Sprite() {
	Vector2f position = mText.Get_Local_Char_Position(mCursorPosition);

	if (position.X() > 0.0) {
		position[0] -= (float)(mCursorSprite.Get_Transform().Get_Local_Scale().X() / 2.0);
	}

	mCursorSprite.Get_Transform().Set_Local_Position(position);
}

double EditableText::Z() const {
	return mDepthTransform.Get_World_Depth();
}

bool EditableText::Should_Cull() const {
	return false;
}

void EditableText::Render() {
	Vector2f corner = Vector2f(Vector2d(mTransform.Get_Local_Position()));
	Vector2f dimensions = mText.Get_Container_Dimensions();
	corner[1] = GE.Render().mMainWindow->Get_Dimensions().Y() - corner.Y() - dimensions.Y();

	glScissor((int)corner.X(), (int)corner.Y(), (int)dimensions.X(), (int)dimensions.Y());
	glEnable(GL_SCISSOR_TEST);

	mText.Render();
	if (mCursorSprite.Is_Enabled()) {
		mCursorSprite.Render();
	}
	glDisable(GL_SCISSOR_TEST);
}
