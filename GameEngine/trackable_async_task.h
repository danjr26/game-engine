#ifndef TRACKABLE_ASYNC_TASK
#define TRACKABLE_ASYNC_TASK

#include "definitions.h"
#include "async_task.h"
#include <atomic>

template<class T>
class TrackableAsyncTask : public AsyncTask {
private:
	std::atomic<bool> mHasChanged;
	std::atomic<T> mProgress;

private:
	void Set_Progress(T in_progress);

public:
	TrackableAsyncTask(double in_time = 0.0, double in_epsilon = 0.0);
	bool Has_Changed();
	T Get_Progress();
};

#endif