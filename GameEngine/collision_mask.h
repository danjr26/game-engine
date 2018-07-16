#ifndef COLLISION_MASK_H
#define COLLISION_MASK_H

#include "mesh_vertex_data.h"
#include "filtered_object.h"
#include "disableable_object.h"
#include "collision.h"
#include "collision_evaluator.h"
#include "transformable_object.h"
#include <tuple>

class CircleCollisionMask;

class CollisionMask : public FilteredObject, public DisableableObject, public TransformableObject {
public:
	virtual Collisionf Accept_Evaluator(CollisionEvaluator* in_evaluator, CollisionMask* in_other) = 0;
	virtual Collisionf Accept_Secondhand_Evaluation(CollisionEvaluator* in_evaluator, CircleCollisionMask* in_other) = 0;
};

#endif