#ifndef CATMULL_ROM_TRANSITIONER_H
#define CATMULL_ROM_TRANSITIONER_H

#include "transitioner.h"
#include "vector.h"
#include "misc.h"

template<class ValT, uint n, class TimeT = double>
struct CatmullRomTransitioner : Transitioner<Vector<ValT, n>, TimeT> {
	Vector<ValT, n> Evaluate(TimeT in_time) override {
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

				Vector<ValT, n> const& vectors[4] = {
					(i == 0) ? keys[i].value : keys[i - 1].value,
					keys[i].value,
					keys[i + 1].value,
					(i == keys.size() - 2) ? keys[i + 1].value : keys[i + 2].value
				};
					
				TimeT ts[4];
				ts[0] = (TimeT)0;
				for (uint j = 1; j < 4; j++) {
					ts[j] = ts[j - 1] + sqrt((vectors[j] - vectors[j - 1]).Magnitude());
				}

				interT = (interT * (ts[2] - ts[1])) + ts[1];

				Vector<ValT, n> reduced1[3] = {
					vectors[0] * Inv_Lerp(ts[1], ts[0], t) + vectors[1] * Inv_Lerp(ts[0], ts[1], t),
					vectors[1] * Inv_Lerp(ts[2], ts[1], t) + vectors[2] * Inv_Lerp(ts[1], ts[2], t),
					vectors[2] * Inv_Lerp(ts[3], ts[2], t) + vectors[3] * Inv_Lerp(ts[2], ts[3], t)
				};

				Vector<ValT, n> reduced2[2] = {
					reduced1[0] * Inv_Lerp(ts[2], ts[0], t) + reduced1[1] * Inv_Lerp(ts[0], ts[2], t),
					reduced1[1] * Inv_Lerp(ts[3], ts[1], t) + reduced1[2] * Inv_Lerp(ts[1], ts[3], t)
				};

				Vector<ValT, n> reduced3 = 
					reduced2[0] * Inv_Lerp(ts[2], ts[1], t) + reduced2[1] * Inv_Lerp(ts[1], ts[2], t);

				return reduced3;
			}
		}

		return keys.back().value;
	}
};

#endif