#ifndef PROGRESS_TRACKER_H
#define PROGRESS_TRACKER_H

class ProgressTracker {
public:
	double	accumulation;
	double	maxAccumulation;
	bool	capAtMax;

	ProgressTracker	(double in_maxAccumulation, bool in_capAtMax);
	void	Update			(double in_newAccumulation);
	double	Progress		() const;
	double	Inv_Progress	() const;
	bool	Is_Done			() const;

	template<class T>
	T Lin_Interp(T in_a, T in_b) const {
		double progress = Progress(); 
		double inverseProgress = 1.0 - progress;
		return (T)(in_a * inverseProgress + in_b * progress);
	} 
};

#endif