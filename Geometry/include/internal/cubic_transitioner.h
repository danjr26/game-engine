#ifndef CUBIC_TRANSITIONER_H
#define CUBIC_TRANSITIONER_H

#include "transitioner.h"

template<class ValT, class TimeT = double>
struct CubicTransitioner : Transitioner<ValT, TimeT> {
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
				interT = interT * interT * (3 - 2 * interT);
				return (ValT)(mKeys[i].mValue * ((TimeT)1 - interT) + mKeys[i + 1].mValue * interT);
			}
		}

		return mKeys.back().mValue;
	}
};

#endif