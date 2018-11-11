#ifndef ASYNC_TASK_HANDLER
#define ASYNC_TASK_HANDLER

#include "async_task.h"
#include "definitions.h"
#include "clock.h"
#include <thread>
#include <vector>
#include <mutex>
#include <list>

class AsyncTaskManager {
public:
	using TaskIterator = typename std::list<AsyncTask*>::iterator;
private:
	mutable std::recursive_mutex mutex;
	const Clock& clock;
	std::vector<std::thread*> threads;
	std::list<AsyncTask*> tasks;
	bool terminate;

private:
	 TaskIterator Get_Next_Task(bool in_useEpsilon);

public:
	AsyncTaskManager(const Clock& in_clock, uint in_nThreads = 0);
	~AsyncTaskManager();
	bool Thread_Entry();
	void Add(AsyncTask* in_task);
	void Remove(TaskIterator in_it);
	AsyncTask* Get(TaskIterator in_it);
	void Terminate();
	bool Is_Terminating() const;
	void Help_Until_Empty();
	void Help_Until(const std::function<bool()>& in_whenFinished);

	static void Call_Thread_Entry(AsyncTaskManager& in_handler);
};

#endif