#include "in_place_collision_evaluator2.h"
#include "exceptions.h"
#include "misc.h"
#include "range.h"


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
	Collision2d collision, testCollision;

	AxisAlignedRectangled rectangle = in_mask1->Get_Transformed_Rectangle();
	Circled circle = in_mask2->Get_Transformed_Circle();

	Vector2d corners[4];
	rectangle.Get_Corners(corners);

	AxisAlignedRectangleCollisionMask xRectangleMask = AxisAlignedRectangleCollisionMask(
		AxisAlignedRectangled::From_Extrema(
			rectangle.Get_Minima() - Vector2d(circle.Get_Radius(), 0), 
			rectangle.Get_Maxima() + Vector2d(circle.Get_Radius(), 0)
		),
		true
	);

	AxisAlignedRectangleCollisionMask yRectangleMask = AxisAlignedRectangleCollisionMask(
		AxisAlignedRectangled::From_Extrema(
			rectangle.Get_Minima() - Vector2d(0, circle.Get_Radius()),
			rectangle.Get_Maxima() + Vector2d(0, circle.Get_Radius())
		),
		true
	);

	Point2CollisionMask pointMask = Point2CollisionMask(circle.Get_Center(), true);
	CircleCollisionMask transformedCircleMask = CircleCollisionMask(circle, true);

	if ((testCollision = Evaluate_Typed(&xRectangleMask, &pointMask)).didCollide ||
		(testCollision = Evaluate_Typed(&yRectangleMask, &pointMask)).didCollide) {

		collision = testCollision;
	}

	if (!collision.didCollide) {
		for (uint i = 0; i < 4; i++) {
			pointMask = Point2CollisionMask(corners[i], true);
			if ((testCollision = Evaluate_Typed(&transformedCircleMask, &pointMask)).didCollide) {
				collision = testCollision;
				break;
			}
		}
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(AxisAlignedRectangleCollisionMask* in_mask1, HalfSpace2CollisionMask* in_mask2) {
	Collision2d collision;

	AxisAlignedRectangled rectangle = in_mask1->Get_Transformed_Rectangle();
	HalfSpace2d halfSpace = in_mask2->Get_Transformed_Half_Space();

	Vector2d corners[4];
	rectangle.Get_Corners(corners);

	Collision2d testCollision;
	HalfSpace2CollisionMask transformedHalfSpaceMask = HalfSpace2CollisionMask(halfSpace, true);
	for (uint i = 0; i < 4; i++) {
		Point2CollisionMask pointMask = Point2CollisionMask(corners[i], true);
		if ((testCollision = Evaluate_Typed(&transformedHalfSpaceMask, &pointMask)).didCollide) {
			collision = testCollision;
			break;
		}
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(AxisAlignedRectangleCollisionMask * in_mask1, Line2CollisionMask * in_mask2) {
	Collision2d collision;

	AxisAlignedRectangled rectangle = in_mask1->Get_Transformed_Rectangle();
	Line2d line = in_mask2->Get_Transformed_Line();

	Vector2d center = rectangle.Get_Center();
	Vector2d minima = rectangle.Get_Minima();
	Vector2d maxima = rectangle.Get_Maxima();
	Vector2d point = line.Get_Point();
	Vector2d direction = line.Get_Direction();
	Vector2i sign = Vector2i(Sign(direction.X()), Sign(direction.Y()));

	if (direction.Is_Zero()) {
		if (Evaluate_Typed(
			&AxisAlignedRectangleCollisionMask(rectangle, true),
			&Point2CollisionMask(point, true)
		).didCollide) {
			collision.didCollide = true;
			collision.collisionPoint = point;
		}

		return collision;
	}

	if (direction.X() == 0.0) {
		if (Is_Between_Inc<double>(point.Y(), minima.Y(), maxima.Y())) {
			collision.didCollide = true;
			collision.collisionPoint = (point.Y() < minima.Y()) ? Vector2d(point.X(), minima.Y()) : (point.Y() > maxima.Y()) ? Vector2d(point.X(), maxima.Y()) : point;
		}
		return collision;
	}

	if (direction.Y() == 0.0) { 
		if (Is_Between_Inc<double>(point.X(), minima.X(), maxima.X())) {
			collision.didCollide = true;
			collision.collisionPoint = (point.X() < minima.X()) ? Vector2d(minima.X(), point.Y()) : (point.X() > maxima.X()) ? Vector2d(maxima.X(), point.Y()) : point;
		}
		return collision;
	}

	Collision2d xCollision, yCollision;
	double xT, yT;

	if (point.X() < center.X()) {
		double tAtX1 = (minima.X() - point.X()) / direction.X();
		if (Is_Between_Inc<double>(point.Y() + direction.Y() * tAtX1, minima.Y(), maxima.Y())) {
			xCollision.collisionPoint = point + direction * tAtX1;
			xCollision.didCollide = true;
			xT = tAtX1;
		}
	}
	else {
		double tAtX2 = (maxima.X() - point.X()) / direction.X();
		if (Is_Between_Inc<double>(point.Y() + direction.Y() * tAtX2, minima.Y(), maxima.Y())) {
			xCollision.collisionPoint = point + direction * tAtX2;
			xCollision.didCollide = true;
			xT = tAtX2;
		}
	}

	if (point.Y() < center.Y()) {
		double tAtY1 = (minima.Y() - point.Y()) / direction.Y();
		if (Is_Between_Inc<double>(point.X() + direction.X() * tAtY1, minima.X(), maxima.X())) {
			yCollision.collisionPoint = point + direction * tAtY1;
			yCollision.didCollide = true;
			yT = tAtY1;
		}
	}
	else {
		double tAtY2 = (maxima.Y() - point.Y()) / direction.Y();
		if (Is_Between_Inc<double>(point.X() + direction.X() * tAtY2, minima.X(), maxima.X())) {
			yCollision.collisionPoint = point + direction * tAtY2;
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

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(AxisAlignedRectangleCollisionMask* in_mask1, LineSegment2CollisionMask* in_mask2) {
	Collision2d collision;

	AxisAlignedRectangled rectangle = in_mask1->Get_Transformed_Rectangle();
	LineSegment2d lineSegment = in_mask2->Get_Transformed_Line_Segment();

	Vector2d center = rectangle.Get_Center();
	Vector2d minima = rectangle.Get_Minima();
	Vector2d maxima = rectangle.Get_Maxima();
	Vector2d point1 = lineSegment.Get_Point1();
	Vector2d point2 = lineSegment.Get_Point2();
	Vector2d direction = lineSegment.Get_Direction();
	Vector2i sign = Vector2i(Sign(direction.X()), Sign(direction.Y()));

	if (direction.Is_Zero()) {
		if (Evaluate_Typed(
			&AxisAlignedRectangleCollisionMask(rectangle, true),
			&Point2CollisionMask(point1, true)
		).didCollide) {

			collision.didCollide = true;
			collision.collisionPoint = point1;
		}

		return collision;
	}

	if (direction.X() == 0.0) {
		if (sign.Y() != -Sign(minima.Y() - point1.Y()) && sign.Y() != Sign(minima.Y() - point2.Y())) {
			collision.didCollide = true;
			collision.collisionPoint = Vector2d(0.0, minima.Y());
		}
		else if (sign.Y() != -Sign(maxima.Y() - point1.Y()) && sign.Y() != Sign(maxima.Y() - point2.Y())) {
			collision.didCollide = true;
			collision.collisionPoint = Vector2d(0.0, maxima.Y());
		}
		return collision;
	}

	if (direction.Y() == 0.0) {
		if (sign.X() != -Sign(minima.X() - point1.X()) && sign.X() != Sign(minima.X() - point2.X())) {
			collision.didCollide = true;
			collision.collisionPoint = Vector2d(minima.X(), 0.0);
		}
		else if (sign.X() != -Sign(maxima.X() - point1.X()) && sign.X() != Sign(maxima.X() - point2.X())) {
			collision.didCollide = true;
			collision.collisionPoint = Vector2d(maxima.X(), 0.0);
		}
		return collision;
	}

	Collision2d xCollision, yCollision;
	double xT, yT;
	double endT = (point2.X() - point1.X()) / direction.X();

	double tAtX1 = (minima.X() - point1.X()) / direction.X();
	if (Is_Between_Inc<double>(tAtX1, 0.0, endT) && Is_Between_Inc<double>(point1.Y() + direction.Y() * tAtX1, minima.Y(), maxima.Y())) {
		xCollision.collisionPoint = point1 + direction * tAtX1;
		xCollision.didCollide = true;
		xT = tAtX1;
	}

	double tAtX2 = (maxima.X() - point1.X()) / direction.X();
	if (Is_Between_Inc<double>(tAtX2, 0.0, endT) && Is_Between_Inc<double>(point1.Y() + direction.Y() * tAtX2, minima.Y(), maxima.Y())) {
		xCollision.collisionPoint = point1 + direction * tAtX2;
		xCollision.didCollide = true;
		xT = tAtX2;
	}

	double tAtY1 = (minima.Y() - point1.Y()) / direction.Y();
	if (Is_Between_Inc<double>(tAtY1, 0.0, endT) && Is_Between_Inc<double>(point1.X() + direction.X() * tAtY1, minima.X(), maxima.X())) {
		yCollision.collisionPoint = point1 + direction * tAtY1;
		yCollision.didCollide = true;
		yT = tAtY1;
	}

	double tAtY2 = (maxima.Y() - point1.Y()) / direction.Y();
	if (Is_Between_Inc<double>(tAtY2, 0.0, endT) && Is_Between_Inc<double>(point1.X() + direction.X() * tAtY2, minima.X(), maxima.X())) {
		yCollision.collisionPoint = point1 + direction * tAtY2;
		yCollision.didCollide = true;
		yT = tAtY2;
	}

	if (xCollision.didCollide && yCollision.didCollide) {
		collision = (xT < yT) ? xCollision : yCollision;
	}
	else {
		collision = (xCollision.didCollide) ? xCollision : yCollision;
	}

	return collision;
}
	

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(AxisAlignedRectangleCollisionMask* in_mask1, Point2CollisionMask* in_mask2) {
	Collision2d collision;

	AxisAlignedRectangled rectangle = in_mask1->Get_Transformed_Rectangle();
	Vector2d point = in_mask2->Get_Transformed_Point();

	collision.didCollide =
		Is_Between_Inc<double>(point.X(), rectangle.Get_Minima().X(), rectangle.Get_Maxima().X()) &&
		Is_Between_Inc<double>(point.Y(), rectangle.Get_Minima().Y(), rectangle.Get_Maxima().Y());

	if (collision.didCollide) {
		collision.collisionPoint = point;
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(AxisAlignedRectangleCollisionMask * in_mask1, Ray2CollisionMask * in_mask2) {
	Collision2d collision;

	AxisAlignedRectangled rectangle = in_mask1->Get_Transformed_Rectangle();
	Ray2d ray = in_mask2->Get_Transformed_Ray();

	Vector2d center = rectangle.Get_Center();
	Vector2d minima = rectangle.Get_Minima();
	Vector2d maxima = rectangle.Get_Maxima();
	Vector2d point = ray.Get_Point();
	Vector2d direction = ray.Get_Direction();
	Vector2i sign = Vector2i(Sign(direction.X()), Sign(direction.Y()));

	if (direction.Is_Zero()) {
		if (Evaluate_Typed(
			&AxisAlignedRectangleCollisionMask(rectangle, true),
			&Point2CollisionMask(point, true)
		).didCollide) {

			collision.didCollide = true;
			collision.collisionPoint = point;
		}

		return collision;
	}

	if (direction.X() == 0.0) {
		if (sign.Y() == Sign(minima.Y() - point.Y())) {
			collision.didCollide = true;
			collision.collisionPoint = Vector2d(0.0, minima.Y());
		}
		else if (sign.Y() == Sign(maxima.Y() - point.Y())) {
			collision.didCollide = true;
			collision.collisionPoint = Vector2d(0.0, maxima.Y());
		}
		return collision;
	}

	if (direction.Y() == 0.0) {
		if (sign.X() == Sign(minima.X() - point.X())) {
			collision.didCollide = true;
			collision.collisionPoint = Vector2d(minima.X(), 0.0);
		}
		else if (sign.X() == Sign(maxima.X() - point.X())) {
			collision.didCollide = true;
			collision.collisionPoint = Vector2d(maxima.X(), 0.0);
		}
		return collision;
	}

	Collision2d xCollision, yCollision;
	double xT, yT;

	double tAtX1 = (minima.X() - point.X()) / direction.X();
	if (tAtX1 >= 0 && Is_Between_Inc<double>(point.Y() + direction.Y() * tAtX1, minima.Y(), maxima.Y())) {
		xCollision.collisionPoint = point + direction * tAtX1;
		xCollision.didCollide = true;
		xT = tAtX1;
	}

	double tAtX2 = (maxima.X() - point.X()) / direction.X();
	if (tAtX2 >= 0 && Is_Between_Inc<double>(point.Y() + direction.Y() * tAtX2, minima.Y(), maxima.Y())) {
		xCollision.collisionPoint = point + direction * tAtX2;
		xCollision.didCollide = true;
		xT = tAtX2;
	}

	double tAtY1 = (minima.Y() - point.Y()) / direction.Y();
	if (tAtY1 >= 0 && Is_Between_Inc<double>(point.X() + direction.X() * tAtY1, minima.X(), maxima.X())) {
		yCollision.collisionPoint = point + direction * tAtY1;
		yCollision.didCollide = true;
		yT = tAtY1;
	}

	double tAtY2 = (maxima.Y() - point.Y()) / direction.Y();
	if (tAtY2 >= 0 && Is_Between_Inc<double>(point.X() + direction.X() * tAtY2, minima.X(), maxima.X())) {
		yCollision.collisionPoint = point + direction * tAtY2;
		yCollision.didCollide = true;
		yT = tAtY2;
	}

	if (xCollision.didCollide && yCollision.didCollide) {
		collision = (xT < yT) ? xCollision : yCollision;
	}
	else {
		collision = (xCollision.didCollide) ? xCollision : yCollision;
	}

	return collision;
}

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

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(CircleCollisionMask* in_mask1, HalfSpace2CollisionMask* in_mask2) {
	Collision2d collision;

	Circled circle = in_mask1->Get_Transformed_Circle();
	HalfSpace2d halfSpace = in_mask2->Get_Transformed_Half_Space();

	double circleProjectionCoefficient = halfSpace.Get_Direction().Dot(circle.Get_Center());
	double halfSpaceProjectionCoefficient = halfSpace.Get_Projection_Coefficient();
	double radius = circle.Get_Radius();

	if (halfSpaceProjectionCoefficient < circleProjectionCoefficient + radius) {
		collision.didCollide = true;
		collision.collisionPoint = halfSpace.Get_Direction() * (Greater_Of<double>(circleProjectionCoefficient - radius, halfSpaceProjectionCoefficient) + circleProjectionCoefficient + radius) / 2.0;
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(CircleCollisionMask* in_mask1, Line2CollisionMask* in_mask2) {
	Circled circle = in_mask1->Get_Transformed_Circle();
	Line2d line = in_mask2->Get_Transformed_Line();

	Point2CollisionMask pointMask = Point2CollisionMask(line.Get_Projection(circle.Get_Center()), true);
	CircleCollisionMask circleMask = CircleCollisionMask(circle, true);

	return Evaluate_Typed(&circleMask, &pointMask);
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(CircleCollisionMask* in_mask1, LineSegment2CollisionMask* in_mask2) {
	Collision2d collision;

	Circled circle = in_mask1->Get_Transformed_Circle();
	LineSegment2d lineSegment = in_mask2->Get_Transformed_Line_Segment();

	Point2CollisionMask pointMask1 = Point2CollisionMask(lineSegment.Get_Projection(circle.Get_Center()), true);
	Point2CollisionMask pointMask2 = Point2CollisionMask(lineSegment.Get_Point1(), true);
	Point2CollisionMask pointMask3 = Point2CollisionMask(lineSegment.Get_Point2(), true);
	
	collision = Evaluate_Typed(in_mask1, &pointMask1);

	if (!collision.didCollide || 
		!Is_Between_Inc<double>(
			lineSegment.Get_Projection_Coefficient(collision.collisionPoint), 
			lineSegment.Get_Projection_Coefficient1(), 
			lineSegment.Get_Projection_Coefficient2())) {

		collision = Evaluate_Typed(in_mask1, &pointMask2);
		if (!collision.didCollide) {
			collision = Evaluate_Typed(in_mask1, &pointMask3);
		}
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(CircleCollisionMask* in_mask1, Point2CollisionMask* in_mask2) {
	Collision2d collision;

	Circled circle = in_mask1->Get_Transformed_Circle();
	Vector2d point = in_mask2->Get_Transformed_Point();

	if ((point - circle.Get_Center()).Dot_Self() <= circle.Get_Radius() * circle.Get_Radius()) {
		collision.didCollide = true;
		collision.collisionPoint = point;
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(CircleCollisionMask* in_mask1, Ray2CollisionMask* in_mask2) {
	Collision2d collision;

	Circled circle = in_mask1->Get_Transformed_Circle();
	Ray2d ray = in_mask2->Get_Transformed_Ray();

	Point2CollisionMask pointMask1 = Point2CollisionMask(ray.Get_Projection(circle.Get_Center()), true);
	Point2CollisionMask pointMask2 = Point2CollisionMask(ray.Get_Point(), true);

	collision = Evaluate_Typed(in_mask1, &pointMask1);

	if (!collision.didCollide || ray.Get_Projection_Coefficient(collision.collisionPoint) < ray.Get_Projection_Coefficient()) {
		collision = Evaluate_Typed(in_mask1, &pointMask2);
	}
	
	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(HalfSpace2CollisionMask* in_mask1, HalfSpace2CollisionMask* in_mask2) {
	HalfSpace2d halfSpace1 = in_mask1->Get_Transformed_Half_Space();
	HalfSpace2d halfSpace2 = in_mask2->Get_Transformed_Half_Space();

	Line2d line1 = Line2d::From_Point_Direction(halfSpace1.Get_Point(), halfSpace1.Get_Direction().Orthogonal());
	Line2d line2 = Line2d::From_Point_Direction(halfSpace2.Get_Point(), halfSpace2.Get_Direction().Orthogonal());

	Line2CollisionMask lineMask1 = Line2CollisionMask(line1, true);
	Line2CollisionMask lineMask2 = Line2CollisionMask(line2, true);

	return Evaluate_Typed(&lineMask1, &lineMask2);
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(HalfSpace2CollisionMask* in_mask1, Line2CollisionMask* in_mask2) {
	Collision2d collision;

	HalfSpace2d halfSpace = in_mask1->Get_Transformed_Half_Space();

	Line2d line1 = in_mask2->Get_Transformed_Line();
	Line2d line2 = Line2d::From_Point_Direction(halfSpace.Get_Point(), halfSpace.Get_Direction().Orthogonal());

	Line2CollisionMask lineMask1 = Line2CollisionMask(line1, true);
	Line2CollisionMask lineMask2 = Line2CollisionMask(line2, true);
	HalfSpace2CollisionMask halfSpaceMask = HalfSpace2CollisionMask(halfSpace);

	collision = Evaluate_Typed(&lineMask1, &lineMask2);

	if (!collision.didCollide) {
		collision = Evaluate_Typed(in_mask1, &Point2CollisionMask(line1.Get_Point()));
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(HalfSpace2CollisionMask* in_mask1, Point2CollisionMask* in_mask2) {
	Collision2d collision;

	HalfSpace2d halfSpace = in_mask1->Get_Transformed_Half_Space();
	Vector2d point = in_mask2->Get_Transformed_Point();

	if (halfSpace.Get_Projection_Coefficient() <= halfSpace.Get_Direction().Dot(point)) {
		collision.didCollide = true;
		collision.collisionPoint = point;
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(LineSegment2CollisionMask* in_mask1, Point2CollisionMask* in_mask2) {
	Collision2d collision;

	LineSegment2d lineSegment = in_mask1->Get_Transformed_Line_Segment();
	Vector2d point = in_mask2->Get_Transformed_Point();

	Line2d line = Line2d::From_Point_Direction(lineSegment.Get_Point1(), lineSegment.Get_Offset());

	double projectionCoefficient = lineSegment.Get_Direction().Dot(point);

	if (Is_Between_Inc<double>(projectionCoefficient, lineSegment.Get_Projection_Coefficient1(), lineSegment.Get_Projection_Coefficient2())) {
		collision = Evaluate_Typed(&Line2CollisionMask(line, true), &Point2CollisionMask(point, true));
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(Point2CollisionMask* in_mask1, Point2CollisionMask* in_mask2) {
	Collision2d collision;

	Vector2d point1 = in_mask1->Get_Transformed_Point();
	Vector2d point2 = in_mask1->Get_Transformed_Point();

	if (point1 == point2) {
		collision.didCollide = true;
		collision.collisionPoint = point1;
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(Point2CollisionMask * in_mask1, RectangleCollisionMask * in_mask2) {
	Collision2d collision;

	Rectangled rectangle = in_mask2->Get_Transformed_Rectangle();
	Vector2d point = in_mask1->Get_Transformed_Point();

	Vector2d origin = rectangle.Get_Origin();
	Vector2d axes[2];
	rectangle.Get_Axes(axes);

	double pointDotAxis1 = axes[0].Dot(point - origin);
	double pointDotAxis2 = axes[1].Dot(point - origin);

	double axisMax1 = axes[0].Dot_Self();
	double axisMax2 = axes[1].Dot_Self();

	if (Is_Between_Inc<double>(pointDotAxis1, 0, axisMax1) && Is_Between_Inc<double>(pointDotAxis2, 0, axisMax2)) {
		collision.didCollide = true;
		collision.collisionPoint = point;
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(Point2CollisionMask * in_mask1, Triangle2CollisionMask * in_mask2) {
	Collision2d collision;

	Triangle2d triangle = in_mask2->Get_Transformed_Triangle();
	Vector2d point = in_mask1->Get_Transformed_Point();

	Vector2d points[3];
	triangle.Get_Points(points);

	Vector2d lazyNormals[3];
	triangle.Get_Lazy_Normals(lazyNormals);

	Point2CollisionMask pointMask = Point2CollisionMask(point, true);

	collision.didCollide = true;
	for (uint i = 0; i < 3; i++) {
		HalfSpace2CollisionMask halfSpaceMask = HalfSpace2CollisionMask(HalfSpace2d::From_Point_Direction(points[i], lazyNormals[i]), true);
		if (!Evaluate_Typed(&halfSpaceMask, &pointMask).didCollide) {
			collision.didCollide = false;
			break;
		}
	}

	if (collision.didCollide) collision.collisionPoint = point;

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(Triangle2CollisionMask * in_mask1, Triangle2CollisionMask * in_mask2) {
	Collision2d collision;



	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(Line2CollisionMask * in_mask1, Line2CollisionMask * in_mask2) {
	Collision2d collision;

	Line2d line1 = in_mask1->Get_Transformed_Line();
	Line2d line2 = in_mask2->Get_Transformed_Line();

	Matrix<double, 2, 3> matrix;
	matrix.Column(0, line1.Get_Direction());
	matrix.Column(1, -line2.Get_Direction());
	matrix.Column(2, line2.Get_Point() - line2.Get_Point());
	matrix.RREFify();

	if (!matrix.Row_Is_Zero(1)) {
		collision.didCollide = true;
		collision.collisionPoint = line1.Get_Point() + line1.Get_Direction() * matrix.Element(0, 2);
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(Line2CollisionMask * in_mask1, LineSegment2CollisionMask * in_mask2) {
	Collision2d collision;

	Line2d line = in_mask1->Get_Transformed_Line();
	LineSegment2d lineSegment = in_mask2->Get_Transformed_Line_Segment();

	Line2d derivedLine = Line2d::From_Points(lineSegment.Get_Point1(), lineSegment.Get_Point2());

	Line2CollisionMask lineMask1 = Line2CollisionMask(line, true);
	Line2CollisionMask lineMask2 = Line2CollisionMask(derivedLine, true);

	Vector2d direction = derivedLine.Get_Direction();

	Collision2d testCollision;
	if ((testCollision = Evaluate_Typed(&lineMask1, &lineMask2)).didCollide &&
		Is_Between_Inc<double>(
			direction.Dot(testCollision.collisionPoint),
			direction.Dot(lineSegment.Get_Point1()),
			direction.Dot(lineSegment.Get_Point2())
			)) {

		collision = testCollision;
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(Line2CollisionMask * in_mask1, Point2CollisionMask * in_mask2) {
	Collision2d collision;

	Line2d line = in_mask1->Get_Transformed_Line();
	Vector2d point = in_mask2->Get_Transformed_Point();

	Vector2d pointOffset = point - line.Get_Point();
	Vector2d direction = line.Get_Direction();

	if (pointOffset.X() / direction.X() == pointOffset.Y() / direction.Y()) {
		collision.didCollide = true;
		collision.collisionPoint = point;
	}

	return collision;
}
