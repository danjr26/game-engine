#include "async_task_manager.h"
#include "log.h"
#include "packaged_async_task.h"

AsyncTaskManager::AsyncTaskManager(const Clock& i_clock, uint i_nThreads) :
mClock(i_clock),
mThreads(),
mTasks(),
mTerminate(false) {
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	i_nThreads = (i_nThreads == 0) ? std::thread::hardware_concurrency() : i_nThreads;
	mThreads.reserve(i_nThreads);
	for (uint i = 0; i < i_nThreads; i++) {
		mThreads.push_back(new std::thread(callThreadEntry, std::ref(*this)));
	}
}

AsyncTaskManager::~AsyncTaskManager() {
	terminate();
	while (mThreads.size() != 0) {
		if (mThreads[0]->joinable()) {
			mThreads[0]->join();
		}
		std::lock_guard<std::recursive_mutex> lock(mMutex);
		mThreads.erase(mThreads.begin());
	}
}

bool AsyncTaskManager::threadEntry() {
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

		task = get(taskIt);
		remove(taskIt);
	}
	task->run();
	return true;
}

AsyncTaskManager::TaskIterator AsyncTaskManager::Get_Next_Task(bool i_useEpsilon) {
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	double threshold;
	double now = mClock.now();
	TaskIterator it;
	for (it = mTasks.begin(); it != mTasks.end(); it++) {
		threshold = (i_useEpsilon) ? (*it)->mTime - (*it)->mEpsilon : (*it)->mTime;
		if (now >= threshold) {
			return it;
		}
	}
	return it;
}

void AsyncTaskManager::add(AsyncTask* i_task) {
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	for (auto it = mTasks.begin(); it != mTasks.end(); it++) {
		if (i_task->mTime >= (*it)->mTime) {
			mTasks.insert(it, i_task);
			return;
		}
	}
	mTasks.push_back(i_task);
}

void AsyncTaskManager::remove(TaskIterator i_it) {
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	mTasks.erase(i_it);
}

AsyncTask* AsyncTaskManager::get(TaskIterator i_it) {
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	return (*i_it);
}

void AsyncTaskManager::terminate() {
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	mTerminate = true;
}

bool AsyncTaskManager::isTerminating() const {
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	return mTerminate;
}

void AsyncTaskManager::helpUntilEmpty() {
	while (threadEntry());
}

void AsyncTaskManager::callThreadEntry(AsyncTaskManager& i_handler) {
	while (!i_handler.mTerminate) {
		if (!i_handler.threadEntry()) {
			std::this_thread::sleep_for(std::chrono::microseconds(10));
		}
	}
}
