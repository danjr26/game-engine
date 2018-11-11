#ifndef EDITABLE_TEXT_H
#define EDITABLE_TEXT_H

#include "text2.h"
#include "input_listener.h"
#include "per_frame_updateable_object.h"
#include "text_input_context.h"
#include "sprite.h"
#include "stepper.h"

class EditableText : public RenderableObject, public DeepTransformableObject2f, public PerFrameUpdateableObject {
private:
	const float cursorWidth = 1.0f;

	InputListener input;
	Text2 text;
	Sprite cursorSprite;
	Stepper cursorBlinkStepper;
	uint cursorPos;

public:
	EditableText(TextInputContext* in_context, FontFaceRasterSet* in_rasterSet);
	void Set_Cursor_Position(uint in_cursorPos);
	void Increment_Cursor();
	void Decrement_Cursor();
	void Lower_Cursor();
	void Raise_Cursor();
	void Update(double in_dt) override;

	virtual double Z() const override;
	virtual bool Should_Cull() const override;
	virtual void Render() override;

private:
	void Update_Cursor_Sprite();
};

#endif