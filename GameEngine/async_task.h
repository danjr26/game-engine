#ifndef ASYNC_TASK_H
#define ASYNC_TASK_H

#include <thread>

class AsyncTaskManager;

class AsyncTask {
	friend class AsyncTaskManager;
private:
	double time;
	double epsilon;

public:
	AsyncTask(double in_time = 0.0, double in_epsilon = 0.0);

protected:
	virtual void Run() = 0;
};

/*
class TestAsyncTask : public AsyncTask {
public:
	std::string message;

	TestAsyncTask(std::string in_message, double in_time = 0.0, double in_epsilon = 0.0);

protected:
	void Run() override final;
};
*/

#endif