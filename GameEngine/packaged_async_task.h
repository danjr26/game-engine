#ifndef PACKAGED_ASYNC_TASK_H
#define PACKAGED_ASYNC_TASK_H

#include "async_task.h"
#include <functional>

template<class T>
class PackagedAsyncTask : public AsyncTask {
private:
	const std::function<T(void)> func;

public:
	PackagedAsyncTask(const std::function<T(void)>& in_function) :
		func(in_function)
	{}

	PackagedAsyncTask(const PackagedAsyncTask<T>& in_other) :
		func(in_other.func) 
	{}
	
	bool Is_Valid() {
		return bool(func);
	}

protected:
	void _Run() override final {
		if (!func) {
			throw InvalidArgumentException();
		}
		func();
	}
};

#endif
