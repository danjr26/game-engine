#ifndef TRANSITIONER_H
#define TRANSITIONER_H

#include "misc.h"
#include "definitions.h"

template<class ValT, class TimeT = double>
struct Transitioner {
	struct Key {
		ValT mValue;
		TimeT mDuration;
	};

	std::vector<Key> mKeys;

	virtual ValT evaluate(TimeT in_time) = 0;

	TimeT getTotalDuration() {
		TimeT sum = 0;
		for (auto it = mKeys.begin(); it != mKeys.end(); it++) {
			sum += it->duration;
		}
		return sum;
	}
};

#endif