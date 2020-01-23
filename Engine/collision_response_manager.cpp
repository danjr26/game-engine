#include "collision_response_manager.h"

void CollisionResponseManager::add(CollisionMask2d* in_mask, CollisionResponder* in_responder) {
	mResponders[in_mask] = in_responder;
}

CollisionResponder* CollisionResponseManager::get(CollisionMask2d* in_mask) {
	return has(in_mask) ? mResponders[in_mask] : nullptr;
}

void CollisionResponseManager::remove(CollisionMask2d* in_mask) {
	mResponders.erase(in_mask);
}

bool CollisionResponseManager::has(CollisionMask2d* in_mask) const {
	return mResponders.count(in_mask);
}
