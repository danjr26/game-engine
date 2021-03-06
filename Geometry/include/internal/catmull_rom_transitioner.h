#ifndef CATMULL_ROM_TRANSITIONER_H
#define CATMULL_ROM_TRANSITIONER_H

#include "transitioner.h"

template<class ValT, uint n, class TimeT = double>
struct CatmullRomTransitioner : public Transitioner<Vector<ValT, n>, TimeT> {
	Vector<ValT, n> evaluate(TimeT i_time) override {
		using Transitioner<Vector<ValT, n>, TimeT>::mKeys;
		if (mKeys.empty()) fail();

		if (i_time < 0) {
			return mKeys.front().value;
		}

		TimeT t = 0;
		for (uint i = 0; i < mKeys.size() - 1; i++) {
			t += mKeys[i].duration;
			if (t >= i_time) {
				TimeT interT = (i_time - (t - mKeys[i].duration)) / (mKeys[i].duration);

				Vector<ValT, n> const& vectors[4] = {
					(i == 0) ? mKeys[i].value : mKeys[i - 1].value,
					mKeys[i].value,
					mKeys[i + 1].value,
					(i == mKeys.size() - 2) ? mKeys[i + 1].value : mKeys[i + 2].value
				};
					
				TimeT ts[4];
				ts[0] = (TimeT)0;
				for (uint j = 1; j < 4; j++) {
					ts[j] = ts[j - 1] + sqrt((vectors[j] - vectors[j - 1]).magnitude());
				}

				interT = (interT * (ts[2] - ts[1])) + ts[1];

				Vector<ValT, n> reduced1[3] = {
					vectors[0] * invLerp(ts[1], ts[0], t) + vectors[1] * invLerp(ts[0], ts[1], t),
					vectors[1] * invLerp(ts[2], ts[1], t) + vectors[2] * invLerp(ts[1], ts[2], t),
					vectors[2] * invLerp(ts[3], ts[2], t) + vectors[3] * invLerp(ts[2], ts[3], t)
				};

				Vector<ValT, n> reduced2[2] = {
					reduced1[0] * invLerp(ts[2], ts[0], t) + reduced1[1] * invLerp(ts[0], ts[2], t),
					reduced1[1] * invLerp(ts[3], ts[1], t) + reduced1[2] * invLerp(ts[1], ts[3], t)
				};

				Vector<ValT, n> reduced3 = 
					reduced2[0] * invLerp(ts[2], ts[1], t) + reduced2[1] * invLerp(ts[1], ts[2], t);

				return reduced3;
			}
		}

		return mKeys.back().value;
	}
};

#endif