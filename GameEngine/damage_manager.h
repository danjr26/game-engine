#ifndef DAMAGE_MANAGER_H
#define DAMAGE_MANAGER_H

#include "damage_receiver.h"
#include "collision_mask.h"
#include <unordered_map>
#include <unordered_set>

class DamageManager {
private:
	std::unordered_map<CollisionMask2d*, DamageReceiver*> mByCollisionMask;
	std::unordered_set<DamageReceiver*> mReceivers;

public:
	DamageManager();
	
	void add(DamageReceiver* in_receiver);
	void remove(DamageReceiver* in_receiver);
	bool has(DamageReceiver* in_receiver) const;

	void add(CollisionMask2d* in_mask, DamageReceiver* in_receiver);
	void remove(CollisionMask2d* in_mask);
	bool has(CollisionMask2d* in_mask);
	DamageReceiver* get(CollisionMask2d* in_mask) const;
};

#endif