#include "async_task.h"
#include "log.h"

AsyncTask::AsyncTask(double in_time, double in_epsilon) :
	time(in_time),
	epsilon(in_epsilon)
{}
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