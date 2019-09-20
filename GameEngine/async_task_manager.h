#ifndef ASYNC_TASK_HANDLER
#define ASYNC_TASK_HANDLER

#include "async_task.h"
#include "definitions.h"
#include "clock.h"
#include <thread>
#include <vector>
#include <mutex>
#include <list>
#include <functional>

class AsyncTaskManager {
public:
	using TaskIterator = typename std::list<AsyncTask*>::iterator;
private:
	mutable std::recursive_mutex mMutex;
	const Clock& mClock;
	std::vector<std::thread*> mThreads;
	std::list<AsyncTask*> mTasks;
	bool mTerminate;

private:
	 TaskIterator Get_Next_Task(bool in_useEpsilon);

public:
	AsyncTaskManager(const Clock& in_clock, uint in_nThreads = 0);
	~AsyncTaskManager();
	bool threadEntry();
	void add(AsyncTask* in_task);
	void remove(TaskIterator in_it);
	AsyncTask* get(TaskIterator in_it);
	void terminate();
	bool isTerminating() const;
	void helpUntilEmpty();
	void helpUntil(const std::function<bool()>& in_whenFinished);

	static void callThreadEntry(AsyncTaskManager& in_handler);
};

#endif