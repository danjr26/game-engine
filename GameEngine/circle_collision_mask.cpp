#include "circle_collision_mask.h"

CircleCollisionMask::CircleCollisionMask(const Circled& in_circle) :
	circle(in_circle) 
{}

Circled& CircleCollisionMask::Get_Circle() {
	return circle;
}

Circled CircleCollisionMask::Get_Transformed_Circle() {
	Circled out = circle;
	out.Apply_Transform(transform);
	return out;
	/*double scale = 1.0;
	for (Transform* t = &transform; t != nullptr; t = t->Get_Parent()) {
		scale *= max(t->Get_Local_Scale().X(), t->Get_Local_Scale().Y());
	}
	return Circled::From_Point_Radius(
		Vector2d(transform.Apply_To_Local_Point(Vector3d(circle.Get_Center(), 0.0))), 
		circle.Get_Radius() * scale
	);*/
}

Collision2d CircleCollisionMask::Accept_Evaluator(CollisionEvaluator2* in_evaluator, CollisionMask2* in_other) {
	return in_other->Accept_Secondhand_Evaluator(in_evaluator, this);
}

Collision2d CircleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, AxisAlignedRectangleCollisionMask* in_other) {
	return in_evaluator->Evaluate_Typed(in_other, this);
}

Collision2d CircleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, CircleCollisionMask* in_other) {
	return in_evaluator->Evaluate_Typed(this, in_other);
}

Collision2d CircleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, HalfSpace2CollisionMask* in_other) {
	return in_evaluator->Evaluate_Typed(this, in_other);
}

Collision2d CircleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2 * in_evaluator, Line2CollisionMask * in_other) {
	return in_evaluator->Evaluate_Typed(this, in_other);
}

Collision2d CircleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2 * in_evaluator, LineSegment2CollisionMask * in_other) {
	return in_evaluator->Evaluate_Typed(this, in_other);
}

Collision2d CircleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2 * in_evaluator, Point2CollisionMask * in_other) {
	return in_evaluator->Evaluate_Typed(this, in_other);
}

Collision2d CircleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2 * in_evaluator, Ray2CollisionMask * in_other) {
	return in_evaluator->Evaluate_Typed(this, in_other);
}

Collision2d CircleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2 * in_evaluator, RectangleCollisionMask * in_other) {
	return in_evaluator->Evaluate_Typed(this, in_other);
}

Collision2d CircleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2 * in_evaluator, Triangle2CollisionMask * in_other) {
	return in_evaluator->Evaluate_Typed(this, in_other);
}



