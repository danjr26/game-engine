#ifndef COLLISION_RESPONSE_MANAGER_H
#define COLLISION_RESPONSE_MANAGER_H

#include "collision_mask.h"
#include "collision_responder.h"
#include <map>

class CollisionResponseManager {
private:
	std::unordered_map<CollisionMask2d*, CollisionResponder*> mResponders;

public:
	void add(CollisionMask2d* in_mask, CollisionResponder* in_responder);
	CollisionResponder* get(CollisionMask2d* in_mask);
	void remove(CollisionMask2d* in_mask);
	bool has(CollisionMask2d* in_mask) const;
};

#endif