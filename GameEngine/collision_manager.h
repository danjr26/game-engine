#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include "collision_context.h"

template<class T>
class CollisionManager {
private:
	std::vector<CollisionContext<T, 2>*> context2s;
	//std::vector<CollisionContext3*> context3s;

	CollisionContext<T, 2>* activeContext2;
	//CollisionContext3* activeContext3;

public:
	CollisionManager();

	void Update();

	void Add(CollisionContext<T, 2>* in_context);
	//void Add(CollisionContext3* in_context);

	void Remove(CollisionContext<T, 2>* in_context);
	//void Remove(CollisionContext3* in_context);

	bool Has(CollisionContext<T, 2>* in_context);

	void Make_Active(CollisionContext<T, 2>* in_context);

	CollisionContext<T, 2>* Get_Active2();
};

using CollisionManagerf = CollisionManager<float>;
using CollisionManagerd = CollisionManager<double>;

#endif