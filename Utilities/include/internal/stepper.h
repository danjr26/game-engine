#ifndef STEPPER_H
#define STEPPER_H

template<class T>
struct Stepper {
	T mStep;
	T mAccum;

	Stepper(T i_step = 0, T i_accum = 0) :
	mStep(i_step),
	mAccum(i_accum)
	{}

	void step(T i_accum) {
		stepNumber(i_accum);
	}

	size_t stepNumber(T i_accum) {
		mAccum += i_accum;

		if (!mStep) return 0;

		size_t nSteps = 0;
		if (mStep) {
			nSteps = (size_t)(mAccum / mStep);
			mAccum -= mStep * nSteps;
		}
		else {
			nSteps = 1;
			mAccum = 0.0f;
		}
		return nStepsPassed;
	}

	T stepTotal(T i_accum) {
		return stepNumber(i_accum) * mStep;
	}
};

using Stepperf = Stepper<float>;
using Stepperd = Stepper<double>;

#endif