#ifndef COLLISION_MASK_H
#define COLLISION_MASK_H
#include "mesh_instance.h"

class CollisionMask3 {
protected:
	enum Type {
		sphere_tree
	} type;

public:
	CollisionMask3	(Type type);
	~CollisionMask3	();

	virtual Collision Is_Colliding(CollisionMask3& in_otherMask);

	Type Get_Type();
};

class SphereTreeCollisionMask : public CollisionMask3 {
private:
	Transform3d&		transform;
	CustomArray
		<SphereNode>	sphereTree;

public:
				SphereTreeCollisionMask	(Transform3d& in_transform, CustomArray<SphereNode> in_sphereTree);
				~SphereTreeCollisionMask();
	Collision	Is_Colliding			(CollisionMask3& in_otherMask) override;
	Collision	Is_Colliding			(SphereTreeCollisionMask& in_otherMask);

private:
	Collision Is_Colliding_Sphere_Tree(SphereTreeCollisionMask& in_otherMask);

	static Collision Triangles_Are_Colliding(const SphereNode* in_sphere1, const SphereNode* in_sphere2);
	static Collision Spheres_Are_Colliding(const SphereNode* in_sphere1, const SphereNode* in_sphere2);
	//static Collision Sphere_And_Triangle_Are_Colliding(const SphereNode* in_sphere, const SphereNode* in_triangle);
};

#endif
