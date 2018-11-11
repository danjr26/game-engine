#include "collision_manager.h"

template<class T>
CollisionManager<T>::CollisionManager() :
	activeContext2(nullptr)
{}

template<class T>
void CollisionManager<T>::Update() {
	for (auto it = context2s.begin(); it < context2s.end(); it++) {
		(*it)->Update();
	}

/*	
	for (auto it = context3s.begin(); it < context3s.end(); it++) {
		(*it)->Update();
	}
*/
}

template<class T>
void CollisionManager<T>::Add(CollisionContext<T, 2>* in_context) {
	if (activeContext2 == nullptr) {
		activeContext2 = in_context;
	}

	context2s.push_back(in_context);
}

/*
void CollisionManager<T>::Add(CollisionContext3* in_context) {
	context3s.push_back(in_context);
}
*/

template<class T>
void CollisionManager<T>::Remove(CollisionContext<T, 2>* in_context) {
	auto position = std::find(context2s.begin(), context2s.end(), in_context);
	if (position != context2s.end()) {
		context2s.erase(position);
	}

	if (activeContext2 == in_context) {
		activeContext2 = nullptr;
	}
}

template<class T>
bool CollisionManager<T>::Has(CollisionContext<T, 2>* in_context) {
	return std::find(context2s.begin(), context2s.end(), in_context) != context2s.end();
}

template<class T>
void CollisionManager<T>::Make_Active(CollisionContext<T, 2>* in_context) {
	if (!Has(in_context)) {
		Add(in_context);
	}
	activeContext2 = in_context;
}

template<class T>
CollisionContext<T, 2>* CollisionManager<T>::Get_Active2() {
	return activeContext2;
}

/*
void CollisionManager<T>::Remove(CollisionContext3* in_context) {
	auto position = std::find(context3s.begin(), context3s.end(), in_context);
	if (position != context3s.end()) {
		context3s.erase(position);
	}
}
*/

template class CollisionManager<float>;
template class CollisionManager<double>;