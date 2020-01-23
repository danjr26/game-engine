#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include "collision_context.h"

template<class T>
class CollisionManager {
private:
	std::vector<CollisionContext<T, 2>*> mContext2s;
	//std::vector<CollisionContext3*> context3s;

	CollisionContext<T, 2>* mActiveContext2;
	//CollisionContext3* activeContext3;

public:
	CollisionManager();

	void update();

	void add(CollisionContext<T, 2>* i_context);
	//void add(CollisionContext3* i_context);

	void remove(CollisionContext<T, 2>* i_context);
	//void remove(CollisionContext3* i_context);

	bool has(CollisionContext<T, 2>* i_context);

	void makeActive(CollisionContext<T, 2>* i_context);

	CollisionContext<T, 2>* getActive2();
};

using CollisionManagerf = CollisionManager<float>;
using CollisionManagerd = CollisionManager<double>;

#endif