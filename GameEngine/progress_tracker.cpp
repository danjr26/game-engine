#include "progress_tracker.h"

ProgressTracker::ProgressTracker(double in_maxAccumulation, bool in_capAtMax)  :
	accumulation	(0.0),
	maxAccumulation	(in_maxAccumulation),
	capAtMax		(in_capAtMax)
{}

void ProgressTracker::Update(double in_newAccumulation) {
	accumulation += in_newAccumulation;
	if (capAtMax)
		accumulation = Clamp(accumulation, 0.0, maxAccumulation);
}

double ProgressTracker::Progress() const {
	return accumulation / maxAccumulation;
}

double ProgressTracker::Inv_Progress() const {
	return 1.0 - accumulation / maxAccumulation;
}

bool ProgressTracker::Is_Done() const {
	return accumulation >= maxAccumulation;
}