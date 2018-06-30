#include "stepper.h"

Stepper::Stepper(double in_step) :
	step(in_step),
	accumulation(0.0) 
{}

Stepper::Stepper(double in_step, double in_accum) :
	step(in_step),
	accumulation(in_accum)
{}

Stepper::~Stepper()
{}

int Stepper::Step_Number(double in_newAccumulation) {
	accumulation = accumulation.load() + in_newAccumulation;
	if (accumulation >= step) {
		int nStepsPassed;
		if (step != 0.0) {
			nStepsPassed = (int)(accumulation / step);
			accumulation = accumulation.load() - step * nStepsPassed;
		}
		else {
			nStepsPassed = 1;
			accumulation = 0.0f;
		}
		return nStepsPassed;
	}
	return 0;
}

double Stepper::Step_Total(double in_newAccumulation) {
	return Step_Number(in_newAccumulation) * step;
}