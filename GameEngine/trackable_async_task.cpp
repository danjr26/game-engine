#include "trackable_async_task.h"

template<class T>
void TrackableAsyncTask<T>::Set_Progress(T in_progress) {}

template<class T>
TrackableAsyncTask<T>::TrackableAsyncTask(double in_time, double in_epsilon) :
AsyncTask(in_time, in_epsilon),
mHasChanged(false),
mProgress(0)
{}

template<class T>
bool TrackableAsyncTask<T>::hasChanged() {
	return mHasChanged.load();
}

template<class T>
T TrackableAsyncTask<T>::getProgress() {
	return mProgress.load();
}
