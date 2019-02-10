#include "stepper.h"

Stepper::Stepper(double in_step) :
	mStep(in_step),
	mAccum(0.0) 
{}

Stepper::Stepper(double in_step, double in_accum) :
	mStep(in_step),
	mAccum(in_accum)
{}

Stepper::~Stepper()
{}

int Stepper::stepNumber(double in_newAccumulation) {
	mAccum = mAccum.load() + in_newAccumulation;
	if (mAccum >= mStep) {
		int nStepsPassed;
		if (mStep != 0.0) {
			nStepsPassed = (int)(mAccum / mStep);
			mAccum = mAccum.load() - mStep * nStepsPassed;
		}
		else {
			nStepsPassed = 1;
			mAccum = 0.0f;
		}
		return nStepsPassed;
	}
	return 0;
}

double Stepper::stepTotal(double in_newAccumulation) {
	return stepNumber(in_newAccumulation) * mStep;
}