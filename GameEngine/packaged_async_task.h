#ifndef PACKAGED_ASYNC_TASK_H
#define PACKAGED_ASYNC_TASK_H

#include "async_task.h"
#include <functional>

template<class T>
class PackagedAsyncTask : public AsyncTask {
private:
	std::function<T(void)> func;

public:
	PackagedAsyncTask(std::function<T(void)> in_function) :
		func(in_function)
	{}
	
protected:
	void Run() override final {
		func();
	}
};

#endif
