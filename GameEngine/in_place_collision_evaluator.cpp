#include "in_place_collision_evaluator.h"
#include "exceptions.h"
#include "misc.h"
#include "range.h"
#include "projection_overlap.h"

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AxisAlignedRectangleCollisionMask<T>& in_mask1, AxisAlignedRectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	AxisAlignedBox<T, 2> rectangle1 = in_mask1.Get_Transformed_Rectangle();
	AxisAlignedBox<T, 2> rectangle2 = in_mask2.Get_Transformed_Rectangle();

	Range<T> xRange1 = Range<T>(rectangle1.Get_Minima().X(), rectangle1.Get_Maxima().X());
	Range<T> yRange1 = Range<T>(rectangle1.Get_Minima().Y(), rectangle1.Get_Maxima().Y());
	Range<T> xRange2 = Range<T>(rectangle2.Get_Minima().X(), rectangle2.Get_Maxima().X());
	Range<T> yRange2 = Range<T>(rectangle2.Get_Minima().Y(), rectangle2.Get_Maxima().Y());

	Range<T> xCollRange, yCollRange;

	bool xColl = xRange1.Intersection(xRange2, xCollRange);
	bool yColl = yRange2.Intersection(yRange2, yCollRange);

	collision.didCollide = xColl && yColl;

	if (collision.didCollide && returnPoint) {
		collision.collisionPoint = Vector<T, 2>(xCollRange.Get_Mean(), yCollRange.Get_Mean());
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = (yColl) ? Vector<T, 2>(1, 0) : Vector<T, 2>(0, 1);
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AxisAlignedRectangleCollisionMask<T>& in_mask1, CircleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision, testCollision;

	AxisAlignedBox<T, 2> rectangle = in_mask1.Get_Transformed_Rectangle();
	Circle<T> circle = in_mask2.Get_Transformed_Circle();

	Vector<T, 2> corners[4];
	rectangle.Get_Corners(corners);
	Vector<T, 2> rectCenter = rectangle.Get_Center();

	AxisAlignedRectangleCollisionMask<T> xRectangleMask = AxisAlignedRectangleCollisionMask<T>(
		AxisAlignedBox<T, 2>::From_Extrema(
			rectangle.Get_Minima() - Vector<T, 2>(circle.Get_Radius(), 0), 
			rectangle.Get_Maxima() + Vector<T, 2>(circle.Get_Radius(), 0)
		),
		true
	);

	AxisAlignedRectangleCollisionMask<T> yRectangleMask = AxisAlignedRectangleCollisionMask<T>(
		AxisAlignedBox<T, 2>::From_Extrema(
			rectangle.Get_Minima() - Vector<T, 2>(0, circle.Get_Radius()),
			rectangle.Get_Maxima() + Vector<T, 2>(0, circle.Get_Radius())
		),
		true
	);

	Point2CollisionMask<T> pointMask = Point2CollisionMask<T>(circle.Get_Center(), true);
	CircleCollisionMask<T> transformedCircleMask = CircleCollisionMask<T>(circle, true);

	if ((testCollision = Evaluate_Typed(xRectangleMask, pointMask)).didCollide) {
		collision = testCollision;

		if (returnPoint) {
			collision.collisionPoint[0] += Sign(rectCenter.X() - collision.collisionPoint.X()) *
				Min<T>(circle.Get_Radius(), xRectangleMask.Get_Rectangle().Get_Dimensions().X() / 2);
		}

		return collision;
	}

	if ((testCollision = Evaluate_Typed(yRectangleMask, pointMask)).didCollide) {
		collision = testCollision;

		if (returnPoint) {
			collision.collisionPoint[1] += Sign(rectCenter.Y() - collision.collisionPoint.Y()) *
				Min<T>(circle.Get_Radius(), yRectangleMask.Get_Rectangle().Get_Dimensions().Y() / 2);
		}

		return collision;
	}

	if (!collision.didCollide) {
		for (uint i = 0; i < 4; i++) {
			pointMask = Point2CollisionMask<T>(corners[i], true);
			if ((testCollision = Evaluate_Typed(transformedCircleMask, pointMask)).didCollide) {
				collision = testCollision;
				break;
			}
		}
	}

	if (!collision.didCollide && returnSeparator ) {
		if (Is_Between_Inc(circle.Get_Center().X(), rectangle.Get_Minima().X(), rectangle.Get_Maxima().X())) {
			collision.separator = Vector<T, 2>(0, 1);
		}
		else if (Is_Between_Inc(circle.Get_Center().Y(), rectangle.Get_Minima().Y(), rectangle.Get_Maxima().Y())) {
			collision.separator = Vector<T, 2>(1, 0);
		}
		else {
			T cornerDistances[4];
			for (uint i = 0; i < 4; i++) {
				cornerDistances[i] = (circle.Get_Center() - corners[i]).Dot_Self();
			}

			uint closest = 0;
			for (uint i = 1; i < 4; i++) {
				if (cornerDistances[i] < cornerDistances[closest]) {
					closest = i;
				}
			}

			collision.separator = (circle.Get_Center() - corners[closest]);
		}
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AxisAlignedRectangleCollisionMask<T>& in_mask1, HalfSpace2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	AxisAlignedBox<T, 2> rectangle = in_mask1.Get_Transformed_Rectangle();
	HalfSpace<T, 2> halfSpace = in_mask2.Get_Transformed_Half_Space();

	Vector<T, 2> corners[4];
	rectangle.Get_Corners(corners);

	Collision<T, 2> testCollision;
	HalfSpace2CollisionMask<T> transformedHalfSpaceMask = HalfSpace2CollisionMask<T>(halfSpace, true);
	for (uint i = 0; i < 4; i++) {
		Point2CollisionMask<T> pointMask = Point2CollisionMask<T>(corners[i], true);
		if ((testCollision = Evaluate_Typed(transformedHalfSpaceMask, pointMask)).didCollide) {
			collision = testCollision;
			break;
		}
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AxisAlignedRectangleCollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	AxisAlignedBox<T, 2> rectangle = in_mask1.Get_Transformed_Rectangle();
	Line<T, 2> line = in_mask2.Get_Transformed_Line();

	Vector<T, 2> center = rectangle.Get_Center();
	Vector<T, 2> minima = rectangle.Get_Minima();
	Vector<T, 2> maxima = rectangle.Get_Maxima();
	Vector<T, 2> point = line.Get_Point();
	Vector<T, 2> direction = line.Get_Direction();
	Vector2i sign = Vector2i(Sign(direction.X()), Sign(direction.Y()));

	if (direction.Is_Zero()) {
		if (Evaluate_Typed(
			AxisAlignedRectangleCollisionMask<T>(rectangle, true),
			Point2CollisionMask<T>(point, true)
		).didCollide) {
			collision.didCollide = true;
			if (returnPoint) {
				collision.collisionPoint = point;
			}
		}

		return collision;
	}

	if (direction.X() == 0.0) {
		if (Is_Between_Inc<T>(point.Y(), minima.Y(), maxima.Y())) {
			collision.didCollide = true;
			if (returnPoint) {
				collision.collisionPoint = (point.Y() < minima.Y()) ? Vector<T, 2>(point.X(), minima.Y()) : (point.Y() > maxima.Y()) ? Vector<T, 2>(point.X(), maxima.Y()) : point;
			}
		}
		return collision;
	}

	if (direction.Y() == 0.0) { 
		if (Is_Between_Inc<T>(point.X(), minima.X(), maxima.X())) {
			collision.didCollide = true;
			if (returnPoint) {
				collision.collisionPoint = (point.X() < minima.X()) ? Vector<T, 2>(minima.X(), point.Y()) : (point.X() > maxima.X()) ? Vector<T, 2>(maxima.X(), point.Y()) : point;
			}
		}
		return collision;
	}

	Collision<T, 2> xCollision, yCollision;
	T xT, yT;

	if (point.X() < center.X()) {
		T tAtX1 = (minima.X() - point.X()) / direction.X();
		if (Is_Between_Inc<T>(point.Y() + direction.Y() * tAtX1, minima.Y(), maxima.Y())) {
			if (returnPoint) {
				xCollision.collisionPoint = point + direction * tAtX1;
			}
			xCollision.didCollide = true;
			xT = tAtX1;
		}
	}
	else {
		T tAtX2 = (maxima.X() - point.X()) / direction.X();
		if (Is_Between_Inc<T>(point.Y() + direction.Y() * tAtX2, minima.Y(), maxima.Y())) {
			if (returnPoint) {
				xCollision.collisionPoint = point + direction * tAtX2;
			}
			xCollision.didCollide = true;
			xT = tAtX2;
		}
	}

	if (point.Y() < center.Y()) {
		T tAtY1 = (minima.Y() - point.Y()) / direction.Y();
		if (Is_Between_Inc<T>(point.X() + direction.X() * tAtY1, minima.X(), maxima.X())) {
			if (returnPoint) {
				yCollision.collisionPoint = point + direction * tAtY1;
			}
			yCollision.didCollide = true;
			yT = tAtY1;
		}
	}
	else {
		T tAtY2 = (maxima.Y() - point.Y()) / direction.Y();
		if (Is_Between_Inc<T>(point.X() + direction.X() * tAtY2, minima.X(), maxima.X())) {
			if (returnPoint) {
				yCollision.collisionPoint = point + direction * tAtY2;
			}
			yCollision.didCollide = true;
			yT = tAtY2;
		}
	}

	if (xCollision.didCollide && yCollision.didCollide) {
		if (xT >= 0) {
			if (yT >= 0) {
				collision = (xT < yT) ? xCollision : yCollision;
			}
			else {
				collision = yCollision;
			}
		}
		else if (yT > 0) {
			collision = yCollision;
		}
	}
	else {
		collision = (xCollision.didCollide) ? xCollision : yCollision;
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = direction.Orthogonal();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AxisAlignedRectangleCollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	AxisAlignedBox<T, 2> rectangle = in_mask1.Get_Transformed_Rectangle();
	LineSegment<T, 2> lineSegment = in_mask2.Get_Transformed_Line_Segment();

	Vector<T, 2> center = rectangle.Get_Center();
	Vector<T, 2> minima = rectangle.Get_Minima();
	Vector<T, 2> maxima = rectangle.Get_Maxima();
	Vector<T, 2> point1 = lineSegment.Get_Point1();
	Vector<T, 2> point2 = lineSegment.Get_Point2();
	Vector<T, 2> direction = lineSegment.Get_Direction();
	Vector2i sign = Vector2i(Sign(direction.X()), Sign(direction.Y()));

	if (direction.Is_Zero()) {
		if (Evaluate_Typed(
			AxisAlignedRectangleCollisionMask<T>(rectangle, true),
			Point2CollisionMask<T>(point1, true)
		).didCollide) {

			collision.didCollide = true;
			if (returnPoint) {
				collision.collisionPoint = point1;
			}
		}

		return collision;
	}

	if (direction.X() == 0.0) {
		if (Is_Between_Inc(point1.X(), minima.X(), maxima.X())) {
			if (sign.Y() != -Sign(minima.Y() - point1.Y()) && sign.Y() != Sign(minima.Y() - point2.Y())) {
				collision.didCollide = true;
				if (returnPoint) {
					collision.collisionPoint = Vector<T, 2>(point1.X(), minima.Y());
				}
			}
			else if (sign.Y() != -Sign(maxima.Y() - point1.Y()) && sign.Y() != Sign(maxima.Y() - point2.Y())) {
				collision.didCollide = true;
				if (returnPoint) {
					collision.collisionPoint = Vector<T, 2>(point1.X(), maxima.Y());
				}
			}
		}
		return collision;
	}

	if (direction.Y() == 0.0) {
		if (Is_Between_Inc(point1.Y(), minima.Y(), maxima.Y())) {
			if (sign.X() != -Sign(minima.X() - point1.X()) && sign.X() != Sign(minima.X() - point2.X())) {
				collision.didCollide = true;
				if (returnPoint) {
					collision.collisionPoint = Vector<T, 2>(minima.X(), point1.Y());
				}
			}
			else if (sign.X() != -Sign(maxima.X() - point1.X()) && sign.X() != Sign(maxima.X() - point2.X())) {
				collision.didCollide = true;
				if (returnPoint) {
					collision.collisionPoint = Vector<T, 2>(maxima.X(), point1.Y());
				}
			}
		}
		return collision;
	}

	Collision<T, 2> xCollision, yCollision;
	T xT, yT;
	T endT = (point2.X() - point1.X()) / direction.X();

	T tAtX1 = (minima.X() - point1.X()) / direction.X();
	if (Is_Between_Inc<T>(tAtX1, 0.0, endT) && Is_Between_Inc<T>(point1.Y() + direction.Y() * tAtX1, minima.Y(), maxima.Y())) {
		if (returnPoint) {
			xCollision.collisionPoint = point1 + direction * tAtX1;
		}
		xCollision.didCollide = true;
		xT = tAtX1;
	}

	T tAtX2 = (maxima.X() - point1.X()) / direction.X();
	if (Is_Between_Inc<T>(tAtX2, 0.0, endT) && Is_Between_Inc<T>(point1.Y() + direction.Y() * tAtX2, minima.Y(), maxima.Y())) {
		if (returnPoint) {
			xCollision.collisionPoint = point1 + direction * tAtX2;
		}
		xCollision.didCollide = true;
		xT = tAtX2;
	}

	T tAtY1 = (minima.Y() - point1.Y()) / direction.Y();
	if (Is_Between_Inc<T>(tAtY1, 0.0, endT) && Is_Between_Inc<T>(point1.X() + direction.X() * tAtY1, minima.X(), maxima.X())) {
		if (returnPoint) {
			yCollision.collisionPoint = point1 + direction * tAtY1;
		}
		yCollision.didCollide = true;
		yT = tAtY1;
	}

	T tAtY2 = (maxima.Y() - point1.Y()) / direction.Y();
	if (Is_Between_Inc<T>(tAtY2, 0.0, endT) && Is_Between_Inc<T>(point1.X() + direction.X() * tAtY2, minima.X(), maxima.X())) {
		if (returnPoint) {
			yCollision.collisionPoint = point1 + direction * tAtY2;
		}
		yCollision.didCollide = true;
		yT = tAtY2;
	}

	if (xCollision.didCollide && yCollision.didCollide) {
		collision = (xT < yT) ? xCollision : yCollision;
	}
	else {
		collision = (xCollision.didCollide) ? xCollision : yCollision;
	}

	if (!collision.didCollide && returnSeparator) {
		if (Is_Between_Inc<T>(point1.Y() + direction.Y() * tAtX1, minima.Y(), maxima.Y()) ||
			Is_Between_Inc<T>(point1.Y() + direction.Y() * tAtX2, minima.Y(), maxima.Y())) {

			collision.separator = Vector<T, 2>(1, 0);
		}
		else if (
			Is_Between_Inc<T>(point1.X() + direction.X() * tAtY1, minima.X(), maxima.X()) ||
			Is_Between_Inc<T>(point1.X() + direction.X() * tAtY2, minima.X(), maxima.X())) {

			collision.separator = Vector<T, 2>(0, 1);
		}
		else {
			collision.separator = direction.Orthogonal();
		}
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AxisAlignedRectangleCollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	AxisAlignedBox<T, 2> rectangle = in_mask1.Get_Transformed_Rectangle();
	Vector<T, 2> point = in_mask2.Get_Transformed_Point();

	bool xColl = Is_Between_Inc<T>(point.X(), rectangle.Get_Minima().X(), rectangle.Get_Maxima().X());
	bool yColl = Is_Between_Inc<T>(point.Y(), rectangle.Get_Minima().Y(), rectangle.Get_Maxima().Y());

	collision.didCollide = xColl && yColl;

	if (collision.didCollide && returnPoint) {
		collision.collisionPoint = point;
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = (yColl) ? Vector<T, 2>(1, 0) : Vector<T, 2>(0, 1);
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AxisAlignedRectangleCollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	AxisAlignedBox<T, 2> rectangle = in_mask1.Get_Transformed_Rectangle();
	Ray<T, 2> ray = in_mask2.Get_Transformed_Ray();

	Vector<T, 2> center = rectangle.Get_Center();
	Vector<T, 2> minima = rectangle.Get_Minima();
	Vector<T, 2> maxima = rectangle.Get_Maxima();
	Vector<T, 2> point = ray.Get_Point();
	Vector<T, 2> direction = ray.Get_Direction();
	Vector2i sign = Vector2i(Sign(direction.X()), Sign(direction.Y()));

	if (direction.Is_Zero()) {
		if (Evaluate_Typed(
			AxisAlignedRectangleCollisionMask<T>(rectangle, true),
			Point2CollisionMask<T>(point, true)
		).didCollide) {

			collision.didCollide = true;
			if (returnPoint) {
				collision.collisionPoint = point;
			}
		}

		return collision;
	}

	if (direction.X() == 0.0) {
		if (Is_Between_Inc(point.X(), minima.X(), maxima.X())) {
			if (sign.Y() == Sign(minima.Y() - point.Y())) {
				collision.didCollide = true;
				if (returnPoint) {
					collision.collisionPoint = Vector<T, 2>(0.0, minima.Y());
				}
			}
			else if (sign.Y() == Sign(maxima.Y() - point.Y())) {
				collision.didCollide = true;
				if (returnPoint) {
					collision.collisionPoint = Vector<T, 2>(0.0, maxima.Y());
				}
			}
		}
		return collision;
	}

	if (direction.Y() == 0.0) {
		if (Is_Between_Inc(point.Y(), minima.Y(), maxima.Y())) {
			if (sign.X() == Sign(minima.X() - point.X())) {
				collision.didCollide = true;
				if (returnPoint) {
					collision.collisionPoint = Vector<T, 2>(minima.X(), 0.0);
				}
			}
			else if (sign.X() == Sign(maxima.X() - point.X())) {
				collision.didCollide = true;
				if (returnPoint) {
					collision.collisionPoint = Vector<T, 2>(maxima.X(), 0.0);
				}
			}
		}
		return collision;
	}

	Collision<T, 2> xCollision, yCollision;
	T xT, yT;

	T tAtX1 = (minima.X() - point.X()) / direction.X();
	if (tAtX1 >= 0 && Is_Between_Inc<T>(point.Y() + direction.Y() * tAtX1, minima.Y(), maxima.Y())) {
		if (returnPoint) {
			xCollision.collisionPoint = point + direction * tAtX1;
		}
		xCollision.didCollide = true;
		xT = tAtX1;
	}

	T tAtX2 = (maxima.X() - point.X()) / direction.X();
	if (tAtX2 >= 0 && Is_Between_Inc<T>(point.Y() + direction.Y() * tAtX2, minima.Y(), maxima.Y())) {
		if (returnPoint) {
			xCollision.collisionPoint = point + direction * tAtX2;
		}
		xCollision.didCollide = true;
		xT = tAtX2;
	}

	T tAtY1 = (minima.Y() - point.Y()) / direction.Y();
	if (tAtY1 >= 0 && Is_Between_Inc<T>(point.X() + direction.X() * tAtY1, minima.X(), maxima.X())) {
		if (returnPoint) {
			yCollision.collisionPoint = point + direction * tAtY1;
		}
		yCollision.didCollide = true;
		yT = tAtY1;
	}

	T tAtY2 = (maxima.Y() - point.Y()) / direction.Y();
	if (tAtY2 >= 0 && Is_Between_Inc<T>(point.X() + direction.X() * tAtY2, minima.X(), maxima.X())) {
		if (returnPoint) {
			yCollision.collisionPoint = point + direction * tAtY2;
		}
		yCollision.didCollide = true;
		yT = tAtY2;
	}

	if (xCollision.didCollide && yCollision.didCollide) {
		collision = (xT < yT) ? xCollision : yCollision;
	}
	else {
		collision = (xCollision.didCollide) ? xCollision : yCollision;
	}

	if (!collision.didCollide && returnSeparator) {
		if (Is_Between_Inc<T>(point.Y() + direction.Y() * tAtX1, minima.Y(), maxima.Y()) ||
			Is_Between_Inc<T>(point.Y() + direction.Y() * tAtX2, minima.Y(), maxima.Y())) {

			collision.separator = Vector<T, 2>(1, 0);
		}
		else if (
			Is_Between_Inc<T>(point.X() + direction.X() * tAtY1, minima.X(), maxima.X()) ||
			Is_Between_Inc<T>(point.X() + direction.X() * tAtY2, minima.X(), maxima.X())) {

			collision.separator = Vector<T, 2>(0, 1);
		}
		else {
			collision.separator = direction.Orthogonal();
		}
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AxisAlignedRectangleCollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	AxisAlignedBox<T, 2> aaRectangle = in_mask1.Get_Transformed_Rectangle();
	Box<T, 2> rectangle = in_mask2.Get_Transformed_Rectangle();

	AxisAlignedRectangleCollisionMask<T> aaRectangleMask = AxisAlignedRectangleCollisionMask<T>(aaRectangle, true);
	RectangleCollisionMask<T> rectangleMask = RectangleCollisionMask<T>(rectangle, true);

	Vector<T, 2> corners[4];
	rectangle.Get_Corners(corners);

	for (uint i = 0; i < 4; i++) {
		collision = Evaluate_Typed(aaRectangleMask, LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(corners[i], corners[(i + 1) % 3]), true));
		if (collision.didCollide) {
			return collision;
		}
	}

	collision = Evaluate_Typed(Point2CollisionMask<T>(aaRectangle.Get_Center(), true), rectangleMask);

	if (!collision.didCollide && returnSeparator) {
		Vector<T, 2> aaCorners[4];
		aaRectangle.Get_Corners(aaCorners);

		Vector<T, 2> projectTo[4] = {
			Vector<T, 2>(1, 0),
			Vector<T, 2>(0, 1)
		};
		rectangle.Get_Axes(&projectTo[2]);
		uint index = Find_Projection_Separation(corners, 4, aaCorners, 4, projectTo, 4);
		if (index == 4) {
			throw ProcessFailureException();
		}
		collision.separator = projectTo[index];
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AxisAlignedRectangleCollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	AxisAlignedBox<T, 2> rectangle = in_mask1.Get_Transformed_Rectangle();
	Triangle<T, 2> triangle = in_mask2.Get_Transformed_Triangle();

	AxisAlignedRectangleCollisionMask<T> aaRectangleMask = AxisAlignedRectangleCollisionMask<T>(rectangle, true);
	Triangle2CollisionMask<T> triangleMask = Triangle2CollisionMask<T>(triangle, true);

	Vector<T, 2> corners[3];
	triangle.Get_Points(corners);

	for (uint i = 0; i < 3; i++) {
		collision = Evaluate_Typed(aaRectangleMask, LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(corners[i], corners[(i + 1) % 3]), true));
		if (collision.didCollide) {
			return collision;
		}
	}

	collision = Evaluate_Typed(Point2CollisionMask<T>(rectangle.Get_Center(), true), triangleMask);

	if (!collision.didCollide && returnSeparator) {
		Vector<T, 2> aaCorners[4];
		rectangle.Get_Corners(aaCorners);

		Vector<T, 2> projectTo[5] = {
			Vector<T, 2>(1, 0),
			Vector<T, 2>(0, 1)
		};
		triangle.Get_Lazy_Normals(&projectTo[2]);
		uint index = Find_Projection_Separation(corners, 3, aaCorners, 4, projectTo, 5);
		if (index == 5) {
			throw ProcessFailureException();
		}
		collision.separator = projectTo[index];
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(CircleCollisionMask<T>& in_mask1, CircleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	Circle<T> circle1 = in_mask1.Get_Transformed_Circle();
	Circle<T> circle2 = in_mask2.Get_Transformed_Circle();

	const T radii = circle1.Get_Radius() + circle2.Get_Radius();
	const T radiiSquared = radii * radii;
	const Vector<T, 2> offset = circle2.Get_Center() - circle1.Get_Center();
	const T offsetDistanceSquared = offset.Dot_Self();

	collision.didCollide = offsetDistanceSquared <= radiiSquared;
	if (collision.didCollide) {
		collision.collisionPoint = circle1.Get_Center() + offset / 2.0;
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = offset;
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(CircleCollisionMask<T>& in_mask1, HalfSpace2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	Circle<T> circle = in_mask1.Get_Transformed_Circle();
	HalfSpace<T, 2> halfSpace = in_mask2.Get_Transformed_Half_Space();

	T circleProjectionCoefficient = halfSpace.Get_Direction().Dot(circle.Get_Center());
	T halfSpaceProjectionCoefficient = halfSpace.Get_Projection_Coefficient();
	T radius = circle.Get_Radius();

	if (halfSpaceProjectionCoefficient < circleProjectionCoefficient + radius) {
		collision.didCollide = true;
		if (returnPoint) {
			collision.collisionPoint = halfSpace.Get_Direction() * (Max<T>(circleProjectionCoefficient - radius, halfSpaceProjectionCoefficient) + circleProjectionCoefficient + radius) / 2.0;
		}
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(CircleCollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	Circle<T> circle = in_mask1.Get_Transformed_Circle();
	Line<T, 2> line = in_mask2.Get_Transformed_Line();

	Point2CollisionMask<T> pointMask = Point2CollisionMask<T>(line.Get_Projection(circle.Get_Center()), true);
	CircleCollisionMask<T> circleMask = CircleCollisionMask<T>(circle, true);

	collision = Evaluate_Typed(circleMask, pointMask);

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(CircleCollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	Circle<T> circle = in_mask1.Get_Transformed_Circle();
	LineSegment<T, 2> lineSegment = in_mask2.Get_Transformed_Line_Segment();

	CircleCollisionMask<T> circleMask = CircleCollisionMask<T>(circle, true);

	Vector<T, 2> projectionPoint = lineSegment.Get_Projection(circle.Get_Center());

	Point2CollisionMask<T> pointMask1 = Point2CollisionMask<T>(projectionPoint, true);
	Point2CollisionMask<T> pointMask2 = Point2CollisionMask<T>(lineSegment.Get_Point1(), true);
	Point2CollisionMask<T> pointMask3 = Point2CollisionMask<T>(lineSegment.Get_Point2(), true);
	
	if (Is_Between_Inc<T>(
		lineSegment.Get_Projection_Coefficient(projectionPoint),
		lineSegment.Get_Projection_Coefficient1(),
		lineSegment.Get_Projection_Coefficient2())) {

		collision = Evaluate_Typed(circleMask, pointMask1);
	}
	else {
		collision = Evaluate_Typed(circleMask, pointMask2);

		if (!collision.didCollide) {
			collision = Evaluate_Typed(circleMask, pointMask3);
		}
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(CircleCollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	Circle<T> circle = in_mask1.Get_Transformed_Circle();
	Vector<T, 2> point = in_mask2.Get_Transformed_Point();

	if ((point - circle.Get_Center()).Dot_Self() <= circle.Get_Radius() * circle.Get_Radius()) {
		collision.didCollide = true;
		if (returnPoint) {
			collision.collisionPoint = point;
		}
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = point - circle.Get_Center();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(CircleCollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	Circle<T> circle = in_mask1.Get_Transformed_Circle();
	Ray<T, 2> ray = in_mask2.Get_Transformed_Ray();

	Ray2CollisionMask<T> rayMask = Ray2CollisionMask<T>(ray, true);

	Vector<T, 2> projectionPoint = ray.Get_Projection(circle.Get_Center());

	Point2CollisionMask<T> pointMask1 = Point2CollisionMask<T>(projectionPoint, true);
	Point2CollisionMask<T> pointMask2 = Point2CollisionMask<T>(ray.Get_Point(), true);

	if (ray.Get_Projection_Coefficient(projectionPoint) >= ray.Get_Projection_Coefficient()) {
		collision = Evaluate_Typed(pointMask1, rayMask);
	}

	if (!collision.didCollide) {
		collision = Evaluate_Typed(pointMask2, rayMask);
	}
	
	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(CircleCollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	Circle<T> circle = in_mask1.Get_Transformed_Circle();
	Box<T, 2> rectangle = in_mask2.Get_Transformed_Rectangle();

	Vector<T, 2> center = rectangle.Get_Center();
	CircleCollisionMask<T> circleMask = CircleCollisionMask<T>(circle, true);

	Vector<T, 2> corners[4];
	rectangle.Get_Corners(corners);

	Vector<T, 2> axes[2];
	rectangle.Get_Axes(axes);

	Vector<T, 2> newAxes[2][2] = {
		{axes[0], axes[1]},
		{axes[0], axes[1]}
	};
	newAxes[0][0].Add_To_Magnitude(circle.Get_Radius() * 2);
	newAxes[1][1].Add_To_Magnitude(circle.Get_Radius() * 2);

	RectangleCollisionMask<T> rectangleMasks[2] = {
		RectangleCollisionMask<T>(Box<T, 2>::From_Center_Axes(center, &newAxes[0][0]), true),
		RectangleCollisionMask<T>(Box<T, 2>::From_Center_Axes(center, &newAxes[1][0]), true)
	};

	Point2CollisionMask<T> pointMask = Point2CollisionMask<T>(circle.Get_Center(), true);

	Collision<T, 2> testCollision;
	for (uint i = 0; i < 2; i++) {
		if ((testCollision = Evaluate_Typed(pointMask, rectangleMasks[i])).didCollide) {
			collision = testCollision;
			if (returnPoint) {
				collision.collisionPoint[i] += newAxes[i][i].Normalized()[i] *
					Sign((center - collision.collisionPoint).Dot(newAxes[i][i])) *
					Min<T>(circle.Get_Radius(), newAxes[i][i].Magnitude() / (T)2);
			}
			return collision;
		}
	}

	if (!collision.didCollide) {
		for (uint i = 0; i < 4; i++) {
			pointMask = Point2CollisionMask<T>(corners[i]);
			if ((testCollision = Evaluate_Typed(circleMask, pointMask)).didCollide) {
				collision = testCollision;
				break;
			}
		}
	}

	if (!collision.didCollide && returnSeparator) {
		Vector<T, 2> normAxes[2] = {
			axes[0].Normalized(),
			axes[1].Normalized()
		};

		for (uint i = 0; i < 2; i++) {
			Range<T> rectRange(corners[0].Projection_Coeff(axes[i]), corners[3].Projection_Coeff(axes[i]));
			Range<T> circleRange(
				(circle.Get_Center() - normAxes[i] * circle.Get_Radius()).Projection_Coeff(axes[i]),
				(circle.Get_Center() + normAxes[i] * circle.Get_Radius()).Projection_Coeff(axes[i])
			);
			if (!rectRange.Intersection(circleRange)) {
				collision.separator = axes[i];
				return collision;
			}
		}

		throw ProcessFailureException();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(CircleCollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	Circle<T> circle = in_mask1.Get_Transformed_Circle();
	Triangle<T, 2> triangle = in_mask2.Get_Transformed_Triangle();

	CircleCollisionMask<T> circleMask = CircleCollisionMask<T>(circle, true);

	Vector<T, 2> corners[3];
	triangle.Get_Points(corners);

	LineSegment<T, 2> lineSegments[3] = {
		LineSegment<T, 2>::From_Points(corners[0], corners[1]),
		LineSegment<T, 2>::From_Points(corners[1], corners[2]),
		LineSegment<T, 2>::From_Points(corners[2], corners[0])
	};

	T selfProjections[3] = {
		lineSegments[0].Get_Projection_Coefficient1(),
		lineSegments[1].Get_Projection_Coefficient1(),
		lineSegments[2].Get_Projection_Coefficient1()
	};

	Vector<T, 2> centerProjection;
	for (uint i = 0; i < 3; i++) {
		centerProjection = lineSegments[i].Get_Projection(circle.Get_Center());
		collision = Evaluate_Typed(circleMask, Point2CollisionMask<T>(centerProjection, true));
		if (collision.didCollide &&
			Is_Between_Inc(
				lineSegments[i].Get_Projection_Coefficient(centerProjection),
				selfProjections[i],
				selfProjections[(i + 1) % 3]
			)) {
			return collision;
		}
	}

	for (uint i = 0; i < 3; i++) {
		collision = Evaluate_Typed(circleMask, Point2CollisionMask<T>(corners[i], true));
		if (collision.didCollide) {
			return collision;
		}
	}

	if (!collision.didCollide && returnSeparator) {
		Vector<T, 2> axes[3];
		triangle.Get_Lazy_Normals(axes);

		Vector<T, 2> normAxes[3] = {
			axes[0].Normalized(),
			axes[1].Normalized(),
			axes[2].Normalized()
		};

		for (uint i = 0; i < 3; i++) {
			Range<T> triangleRange(corners[0].Projection_Coeff(axes[i]), corners[3].Projection_Coeff(axes[i]));
			Range<T> circleRange(
				(circle.Get_Center() - normAxes[i] * circle.Get_Radius()).Projection_Coeff(axes[i]),
				(circle.Get_Center() + normAxes[i] * circle.Get_Radius()).Projection_Coeff(axes[i])
			);
			if (!triangleRange.Intersection(circleRange)) {
				collision.separator = axes[i];
				return collision;
			}
		}

		throw ProcessFailureException();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(HalfSpace2CollisionMask<T>& in_mask1, HalfSpace2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	HalfSpace<T, 2> halfSpace1 = in_mask1.Get_Transformed_Half_Space();
	HalfSpace<T, 2> halfSpace2 = in_mask2.Get_Transformed_Half_Space();

	Line<T, 2> line1 = Line<T, 2>::From_Point_Direction(halfSpace1.Get_Point(), halfSpace1.Get_Direction().Orthogonal());
	Line<T, 2> line2 = Line<T, 2>::From_Point_Direction(halfSpace2.Get_Point(), halfSpace2.Get_Direction().Orthogonal());

	Line2CollisionMask<T> lineMask1 = Line2CollisionMask<T>(line1, true);
	Line2CollisionMask<T> lineMask2 = Line2CollisionMask<T>(line2, true);

	Collision<T, 2> testCollision = Evaluate_Typed(lineMask1, lineMask2);

	if (testCollision.didCollide) {
		collision = testCollision;
	}
	else if (halfSpace1.Get_Direction().Dot(halfSpace2.Get_Direction()) >= 0.0) {
		collision.didCollide = true;
		if (returnPoint) {
			collision.collisionPoint =
				(halfSpace1.Get_Direction().Dot(halfSpace1.Get_Point()) >= halfSpace1.Get_Direction().Dot(halfSpace2.Get_Point())) ?
				halfSpace1.Get_Point() : halfSpace2.Get_Point();
		}
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = halfSpace1.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(HalfSpace2CollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	HalfSpace<T, 2> halfSpace = in_mask1.Get_Transformed_Half_Space();

	Line<T, 2> line1 = in_mask2.Get_Transformed_Line();
	Line<T, 2> line2 = Line<T, 2>::From_Point_Direction(halfSpace.Get_Point(), halfSpace.Get_Direction().Orthogonal());

	Line2CollisionMask<T> lineMask1 = Line2CollisionMask<T>(line1, true);
	Line2CollisionMask<T> lineMask2 = Line2CollisionMask<T>(line2, true);
	HalfSpace2CollisionMask<T> halfSpaceMask = HalfSpace2CollisionMask<T>(halfSpace);

	collision = Evaluate_Typed(lineMask1, lineMask2);

	if (!collision.didCollide) {
		collision = Evaluate_Typed(in_mask1, Point2CollisionMask<T>(line1.Get_Point()));
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(HalfSpace2CollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	HalfSpace<T, 2> halfSpace = in_mask1.Get_Transformed_Half_Space();
	LineSegment<T, 2> lineSegment = in_mask2.Get_Transformed_Line_Segment();

	HalfSpace2CollisionMask<T> halfSpaceMask = HalfSpace2CollisionMask<T>(halfSpace, true);
	Line2CollisionMask<T> lineMask = Line2CollisionMask<T>(
		Line<T, 2>::From_Point_Direction(halfSpace.Get_Point(), halfSpace.Get_Direction()), true);
	LineSegment2CollisionMask<T> lineSegmentMask = LineSegment2CollisionMask<T>(lineSegment, true);

	Collision testCollision = Evaluate_Typed(lineMask, lineSegmentMask);
	if (testCollision.didCollide) {
		collision = testCollision;
	}
	else {
		collision = Evaluate_Typed(halfSpaceMask, Point2CollisionMask<T>(lineSegment.Get_Point1(), true));
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(HalfSpace2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	HalfSpace<T, 2> halfSpace = in_mask1.Get_Transformed_Half_Space();
	Vector<T, 2> point = in_mask2.Get_Transformed_Point();

	if (halfSpace.Get_Projection_Coefficient() <= halfSpace.Get_Direction().Dot(point)) {
		collision.didCollide = true;
		if (returnPoint) {
			collision.collisionPoint = point;
		}
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(HalfSpace2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	HalfSpace<T, 2> halfSpace = in_mask1.Get_Transformed_Half_Space();
	Ray<T, 2> ray = in_mask2.Get_Transformed_Ray();

	HalfSpace2CollisionMask<T> halfSpaceMask = HalfSpace2CollisionMask<T>(halfSpace, true);
	Line2CollisionMask<T> lineMask = Line2CollisionMask<T>(
		Line<T, 2>::From_Point_Direction(halfSpace.Get_Point(), halfSpace.Get_Direction().Orthogonal()), true);
	Ray2CollisionMask<T> rayMask = Ray2CollisionMask<T>(ray, true);

	Collision testCollision = Evaluate_Typed(lineMask, rayMask);
	if (testCollision.didCollide) {
		collision = testCollision;
	}
	else {
		collision = Evaluate_Typed(halfSpaceMask, Point2CollisionMask<T>(ray.Get_Point(), true));
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(HalfSpace2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	HalfSpace halfSpace = in_mask1.Get_Transformed_Half_Space();
	Box<T, 2> rectangle = in_mask2.Get_Transformed_Rectangle();

	Vector<T, 2> center = rectangle.Get_Center();

	HalfSpace2CollisionMask<T> halfSpaceMask = HalfSpace2CollisionMask<T>(halfSpace, true);
	Line2CollisionMask<T> lineMask = Line2CollisionMask<T>(
		Line<T, 2>::From_Point_Direction(halfSpace.Get_Point(), halfSpace.Get_Point().Orthogonal()), true);
	RectangleCollisionMask<T> rectangleMask = RectangleCollisionMask<T>(rectangle, true);

	collision = Evaluate_Typed(lineMask, rectangleMask);
	
	if (!collision.didCollide) {
		collision = Evaluate_Typed(halfSpaceMask, Point2CollisionMask<T>(center, true));
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(HalfSpace2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	HalfSpace halfSpace = in_mask1.Get_Transformed_Half_Space();
	Triangle<T, 2> triangle = in_mask2.Get_Transformed_Triangle();

	Vector<T, 2> corners[4];
	triangle.Get_Points(corners);

	HalfSpace2CollisionMask<T> halfSpaceMask = HalfSpace2CollisionMask<T>(halfSpace, true);
	Point2CollisionMask<T> pointMasks[3] = {
		Point2CollisionMask<T>(corners[0], true),
		Point2CollisionMask<T>(corners[1], true),
		Point2CollisionMask<T>(corners[2], true)
	};

	Collision<T, 2> testCollision;
	for (uint i = 0; i < 3; i++) {
		testCollision = Evaluate_Typed(halfSpaceMask, pointMasks[i]);
		if (testCollision.didCollide) {
			collision = testCollision;
			break;
		}
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(LineSegment2CollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	LineSegment<T, 2> lineSegment1 = in_mask1.Get_Transformed_Line_Segment();
	LineSegment<T, 2> lineSegment2 = in_mask2.Get_Transformed_Line_Segment();

	bool thisReturnPoint = returnPoint;
	returnPoint = true;
	Collision<T, 2> testCollision = Evaluate_Typed(
		Line2CollisionMask<T>(Line<T, 2>::From_Points(lineSegment1.Get_Point1(), lineSegment1.Get_Point2()), true),
		Line2CollisionMask<T>(Line<T, 2>::From_Points(lineSegment2.Get_Point1(), lineSegment2.Get_Point2()), true)
	);
	returnPoint = thisReturnPoint;

	if (testCollision.didCollide &&
		Is_Between_Inc(
			lineSegment1.Get_Projection_Coefficient(testCollision.collisionPoint), 
			lineSegment1.Get_Projection_Coefficient1(), 
			lineSegment1.Get_Projection_Coefficient2()) &&
		Is_Between_Inc(
			lineSegment2.Get_Projection_Coefficient(testCollision.collisionPoint), 
			lineSegment2.Get_Projection_Coefficient1(), 
			lineSegment2.Get_Projection_Coefficient2())
		) {

		collision = testCollision;
	}

	if (!collision.didCollide && returnSeparator) {
		Vector<T, 2> points1[2] = {
			lineSegment1.Get_Point1(),
			lineSegment1.Get_Point2()
		};

		Vector<T, 2> points2[2] = {
			lineSegment2.Get_Point1(),
			lineSegment2.Get_Point2()
		};

		Vector<T, 2> projectTo[2] = {
			lineSegment1.Get_Direction().Orthogonal(),
			lineSegment2.Get_Direction().Orthogonal()
		};
		uint index = Find_Projection_Separation(points1, 2, points2, 2, projectTo, 2);
		if (index == 2) {
			throw ProcessFailureException();
		}
		collision.separator = projectTo[index];
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(LineSegment2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	LineSegment<T, 2> lineSegment = in_mask1.Get_Transformed_Line_Segment();
	Vector<T, 2> point = in_mask2.Get_Transformed_Point();

	Line<T, 2> line = Line<T, 2>::From_Point_Direction(lineSegment.Get_Point1(), lineSegment.Get_Offset());

	T projectionCoefficient = lineSegment.Get_Direction().Dot(point);

	if (Is_Between_Inc<T>(projectionCoefficient, lineSegment.Get_Projection_Coefficient1(), lineSegment.Get_Projection_Coefficient2())) {
		collision = Evaluate_Typed(Line2CollisionMask<T>(line, true), Point2CollisionMask<T>(point, true));
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = point - (lineSegment.Get_Point1() + lineSegment.Get_Direction() * projectionCoefficient);
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(LineSegment2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	LineSegment<T, 2> lineSegment = in_mask1.Get_Transformed_Line_Segment();
	Ray<T, 2> ray = in_mask2.Get_Transformed_Ray();

	bool thisReturnPoint = returnPoint;
	returnPoint = true;
	Collision<T, 2> testCollision = Evaluate_Typed(
		Line2CollisionMask<T>(Line<T, 2>::From_Points(lineSegment.Get_Point1(), lineSegment.Get_Point2()), true),
		Line2CollisionMask<T>(Line<T, 2>::From_Point_Direction(ray.Get_Point(), ray.Get_Direction()), true)
	);
	returnPoint = thisReturnPoint;

	if (testCollision.didCollide &&
		Is_Between_Inc(
			lineSegment.Get_Projection_Coefficient(testCollision.collisionPoint),
			lineSegment.Get_Projection_Coefficient1(),
			lineSegment.Get_Projection_Coefficient2()) &&
		ray.Get_Projection_Coefficient(testCollision.collisionPoint) >= ray.Get_Projection_Coefficient()) {

		collision = testCollision;
	}

	if (!collision.didCollide && returnSeparator) {
		Vector<T, 2> rayNormal = ray.Get_Direction().Orthogonal();
		if (Sign((lineSegment.Get_Point1() - ray.Get_Point()).Dot(rayNormal) -
				ray.Get_Point().Dot(rayNormal)) ==
			Sign((lineSegment.Get_Point2() - ray.Get_Point()).Dot(rayNormal) -
				ray.Get_Point().Dot(rayNormal))) {

			collision.separator = rayNormal;
		}
		else {
			collision.separator = lineSegment.Get_Direction().Orthogonal();
		}
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(LineSegment2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	LineSegment<T, 2> lineSegment = in_mask1.Get_Transformed_Line_Segment();
	Box<T, 2> rectangle = in_mask2.Get_Transformed_Rectangle();

	RectangleCollisionMask<T> rectangleMask = RectangleCollisionMask<T>(rectangle, true);

	Vector<T, 2> corners[4];
	rectangle.Get_Corners(corners);

	Vector<T, 2> projectionPoint;
	Collision<T, 2> testCollision;
	for (uint i = 0; i < 4; i++) {
		projectionPoint = lineSegment.Get_Projection(corners[i]);
		if (Is_Between_Inc(
			lineSegment.Get_Projection_Coefficient(projectionPoint), 
			lineSegment.Get_Projection_Coefficient1(), 
			lineSegment.Get_Projection_Coefficient2())) {

			testCollision = Evaluate_Typed(Point2CollisionMask<T>(projectionPoint, true), rectangleMask);
				if (testCollision.didCollide) {
					collision = testCollision;
					break;
				}
		}
	}

	if (!collision.didCollide && returnSeparator) {
		Vector<T, 2> points[2] = {
			lineSegment.Get_Point1(),
			lineSegment.Get_Point2()
		};

		Vector<T, 2> projectTo[3] = {
			lineSegment.Get_Direction().Orthogonal()
		};
		rectangle.Get_Axes(&projectTo[1]);
		uint index = Find_Projection_Separation(points, 2, corners, 4, projectTo, 3);
		if (index == 3) {
			throw ProcessFailureException();
		}
		collision.separator = projectTo[index];
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(LineSegment2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	LineSegment<T, 2> lineSegment = in_mask1.Get_Transformed_Line_Segment();
	Triangle<T, 2> triangle = in_mask2.Get_Transformed_Triangle();

	Vector<T, 2> corners[3];
	triangle.Get_Points(corners);

	Vector<T, 2> projectionPoints[3];
	for (uint i = 0; i < 3; i++) {
		projectionPoints[i] = lineSegment.Get_Projection(corners[i]);
	}

	Vector<T, 2> diff1, diff2;
	Vector<T, 2> testPoint;
	for (uint i = 0; i < 3; i++) {
		diff1 = corners[i] - projectionPoints[i];
		diff2 = corners[(i + 1) % 3] - projectionPoints[(i + 1) % 3];
		if (diff1.Dot(diff2) < 0.0) {
			testPoint = projectionPoints[i].Lerp(diff2.Magnitude() / diff1.Magnitude(), projectionPoints[(i + 1) % 3]);
			if (Is_Between_Inc(
					lineSegment.Get_Projection_Coefficient(testPoint),
					lineSegment.Get_Projection_Coefficient1(),
					lineSegment.Get_Projection_Coefficient2()
				)) {
				collision.didCollide = true;
				if (returnPoint) {
					collision.collisionPoint = testPoint;
				}
				break;
			}
		}
	}

	if (!collision.didCollide && returnSeparator) {
		Vector<T, 2> points[2] = {
			lineSegment.Get_Point1(),
			lineSegment.Get_Point2()
		};

		Vector<T, 2> projectTo[3] = {
			lineSegment.Get_Direction().Orthogonal()
		};
		triangle.Get_Lazy_Normals(&projectTo[1]);
		uint index = Find_Projection_Separation(points, 2, corners, 3, projectTo, 4);
		if (index == 4) {
			throw ProcessFailureException();
		}
		collision.separator = projectTo[index];
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Point2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	Vector<T, 2> point1 = in_mask1.Get_Transformed_Point();
	Vector<T, 2> point2 = in_mask1.Get_Transformed_Point();

	if (point1 == point2) {
		collision.didCollide = true;
		if (returnPoint) {
			collision.collisionPoint = point1;
		}
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = point1 - point2;
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Point2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	Vector<T, 2> point = in_mask1.Get_Transformed_Point();
	Ray<T, 2> ray = in_mask2.Get_Transformed_Ray();

	Collision<T, 2> testCollision = Evaluate_Typed(
		Line2CollisionMask<T>(Line<T, 2>::From_Point_Direction(ray.Get_Point(), ray.Get_Direction()), true),
		Point2CollisionMask<T>(point, true)
	);

	if (testCollision.didCollide && ray.Get_Projection_Coefficient(point) >= ray.Get_Projection_Coefficient()) {
		collision = testCollision;
	}

	if (!collision.didCollide && returnSeparator) {
		if (ray.Get_Projection_Coefficient(point) >= ray.Get_Projection_Coefficient()) {
			collision.separator = testCollision.separator;
		}
		else {
			collision.separator = point - ray.Get_Point();
		}
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Point2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	Box<T, 2> rectangle = in_mask2.Get_Transformed_Rectangle();
	Vector<T, 2> point = in_mask1.Get_Transformed_Point();

	Vector<T, 2> origin = rectangle.Get_Origin();
	Vector<T, 2> axes[2];
	rectangle.Get_Axes(axes);

	T pointDotAxis1 = axes[0].Dot(point - origin);
	T pointDotAxis2 = axes[1].Dot(point - origin);

	T axisMax1 = axes[0].Dot_Self();
	T axisMax2 = axes[1].Dot_Self();

	if (Is_Between_Inc<T>(pointDotAxis1, 0, axisMax1) && Is_Between_Inc<T>(pointDotAxis2, 0, axisMax2)) {
		collision.didCollide = true;
		if (returnPoint) {
			collision.collisionPoint = point;
		}
	}

	if (!collision.didCollide && returnSeparator) {
		if (Is_Between_Inc<T>(pointDotAxis1, 0, axisMax1)) {
			collision.separator = axes[1];
		}
		else if (Is_Between_Inc<T>(pointDotAxis2, 0, axisMax2)) {
			collision.separator = axes[0];
		}
		else {
			collision.separator = point - rectangle.Get_Center();
		}
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Point2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	Triangle<T, 2> triangle = in_mask2.Get_Transformed_Triangle();
	Vector<T, 2> point = in_mask1.Get_Transformed_Point();

	Vector<T, 2> points[3];
	triangle.Get_Points(points);

	Vector<T, 2> lazyNormals[3];
	triangle.Get_Lazy_Normals(lazyNormals);

	Point2CollisionMask<T> pointMask = Point2CollisionMask<T>(point, true);

	for (uint i = 0; i < 3; i++) {
		if (Sign(lazyNormals[i].Dot(point - points[i])) > 0) {
			if (returnSeparator) {
				collision.separator = lazyNormals[i];
				return collision;
			}
		}
	}

	collision.didCollide = true;
	if (returnPoint) {
		collision.collisionPoint = point;
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Ray2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	Ray<T, 2> ray1 = in_mask1.Get_Transformed_Ray();
	Ray<T, 2> ray2 = in_mask2.Get_Transformed_Ray();

	bool thisReturnPoint = returnPoint;
	returnPoint = true;
	Collision<T, 2> testCollision = Evaluate_Typed(
		Line2CollisionMask<T>(Line<T, 2>::From_Point_Direction(ray1.Get_Point(), ray1.Get_Direction()), true),
		Line2CollisionMask<T>(Line<T, 2>::From_Point_Direction(ray2.Get_Point(), ray2.Get_Direction()), true)
	);
	returnPoint = thisReturnPoint;

	if (testCollision.didCollide &&
		ray1.Get_Projection_Coefficient(testCollision.collisionPoint) >= ray1.Get_Projection_Coefficient() &&
		ray2.Get_Projection_Coefficient(testCollision.collisionPoint) >= ray2.Get_Projection_Coefficient()) {

		collision = testCollision;
	}

	if (!collision.didCollide && returnSeparator) {
		collision.collisionPoint = ray1.Get_Point() - ray2.Get_Point();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Ray2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	Ray<T, 2> ray = in_mask1.Get_Transformed_Ray();
	Box<T, 2> rectangle = in_mask2.Get_Transformed_Rectangle();

	Vector<T, 2> corners[4];
	rectangle.Get_Corners(corners);

	bool thisReturnPoint = returnPoint;
	returnPoint = true;
	LineSegment2CollisionMask<T> lineSegmentMasks[4] = {
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(corners[0], corners[1]), true),
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(corners[1], corners[3]), true),
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(corners[2], corners[3]), true),
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(corners[2], corners[0]), true)
	};
	returnPoint = thisReturnPoint;

	Ray2CollisionMask<T> rayMask = Ray2CollisionMask<T>(ray, true);

	Collision<T, 2> testCollision;
	for (uint i = 0; i < 4; i++) {
		testCollision = Evaluate_Typed(lineSegmentMasks[i], rayMask);
		if (testCollision.didCollide) {
			if (!collision.didCollide ||
				ray.Get_Projection_Coefficient(testCollision.collisionPoint) <
				ray.Get_Projection_Coefficient(collision.collisionPoint)) {

				collision = testCollision;
			}
		}
	}

	if (!collision.didCollide && returnSeparator) {
		Vector<T, 2> axes[2];
		rectangle.Get_Axes(axes);
		bool oneSided = true;
		Vector<T, 2> rayNormal = ray.Get_Direction().Orthogonal();
		auto sign = Sign((corners[0] - ray.Get_Point()).Dot(rayNormal));
		for (uint i = 1; i < 4; i++) {
			if (Sign((corners[0] - ray.Get_Point()).Dot(rayNormal)) != sign) {
				oneSided = false;
			}
		}

		if (oneSided) {
			collision.separator = rayNormal;
		}
		else {
			if (Is_Between_Inc(
				axes[0].Dot(ray.Get_Point()),
				axes[0].Dot(corners[0]),
				axes[0].Dot(corners[3])
			)) {
				collision.separator = axes[1];
			}
			else {
				collision.separator = axes[0];
			}
		}
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Ray2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	Ray<T, 2> ray = in_mask1.Get_Transformed_Ray();
	Triangle triangle = in_mask2.Get_Transformed_Triangle();

	Vector<T, 2> corners[3];
	triangle.Get_Points(corners);

	bool thisReturnPoint = returnPoint;
	returnPoint = true;
	LineSegment2CollisionMask<T> lineSegmentMasks[3] = {
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(corners[0], corners[1]), true),
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(corners[1], corners[2]), true),
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(corners[2], corners[0]), true),
	};
	returnPoint = thisReturnPoint;

	Ray2CollisionMask<T> rayMask = Ray2CollisionMask<T>(ray, true);

	Collision<T, 2> testCollision;
	for (uint i = 0; i < 3; i++) {
		testCollision = Evaluate_Typed(lineSegmentMasks[i], rayMask);
		if (testCollision.didCollide) {
			if (!collision.didCollide ||
				ray.Get_Projection_Coefficient(testCollision.collisionPoint) <
				ray.Get_Projection_Coefficient(collision.collisionPoint)) {

				collision = testCollision;
			}
		}
	}

	if (!collision.didCollide && returnSeparator) {
		if (!collision.didCollide && returnSeparator) {
			Vector<T, 2> axes[3];
			triangle.Get_Lazy_Normals(axes);
			bool oneSided = true;
			Vector<T, 2> rayNormal = ray.Get_Direction().Orthogonal();
			auto sign = Sign((corners[0] - ray.Get_Point()).Dot(rayNormal));
			for (uint i = 1; i < 3; i++) {
				if (Sign((corners[0] - ray.Get_Point()).Dot(rayNormal)) != sign) {
					oneSided = false;
				}
			}

			if (oneSided) {
				collision.separator = rayNormal;
			}
			else {
				Vector<T, 2> point = ray.Get_Point();
				uint index = Find_Projection_Separation(&point, 1, corners, 3, axes, 3);
				if (index == 3) {
					throw ProcessFailureException();
				}
				collision.separator = axes[index];
			}
		}
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(RectangleCollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	Box<T, 2> rectangle1 = in_mask1.Get_Transformed_Rectangle();
	Box<T, 2> rectangle2 = in_mask2.Get_Transformed_Rectangle();

	Vector<T, 2> corners[2][4];
	Vector<T, 2> projectees[2][2];
	Range<T> projections[2][2][2];

	rectangle1.Get_Corners(&corners[0][0]);
	rectangle2.Get_Corners(&corners[1][0]);
	rectangle1.Get_Axes(&projectees[0][0]);
	rectangle2.Get_Axes(&projectees[1][0]);

	uint i2;

	for (uint i = 0; i < 2; i++) {
		i2 = (i == 0) ? 1 : 0;

		for (uint j = 0; j < 2; j++) {
			projectees[i][j] = projectees[i][j].Orthogonal().Normalized();

			projections[i][j][i] = Range(
				projectees[i][j].Dot(corners[i][0]),
				projectees[i][j].Dot(corners[i][j + 1])
			);

			for (uint k = 0; k < 4; k++) {
				projections[i][j][i2].Expand_To(projectees[i][j].Dot(corners[i2][k]));
			}

			if (!projections[i][j][0].Intersection(projections[i][j][1])) {
				if (returnSeparator) {
					collision.separator = projectees[i][j];
				}
				return collision;
			}
		}
	}

	RectangleCollisionMask<T> rectangleMasks[2] = {
		RectangleCollisionMask<T>(rectangle1, true),
		RectangleCollisionMask<T>(rectangle2, true)
	};

	for (uint i = 0; i < 2; i++) {
		bool isInside = true;
		i2 = (i == 0) ? 1 : 0;

		for (uint j = 0; j < 4; j++) {
			if (!Evaluate_Typed(Point2CollisionMask<T>(corners[i][j], true), rectangleMasks[i2]).didCollide) {
				isInside = false;
				break;
			}
		}

		if (isInside) {
			collision.didCollide = true;
			if (returnPoint) {
				collision.collisionPoint = (corners[i][0] + corners[i][1] + corners[i][2] + corners[i][3]) / 4.0;
			}
		}
	}

	for (uint j = 0; j < 4; j++) {
		LineSegment<T, 2> edge = LineSegment<T, 2>::From_Points(corners[0][j], corners[0][(j + 1) % 4]);
		collision = Evaluate_Typed(LineSegment2CollisionMask<T>(edge), rectangleMasks[1]);
		if (collision.didCollide) {
			return collision;
		}
	}

	throw ProcessFailureException();
	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(RectangleCollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	Box<T, 2> rectangle = in_mask1.Get_Transformed_Rectangle();
	Triangle<T, 2> triangle = in_mask2.Get_Transformed_Triangle();

	RectangleCollisionMask<T> rectangleMask = RectangleCollisionMask<T>(rectangle, true);
	Triangle2CollisionMask<T> triangleMask = Triangle2CollisionMask<T>(triangle, true);

	Vector<T, 2> triangleCorners[3];
	triangle.Get_Points(triangleCorners);

	for (uint i = 0; i < 3; i++) {
		collision = Evaluate_Typed(LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(triangleCorners[i], triangleCorners[(i + 1) % 3]), true), rectangleMask);
		if (collision.didCollide) {
			return collision;
		}
	}

	collision = Evaluate_Typed(Point2CollisionMask<T>(rectangle.Get_Center(), true), triangleMask);
	if (collision.didCollide) {
		return collision;
	}

	collision = Evaluate_Typed(Point2CollisionMask<T>(triangleCorners[0], true), rectangleMask);

	if (!collision.didCollide && returnSeparator) {
		Vector<T, 2> rectCorners[4];
		rectangle.Get_Corners(rectCorners);

		Vector<T, 2> projectTo[5];
		rectangle.Get_Axes(projectTo);
		triangle.Get_Lazy_Normals(&projectTo[2]);

		uint index = Find_Projection_Separation(rectCorners, 4, triangleCorners, 3, projectTo, 5);
		if (index == 5) {
			throw ProcessFailureException();
		}
		collision.separator = projectTo[index];
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Triangle2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	Triangle<T, 2> triangle1 = in_mask1.Get_Transformed_Triangle();
	Triangle<T, 2> triangle2 = in_mask2.Get_Transformed_Triangle();

	Vector<T, 2> corners[2][3];
	Vector<T, 2> projectees[2][3];
	Range<T> projections[2][3][2];

	triangle1.Get_Points(&corners[0][0]);
	triangle2.Get_Points(&corners[1][0]);
	triangle1.Get_Point_Offsets(&projectees[0][0]);
	triangle2.Get_Point_Offsets(&projectees[1][0]);

	uint i2;

	// for each triangle
	for (uint i = 0; i < 2; i++) {
		i2 = (i == 0) ? 1 : 0;

		// for each side of that triangle
		for (uint j = 0; j < 3; j++) {
			projectees[i][j] = projectees[i][j].Orthogonal();

			projections[i][j][i] = Range(
				projectees[i][j].Dot(corners[i][j]), 
				projectees[i][j].Dot(corners[i][(j + 2) % 3])
			);

			// for each side of the other triangle
			for (uint k = 0; k < 3; k++) {
				projections[i][j][i2].Expand_To(projectees[i][j].Dot(corners[i2][k]));
			}

			if (!projections[i][j][0].Intersection(projections[i][j][1])) {
				if (returnSeparator) {
					collision.separator = projectees[i][j];
				}
				return collision;
			}
		}
	}

	Triangle2CollisionMask<T> triangleMasks[2] = {
		Triangle2CollisionMask<T>(triangle1, true),
		Triangle2CollisionMask<T>(triangle2, true)
	};

	for (uint i = 0; i < 2; i++) {
		bool isInside = true;
		i2 = (i == 0) ? 1 : 0;
		
		for (uint j = 0; j < 3; j++) {
			if (Evaluate_Typed(Point2CollisionMask<T>(corners[i][j], true), triangleMasks[i2]).didCollide) {
				collision.didCollide = true;
				collision.collisionPoint = corners[i][j];
				return collision;
			}
		}
	}

	for (uint j = 0; j < 3; j++) {
		LineSegment<T, 2> edge = LineSegment<T, 2>::From_Points(corners[0][j], corners[0][(j + 1) % 3]);
		collision = Evaluate_Typed(LineSegment2CollisionMask<T>(edge), triangleMasks[1]);
		if (collision.didCollide) {
			return collision;
		}
	}

	throw ProcessFailureException();
	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Line2CollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	Line<T, 2> line1 = in_mask1.Get_Transformed_Line();
	Line<T, 2> line2 = in_mask2.Get_Transformed_Line();

	Matrix<T, 2, 3> matrix;
	matrix.Column(0, line1.Get_Direction());
	matrix.Column(1, -line2.Get_Direction());
	matrix.Column(2, line2.Get_Point() - line2.Get_Point());
	matrix.RREFify();

	if (!matrix.Row_Is_Zero(1)) {
		collision.didCollide = true;
		if (returnPoint) {
			collision.collisionPoint = line1.Get_Point() + line1.Get_Direction() * matrix.Element(0, 2);
		}
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = line1.Get_Direction().Orthogonal();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Line2CollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	Line<T, 2> line = in_mask1.Get_Transformed_Line();
	LineSegment<T, 2> lineSegment = in_mask2.Get_Transformed_Line_Segment();

	Line2CollisionMask lineMask(line, true);

	bool thisReturnPoint = returnPoint;
	returnPoint = true;
	Collision<T, 2> testCollision = Evaluate_Typed(
		lineMask,
		Line2CollisionMask<T>(Line<T, 2>::From_Points(lineSegment.Get_Point1(), lineSegment.Get_Point2()), true)
	);
	returnPoint = thisReturnPoint;

	if (testCollision.didCollide && 
		Is_Between_Inc(
			lineSegment.Get_Projection_Coefficient(testCollision.collisionPoint),
			lineSegment.Get_Projection_Coefficient1(),
			lineSegment.Get_Projection_Coefficient2())
		) {

		collision = testCollision;
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = line.Get_Direction().Orthogonal();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Line2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	Line<T, 2> line = in_mask1.Get_Transformed_Line();
	Vector<T, 2> point = in_mask2.Get_Transformed_Point();

	Vector<T, 2> pointOffset = point - line.Get_Point();
	Vector<T, 2> direction = line.Get_Direction();

	if (pointOffset.X() / direction.X() == pointOffset.Y() / direction.Y()) {
		collision.didCollide = true;
		if (returnPoint) {
			collision.collisionPoint = point;
		}
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = line.Get_Direction().Orthogonal();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Line2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	Line<T, 2> line = in_mask1.Get_Transformed_Line();
	Ray<T, 2> ray = in_mask2.Get_Transformed_Ray();

	Line2CollisionMask<T> lineMask(line, true);

	bool thisReturnPoint = returnPoint;
	returnPoint = true;
	Collision<T, 2> testCollision = Evaluate_Typed(
		lineMask,
		Line2CollisionMask<T>(Line<T, 2>::From_Point_Direction(ray.Get_Point(), ray.Get_Direction()), true)
	);
	returnPoint = thisReturnPoint;

	if (testCollision.didCollide && ray.Get_Projection_Coefficient(testCollision.collisionPoint) >= ray.Get_Projection_Coefficient()) {
		collision = testCollision;
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = line.Get_Direction().Orthogonal();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Line2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	Line<T, 2> line = in_mask1.Get_Transformed_Line();
	Box<T, 2> rectangle = in_mask2.Get_Transformed_Rectangle();

	RectangleCollisionMask<T> rectangleMask = RectangleCollisionMask<T>(rectangle, true);

	Vector<T, 2> corners[4];
	rectangle.Get_Corners(corners);
	
	Vector<T, 2> projectionPoint;
	for (uint i = 0; i < 4; i++) {
		projectionPoint = line.Get_Projection(corners[i]);
		collision = Evaluate_Typed(Point2CollisionMask<T>(projectionPoint, true), rectangleMask);
		if (collision.didCollide) {
			return collision;
		}
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = line.Get_Direction().Orthogonal();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Line2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	Line<T, 2> line = in_mask1.Get_Transformed_Line();
	Triangle<T, 2> triangle = in_mask2.Get_Transformed_Triangle();

	Vector<T, 2> corners[3];
	triangle.Get_Points(corners);

	Vector<T, 2> projectionPoints[3];
	for (uint i = 0; i < 3; i++) {
		projectionPoints[i] = line.Get_Projection(corners[i]);
	}

	Vector<T, 2> diff1, diff2;
	for (uint i = 0; i < 3; i++) {
		diff1 = corners[i] - projectionPoints[i];
		diff2 = corners[(i + 1) % 3] - projectionPoints[(i + 1) % 3];
		if (diff1.Dot(diff2) < 0.0) {
			collision.didCollide = true;
			if (returnPoint) {
				collision.collisionPoint = projectionPoints[i].Lerp(diff2.Magnitude() / diff1.Magnitude(), projectionPoints[(i + 1) % 3]);
			}
			break;
		}
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = line.Get_Direction().Orthogonal();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AxisAlignedHalfSpace2CollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	AxisAlignedHalfSpace<T, 2> halfSpace = in_mask1.Get_Transformed_Half_Space();
	LineSegment<T, 2> lineSegment = in_mask2.Get_Transformed_Line_Segment();

	uint dimension = halfSpace.Get_Dimension();
	uint otherDimension = (dimension == 0) ? 1 : 0;

	Vector<T, 2> point1 = lineSegment.Get_Point1();
	Vector<T, 2> point2 = lineSegment.Get_Point2();

	Range<T> range = Range<T>(point1[dimension], point2[dimension]);

	if (Is_Between_Inc(
		halfSpace.Get_Value(),
		range.Get_Low(),
		range.Get_High()
	)) {
		collision.didCollide = true;
		if (returnPoint) {
			collision.collisionPoint[dimension] = halfSpace.Get_Value();
			collision.collisionPoint[otherDimension] = Lerp(
				point1[otherDimension], 
				point2[otherDimension], 
				halfSpace.Get_Value(), 
				point1[dimension], 
				point2[dimension]
			);
		}
	}
	else if (
		Ceq_Switch(point1[dimension], halfSpace.Get_Value(), halfSpace.Is_Positive()) ||
		Ceq_Switch(point2[dimension], halfSpace.Get_Value(), halfSpace.Is_Positive())) {

		collision.didCollide = true;
		if (returnPoint) {
			collision.collisionPoint = lineSegment.Get_Center();
		}
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AxisAlignedHalfSpace2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	AxisAlignedHalfSpace<T, 2> halfSpace = in_mask1.Get_Transformed_Half_Space();
	Vector<T, 2> point = in_mask2.Get_Transformed_Point();

	T pointValue = point[halfSpace.Get_Dimension()];
	T halfSpaceValue = halfSpace.Get_Value();

	if (Ceq_Switch(pointValue, halfSpaceValue, halfSpace.Is_Positive())) {
		collision.didCollide = true;
		if (returnPoint) {
			collision.collisionPoint = point;
		}
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AxisAlignedHalfSpace2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	AxisAlignedHalfSpace<T, 2> halfSpace = in_mask1.Get_Transformed_Half_Space();
	Ray<T, 2> ray = in_mask2.Get_Transformed_Ray();

	AxisAlignedHalfSpace2CollisionMask<T> halfSpaceMask = AxisAlignedHalfSpace2CollisionMask<T>(halfSpace, true);
	Line2CollisionMask<T> lineMask = Line2CollisionMask<T>(Line<T, 2>::From_Point_Direction(ray.Get_Point(), ray.Get_Direction()), true);

	bool thisReturnPoint = returnPoint;
	returnPoint = true;
	Collision<T, 2> testCollision = Evaluate_Typed(halfSpaceMask, lineMask);
	returnPoint = thisReturnPoint;

	if (testCollision.didCollide && ray.Get_Projection_Coefficient(testCollision.collisionPoint) >= ray.Get_Projection_Coefficient()) {
		collision.didCollide = true;
		if (returnPoint) {
			collision.collisionPoint = testCollision.collisionPoint;
		}
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AxisAlignedHalfSpace2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Box<T, 2> rectangle = in_mask2.Get_Transformed_Rectangle();

	Vector<T, 2> corners[4];
	rectangle.Get_Corners(corners);

	uint dimension = in_mask1.Get_Half_Space().Get_Dimension();

	uint minIndex = 0;
	uint maxIndex = 0;
	for (uint i = 1; i < 4; i++) {
		if (corners[i][dimension] < corners[minIndex][dimension]) minIndex = i;
		if (corners[i][dimension] > corners[maxIndex][dimension]) maxIndex = i;
	}

	return Evaluate_Typed(
		in_mask1,
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(corners[minIndex], corners[maxIndex]), true)
	);
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AxisAlignedHalfSpace2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	AxisAlignedHalfSpace<T, 2> halfSpace = in_mask1.Get_Transformed_Half_Space();
	Triangle<T, 2> triangle = in_mask2.Get_Transformed_Triangle();

	Vector<T, 2> corners[3];
	triangle.Get_Points(corners);

	uint dimension = halfSpace.Get_Dimension();

	uint minIndex = 0;
	uint maxIndex = 0;
	for (uint i = 1; i < 3; i++) {
		if (!collision.didCollide && Ceq_Switch(halfSpace.Get_Value(), corners[i][dimension], halfSpace.Is_Positive())) {
			collision.didCollide = true;
			if (!returnPoint) {
				return collision;
			}
		}
		if (corners[i][dimension] < corners[minIndex][dimension]) minIndex = i;
		if (corners[i][dimension] > corners[maxIndex][dimension]) maxIndex = i;
	}

	if (returnPoint && collision.didCollide) {
		collision = Evaluate_Typed(
			AxisAlignedHalfSpace2CollisionMask<T>(halfSpace, true),
			LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(corners[minIndex], corners[maxIndex]), true)
		);
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AxisAlignedHalfSpace2CollisionMask<T>& in_mask1, AxisAlignedHalfSpace2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	AxisAlignedHalfSpace<T, 2> halfSpace1 = in_mask1.Get_Transformed_Half_Space();
	AxisAlignedHalfSpace<T, 2> halfSpace2 = in_mask2.Get_Transformed_Half_Space();

	if (halfSpace1.Get_Dimension() == halfSpace2.Get_Dimension()) {
		uint dimension = halfSpace1.Get_Dimension();
		if (Ceq_Switch(halfSpace2.Get_Value(), halfSpace1.Get_Value(), halfSpace1.Is_Positive())) {
			collision.didCollide = true;
			if (returnPoint) {
				collision.collisionPoint[dimension] = halfSpace2.Get_Value();
			}
		}
		else if (Ceq_Switch(halfSpace1.Get_Value(), halfSpace2.Get_Value(), halfSpace2.Is_Positive())) {
			collision.didCollide = true;
			if (returnPoint) {
				collision.collisionPoint[dimension] = halfSpace1.Get_Value();
			}
		}
	}
	else {
		collision.didCollide = true;
		if (returnPoint) {
			collision.collisionPoint[halfSpace1.Get_Dimension()] = halfSpace1.Get_Value();
			collision.collisionPoint[halfSpace2.Get_Dimension()] = halfSpace2.Get_Value();
		}
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = halfSpace1.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AxisAlignedHalfSpace2CollisionMask<T>& in_mask1, AxisAlignedLine2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	AxisAlignedHalfSpace<T, 2> halfSpace = in_mask1.Get_Transformed_Half_Space();
	AxisAlignedLine<T, 2> line = in_mask2.Get_Transformed_Line();

	if (halfSpace.Get_Dimension() == line.Get_Dimension()) {
		uint dimension = halfSpace.Get_Dimension();
		if (Ceq_Switch(line.Get_Value(), halfSpace.Get_Value(), halfSpace.Is_Positive())) {
			collision.didCollide = true;
			if (returnPoint) {
				collision.collisionPoint[dimension] = line.Get_Value();
			}
		}
	}
	else {
		collision.didCollide = true;
		if (returnPoint) {
			collision.collisionPoint[halfSpace.Get_Dimension()] = halfSpace.Get_Value();
			collision.collisionPoint[line.Get_Dimension()] = line.Get_Value();
		}
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AxisAlignedHalfSpace2CollisionMask<T>& in_mask1, AxisAlignedRectangleCollisionMask<T>& in_mask2) {
	AxisAlignedBox<T, 2> rectangle = in_mask2.Get_Transformed_Rectangle();

	return Evaluate_Typed(
		in_mask1,
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(rectangle.Get_Minima(), rectangle.Get_Maxima()), true)
	);
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AxisAlignedHalfSpace2CollisionMask<T>& in_mask1, CircleCollisionMask<T>& in_mask2) {
	Circle<T> circle = in_mask2.Get_Transformed_Circle();

	uint dimension = in_mask1.Get_Half_Space().Get_Dimension();

	return Evaluate_Typed(
		in_mask1,
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(circle.Get_Extrema(dimension, false), circle.Get_Extrema(dimension, true)), true)
	);
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AxisAlignedHalfSpace2CollisionMask<T>& in_mask1, HalfSpace2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	AxisAlignedHalfSpace<T, 2> aaHalfSpace = in_mask1.Get_Transformed_Half_Space();
	HalfSpace<T, 2> halfSpace = in_mask2.Get_Transformed_Half_Space();

	Line<T, 2> line = Line<T, 2>::From_Point_Direction(halfSpace.Get_Point(), halfSpace.Get_Direction().Orthogonal());

	AxisAlignedHalfSpace2CollisionMask<T> aaHalfSpaceMask = AxisAlignedHalfSpace2CollisionMask<T>(aaHalfSpace, true);
	Line2CollisionMask<T> lineMask = Line2CollisionMask<T>(line, true);

	collision = Evaluate_Typed(aaHalfSpaceMask, lineMask);

	if (!collision.didCollide && Ceq_Switch(halfSpace.Get_Point()[aaHalfSpace.Get_Dimension()], aaHalfSpace.Get_Value(), aaHalfSpace.Is_Positive())) {
		collision.didCollide = true;
		if (returnPoint) {
			collision.collisionPoint = halfSpace.Get_Point();
		}
	}

	if (!collision.didCollide && returnSeparator) {
		collision.separator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AxisAlignedHalfSpace2CollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	AxisAlignedHalfSpace<T, 2> halfSpace = in_mask1.Get_Transformed_Half_Space();
	Line<T, 2> line = in_mask2.Get_Transformed_Line();

	uint dimension = halfSpace.Get_Dimension();

	if (line.Get_Direction()[dimension] != 0.0) {
		collision.didCollide = true;
		if (returnPoint) {
			collision.collisionPoint = line.Get_Point();
			collision.collisionPoint += line.Get_Direction() * (halfSpace.Get_Value() - line.Get_Point()[dimension]) / line.Get_Direction()[dimension];
		}
	}
	else if (Ceq_Switch(line.Get_Point()[dimension], halfSpace.Get_Value(), halfSpace.Is_Positive())) {
		collision.didCollide = true;
		if (returnPoint) {
			collision.collisionPoint = line.Get_Point();
		}
	} 

	if (!collision.didCollide && returnSeparator) {
		collision.separator = halfSpace.Get_Direction();
	}

	return collision;
}

template class InPlaceCollisionEvaluator<float, 2>;
template class InPlaceCollisionEvaluator<double, 2>;