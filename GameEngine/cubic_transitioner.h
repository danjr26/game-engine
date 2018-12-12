#ifndef CUBIC_TRANSITIONER_H
#define CUBIC_TRANSITIONER_H

#include "transitioner.h"

template<class ValT, class TimeT = double>
struct CubicTransitioner : Transitioner<ValT, TimeT> {
	ValT Evaluate(TimeT in_time) override {
		if (keys.empty()) {
			throw ProcessFailureException();
		}

		if (in_time < 0) {
			return keys.front().value;
		}

		TimeT t = 0;
		for (uint i = 0; i < keys.size() - 1; i++) {
			t += keys[i].duration;
			if (t >= in_time) {
				TimeT interT = (in_time - (t - keys[i].duration)) / (keys[i].duration);
				interT = interT * interT * (3 - 2 * interT);
				return keys[i].value * ((TimeT)1 - interT) + keys[i + 1].value * interT;
			}
		}

		return keys.back().value;
	}
};

#endif