#ifndef LINEAR_TRANSITIONER_H
#define LINEAR_TRANSITIONER_H

#include "transitioner.h"
#include "exceptions.h"

template<class ValT, class TimeT = double>
struct LinearTransitioner : Transitioner<ValT, TimeT> {
	ValT evaluate(TimeT in_time) override {
		if (mKeys.empty()) {
			throw ProcessFailureException();
		}

		if (in_time < 0) {
			return mKeys.front().mValue;
		}

		TimeT t = 0;
		for (uint i = 0; i < mKeys.size() - 1; i++) {
			t += mKeys[i].mDuration;
			if (t >= in_time) {
				TimeT interT = (in_time - (t - mKeys[i].mDuration)) / (mKeys[i].mDuration);
				return mKeys[i].mValue * ((TimeT)1 - interT) + mKeys[i + 1].mValue * interT;
			}
		}

		return mKeys.back().mValue;
	}
};

#endif