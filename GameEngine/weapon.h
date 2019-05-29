#ifndef WEAPON_H
#define WEAPON_H

#include <unordered_set>
#include "collision_mask.h"
#include "deep_transformable_object.h"

class Weapon : public DeepTransformableObject2d {
public:
	struct Feedback {
		Vector2d mImpulse;
	};
	std::unordered_set<CollisionMask2d*> mIgnoreMasks;

public:
	virtual void update(double in_dt, Feedback* out_feedback = nullptr) = 0;
	virtual void startFire() = 0;
	virtual void endFire() = 0;
	void addIgnoreMask(CollisionMask2d* in_mask);
	void removeIgnoreMask(CollisionMask2d* in_mask);
	bool hasIgnoreMask(CollisionMask2d* in_mask);
	void clearIgnoreMasks();

private:
	virtual void pushIgnoreMasks();
};

#endif