#ifndef TEXT_INPUT_CONTEXT_H
#define TEXT_INPUT_CONTEXT_H

#include "input_context.h"

class TextInputContext : public InputContext {
public:
	enum Action {
		character = 0,
		backspace = '~' - ' ' + 1,
		_delete,
		newline,
		cursor_left,
		cursor_right,
		cursor_up,
		cursor_down,
		cursor_begin,
		cursor_end,
		count
	};

	TextInputContext();
	bool processRawEvent(const RawInputEvent& in_event) override;
};

#endif
