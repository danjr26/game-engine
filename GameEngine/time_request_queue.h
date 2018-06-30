#ifndef TRQ_H
#define TRQ_H

#include <thread>
#include "clock.h"
#include "misc.h"
#include "mutex_protected.h"

class TimeRequestQueue;

class TimeRequest : protected MutexProtected {
	friend class TimeRequestQueue;
public:
	double	step;
	Clock*	clock;

protected:
	uint	trqindex;
	double	timeleft;
	bool	paused;


public:
			TimeRequest		(Clock* in_clock, double in_step);
	virtual	~TimeRequest	();
	void	Pause			();
	void	Resume			();
protected:
	virtual void Activate	();

};

class TimeRequestQueue : public MutexProtected {
private:	
	IndexedArray<TimeRequest> requests;

	std::thread**	threads;  
	uint			nthreads;

	double			lasttime;

public:
			TimeRequestQueue	();
			~TimeRequestQueue	();
	void	Add					(TimeRequest* request);
	void	Remove				(TimeRequest* request);
	void	Update_One			();

	static void Updater_Thread_Entry();
};

class MechTR : public TimeRequest {
private:
	MechanicalComponent*	component;

public:
	MechTR(Clock* in_clock, MechanicalComponent* component, double step);
	virtual void Activate();
};

class RenderTR : public TimeRequest {
public:
	RenderTR(Clock* in_clock, double step);
	virtual void Activate();
};

class CollisionTR : public TimeRequest {
public:
	CollisionTR(Clock* in_clock, double t);
	virtual void Activate();
};

class AITR : public TimeRequest {
private:
	AIComponent* component;

public:
	AITR(Clock* in_clock, AIComponent* component, double t);
	virtual void Activate();
};

class InputTR : public TimeRequest {
public:
	InputTR(Clock* in_clock, double t);
	void Activate();
};

#endif