#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include "component.h"
#include "text.h"

//#define GENG_PERF

#ifdef GENG_PERF
class Profiler {
public:
	static double*		times;
	static string*		keys;
	static int			nkeys;
	static Profiler**	profs;
	static int			nprofs;

	string key;
	double time;

			Profiler	(string key);
			~Profiler	();
	void	End			();

	static void Write();
};
#endif

class FPSCounter : public MechanicalComponent {
public:
	Text2D*	text;
	Stepper	refreshstepper;
	double lasttime;
	double maxtime;


	FPSCounter			(double step);
	~FPSCounter			();
	virtual void Update	(double t);
};

#endif
