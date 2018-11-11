#include "async_task.h"
#include "log.h"

AsyncTask::AsyncTask(double in_time, double in_epsilon) :
	time(in_time),
	epsilon(in_epsilon),
	state(State::notRun)
{}

AsyncTask::State AsyncTask::Get_State() const {
	return state;
}
void AsyncTask::Wait_For_Finish() {
	while (state != finished) {
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}
}
void AsyncTask::Run() {
	state = running;
	_Run();
	state = finished;
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