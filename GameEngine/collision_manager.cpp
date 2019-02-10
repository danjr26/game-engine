#include "collision_manager.h"

template<class T>
CollisionManager<T>::CollisionManager() :
	mContext2s(),
	mActiveContext2(nullptr)
{}

template<class T>
void CollisionManager<T>::update() {
	for (auto it = mContext2s.begin(); it < mContext2s.end(); it++) {
		(*it)->update();
	}

/*	
	for (auto it = context3s.begin(); it < context3s.end(); it++) {
		(*it)->update();
	}
*/
}

template<class T>
void CollisionManager<T>::add(CollisionContext<T, 2>* in_context) {
	if (mActiveContext2 == nullptr) {
		mActiveContext2 = in_context;
	}

	mContext2s.push_back(in_context);
}

/*
void CollisionManager<T>::add(CollisionContext3* in_context) {
	context3s.push_back(in_context);
}
*/

template<class T>
void CollisionManager<T>::remove(CollisionContext<T, 2>* in_context) {
	auto position = std::find(mContext2s.begin(), mContext2s.end(), in_context);
	if (position != mContext2s.end()) {
		mContext2s.erase(position);
	}

	if (mActiveContext2 == in_context) {
		mActiveContext2 = nullptr;
	}
}

template<class T>
bool CollisionManager<T>::has(CollisionContext<T, 2>* in_context) {
	return std::find(mContext2s.begin(), mContext2s.end(), in_context) != mContext2s.end();
}

template<class T>
void CollisionManager<T>::makeActive(CollisionContext<T, 2>* in_context) {
	if (!has(in_context)) {
		add(in_context);
	}
	mActiveContext2 = in_context;
}

template<class T>
CollisionContext<T, 2>* CollisionManager<T>::getActive2() {
	return mActiveContext2;
}

/*
void CollisionManager<T>::remove(CollisionContext3* in_context) {
	auto position = std::find(context3s.begin(), context3s.end(), in_context);
	if (position != context3s.end()) {
		context3s.erase(position);
	}
}
*/

template class CollisionManager<float>;
template class CollisionManager<double>;