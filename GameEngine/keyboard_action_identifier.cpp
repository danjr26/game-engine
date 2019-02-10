#include "keyboard_action_identifier.h"

KeyboardActionIdentifier::KeyboardActionIdentifier(RawInputEvent::Type in_type, Key in_key, bool in_acceptRepeat) :
	mType(in_type),
	mKey(in_key),
	mAcceptRepeat(in_acceptRepeat)
{}

KeyboardActionIdentifier::KeyboardActionIdentifier(RawInputEvent::Type in_type, char in_character, bool in_acceptRepeat) :
	mType(in_type),
	mCharacter(in_character),
	mAcceptRepeat(in_acceptRepeat)
{}

bool KeyboardActionIdentifier::operator()(const RawInputEvent& in_event, const RawInputState& in_state) const {
	if (mType == in_event.mType) {
		switch (mType) {
		case RawInputEvent::character:
			return mCharacter == in_event.mKeyboard.mCharacter && ((in_event.mKeyboard.mRepeat) ? mAcceptRepeat : true);
		case RawInputEvent::key_down:
		case RawInputEvent::key_up:
			return mKey == in_event.mKeyboard.mKey && ((in_event.mKeyboard.mRepeat) ? mAcceptRepeat : true);
		}
	}
	return false;
}