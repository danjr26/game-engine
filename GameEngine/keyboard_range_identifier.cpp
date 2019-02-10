#include "keyboard_range_identifier.h"

KeyboardRangeIdentifier::KeyboardRangeIdentifier(Key in_lowKey, Key in_highKey,
	float in_lowValue, float in_midValue, float in_highValue, bool in_acceptRepeat) :
	mLowKey(in_lowKey),
	mHighKey(in_highKey),
	mLowValue(in_lowValue),
	mMidValue(in_midValue),
	mHighValue(in_highValue),
	mAcceptRepeat(in_acceptRepeat)
{}

float KeyboardRangeIdentifier::operator()(const RawInputEvent& in_event, const RawInputState& in_state) const {
	if ((in_event.mKeyboard.mRepeat) ? mAcceptRepeat : true) {
		if (in_event.mType == RawInputEvent::Type::key_down) {
			if (in_event.mKeyboard.mKey == mLowKey) {
				return mLowValue;
			}
			if (in_event.mKeyboard.mKey == mHighKey) {
				return mHighValue;
			}
		}
		if (in_event.mType == RawInputEvent::Type::key_up) {
			if (in_event.mKeyboard.mKey == mLowKey) {
				if (in_state.mKeyboard.isDown(mHighKey)) {
					return mHighValue;
				}
				else {
					return mMidValue;
				}
			}
			if (in_event.mKeyboard.mKey == mHighKey) {
				if (in_state.mKeyboard.isDown(mLowKey)) {
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
