#ifndef ASYNC_TASK_H
#define ASYNC_TASK_H

#include <thread>
#include <atomic>

class AsyncTaskManager;

class AsyncTask {
	friend class AsyncTaskManager;
public:
	enum State {
		notRun,
		running,
		finished
	};
private:
	double mTime;
	double mEpsilon;
	std::atomic<State> mState;

public:
	AsyncTask(double in_time = 0.0, double in_epsilon = 0.0);
	State Get_State() const;
	void Wait_For_Finish();

private:
	void Run();
protected:
	virtual void _Run() = 0;
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