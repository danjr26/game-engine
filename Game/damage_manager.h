#ifndef DAMAGE_MANAGER_H
#define DAMAGE_MANAGER_H

#include "damage_receiver.h"
#include "collider.h"
#include <unordered_map>
#include <unordered_set>

class DamageManager {
private:
	std::unordered_map<Collider2d*, DamageReceiver*> mByCollider;
	std::unordered_set<DamageReceiver*> mReceivers;

public:
	DamageManager();
	
	void add(DamageReceiver* i_receiver);
	void remove(DamageReceiver* i_receiver);
	bool has(DamageReceiver* i_receiver) const;

	void add(Collider2d* i_mask, DamageReceiver* i_receiver);
	void remove(Collider2d* i_mask);
	bool has(Collider2d* i_mask);
	DamageReceiver* get(Collider2d* i_mask) const;
};

#endif