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
	 TaskIterator Get_Next_Task(bool i_useEpsilon);

public:
	AsyncTaskManager(const Clock& i_clock, uint i_nThreads = 0);
	~AsyncTaskManager();
	bool threadEntry();
	void add(AsyncTask* i_task);
	void remove(TaskIterator i_it);
	AsyncTask* get(TaskIterator i_it);
	void terminate();
	bool isTerminating() const;
	void helpUntilEmpty();
	void helpUntil(const std::function<bool()>& i_whenFinished);

	static void callThreadEntry(AsyncTaskManager& i_handler);
};

#endif