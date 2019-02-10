#include "async_task.h"
#include "log.h"

AsyncTask::AsyncTask(double in_time, double in_epsilon) :
	mTime(in_time),
	mEpsilon(in_epsilon),
	mState(State::notRun)
{}

AsyncTask::State AsyncTask::Get_State() const {
	return mState;
}
void AsyncTask::Wait_For_Finish() {
	while (mState != finished) {
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}
}
void AsyncTask::Run() {
	mState = running;
	_Run();
	mState = finished;
}
/*

TestAsyncTask::TestAsyncTask(std::string in_message, double in_time, double in_epsilon) :
	AsyncTask(in_time, in_epsilon),
	message(in_message)
{}

void TestAsyncTask::Run() {
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	Log::main(message);
}
*/