#include "axis_aligned_rectangle_collision_mask.h"

AxisAlignedRectangleCollisionMask::AxisAlignedRectangleCollisionMask(const AxisAlignedRectangled& in_rectangle) :
	rectangle(in_rectangle)
{}

AxisAlignedRectangled& AxisAlignedRectangleCollisionMask::Get_Rectangle() {
	return rectangle;
}

AxisAlignedRectangled AxisAlignedRectangleCollisionMask::Get_Transformed_Rectangle() {
	Vector2d corners[4];
	corners[0] = rectangle.Get_Minima();
	corners[1] = rectangle.Get_Maxima();
	corners[2] = Vector2d(corners[0].X(), corners[1].Y());
	corners[3] = Vector2d(corners[1].X(), corners[0].Y());

	for (uint i = 0; i < 4; i++) {
		corners[i] = Vector2d(transform.Apply_To_Local_Point(Vector3d(corners[i], 0.0)));
	}

	return AxisAlignedRectangled::From_Bounded_Points(4, corners);
}

Collision2d AxisAlignedRectangleCollisionMask::Accept_Evaluator(CollisionEvaluator2* in_evaluator, CollisionMask* in_other) {
	return in_other->Accept_Secondhand_Evaluator(in_evaluator, this);
}

Collision2d AxisAlignedRectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, AxisAlignedRectangleCollisionMask* in_other) {
	return in_evaluator->Evaluate_Typed(this, in_other);
}

Collision2d AxisAlignedRectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, CircleCollisionMask* in_other) {
	return in_evaluator->Evaluate_Typed(this, in_other);
}

Collision2d AxisAlignedRectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, HalfSpace2CollisionMask* in_other) {
	return in_evaluator->Evaluate_Typed(this, in_other);
}

Collision2d AxisAlignedRectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, Line2CollisionMask* in_other) {
	return in_evaluator->Evaluate_Typed(this, in_other);
}

Collision2d AxisAlignedRectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, LineSegment2CollisionMask* in_other) {
	return in_evaluator->Evaluate_Typed(this, in_other);
}

Collision2d AxisAlignedRectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2 * in_evaluator, Point2CollisionMask * in_other) {
	return in_evaluator->Evaluate_Typed(this, in_other);
}

Collision2d AxisAlignedRectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2 * in_evaluator, Ray2CollisionMask * in_other) {
	return in_evaluator->Evaluate_Typed(this, in_other);
}

Collision2d AxisAlignedRectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2 * in_evaluator, RectangleCollisionMask * in_other) {
	return in_evaluator->Evaluate_Typed(this, in_other);
}

Collision2d AxisAlignedRectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2 * in_evaluator, Triangle2CollisionMask * in_other) {
	return in_evaluator->Evaluate_Typed(this, in_other);
}
