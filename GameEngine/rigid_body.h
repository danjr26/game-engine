#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "collision_mask.h"
#include "vector.h"
#include "unclamped_rotation.h"

template<uint n>
class RigidBody : public TransformableObject {
private:
	CollisionMask<T, n>* collisionMask;
	Vector<double, n> linearVelocity;
	URotationd angularVelocity;
	double mass;
	double momentOfInertia;

public:
	
};

#endif