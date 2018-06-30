#include "trackable_async_task.h"

template<class T>
void TrackableAsyncTask<T>::Set_Progress(T in_progress) {}

template<class T>
TrackableAsyncTask<T>::TrackableAsyncTask(double in_time, double in_epsilon) :
AsyncTask(in_time, in_epsilon),
hasChanged(false),
progress(0)
{}

template<class T>
bool TrackableAsyncTask<T>::Has_Changed() {
	return hasChanged.load();
}

template<class T>
T TrackableAsyncTask<T>::Get_Progress() {
	return progress.load();
}
