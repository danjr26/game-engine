#include "weapon.h"

void Weapon::addIgnoreMask(CollisionMask2d* in_mask) {
	mIgnoreMasks.insert(in_mask);
	pushIgnoreMasks();
}

void Weapon::removeIgnoreMask(CollisionMask2d* in_mask) {
	mIgnoreMasks.erase(in_mask);
	pushIgnoreMasks();
}

bool Weapon::hasIgnoreMask(CollisionMask2d* in_mask) {
	return mIgnoreMasks.count(in_mask);
}

void Weapon::clearIgnoreMasks() {
	mIgnoreMasks.clear();
	pushIgnoreMasks();
}

void Weapon::pushIgnoreMasks() 
{}
