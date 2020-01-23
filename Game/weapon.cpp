#include "weapon.h"

void Weapon::addIgnoreMask(Collider2d* i_mask) {
	mIgnoreMasks.insert(i_mask);
	pushIgnoreMasks();
}

void Weapon::removeIgnoreMask(Collider2d* i_mask) {
	mIgnoreMasks.erase(i_mask);
	pushIgnoreMasks();
}

bool Weapon::hasIgnoreMask(Collider2d* i_mask) {
	return mIgnoreMasks.count(i_mask);
}

void Weapon::clearIgnoreMasks() {
	mIgnoreMasks.clear();
	pushIgnoreMasks();
}

void Weapon::pushIgnoreMasks() 
{}
