#include "in_place_collision_evaluator2.h"
#include "exceptions.h"
#include "misc.h"
#include "range.h"

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(CircleCollisionMask* in_mask1, CircleCollisionMask* in_mask2) {
	Collision2d collision;

	Circled circle1 = in_mask1->Get_Transformed_Circle();
	Circled circle2 = in_mask2->Get_Transformed_Circle();

	const double radii = circle1.Get_Radius() + circle2.Get_Radius();
	const double radiiSquared = radii * radii;
	const Vector2d offset = circle2.Get_Center() - circle1.Get_Center();
	const double offsetDistanceSquared = offset.Dot_Self();

	collision.didCollide = offsetDistanceSquared <= radiiSquared;
	if (collision.didCollide) {
		collision.collisionPoint = circle1.Get_Center() + offset / 2.0;
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(AxisAlignedRectangleCollisionMask* in_mask1, AxisAlignedRectangleCollisionMask* in_mask2) {
	Collision2d collision;

	AxisAlignedRectangled rectangle1 = in_mask1->Get_Transformed_Rectangle();
	AxisAlignedRectangled rectangle2 = in_mask2->Get_Transformed_Rectangle();

	Ranged xRange1 = Ranged(rectangle1.Get_Minima().X(), rectangle1.Get_Maxima().X());
	Ranged yRange1 = Ranged(rectangle1.Get_Minima().Y(), rectangle1.Get_Maxima().Y());
	Ranged xRange2 = Ranged(rectangle2.Get_Minima().X(), rectangle2.Get_Maxima().X());
	Ranged yRange2 = Ranged(rectangle2.Get_Minima().Y(), rectangle2.Get_Maxima().Y());

	Ranged xCollRange, yCollRange;

	collision.didCollide = xRange1.Intersection(xRange2, xCollRange) && yRange2.Intersection(yRange2, yCollRange);

	if (collision.didCollide) {
		collision.collisionPoint = Vector2d(xCollRange.Get_Mean(), yCollRange.Get_Mean());
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(AxisAlignedRectangleCollisionMask* in_mask1, CircleCollisionMask* in_mask2) {
	Collision2d collision;

	AxisAlignedRectangled rectangle = in_mask1->Get_Transformed_Rectangle();
	Circled circle = in_mask2->Get_Transformed_Circle();

	

	return collision;
}
