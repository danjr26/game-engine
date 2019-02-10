#ifndef HERMITE_TRANSITIONER_H
#define HERMITE_TRANSITIONER_H

#include "transitioner.h"
#include "located_vector.h"
#include "vector.h"

template<class ValT, uint n, class TimeT = double>
struct HermiteTransitioner : Transitioner<LocatedVector<ValT, n>, TimeT> {
	LocatedVector<ValT, n> Evaluate(TimeT in_time) override {
		if (mKeys.empty()) {
			throw ProcessFailureException();
		}

		in_time = Clamp_Floor<TimeT>(in_time, 0);

		TimeT t = 0;
		for (uint i = 0; i < mKeys.size() - 1; i++) {
			t += mKeys[i].mDuration;
			if (t >= in_time) {
				TimeT interT = (in_time - (t - mKeys[i].mDuration)) / (mKeys[i].mDuration);
				TimeT interT2 = interT * interT;
				TimeT interT3 = interT2 * interT;

				LocatedVector<ValT, n> locatedVector;

				locatedVector.mPosition = 
					mKeys[i].mValue.mPosition * (2 * interT3 - 3 * interT2 + 1) +
					mKeys[i].mValue.mVector * (interT3 - 2 * interT2 + interT) +
					mKeys[i + 1].mValue.mPosition * (-2 * interT3 + 3 * interT2) +
					mKeys[i + 1].mValue.mVector * (interT3 - interT2);

				locatedVector.mVector = 
					mKeys[i].mValue.mPosition * (6 * interT2 - 6 * interT) +
					mKeys[i].mValue.mVector * (3 * interT2 - 4 * interT + 1) +
					mKeys[i + 1].mValue.mPosition * (-6 * interT2 + 6 * interT) +
					mKeys[i + 1].mValue.mVector * (3 * interT2 - 2 * interT);

				return locatedVector;
			}
		}

		return mKeys.back().mValue;
	}

	void Fill_Uniform_Durations(TimeT in_val = (TimeT)1) {
		for (auto it = mKeys.begin(); it != mKeys.end(); it++) {
			it->mDuration = in_val;
		}
	}

	void Fill_Distance_Durations() {
		for (uint i = 0; i < mKeys.size() - 1; i++) {
			mKeys[i].mDuration = (mKeys[i + 1].mValue.mPosition - mKeys[i].mValue.mPosition).Magnitude();
		}
		mKeys[mKeys.size() - 1].mDuration = (TimeT)0;
	}

	void Fill_Basic_Tangents() {
		if (mKeys.empty()) return;
		Fill_Basic_Tangents(mKeys.front().mValue.mPosition, mKeys.back().mValue.mPosition);
	}

	void Fill_Basic_Tangents(const Vector<ValT, n>& in_control1, const Vector<ValT, n>& in_control2) {
		if (mKeys.size() == 0) {
			return;
		}
		else if (mKeys.size() == 1) {
			mKeys[0].mValue.mVector = Vector<ValT, n>();
		}

		for (uint i = 0; i < mKeys.size(); i++) {
			const Vector<ValT, n>& v0 = (i == 0) ? in_control1 : mKeys[i - 1].mValue.mPosition;
			const Vector<ValT, n>& v1 = mKeys[i].mValue.mPosition;
			const Vector<ValT, n>& v2 = (i == mKeys.size() - 1) ? in_control2 : mKeys[i + 1].mValue.mPosition;
			TimeT t1 = (i == 0) ? mKeys[i].mDuration : mKeys[i - 1].mDuration;
			TimeT t2 = (i == mKeys.size() - 1) ? mKeys[i - 1].mDuration : mKeys[i].mDuration;
			//mKeys[i].mValue.mVector = ((v1 - v0) + (v2 - v1)) / (TimeT)2;
			mKeys[i].mValue.mVector = ((v1 - v0) + (v2 - v1)) / (TimeT)2;
		}
	}

	void Fill_Cardinal_Tangents(TimeT in_tension = (TimeT)0) {
		if (mKeys.empty()) return;
		Fill_Cardinal_Tangents(mKeys.front().mValue.mPosition, mKeys.back().mValue.mPosition, in_tension);
	}

	void Fill_Cardinal_Tangents(const Vector<ValT, n>& in_control1, const Vector<ValT, n>& in_control2, TimeT in_tension = (TimeT)0) {
		if (mKeys.size() == 0) {
			return;
		}
		else if (mKeys.size() == 1) {
			mKeys[0].mValue.mVector = Vector<ValT, n>();
		}

		for (uint i = 0; i < mKeys.size(); i++) {
			const Vector<ValT, n>& v1 = (i == 0) ? in_control1 : mKeys[i - 1].mValue.mPosition;
			const Vector<ValT, n>& v2 = (i == mKeys.size() - 1) ? in_control2 : mKeys[i + 1].mValue.mPosition;
			TimeT t1 = (i == 0) ? mKeys[i].mDuration : mKeys[i - 1].mDuration;
			TimeT t2 = (i == mKeys.size() - 1) ? mKeys[i - 1].mDuration : mKeys[i].mDuration;
			//mKeys[i].mValue.mVector = (v1 - v2) / 2 * ((TimeT)1 - in_tension);
			mKeys[i].mValue.mVector = (v1 - v2) / (t1 + t2) * ((TimeT)1 - in_tension);
			//mKeys[i].mValue.mVector = (v1 * t1 - v2 * t2) / (t1 + t2) / (t1 + t2) * ((TimeT)1 - in_tension);
		}
	}
};
	
#endif