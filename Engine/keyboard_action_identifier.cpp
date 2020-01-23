#include "keyboard_action_identifier.h"

KeyboardActionIdentifier::KeyboardActionIdentifier(RawInputEvent::Type i_type, Key i_key, bool i_acceptRepeat) :
	mType(i_type),
	mKey(i_key),
	mAcceptRepeat(i_acceptRepeat)
{}

KeyboardActionIdentifier::KeyboardActionIdentifier(RawInputEvent::Type i_type, char i_character, bool i_acceptRepeat) :
	mType(i_type),
	mCharacter(i_character),
	mAcceptRepeat(i_acceptRepeat)
{}

bool KeyboardActionIdentifier::operator()(const RawInputEvent& i_event, const RawInputState& i_state) const {
	if (mType == i_event.mType) {
		switch (mType) {
		case RawInputEvent::character:
			return mCharacter == i_event.mKeyboard.mCharacter && ((i_event.mKeyboard.mRepeat) ? mAcceptRepeat : true);
		case RawInputEvent::key_down:
		case RawInputEvent::key_up:
			return mKey == i_event.mKeyboard.mKey && ((i_event.mKeyboard.mRepeat) ? mAcceptRepeat : true);
		}
	}
	return false;
}