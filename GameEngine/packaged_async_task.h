#ifndef PACKAGED_ASYNC_TASK_H
#define PACKAGED_ASYNC_TASK_H

#include "async_task.h"
#include <functional>

template<class T>
class PackagedAsyncTask : public AsyncTask {
private:
	const std::function<T(void)> mFunc;

public:
	PackagedAsyncTask(const std::function<T(void)>& in_function) :
		mFunc(in_function)
	{}

	PackagedAsyncTask(const PackagedAsyncTask<T>& in_other) :
		mFunc(in_other.mFunc) 
	{}
	
	bool Is_Valid() {
		return bool(mFunc);
	}

protected:
	void _Run() override final {
		if (!mFunc) {
			throw InvalidArgumentException();
		}
		mFunc();
	}
};

#endif
