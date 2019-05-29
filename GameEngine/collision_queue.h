#ifndef COLLISION_QUEUE_H
#define COLLISION_QUEUE_H

#include "collision.h"
#include <list>

template<class T, uint n>
using CollisionQueue = std::list<Collision<T, n>>;

using CollisionQueue2f = CollisionQueue<float, 2>;
using CollisionQueue2d = CollisionQueue<double, 2>;
using CollisionQueue3f = CollisionQueue<float, 3>;
using CollisionQueue3d = CollisionQueue<double, 3>;

#endif