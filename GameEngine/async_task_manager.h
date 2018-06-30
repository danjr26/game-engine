#ifndef ASYNC_TASK_HANDLER
#define ASYNC_TASK_HANDLER

#include "async_task.h"
#include "definitions.h"
#include "clock.h"
#include <thread>
#include <vector>
#include <mutex>

class AsyncTaskManager {
private:
	mutable std::recursive_mutex mutex;
	const Clock& clock;
	std::vector<std::thread*> threads;
	std::vector<AsyncTask*> tasks;
	bool terminate;

private:
	 int Get_Next_Task_Index(bool in_useEpsilon);

public:
	AsyncTaskManager(const Clock& in_clock, uint in_nTasks = 16, uint in_nThreads = 0);
	~AsyncTaskManager();
	void Thread_Entry();
	void Add(AsyncTask* in_task);
	void Remove(uint in_index);
	AsyncTask* Get(uint in_index);
	void Terminate();
	bool Is_Terminating() const;

	static void Call_Thread_Entry(AsyncTaskManager& in_handler);
};

#endif