#ifndef LINEAR_TRANSITIONER_H
#define LINEAR_TRANSITIONER_H

#include "transitioner.h"

template<class ValT, class TimeT = double>
struct LinearTransitioner : public Transitioner<ValT, TimeT> {
	ValT evaluate(TimeT i_time) override {
		using Transitioner<ValT, TimeT>::mKeys;
		if (mKeys.empty()) fail();

		if (i_time < 0) {
			return mKeys.front().mValue;
		}

		TimeT t = 0;
		for (uint i = 0; i < mKeys.size() - 1; i++) {
			t += mKeys[i].mDuration;
			if (t >= i_time) {
				TimeT interT = (i_time - (t - mKeys[i].mDuration)) / (mKeys[i].mDuration);
				return mKeys[i].mValue * ((TimeT)1 - interT) + mKeys[i + 1].mValue * interT;
			}
		}

		return mKeys.back().mValue;
	}
};

#endif