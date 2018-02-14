#include "trq.h"
#include "framework.h"

TimeRequestQueue::TimeRequestQueue() :
requests(4096, offsetof(TimeRequest, trqindex)),
lasttime(0.0) {
	std::lock_guard<std::mutex> lock(mutex);
	nthreads = 2;
	threads = new thread*[nthreads];
	//for (int i = 0; i < nthreads; i++)
	//	threads[i] = new thread(Updater_Thread_Entry);
}

TimeRequestQueue::~TimeRequestQueue() {
	std::lock_guard<std::mutex> lock(mutex);
	for (int i = 0; i < nthreads; i++)
		delete threads[i];
	delete[] threads;
}

void TimeRequestQueue::Add(TimeRequest* r) {
	std::lock_guard<std::mutex> lock(mutex);
	for (int i = 0; i < requests.Size(); i++) {
		if (requests[i]->timeleft > r->timeleft) {
			requests.Insert(r, i);
			return;
		}
	}
	requests.Add(r);
}

void TimeRequestQueue::Remove(TimeRequest* r) {
	std::lock_guard<std::mutex> lock(mutex);
	requests.Remove(r);
}

void TimeRequestQueue::Update_One() {
	mutex.lock();

	if (lasttime == 0.0)
		lasttime = GEngine::Get().Clock().Now();
	double now = GEngine::Get().Clock().Now();
	double timepassed = now - lasttime;

	for (int i = 0; i < requests.Size(); i++) {
		if(!requests[i]->paused)
			requests[i]->timeleft -= timepassed;
	}

	for (int i = 0; i < requests.Size(); i++) {
		if (requests[i]->timeleft <= 0.0) {
			TimeRequest* timeRequest = requests[i];
			mutex.unlock();
			timeRequest->Activate();
			return;
		}
	}

	mutex.unlock();
	std::this_thread::yield();
}

void TimeRequestQueue::Updater_Thread_Entry() {
	while (GEngine::Exists())
		GEngine::Get().TRQ().Update_One();
}

TimeRequest::TimeRequest(Clock* in_clock, double in_step) :
clock	(in_clock),
step	(in_step),
timeleft(in_step),
paused	(false) {
	std::lock_guard<std::mutex> lock(mutex);
	GEngine::Get().TRQ().Add(this);
}

TimeRequest::~TimeRequest() {
//	std::lock_guard<std::mutex> lock(mutex);
	if (trqindex >= 0)
		GEngine::Get().TRQ().Remove(this);
}

void TimeRequest::Activate()
{}

void TimeRequest::Pause() {
	std::lock_guard<std::mutex> lock(mutex);
	paused = true;
}

void TimeRequest::Resume() {
	std::lock_guard<std::mutex> lock(mutex);
	paused = false;
}

MechTR::MechTR(Clock* in_clock, MechanicalComponent* component, double step) :
TimeRequest(step),
component(component)
{}

void MechTR::Activate() {
	std::lock_guard<std::mutex> lock(mutex);
	/*static int c = 1;
	static double s = 0;
	static double metas = 0;
	static double a = 0;
	if (GEngine::active->clock.Time() - metas > 1) {
	double per = a * 100;
	OutputDebugStringA((string("\nmech:\t") + to_string((int)(per + 0.5)) + "%").c_str());
	metas = GEngine::active->clock.Time();
	a = 0;
	c = 0;
	}
	c++;
	s = GEngine::active->clock.Time();
	*/
	GEngine::Get().TRQ().Remove(this);
	timeleft += step;
	if (timeleft <= 0.0) {
		
	}
	GEngine::Get().TRQ().Add(this);
	component->Update(step);

	//a += GEngine::active->clock.Time() - s;
}

RenderTR::RenderTR(double step) :
TimeRequest(step)
{}

void RenderTR::Activate() {
	std::lock_guard<std::mutex> lock(mutex);
	static int c = 1;
	static double metas = 0;
	if (GEngine::Get().Clock().Now() - metas >= 1.0) {
		OutputDebugStringA((to_string(c) + " fps\count").c_str());
		metas = GEngine::Get().Clock().Now();
		c = 0;
	}
	c++;

	GEngine::Get().Render().Render_Frame();
	GEngine::Get().TRQ().Remove(this);
	timeleft += step;
	GEngine::Get().TRQ().Add(this);
}

CollisionTR::CollisionTR(double t) :
TimeRequest(t)
{}

void CollisionTR::Activate() {
	std::lock_guard<std::mutex> lock(mutex);
	/*static int c = 1;
	static double s = 0;
	static double metas = 0;
	static double a = 0;
	if (GEngine::Get().Clock().Now() - metas > 1) {
	double per = a * 100;
	OutputDebugStringA((string("\ncoll:\t") + to_string((int)(per + 0.5)) + "%").c_str());
	metas = GEngine::Get().Clock().Now();
	a = 0;
	c = 0;
	}
	c++;
	s = GEngine::Get().Clock().Now();
	*/
	GEngine::Get().TRQ().Remove(this);
	timeleft += step;
	GEngine::Get().TRQ().Add(this);
	GEngine::Get().Collision().Check_Collisions();

	//a += GEngine::Get().Clock().Now() - s;
}

AITR::AITR(AIComponent* component, double t) :
TimeRequest(t),
component(component)
{}

void AITR::Activate() {
	std::lock_guard<std::mutex> lock(mutex);
	/*static int c = 1;
	static double s = 0;
	static double metas = 0;
	static double a = 0;
	if (GEngine::Get().Clock().Now() - metas > 1) {
	double per = a * 100;
	OutputDebugStringA((string("\nai:\t") + to_string((int)(per + 0.5)) + "%").c_str());
	metas = GEngine::Get().Clock().Now();
	a = 0;
	c = 0;
	}
	c++;
	s = GEngine::Get().Clock().Now();
	*/
	GEngine::Get().TRQ().Remove(this);
	timeleft += step;
	GEngine::Get().TRQ().Add(this);
	component->Refresh();

	//a += GEngine::Get().Clock().Now() - s;
}

InputTR::InputTR(double t) :
TimeRequest(t)
{}

void InputTR::Activate() {
	std::lock_guard<std::mutex> lock(mutex);
	/*static int c = 1;
	static double s = 0;
	static double metas = 0;
	static double a = 0;
	if (GEngine::Get().Clock().Now() - metas > 1) {
	double per = a * 100;
	OutputDebugStringA((string("\ninput:\t") + to_string((int)(per + 0.5)) + "%").c_str());
	metas = GEngine::Get().Clock().Now();
	a = 0;
	c = 0;
	}
	c++;
	s = GEngine::Get().Clock().Now();
	*/
	GEngine::Get().TRQ().Remove(this);
	timeleft += step;
	GEngine::Get().TRQ().Add(this);
	GEngine::Get().Input().Process_Input();
	//a += GEngine::Get().Clock().Now() - s;
}