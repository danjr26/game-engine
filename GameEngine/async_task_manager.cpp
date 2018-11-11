#include "async_task_manager.h"
#include "log.h"
#include "packaged_async_task.h"

AsyncTaskManager::AsyncTaskManager(const Clock& in_clock, uint in_nThreads) :
clock(in_clock),
threads(),
tasks(),
terminate(false) {
	std::lock_guard<std::recursive_mutex> lock(mutex);
	in_nThreads = (in_nThreads == 0) ? std::thread::hardware_concurrency() : in_nThreads;
	threads.reserve(in_nThreads);
	for (uint i = 0; i < in_nThreads; i++) {
		threads.push_back(new std::thread(Call_Thread_Entry, std::ref(*this)));
	}
}

AsyncTaskManager::~AsyncTaskManager() {
	Terminate();
	while (threads.size() != 0) {
		if (threads[0]->joinable()) {
			threads[0]->join();
		}
		std::lock_guard<std::recursive_mutex> lock(mutex);
		threads.erase(threads.begin());
	}
}

bool AsyncTaskManager::Thread_Entry() {
	TaskIterator taskIt;
	AsyncTask* task = nullptr;
	{
		std::lock_guard<std::recursive_mutex> lock(mutex);

		taskIt = Get_Next_Task(false);
		if (taskIt == tasks.end()) {
			taskIt = Get_Next_Task(true);
			if (taskIt == tasks.end()) {
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
	std::lock_guard<std::recursive_mutex> lock(mutex);
	double threshold;
	double now = clock.Now();
	TaskIterator it;
	for (it = tasks.begin(); it != tasks.end(); it++) {
		threshold = (in_useEpsilon) ? (*it)->time - (*it)->epsilon : (*it)->time;
		if (now >= threshold) {
			return it;
		}
	}
	return it;
}

void AsyncTaskManager::Add(AsyncTask* in_task) {
	std::lock_guard<std::recursive_mutex> lock(mutex);
	for (auto it = tasks.begin(); it != tasks.end(); it++) {
		if (in_task->time >= (*it)->time) {
			tasks.insert(it, in_task);
			return;
		}
	}
	tasks.push_back(in_task);
}

void AsyncTaskManager::Remove(TaskIterator in_it) {
	std::lock_guard<std::recursive_mutex> lock(mutex);
	tasks.erase(in_it);
}

AsyncTask* AsyncTaskManager::Get(TaskIterator in_it) {
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return (*in_it);
}

void AsyncTaskManager::Terminate() {
	std::lock_guard<std::recursive_mutex> lock(mutex);
	terminate = true;
}

bool AsyncTaskManager::Is_Terminating() const {
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return terminate;
}

void AsyncTaskManager::Help_Until_Empty() {
	while (Thread_Entry());
}

void AsyncTaskManager::Call_Thread_Entry(AsyncTaskManager& in_handler) {
	while (!in_handler.terminate) {
		if (!in_handler.Thread_Entry()) {
			std::this_thread::sleep_for(std::chrono::microseconds(10));
		}
	}
}
