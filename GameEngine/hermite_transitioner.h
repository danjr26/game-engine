#ifndef HERMITE_TRANSITIONER_H
#define HERMITE_TRANSITIONER_H

#include "transitioner.h"
#include "located_vector.h"
#include "vector.h"

template<class ValT, uint n, class TimeT = double>
struct HermiteTransitioner : Transitioner<LocatedVector<ValT, n>, TimeT> {
	LocatedVector<ValT, n> Evaluate(TimeT in_time) override {
		if (keys.empty()) {
			throw ProcessFailureException();
		}

		in_time = Clamp_Floor<TimeT>(in_time, 0);

		TimeT t = 0;
		for (uint i = 0; i < keys.size() - 1; i++) {
			t += keys[i].duration;
			if (t >= in_time) {
				TimeT interT = (in_time - (t - keys[i].duration)) / (keys[i].duration);
				TimeT interT2 = interT * interT;
				TimeT interT3 = interT2 * interT;

				LocatedVector<ValT, n> locatedVector;

				locatedVector.position = 
					keys[i].value.position * (2 * interT3 - 3 * interT2 + 1) +
					keys[i].value.vector * (interT3 - 2 * interT2 + interT) +
					keys[i + 1].value.position * (-2 * interT3 + 3 * interT2) +
					keys[i + 1].value.vector * (interT3 - interT2);

				locatedVector.vector = 
					keys[i].value.position * (6 * interT2 - 6 * interT) +
					keys[i].value.vector * (3 * interT2 - 4 * interT + 1) +
					keys[i + 1].value.position * (-6 * interT2 + 6 * interT) +
					keys[i + 1].value.vector * (3 * interT2 - 2 * interT);

				return locatedVector;
			}
		}

		return keys.back().value;
	}

	void Fill_Uniform_Durations(TimeT in_val = (TimeT)1) {
		for (auto it = keys.begin(); it != keys.end(); it++) {
			it->duration = in_val;
		}
	}

	void Fill_Distance_Durations() {
		for (uint i = 0; i < keys.size() - 1; i++) {
			keys[i].duration = (keys[i + 1].value.position - keys[i].value.position).Magnitude();
		}
		keys[keys.size() - 1].duration = (TimeT)0;
	}

	void Fill_Basic_Tangents() {
		if (keys.empty()) return;
		Fill_Basic_Tangents(keys.front().value.position, keys.back().value.position);
	}

	void Fill_Basic_Tangents(const Vector<ValT, n>& in_control1, const Vector<ValT, n>& in_control2) {
		if (keys.size() == 0) {
			return;
		}
		else if (keys.size() == 1) {
			keys[0].value.vector = Vector<ValT, n>();
		}

		for (uint i = 0; i < keys.size(); i++) {
			const Vector<ValT, n>& v0 = (i == 0) ? in_control1 : keys[i - 1].value.position;
			const Vector<ValT, n>& v1 = keys[i].value.position;
			const Vector<ValT, n>& v2 = (i == keys.size() - 1) ? in_control2 : keys[i + 1].value.position;
			TimeT t1 = (i == 0) ? keys[i].duration : keys[i - 1].duration;
			TimeT t2 = (i == keys.size() - 1) ? keys[i - 1].duration : keys[i].duration;
			//keys[i].value.vector = ((v1 - v0) + (v2 - v1)) / (TimeT)2;
			keys[i].value.vector = ((v1 - v0) + (v2 - v1)) / (TimeT)2;
		}
	}

	void Fill_Cardinal_Tangents(TimeT in_tension = (TimeT)0) {
		if (keys.empty()) return;
		Fill_Cardinal_Tangents(keys.front().value.position, keys.back().value.position, in_tension);
	}

	void Fill_Cardinal_Tangents(const Vector<ValT, n>& in_control1, const Vector<ValT, n>& in_control2, TimeT in_tension = (TimeT)0) {
		if (keys.size() == 0) {
			return;
		}
		else if (keys.size() == 1) {
			keys[0].value.vector = Vector<ValT, n>();
		}

		for (uint i = 0; i < keys.size(); i++) {
			const Vector<ValT, n>& v1 = (i == 0) ? in_control1 : keys[i - 1].value.position;
			const Vector<ValT, n>& v2 = (i == keys.size() - 1) ? in_control2 : keys[i + 1].value.position;
			TimeT t1 = (i == 0) ? keys[i].duration : keys[i - 1].duration;
			TimeT t2 = (i == keys.size() - 1) ? keys[i - 1].duration : keys[i].duration;
			//keys[i].value.vector = (v1 - v2) / 2 * ((TimeT)1 - in_tension);
			keys[i].value.vector = (v1 - v2) / (t1 + t2) * ((TimeT)1 - in_tension);
			//keys[i].value.vector = (v1 * t1 - v2 * t2) / (t1 + t2) / (t1 + t2) * ((TimeT)1 - in_tension);
		}
	}
};
	
#endif