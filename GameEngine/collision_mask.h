#ifndef COLLISION_MASK_H
#define COLLISION_MASK_H
#include "mesh_instance.h"

struct Collision {
	bool		didCollide;
	Vector3d	collisionPoint;
};

class CollisionMask {
public:
	Transform3d*	transform;
	Vector3d*		velocity;
	SphereTree*		tree;

				CollisionMask	(Transform3d* in_transform, Vector3d* in_velocity, SphereTree* in_tree);
				~CollisionMask	();
	Collision	Is_Colliding	(CollisionMask* in_otherMask);
	inline static Collision Hard_Test(
		const SphereNode* in_sphere1, const SphereNode* in_sphere2) {
		if (in_sphere1->tri == nullptr || in_sphere2->tri == nullptr) {
			throw NullPointerException("CollisionMask:Hard_Test:tri");
		}
		return Triangles_Are_Colliding(in_sphere1, in_sphere2);
		/*
		if (in_sphere1->tri) {
			if (in_sphere2->tri)
				return Triangles_Are_Colliding(in_sphere1, in_sphere2);
			else
				return Sphere_And_Triangle_Are_Colliding(in_sphere2, in_sphere1);
		}
		else {
			if (in_sphere2->tri)
				return Sphere_And_Triangle_Are_Colliding(in_sphere1, in_sphere2);
			else
				return Spheres_Are_Colliding(in_sphere1, in_sphere2);
		}*/
	}
	inline static Collision Spheres_Are_Colliding(const SphereNode* in_sphere1,  const SphereNode* in_sphere2) {
		double radius = in_sphere1->radius + in_sphere2->radius;
		Vector3d distance = in_sphere1->worldCenter - in_sphere2->worldCenter;

		if (distance.Dot(distance) <= radius * radius) {
			return Collision{ true, in_sphere2->worldCenter + distance * 0.5 };
		}
		else {
			return Collision{ false, Vector3d() };
		}
	}
	static Collision Triangles_Are_Colliding(const SphereNode* in_sphere1, const SphereNode* in_sphere2);
	//static Collision Sphere_And_Triangle_Are_Colliding(const SphereNode* in_sphere, const SphereNode* in_triangle);
};

#endif
