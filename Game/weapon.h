#ifndef WEAPON_H
#define WEAPON_H

#include "collider.h"
#include "deep_transformable_object.h"

class Weapon : public DeepTransformableObject2d {
public:
	struct Feedback {
		Vector2d mImpulse;
	};
	std::unordered_set<Collider2d*> mIgnoreMasks;

public:
	virtual void update(double i_dt, Feedback* o_feedback = nullptr) = 0;
	virtual void startFire() = 0;
	virtual void endFire() = 0;
	void addIgnoreMask(Collider2d* i_mask);
	void removeIgnoreMask(Collider2d* i_mask);
	bool hasIgnoreMask(Collider2d* i_mask);
	void clearIgnoreMasks();

private:
	virtual void pushIgnoreMasks();
};

#endif