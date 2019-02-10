#ifndef HARD_TRANSITIONER_H
#define HARD_TRANSITIONER_H

#include "transitioner.h"
#include "exceptions.h"

template<class ValT, class TimeT = double>
struct HardTransitioner : Transitioner<ValT, TimeT> {
	ValT evaluate(TimeT in_time) override {
		if (keys.empty()) {
			throw ProcessFailureException();
		}

		if (in_time < 0) {
			return keys.front().value;
		}

		TimeT t = 0;
		for (auto it = keys.begin(); it != keys.end(); it++) {
			t += it->duration;
			if (t >= in_time) {
				return it->value;
			}
		}

		return keys.back().value;
	}
};
	
#endif