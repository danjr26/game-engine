#include "keyboard_range_identifier.h"

KeyboardRangeIdentifier::KeyboardRangeIdentifier(Key i_lowKey, Key i_highKey,
	float i_lowValue, float i_midValue, float i_highValue, bool i_acceptRepeat) :
	mLowKey(i_lowKey),
	mHighKey(i_highKey),
	mLowValue(i_lowValue),
	mMidValue(i_midValue),
	mHighValue(i_highValue),
	mAcceptRepeat(i_acceptRepeat)
{}

float KeyboardRangeIdentifier::operator()(const RawInputEvent& i_event, const RawInputState& i_state) const {
	if ((i_event.mKeyboard.mRepeat) ? mAcceptRepeat : true) {
		if (i_event.mType == RawInputEvent::Type::key_down) {
			if (i_event.mKeyboard.mKey == mLowKey) {
				return mLowValue;
			}
			if (i_event.mKeyboard.mKey == mHighKey) {
				return mHighValue;
			}
		}
		if (i_event.mType == RawInputEvent::Type::key_up) {
			if (i_event.mKeyboard.mKey == mLowKey) {
				if (i_state.mKeyboard.isDown(mHighKey)) {
					return mHighValue;
				}
				else {
					return mMidValue;
				}
			}
			if (i_event.mKeyboard.mKey == mHighKey) {
				if (i_state.mKeyboard.isDown(mLowKey)) {
					return mLowValue;
				}
				else {
					return mMidValue;
				}
			}
		}
	}
	return NAN;
}
