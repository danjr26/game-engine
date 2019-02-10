#include "async_task_manager.h"
#include "log.h"
#include "packaged_async_task.h"

AsyncTaskManager::AsyncTaskManager(const Clock& in_clock, uint in_nThreads) :
mClock(in_clock),
mThreads(),
mTasks(),
mTerminate(false) {
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	in_nThreads = (in_nThreads == 0) ? std::thread::hardware_concurrency() : in_nThreads;
	mThreads.reserve(in_nThreads);
	for (uint i = 0; i < in_nThreads; i++) {
		mThreads.push_back(new std::thread(Call_Thread_Entry, std::ref(*this)));
	}
}

AsyncTaskManager::~AsyncTaskManager() {
	Terminate();
	while (mThreads.size() != 0) {
		if (mThreads[0]->joinable()) {
			mThreads[0]->join();
		}
		std::lock_guard<std::recursive_mutex> lock(mMutex);
		mThreads.erase(mThreads.begin());
	}
}

bool AsyncTaskManager::Thread_Entry() {
	TaskIterator taskIt;
	AsyncTask* task = nullptr;
	{
		std::lock_guard<std::recursive_mutex> lock(mMutex);

		taskIt = Get_Next_Task(false);
		if (taskIt == mTasks.end()) {
			taskIt = Get_Next_Task(true);
			if (taskIt == mTasks.end()) {
				return false;
			}
		}

		task = Get(taskIt);
		Remove(taskIt);
	}
	task->Run();
	return true;
}

AsyncTaskManager::TaskIterator AsyncTaskManager::Get_Next_Task(bool in_useEpsilon) {
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	double threshold;
	double now = mClock.Now();
	TaskIterator it;
	for (it = mTasks.begin(); it != mTasks.end(); it++) {
		threshold = (in_useEpsilon) ? (*it)->mTime - (*it)->mEpsilon : (*it)->mTime;
		if (now >= threshold) {
			return it;
		}
	}
	return it;
}

void AsyncTaskManager::Add(AsyncTask* in_task) {
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	for (auto it = mTasks.begin(); it != mTasks.end(); it++) {
		if (in_task->mTime >= (*it)->mTime) {
			mTasks.insert(it, in_task);
			return;
		}
	}
	mTasks.push_back(in_task);
}

void AsyncTaskManager::Remove(TaskIterator in_it) {
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	mTasks.erase(in_it);
}

AsyncTask* AsyncTaskManager::Get(TaskIterator in_it) {
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	return (*in_it);
}

void AsyncTaskManager::Terminate() {
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	mTerminate = true;
}

bool AsyncTaskManager::Is_Terminating() const {
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	return mTerminate;
}

void AsyncTaskManager::Help_Until_Empty() {
	while (Thread_Entry());
}

void AsyncTaskManager::Call_Thread_Entry(AsyncTaskManager& in_handler) {
	while (!in_handler.mTerminate) {
		if (!in_handler.Thread_Entry()) {
			std::this_thread::sleep_for(std::chrono::microseconds(10));
		}
	}
}
