#ifndef COLLISION_H
#define COLLISION_H
#include "clock.h"
#include "component.h"
#include "collision_mask.h"
#include "oct_tree.h"
#include "trq.h"

template class IndexedArray<CollidableComponent>;
class CollisionManager {
public:
	IndexedArray<CollidableComponent> components;
	OctTree	octTree;
	CollisionTR timeRequest;

			CollisionManager	();
			~CollisionManager	();
	
	void	Check_Collisions	();
	void	Add					(CollidableComponent* in_component);
	void	Remove				(CollidableComponent* in_component);
};


#endif
