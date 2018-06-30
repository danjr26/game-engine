#ifndef STEPPER_H
#define STEPPER_H

#include <atomic>

class Stepper {
public:
	std::atomic<double> step;
	std::atomic<double> accumulation;

	Stepper(double in_step);
	Stepper(double in_step, double in_accumulation);
	~Stepper();
	int	Step_Number(double in_newAccumulation);
	double Step_Total(double in_newAccumulation);
};

#endif