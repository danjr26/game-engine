#ifndef KEYBOARD_TEXT_INPUT_CONTEXT_H
#define KEYBOARD_TEXT_INPUT_CONTEXT_H

#include "text_input_context.h"

class KeyboardTextInputContext : public TextInputContext {
public:
	KeyboardTextInputContext();
	bool Process_Raw_Event(const RawInputEvent& in_event) override;
};

#endif