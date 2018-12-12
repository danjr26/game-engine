#ifndef TRANSITIONER_H
#define TRANSITIONER_H

#include "misc.h"
#include "definitions.h"

template<class ValT, class TimeT = double>
struct Transitioner {
	struct Key {
		ValT value;
		TimeT duration;
	};

	std::vector<Key> keys;

	virtual ValT Evaluate(TimeT in_time) = 0;

	TimeT Get_Total_Duration() {
		TimeT sum = 0;
		for (auto it = keys.begin(); it != keys.end(); it++) {
			sum += it->duration;
		}
		return sum;
	}
};

#endif