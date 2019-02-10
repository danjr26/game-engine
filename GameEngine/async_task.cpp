#include "async_task.h"
#include "log.h"

AsyncTask::AsyncTask(double in_time, double in_epsilon) :
	mTime(in_time),
	mEpsilon(in_epsilon),
	mState(State::notRun)
{}

AsyncTask::State AsyncTask::getState() const {
	return mState;
}
void AsyncTask::waitForFinish() {
	while (mState != finished) {
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}
}
void AsyncTask::run() {
	mState = running;
	_run();
	mState = finished;
}
/*

TestAsyncTask::TestAsyncTask(std::string in_message, double in_time, double in_epsilon) :
	AsyncTask(in_time, in_epsilon),
	message(in_message)
{}

void TestAsyncTask::run() {
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	Log::main(message);
}
*/