#include "damage_manager.h"

DamageManager::DamageManager() {}

void DamageManager::add(DamageReceiver* i_receiver) {
	mReceivers.insert(i_receiver);
}

void DamageManager::remove(DamageReceiver* i_receiver) {
	mReceivers.erase(i_receiver);
}

bool DamageManager::has(DamageReceiver* i_receiver) const {
	return mReceivers.count(i_receiver);
}

void DamageManager::add(Collider2d* i_mask, DamageReceiver* i_receiver) {
	mByCollider[i_mask] = i_receiver;
}

void DamageManager::remove(Collider2d* i_mask) {
	mByCollider.erase(i_mask);
}

bool DamageManager::has(Collider2d* i_mask) {
	return mByCollider.count(i_mask);
}

DamageReceiver* DamageManager::get(Collider2d* i_mask) const {
	return mByCollider.count(i_mask) ? mByCollider.at(i_mask) : nullptr;
}
