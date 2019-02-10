#ifndef EDITABLE_TEXT_H
#define EDITABLE_TEXT_H

#include "text2.h"
#include "input_listener.h"
#include "per_frame_updateable_object.h"
#include "text_input_context.h"
#include "sprite.h"
#include "stepper.h"

class EditableText : public RenderableObject, public DeepTransformableObject2d, public PerFrameUpdateableObject {
private:
	const float mCursorWidth = 1.0f;

	InputListener mInput;
	Text2 mText;
	Sprite mCursorSprite;
	Stepper mCursorBlinkStepper;
	uint mCursorPosition;

public:
	EditableText(TextInputContext* in_context, FontFaceRasterSet* in_rasterSet);
	void setCursorPosition(uint in_cursorPos);
	void incrementCursor();
	void decrementCursor();
	void lowerCursor();
	void raiseCursor();
	void update(double in_dt) override;

	virtual double z() const override;
	virtual bool shouldCull() const override;
	virtual void render() override;

private:
	void updateCursorSprite();
};

#endif