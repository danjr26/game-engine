#ifndef COLLISION_H
#define COLLISION_H

template<class T, uint n>
struct Collision {
	bool didCollide;
	Vector<T, n> collisionPoint;
};

using Collision2f = Collision<float, 2>;
using Collision2d = Collision<double, 2>;
using Collision3f = Collision<float, 3>;
using Collision3d = Collision<double, 3>;

#endif