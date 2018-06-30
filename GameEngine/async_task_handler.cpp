#include "async_task_manager.h"
#include "log.h"

AsyncTaskManager::AsyncTaskManager(const Clock& in_clock, uint in_nTasks, uint in_nThreads) :
clock(in_clock),
threads(),
tasks(),
terminate(false) {
	std::lock_guard<std::recursive_mutex> lock(mutex);
	in_nThreads = (in_nThreads == 0) ? std::thread::hardware_concurrency() : in_nThreads;
	threads.reserve(in_nThreads);
	tasks.reserve(in_nTasks);
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

void AsyncTaskManager::Thread_Entry() {
	int taskIndex = -1;
	AsyncTask* task = nullptr;
	{
		std::lock_guard<std::recursive_mutex> lock(mutex);
		taskIndex = Get_Next_Task_Index(false);
		if (taskIndex < 0) {
			taskIndex = Get_Next_Task_Index(true);
			if (taskIndex < 0) {
				std::this_thread::sleep_for(std::chrono::microseconds(1));
				return;
			}
		}
		task = Get(taskIndex);
		Remove(taskIndex);
	}
	task->Run();
}

int AsyncTaskManager::Get_Next_Task_Index(bool in_useEpsilon) {
	std::lock_guard<std::recursive_mutex> lock(mutex);
	double threshold;
	double now = clock.Now();
	for (int i = 0; i < tasks.size(); i++) {
		threshold = (in_useEpsilon) ? tasks[i]->time - tasks[i]->epsilon : tasks[i]->time;
		if (now >= threshold) {
			return i;
		}
	}
	return -1;
}

void AsyncTaskManager::Add(AsyncTask* in_task) {
	std::lock_guard<std::recursive_mutex> lock(mutex);
	tasks.push_back(in_task);
}

void AsyncTaskManager::Remove(uint in_index) {
	std::lock_guard<std::recursive_mutex> lock(mutex);
	tasks.erase(tasks.begin() + in_index);
}

AsyncTask* AsyncTaskManager::Get(uint in_index) {
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return tasks[in_index];
}

void AsyncTaskManager::Terminate() {
	std::lock_guard<std::recursive_mutex> lock(mutex);
	terminate = true;
}

bool AsyncTaskManager::Is_Terminating() const {
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return terminate;
}

void AsyncTaskManager::Call_Thread_Entry(AsyncTaskManager& in_handler) {
	while (!in_handler.terminate) {
		in_handler.Thread_Entry();
	}
}
