#ifndef CIRCLE_COLLISION_MASK_H
#define CIRCLE_COLLISION_MASK_H

#include "collision_mask.h"

class CircleCollisionMask : public CollisionMask {
private:
	float radius;

public:
	Collisionf Accept_Evaluator(CollisionEvaluator* in_evaluator, CollisionMask* in_other) override {
		in_other->Accept_Secondhand_Evaluation(in_evaluator, this);
	};

	Collisionf Accept_Secondhand_Evaluation(CollisionEvaluator* in_evaluator, CircleCollisionMask* in_other) override {
		in_evaluator->Evaluate_Typed(this, in_other);
	}

	float Get_Radius() const;
	void Set_Radius(float in_radius);
};

#endif