#ifndef HARD_TRANSITIONER_H
#define HARD_TRANSITIONER_H

#include "transitioner.h"

template<class ValT, class TimeT = double>
struct HardTransitioner : Transitioner<ValT, TimeT> {
	ValT evaluate(TimeT i_time) override {
		using Transitioner<ValT, TimeT>::mKeys;

		if (mKeys.empty()) fail();

		if (i_time < 0) {
			return mKeys.front().value;
		}

		TimeT t = 0;
		for (auto it = mKeys.begin(); it != mKeys.end(); it++) {
			t += it->duration;
			if (t >= i_time) {
				return it->value;
			}
		}

		return mKeys.back().value;
	}
};
	
#endif