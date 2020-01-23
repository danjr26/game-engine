#ifndef COLLISION_EVALUATOR_H
#define COLLISION_EVALUATOR_H

#include "collision_mask.h"
#include "exceptions.h"

template<class T, uint n>
class CollisionEvaluator 
{};

template<class T>
class CollisionEvaluator<T, 2> {
protected:
	bool mReturnPoint;
	bool mReturnSeparator;

public:
	CollisionEvaluator();

	void returnPoint(bool in_val);
	void returnSeparator(bool in_val);
	Collision<T, 2> evaluate(CollisionMask<T, 2>& in_mask1, CollisionMask<T, 2>& in_mask2);

	template<class T1, class T2>
	Collision<T, 2> evaluateTyped(T1& in_mask1, T2& in_mask2);
	virtual Collision<T, 2> evaluateTyped(AAHalfSpace2CollisionMask<T>& in_mask1, AAHalfSpace2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AAHalfSpace2CollisionMask<T>& in_mask1, AALine2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AAHalfSpace2CollisionMask<T>& in_mask1, AARectangleCollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AAHalfSpace2CollisionMask<T>& in_mask1, CircleCollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); };
	virtual Collision<T, 2> evaluateTyped(AAHalfSpace2CollisionMask<T>& in_mask1, HalfSpace2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AAHalfSpace2CollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AAHalfSpace2CollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AAHalfSpace2CollisionMask<T>& in_mask1, MST2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AAHalfSpace2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AAHalfSpace2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AAHalfSpace2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AAHalfSpace2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }

	virtual Collision<T, 2> evaluateTyped(AALine2CollisionMask<T>& in_mask1, AALine2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AALine2CollisionMask<T>& in_mask1, AAHalfSpace2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AALine2CollisionMask<T>& in_mask1, AARectangleCollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AALine2CollisionMask<T>& in_mask1, CircleCollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); };
	virtual Collision<T, 2> evaluateTyped(AALine2CollisionMask<T>& in_mask1, HalfSpace2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AALine2CollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AALine2CollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AALine2CollisionMask<T>& in_mask1, MST2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AALine2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AALine2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AALine2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AALine2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }

	virtual Collision<T, 2> evaluateTyped(AARectangleCollisionMask<T>& in_mask1, AARectangleCollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AARectangleCollisionMask<T>& in_mask1, CircleCollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); };
	virtual Collision<T, 2> evaluateTyped(AARectangleCollisionMask<T>& in_mask1, HalfSpace2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AARectangleCollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AARectangleCollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AARectangleCollisionMask<T>& in_mask1, MST2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AARectangleCollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AARectangleCollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AARectangleCollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(AARectangleCollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }

	virtual Collision<T, 2> evaluateTyped(CircleCollisionMask<T>& in_mask1, CircleCollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(CircleCollisionMask<T>& in_mask1, HalfSpace2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(CircleCollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(CircleCollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(CircleCollisionMask<T>& in_mask1, MST2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(CircleCollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(CircleCollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(CircleCollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(CircleCollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }

	virtual Collision<T, 2> evaluateTyped(HalfSpace2CollisionMask<T>& in_mask1, HalfSpace2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(HalfSpace2CollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(HalfSpace2CollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(HalfSpace2CollisionMask<T>& in_mask1, MST2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(HalfSpace2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(HalfSpace2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(HalfSpace2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(HalfSpace2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }

	virtual Collision<T, 2> evaluateTyped(Line2CollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(Line2CollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(Line2CollisionMask<T>& in_mask1, MST2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(Line2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(Line2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(Line2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(Line2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }

	virtual Collision<T, 2> evaluateTyped(LineSegment2CollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(LineSegment2CollisionMask<T>& in_mask1, MST2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(LineSegment2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(LineSegment2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(LineSegment2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(LineSegment2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }

	virtual Collision<T, 2> evaluateTyped(MST2CollisionMask<T>& in_mask1, MST2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(MST2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(MST2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(MST2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(MST2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }

	virtual Collision<T, 2> evaluateTyped(Point2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(Point2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(Point2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(Point2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }

	virtual Collision<T, 2> evaluateTyped(Ray2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(Ray2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(Ray2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }

	virtual Collision<T, 2> evaluateTyped(RectangleCollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> evaluateTyped(RectangleCollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }

	virtual Collision<T, 2> evaluateTyped(Triangle2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) { throw NotImplementedException(); return Collision<T, 2>(); }

};

template<class T>
class CollisionEvaluator<T, 3> {
protected:
	bool mReturnPoint;

public:
	CollisionEvaluator() :
		mReturnPoint(true) {}

	void returnPoint(bool in_val) {
		mReturnPoint = in_val;
	}

public:
	Collision<T, 3> evaluate(CollisionMask<T, 3>& in_mask1, CollisionMask<T, 3>& in_mask2);
};

using CollisionEvaluator2f = CollisionEvaluator<float, 2>;
using CollisionEvaluator2d = CollisionEvaluator<double, 2>;
using CollisionEvaluator3f = CollisionEvaluator<float, 3>;
using CollisionEvaluator3d = CollisionEvaluator<double, 3>;

#endif

template<class T>
template<class T1, class T2>
inline Collision<T, 2> CollisionEvaluator<T, 2>::evaluateTyped(T1& in_mask1, T2& in_mask2) {
	return evaluateTyped(in_mask2, in_mask1);
}
