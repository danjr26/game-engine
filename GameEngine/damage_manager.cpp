#include "damage_manager.h"

DamageManager::DamageManager() {}

void DamageManager::add(DamageReceiver* in_receiver) {
	mReceivers.insert(in_receiver);
}

void DamageManager::remove(DamageReceiver* in_receiver) {
	mReceivers.erase(in_receiver);
}

bool DamageManager::has(DamageReceiver* in_receiver) const {
	return mReceivers.count(in_receiver);
}

void DamageManager::add(CollisionMask2d* in_mask, DamageReceiver* in_receiver) {
	mByCollisionMask[in_mask] = in_receiver;
}

void DamageManager::remove(CollisionMask2d* in_mask) {
	mByCollisionMask.erase(in_mask);
}

bool DamageManager::has(CollisionMask2d* in_mask) {
	return mByCollisionMask.count(in_mask);
}

DamageReceiver* DamageManager::get(CollisionMask2d* in_mask) const {
	return mByCollisionMask.count(in_mask) ? mByCollisionMask.at(in_mask) : nullptr;
}
