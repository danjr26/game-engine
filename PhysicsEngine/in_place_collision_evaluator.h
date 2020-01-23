#ifndef COLLISION_EVALUATION_H
#define COLLISION_EVALUATION_H

#include "collision_mask.h"

template<class T, uint n>
Collision<T, n> getCollision();

template<class T>
class _CollisionEvaluator2 {
	Collision<T, 2> evaluateTyped(AAHalfSpace2CollisionMask<T> & in_mask1, AAHalfSpace2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(AAHalfSpace2CollisionMask<T> & in_mask1, AALine2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(AAHalfSpace2CollisionMask<T> & in_mask1, AARectangleCollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(AAHalfSpace2CollisionMask<T> & in_mask1, CircleCollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(AAHalfSpace2CollisionMask<T> & in_mask1, HalfSpace2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(AAHalfSpace2CollisionMask<T> & in_mask1, Line2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(AAHalfSpace2CollisionMask<T> & in_mask1, LineSegment2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(AAHalfSpace2CollisionMask<T> & in_mask1, MST2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(AAHalfSpace2CollisionMask<T> & in_mask1, Point2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(AAHalfSpace2CollisionMask<T> & in_mask1, Ray2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(AAHalfSpace2CollisionMask<T> & in_mask1, RectangleCollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(AAHalfSpace2CollisionMask<T> & in_mask1, Triangle2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(AARectangleCollisionMask<T> & in_mask1, AARectangleCollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(AARectangleCollisionMask<T> & in_mask1, CircleCollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(AARectangleCollisionMask<T> & in_mask1, HalfSpace2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(AARectangleCollisionMask<T> & in_mask1, Line2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(AARectangleCollisionMask<T> & in_mask1, LineSegment2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(AARectangleCollisionMask<T> & in_mask1, MST2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(AARectangleCollisionMask<T> & in_mask1, Point2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(AARectangleCollisionMask<T> & in_mask1, Ray2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(AARectangleCollisionMask<T> & in_mask1, RectangleCollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(AARectangleCollisionMask<T> & in_mask1, Triangle2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(CircleCollisionMask<T> & in_mask1, CircleCollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(CircleCollisionMask<T> & in_mask1, HalfSpace2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(CircleCollisionMask<T> & in_mask1, Line2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(CircleCollisionMask<T> & in_mask1, LineSegment2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(CircleCollisionMask<T> & in_mask1, MST2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(CircleCollisionMask<T> & in_mask1, Point2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(CircleCollisionMask<T> & in_mask1, Ray2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(CircleCollisionMask<T> & in_mask1, RectangleCollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(CircleCollisionMask<T> & in_mask1, Triangle2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(HalfSpace2CollisionMask<T> & in_mask1, HalfSpace2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(HalfSpace2CollisionMask<T> & in_mask1, Line2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(HalfSpace2CollisionMask<T> & in_mask1, LineSegment2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(HalfSpace2CollisionMask<T> & in_mask1, MST2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(HalfSpace2CollisionMask<T> & in_mask1, Point2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(HalfSpace2CollisionMask<T> & in_mask1, Ray2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(HalfSpace2CollisionMask<T> & in_mask1, RectangleCollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(HalfSpace2CollisionMask<T> & in_mask1, Triangle2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(Line2CollisionMask<T> & in_mask1, Line2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(Line2CollisionMask<T> & in_mask1, LineSegment2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(Line2CollisionMask<T> & in_mask1, MST2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(Line2CollisionMask<T> & in_mask1, Point2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(Line2CollisionMask<T> & in_mask1, Ray2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(Line2CollisionMask<T> & in_mask1, RectangleCollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(Line2CollisionMask<T> & in_mask1, Triangle2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(LineSegment2CollisionMask<T> & in_mask1, LineSegment2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(LineSegment2CollisionMask<T> & in_mask1, MST2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(LineSegment2CollisionMask<T> & in_mask1, Point2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(LineSegment2CollisionMask<T> & in_mask1, Ray2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(LineSegment2CollisionMask<T> & in_mask1, RectangleCollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(LineSegment2CollisionMask<T> & in_mask1, Triangle2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(MST2CollisionMask<T> & in_mask1, MST2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(MST2CollisionMask<T> & in_mask1, Point2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(MST2CollisionMask<T> & in_mask1, Ray2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(MST2CollisionMask<T> & in_mask1, RectangleCollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(MST2CollisionMask<T> & in_mask1, Triangle2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(Point2CollisionMask<T> & in_mask1, Point2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(Point2CollisionMask<T> & in_mask1, Ray2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(Point2CollisionMask<T> & in_mask1, RectangleCollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(Point2CollisionMask<T> & in_mask1, Triangle2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(Ray2CollisionMask<T> & in_mask1, Ray2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(Ray2CollisionMask<T> & in_mask1, RectangleCollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(Ray2CollisionMask<T> & in_mask1, Triangle2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(RectangleCollisionMask<T> & in_mask1, RectangleCollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(RectangleCollisionMask<T> & in_mask1, Triangle2CollisionMask<T> & in_mask2);
	Collision<T, 2> evaluateTyped(Triangle2CollisionMask<T> & in_mask1, Triangle2CollisionMask<T> & in_mask2);
};



#endif	