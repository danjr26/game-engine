#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H
#include "clock.h"
#include "component.h"
#include "collision_mask.h"
#include "oct_tree.h"
#include "trq.h"

class CollisionManager {
public:
	IndexedArray<CollidableComponent3> components;
	OctTree	octTree;
	CollisionTR timeRequest;

			CollisionManager	();
			~CollisionManager	();
	
	void	Check_Collisions	();
	void	Add					(CollidableComponent3* in_component);
	void	Remove				(CollidableComponent3* in_component);
};


#endif
