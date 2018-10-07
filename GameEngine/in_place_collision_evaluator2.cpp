#include "in_place_collision_evaluator2.h"
#include "exceptions.h"
#include "misc.h"
#include "range.h"


Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(AxisAlignedRectangleCollisionMask& in_mask1, AxisAlignedRectangleCollisionMask& in_mask2) {
	Collision2d collision;

	AxisAlignedRectangled rectangle1 = in_mask1.Get_Transformed_Rectangle();
	AxisAlignedRectangled rectangle2 = in_mask2.Get_Transformed_Rectangle();

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

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(AxisAlignedRectangleCollisionMask& in_mask1, CircleCollisionMask& in_mask2) {
	Collision2d collision, testCollision;

	AxisAlignedRectangled rectangle = in_mask1.Get_Transformed_Rectangle();
	Circled circle = in_mask2.Get_Transformed_Circle();

	Vector2d corners[4];
	rectangle.Get_Corners(corners);
	Vector2d rectCenter = rectangle.Get_Center();

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

	if ((testCollision = Evaluate_Typed(xRectangleMask, pointMask)).didCollide) {
		collision = testCollision;
		collision.collisionPoint[0] += Sign(rectCenter.X() - collision.collisionPoint.X()) *
			Min(circle.Get_Radius(), xRectangleMask.Get_Rectangle().Get_Dimensions().X() / 2.0);
		return collision;
	}

	if ((testCollision = Evaluate_Typed(yRectangleMask, pointMask)).didCollide) {
		collision = testCollision;
		collision.collisionPoint[1] += Sign(rectCenter.Y() - collision.collisionPoint.Y()) *
			Min(circle.Get_Radius(), yRectangleMask.Get_Rectangle().Get_Dimensions().Y() / 2.0);
		return collision;
	}

	if (!collision.didCollide) {
		for (uint i = 0; i < 4; i++) {
			pointMask = Point2CollisionMask(corners[i], true);
			if ((testCollision = Evaluate_Typed(transformedCircleMask, pointMask)).didCollide) {
				collision = testCollision;
				break;
			}
		}
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(AxisAlignedRectangleCollisionMask& in_mask1, HalfSpace2CollisionMask& in_mask2) {
	Collision2d collision;

	AxisAlignedRectangled rectangle = in_mask1.Get_Transformed_Rectangle();
	HalfSpace2d halfSpace = in_mask2.Get_Transformed_Half_Space();

	Vector2d corners[4];
	rectangle.Get_Corners(corners);

	Collision2d testCollision;
	HalfSpace2CollisionMask transformedHalfSpaceMask = HalfSpace2CollisionMask(halfSpace, true);
	for (uint i = 0; i < 4; i++) {
		Point2CollisionMask pointMask = Point2CollisionMask(corners[i], true);
		if ((testCollision = Evaluate_Typed(transformedHalfSpaceMask, pointMask)).didCollide) {
			collision = testCollision;
			break;
		}
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(AxisAlignedRectangleCollisionMask& in_mask1, Line2CollisionMask& in_mask2) {
	Collision2d collision;

	AxisAlignedRectangled rectangle = in_mask1.Get_Transformed_Rectangle();
	Line2d line = in_mask2.Get_Transformed_Line();

	Vector2d center = rectangle.Get_Center();
	Vector2d minima = rectangle.Get_Minima();
	Vector2d maxima = rectangle.Get_Maxima();
	Vector2d point = line.Get_Point();
	Vector2d direction = line.Get_Direction();
	Vector2i sign = Vector2i(Sign(direction.X()), Sign(direction.Y()));

	if (direction.Is_Zero()) {
		if (Evaluate_Typed(
			AxisAlignedRectangleCollisionMask(rectangle, true),
			Point2CollisionMask(point, true)
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

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(AxisAlignedRectangleCollisionMask& in_mask1, LineSegment2CollisionMask& in_mask2) {
	Collision2d collision;

	AxisAlignedRectangled rectangle = in_mask1.Get_Transformed_Rectangle();
	LineSegment2d lineSegment = in_mask2.Get_Transformed_Line_Segment();

	Vector2d center = rectangle.Get_Center();
	Vector2d minima = rectangle.Get_Minima();
	Vector2d maxima = rectangle.Get_Maxima();
	Vector2d point1 = lineSegment.Get_Point1();
	Vector2d point2 = lineSegment.Get_Point2();
	Vector2d direction = lineSegment.Get_Direction();
	Vector2i sign = Vector2i(Sign(direction.X()), Sign(direction.Y()));

	if (direction.Is_Zero()) {
		if (Evaluate_Typed(
			AxisAlignedRectangleCollisionMask(rectangle, true),
			Point2CollisionMask(point1, true)
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

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(AxisAlignedRectangleCollisionMask& in_mask1, Point2CollisionMask& in_mask2) {
	Collision2d collision;

	AxisAlignedRectangled rectangle = in_mask1.Get_Transformed_Rectangle();
	Vector2d point = in_mask2.Get_Transformed_Point();

	collision.didCollide =
		Is_Between_Inc<double>(point.X(), rectangle.Get_Minima().X(), rectangle.Get_Maxima().X()) &&
		Is_Between_Inc<double>(point.Y(), rectangle.Get_Minima().Y(), rectangle.Get_Maxima().Y());

	if (collision.didCollide) {
		collision.collisionPoint = point;
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(AxisAlignedRectangleCollisionMask& in_mask1, Ray2CollisionMask& in_mask2) {
	Collision2d collision;

	AxisAlignedRectangled rectangle = in_mask1.Get_Transformed_Rectangle();
	Ray2d ray = in_mask2.Get_Transformed_Ray();

	Vector2d center = rectangle.Get_Center();
	Vector2d minima = rectangle.Get_Minima();
	Vector2d maxima = rectangle.Get_Maxima();
	Vector2d point = ray.Get_Point();
	Vector2d direction = ray.Get_Direction();
	Vector2i sign = Vector2i(Sign(direction.X()), Sign(direction.Y()));

	if (direction.Is_Zero()) {
		if (Evaluate_Typed(
			AxisAlignedRectangleCollisionMask(rectangle, true),
			Point2CollisionMask(point, true)
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

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(AxisAlignedRectangleCollisionMask& in_mask1, RectangleCollisionMask& in_mask2) {
	Collision2d collision;

	AxisAlignedRectangled aaRectangle = in_mask1.Get_Transformed_Rectangle();
	Rectangled rectangle = in_mask2.Get_Transformed_Rectangle();

	AxisAlignedRectangleCollisionMask aaRectangleMask = AxisAlignedRectangleCollisionMask(aaRectangle, true);
	RectangleCollisionMask rectangleMask = RectangleCollisionMask(rectangle, true);

	Vector2d corners[4];
	rectangle.Get_Corners(corners);

	for (uint i = 0; i < 4; i++) {
		collision = Evaluate_Typed(aaRectangleMask, LineSegment2CollisionMask(LineSegment2d::From_Points(corners[i], corners[(i + 1) % 3]), true));
		if (collision.didCollide) {
			return collision;
		}
	}

	collision = Evaluate_Typed(Point2CollisionMask(aaRectangle.Get_Center(), true), rectangleMask);

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(AxisAlignedRectangleCollisionMask& in_mask1, Triangle2CollisionMask& in_mask2) {
	Collision2d collision;



	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(CircleCollisionMask& in_mask1, CircleCollisionMask& in_mask2) {
	Collision2d collision;

	Circled circle1 = in_mask1.Get_Transformed_Circle();
	Circled circle2 = in_mask2.Get_Transformed_Circle();

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

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(CircleCollisionMask& in_mask1, HalfSpace2CollisionMask& in_mask2) {
	Collision2d collision;

	Circled circle = in_mask1.Get_Transformed_Circle();
	HalfSpace2d halfSpace = in_mask2.Get_Transformed_Half_Space();

	double circleProjectionCoefficient = halfSpace.Get_Direction().Dot(circle.Get_Center());
	double halfSpaceProjectionCoefficient = halfSpace.Get_Projection_Coefficient();
	double radius = circle.Get_Radius();

	if (halfSpaceProjectionCoefficient < circleProjectionCoefficient + radius) {
		collision.didCollide = true;
		collision.collisionPoint = halfSpace.Get_Direction() * (Max<double>(circleProjectionCoefficient - radius, halfSpaceProjectionCoefficient) + circleProjectionCoefficient + radius) / 2.0;
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(CircleCollisionMask& in_mask1, Line2CollisionMask& in_mask2) {
	Circled circle = in_mask1.Get_Transformed_Circle();
	Line2d line = in_mask2.Get_Transformed_Line();

	Point2CollisionMask pointMask = Point2CollisionMask(line.Get_Projection(circle.Get_Center()), true);
	CircleCollisionMask circleMask = CircleCollisionMask(circle, true);

	return Evaluate_Typed(circleMask, pointMask);
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(CircleCollisionMask& in_mask1, LineSegment2CollisionMask& in_mask2) {
	Collision2d collision;

	Circled circle = in_mask1.Get_Transformed_Circle();
	LineSegment2d lineSegment = in_mask2.Get_Transformed_Line_Segment();

	Point2CollisionMask pointMask1 = Point2CollisionMask(lineSegment.Get_Projection(circle.Get_Center()), true);
	Point2CollisionMask pointMask2 = Point2CollisionMask(lineSegment.Get_Point1(), true);
	Point2CollisionMask pointMask3 = Point2CollisionMask(lineSegment.Get_Point2(), true);
	
	collision = Evaluate_Typed(in_mask1, pointMask1);

	if (!collision.didCollide || 
		!Is_Between_Inc<double>(
			lineSegment.Get_Projection_Coefficient(collision.collisionPoint), 
			lineSegment.Get_Projection_Coefficient1(), 
			lineSegment.Get_Projection_Coefficient2())) {

		collision = Evaluate_Typed(in_mask1, pointMask2);
		if (!collision.didCollide) {
			collision = Evaluate_Typed(in_mask1, pointMask3);
		}
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(CircleCollisionMask& in_mask1, Point2CollisionMask& in_mask2) {
	Collision2d collision;

	Circled circle = in_mask1.Get_Transformed_Circle();
	Vector2d point = in_mask2.Get_Transformed_Point();

	if ((point - circle.Get_Center()).Dot_Self() <= circle.Get_Radius() * circle.Get_Radius()) {
		collision.didCollide = true;
		collision.collisionPoint = point;
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(CircleCollisionMask& in_mask1, Ray2CollisionMask& in_mask2) {
	Collision2d collision;

	Circled circle = in_mask1.Get_Transformed_Circle();
	Ray2d ray = in_mask2.Get_Transformed_Ray();

	Point2CollisionMask pointMask1 = Point2CollisionMask(ray.Get_Projection(circle.Get_Center()), true);
	Point2CollisionMask pointMask2 = Point2CollisionMask(ray.Get_Point(), true);

	collision = Evaluate_Typed(in_mask1, pointMask1);

	if (!collision.didCollide || ray.Get_Projection_Coefficient(collision.collisionPoint) < ray.Get_Projection_Coefficient()) {
		collision = Evaluate_Typed(in_mask1, pointMask2);
	}
	
	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(CircleCollisionMask& in_mask1, RectangleCollisionMask& in_mask2) {
	Collision2d collision;

	Circled circle = in_mask1.Get_Transformed_Circle();
	Rectangled rectangle = in_mask2.Get_Transformed_Rectangle();

	Vector2d center = rectangle.Get_Center();
	CircleCollisionMask circleMask = CircleCollisionMask(circle, true);

	Vector2d corners[4];
	rectangle.Get_Corners(corners);

	Vector2d axes[2];
	rectangle.Get_Axes(axes);

	Vector2d newAxes[2][2] = {
		{axes[0], axes[1]},
		{axes[0], axes[1]}
	};
	newAxes[0][0].Add_To_Magnitude(circle.Get_Radius() * 2);
	newAxes[1][1].Add_To_Magnitude(circle.Get_Radius() * 2);

	RectangleCollisionMask rectangleMasks[2] = {
		RectangleCollisionMask(Rectangled::From_Center_Axes(center, &newAxes[0][0]), true),
		RectangleCollisionMask(Rectangled::From_Center_Axes(center, &newAxes[1][0]), true)
	};

	Point2CollisionMask pointMask = Point2CollisionMask(circle.Get_Center(), true);

	Collision2d testCollision;
	for (uint i = 0; i < 2; i++) {
		if ((testCollision = Evaluate_Typed(pointMask, rectangleMasks[i])).didCollide) {
			collision = testCollision;
			collision.collisionPoint[i] += newAxes[i][i].Normalized()[i] *
				Sign((center - collision.collisionPoint).Dot(newAxes[i][i])) *
				Min(circle.Get_Radius(), newAxes[i][i].Magnitude() / 2.0);
			return collision;
		}
	}

	if (!collision.didCollide) {
		for (uint i = 0; i < 4; i++) {
			pointMask = Point2CollisionMask(corners[i]);
			if ((testCollision = Evaluate_Typed(circleMask, pointMask)).didCollide) {
				collision = testCollision;
				break;
			}
		}
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(CircleCollisionMask& in_mask1, Triangle2CollisionMask& in_mask2) {
	Collision2d collision;

	Circled circle = in_mask1.Get_Transformed_Circle();
	Triangle2d triangle = in_mask2.Get_Transformed_Triangle();

	CircleCollisionMask circleMask = CircleCollisionMask(circle, true);

	Vector2d corners[3];
	triangle.Get_Points(corners);

	LineSegment2d lineSegments[3] = {
		LineSegment2d::From_Points(corners[0], corners[1]),
		LineSegment2d::From_Points(corners[1], corners[2]),
		LineSegment2d::From_Points(corners[2], corners[0])
	};

	double selfProjections[3] = {
		lineSegments[0].Get_Projection_Coefficient1(),
		lineSegments[1].Get_Projection_Coefficient1(),
		lineSegments[2].Get_Projection_Coefficient1()
	};

	Vector2d centerProjection;
	for (uint i = 0; i < 3; i++) {
		centerProjection = lineSegments[i].Get_Projection(circle.Get_Center());
		collision = Evaluate_Typed(circleMask, Point2CollisionMask(centerProjection, true));
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
		collision = Evaluate_Typed(circleMask, Point2CollisionMask(corners[i], true));
		if (collision.didCollide) {
			return collision;
		}
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(HalfSpace2CollisionMask& in_mask1, HalfSpace2CollisionMask& in_mask2) {
	Collision2d collision;

	HalfSpace2d halfSpace1 = in_mask1.Get_Transformed_Half_Space();
	HalfSpace2d halfSpace2 = in_mask2.Get_Transformed_Half_Space();

	Line2d line1 = Line2d::From_Point_Direction(halfSpace1.Get_Point(), halfSpace1.Get_Direction().Orthogonal());
	Line2d line2 = Line2d::From_Point_Direction(halfSpace2.Get_Point(), halfSpace2.Get_Direction().Orthogonal());

	Line2CollisionMask lineMask1 = Line2CollisionMask(line1, true);
	Line2CollisionMask lineMask2 = Line2CollisionMask(line2, true);

	Collision2d testCollision = Evaluate_Typed(lineMask1, lineMask2);

	if (testCollision.didCollide) {
		collision = testCollision;
	}
	else if (halfSpace1.Get_Direction().Dot(halfSpace2.Get_Direction()) >= 0.0) {
		collision.didCollide = true;
		collision.collisionPoint = 
			(halfSpace1.Get_Direction().Dot(halfSpace1.Get_Point()) >= halfSpace1.Get_Direction().Dot(halfSpace2.Get_Point())) ? 
				halfSpace1.Get_Point() : halfSpace2.Get_Point();
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(HalfSpace2CollisionMask& in_mask1, Line2CollisionMask& in_mask2) {
	Collision2d collision;

	HalfSpace2d halfSpace = in_mask1.Get_Transformed_Half_Space();

	Line2d line1 = in_mask2.Get_Transformed_Line();
	Line2d line2 = Line2d::From_Point_Direction(halfSpace.Get_Point(), halfSpace.Get_Direction().Orthogonal());

	Line2CollisionMask lineMask1 = Line2CollisionMask(line1, true);
	Line2CollisionMask lineMask2 = Line2CollisionMask(line2, true);
	HalfSpace2CollisionMask halfSpaceMask = HalfSpace2CollisionMask(halfSpace);

	collision = Evaluate_Typed(lineMask1, lineMask2);

	if (!collision.didCollide) {
		collision = Evaluate_Typed(in_mask1, Point2CollisionMask(line1.Get_Point()));
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(HalfSpace2CollisionMask& in_mask1, LineSegment2CollisionMask& in_mask2) {
	Collision2d collision;

	HalfSpace2d halfSpace = in_mask1.Get_Transformed_Half_Space();
	LineSegment2d lineSegment = in_mask2.Get_Transformed_Line_Segment();

	HalfSpace2CollisionMask halfSpaceMask = HalfSpace2CollisionMask(halfSpace, true);
	Line2CollisionMask lineMask = Line2CollisionMask(
		Line2d::From_Point_Direction(halfSpace.Get_Point(), halfSpace.Get_Direction()), true);
	LineSegment2CollisionMask lineSegmentMask = LineSegment2CollisionMask(lineSegment, true);

	Collision testCollision = Evaluate_Typed(lineMask, lineSegmentMask);
	if (testCollision.didCollide) {
		collision = testCollision;
	}
	else {
		collision = Evaluate_Typed(halfSpaceMask, Point2CollisionMask(lineSegment.Get_Point1(), true));
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(HalfSpace2CollisionMask& in_mask1, Point2CollisionMask& in_mask2) {
	Collision2d collision;

	HalfSpace2d halfSpace = in_mask1.Get_Transformed_Half_Space();
	Vector2d point = in_mask2.Get_Transformed_Point();

	if (halfSpace.Get_Projection_Coefficient() <= halfSpace.Get_Direction().Dot(point)) {
		collision.didCollide = true;
		collision.collisionPoint = point;
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(HalfSpace2CollisionMask& in_mask1, Ray2CollisionMask& in_mask2) {
	Collision2d collision;

	HalfSpace2d halfSpace = in_mask1.Get_Transformed_Half_Space();
	Ray2d ray = in_mask2.Get_Transformed_Ray();

	HalfSpace2CollisionMask halfSpaceMask = HalfSpace2CollisionMask(halfSpace, true);
	Line2CollisionMask lineMask = Line2CollisionMask(
		Line2d::From_Point_Direction(halfSpace.Get_Point(), halfSpace.Get_Direction().Orthogonal()), true);
	Ray2CollisionMask rayMask = Ray2CollisionMask(ray, true);

	Collision testCollision = Evaluate_Typed(lineMask, rayMask);
	if (testCollision.didCollide) {
		collision = testCollision;
	}
	else {
		collision = Evaluate_Typed(halfSpaceMask, Point2CollisionMask(ray.Get_Point(), true));
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(HalfSpace2CollisionMask& in_mask1, RectangleCollisionMask& in_mask2) {
	Collision2d collision;

	HalfSpace halfSpace = in_mask1.Get_Transformed_Half_Space();
	Rectangled rectangle = in_mask2.Get_Transformed_Rectangle();

	Vector2d center = rectangle.Get_Center();

	HalfSpace2CollisionMask halfSpaceMask = HalfSpace2CollisionMask(halfSpace, true);
	Line2CollisionMask lineMask = Line2CollisionMask(
		Line2d::From_Point_Direction(halfSpace.Get_Point(), halfSpace.Get_Point().Orthogonal()), true);
	RectangleCollisionMask rectangleMask = RectangleCollisionMask(rectangle, true);

	collision = Evaluate_Typed(lineMask, rectangleMask);
	
	if (!collision.didCollide) {
		collision = Evaluate_Typed(halfSpaceMask, Point2CollisionMask(center, true));
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(HalfSpace2CollisionMask& in_mask1, Triangle2CollisionMask& in_mask2) {
	Collision2d collision;

	HalfSpace halfSpace = in_mask1.Get_Transformed_Half_Space();
	Triangle2d triangle = in_mask2.Get_Transformed_Triangle();

	Vector2d corners[4];
	triangle.Get_Points(corners);

	HalfSpace2CollisionMask halfSpaceMask = HalfSpace2CollisionMask(halfSpace, true);
	Point2CollisionMask pointMasks[3] = {
		Point2CollisionMask(corners[0], true),
		Point2CollisionMask(corners[1], true),
		Point2CollisionMask(corners[2], true)
	};

	Collision2d testCollision;
	for (uint i = 0; i < 3; i++) {
		testCollision = Evaluate_Typed(halfSpaceMask, pointMasks[i]);
		if (testCollision.didCollide) {
			collision = testCollision;
			break;
		}
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(LineSegment2CollisionMask& in_mask1, LineSegment2CollisionMask& in_mask2) {
	Collision2d collision;

	LineSegment2d lineSegment1 = in_mask1.Get_Transformed_Line_Segment();
	LineSegment2d lineSegment2 = in_mask2.Get_Transformed_Line_Segment();

	Collision2d testCollision = Evaluate_Typed(
		Line2CollisionMask(Line2d::From_Points(lineSegment1.Get_Point1(), lineSegment1.Get_Point2()), true),
		Line2CollisionMask(Line2d::From_Points(lineSegment2.Get_Point1(), lineSegment2.Get_Point2()), true)
	);

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

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(LineSegment2CollisionMask& in_mask1, Point2CollisionMask& in_mask2) {
	Collision2d collision;

	LineSegment2d lineSegment = in_mask1.Get_Transformed_Line_Segment();
	Vector2d point = in_mask2.Get_Transformed_Point();

	Line2d line = Line2d::From_Point_Direction(lineSegment.Get_Point1(), lineSegment.Get_Offset());

	double projectionCoefficient = lineSegment.Get_Direction().Dot(point);

	if (Is_Between_Inc<double>(projectionCoefficient, lineSegment.Get_Projection_Coefficient1(), lineSegment.Get_Projection_Coefficient2())) {
		collision = Evaluate_Typed(Line2CollisionMask(line, true), Point2CollisionMask(point, true));
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(LineSegment2CollisionMask& in_mask1, Ray2CollisionMask& in_mask2) {
	Collision2d collision;

	LineSegment2d lineSegment = in_mask1.Get_Transformed_Line_Segment();
	Ray2d ray = in_mask2.Get_Transformed_Ray();

	Collision2d testCollision = Evaluate_Typed(
		Line2CollisionMask(Line2d::From_Points(lineSegment.Get_Point1(), lineSegment.Get_Point2()), true),
		Line2CollisionMask(Line2d::From_Point_Direction(ray.Get_Point(), ray.Get_Direction()), true)
	);

	if (testCollision.didCollide &&
		Is_Between_Inc(
			lineSegment.Get_Projection_Coefficient(testCollision.collisionPoint),
			lineSegment.Get_Projection_Coefficient1(),
			lineSegment.Get_Projection_Coefficient2()) &&
		ray.Get_Projection_Coefficient(testCollision.collisionPoint) >= ray.Get_Projection_Coefficient()) {

		collision = testCollision;
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(Point2CollisionMask& in_mask1, Point2CollisionMask& in_mask2) {
	Collision2d collision;

	Vector2d point1 = in_mask1.Get_Transformed_Point();
	Vector2d point2 = in_mask1.Get_Transformed_Point();

	if (point1 == point2) {
		collision.didCollide = true;
		collision.collisionPoint = point1;
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(Point2CollisionMask& in_mask1, Ray2CollisionMask& in_mask2) {
	Collision2d collision;

	Ray2d ray = in_mask2.Get_Transformed_Ray();

	Collision2d testCollision = Evaluate_Typed(
		Line2CollisionMask(Line2d::From_Point_Direction(ray.Get_Point(), ray.Get_Direction()), true),
		in_mask1
	);

	if (testCollision.didCollide && ray.Get_Projection_Coefficient(testCollision.collisionPoint) >= ray.Get_Projection_Coefficient()) {
		collision = testCollision;
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(Point2CollisionMask& in_mask1, RectangleCollisionMask& in_mask2) {
	Collision2d collision;

	Rectangled rectangle = in_mask2.Get_Transformed_Rectangle();
	Vector2d point = in_mask1.Get_Transformed_Point();

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

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(Point2CollisionMask& in_mask1, Triangle2CollisionMask& in_mask2) {
	Collision2d collision;

	Triangle2d triangle = in_mask2.Get_Transformed_Triangle();
	Vector2d point = in_mask1.Get_Transformed_Point();

	Vector2d points[3];
	triangle.Get_Points(points);

	Vector2d lazyNormals[3];
	triangle.Get_Lazy_Normals(lazyNormals);

	Point2CollisionMask pointMask = Point2CollisionMask(point, true);

	collision.didCollide = true;
	for (uint i = 0; i < 3; i++) {
		HalfSpace2CollisionMask halfSpaceMask = HalfSpace2CollisionMask(HalfSpace2d::From_Point_Direction(points[i], lazyNormals[i]), true);
		if (!Evaluate_Typed(halfSpaceMask, pointMask).didCollide) {
			collision.didCollide = false;
			break;
		}
	}

	if (collision.didCollide) collision.collisionPoint = point;

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(Ray2CollisionMask& in_mask1, Ray2CollisionMask& in_mask2) {
	Collision2d collision;

	Ray2d ray1 = in_mask1.Get_Transformed_Ray();
	Ray2d ray2 = in_mask2.Get_Transformed_Ray();

	Collision2d testCollision = Evaluate_Typed(
		Line2CollisionMask(Line2d::From_Point_Direction(ray1.Get_Point(), ray1.Get_Direction()), true),
		Line2CollisionMask(Line2d::From_Point_Direction(ray2.Get_Point(), ray2.Get_Direction()), true)
	);

	if (testCollision.didCollide &&
		ray1.Get_Projection_Coefficient(testCollision.collisionPoint) >= ray1.Get_Projection_Coefficient() &&
		ray2.Get_Projection_Coefficient(testCollision.collisionPoint) >= ray2.Get_Projection_Coefficient()) {

		collision = testCollision;
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(Ray2CollisionMask& in_mask1, RectangleCollisionMask& in_mask2) {
	Collision2d collision;

	Ray2d ray = in_mask1.Get_Transformed_Ray();
	Rectangled rectangle = in_mask2.Get_Transformed_Rectangle();

	Vector2d corners[4];
	rectangle.Get_Corners(corners);

	LineSegment2CollisionMask lineSegmentMasks[4] = {
		LineSegment2CollisionMask(LineSegment2d::From_Points(corners[0], corners[1]), true),
		LineSegment2CollisionMask(LineSegment2d::From_Points(corners[1], corners[2]), true),
		LineSegment2CollisionMask(LineSegment2d::From_Points(corners[2], corners[3]), true),
		LineSegment2CollisionMask(LineSegment2d::From_Points(corners[3], corners[0]), true)
	};

	Ray2CollisionMask rayMask = Ray2CollisionMask(ray, true);

	Collision2d testCollision;
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

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(Ray2CollisionMask& in_mask1, Triangle2CollisionMask& in_mask2) {
	Collision2d collision;

	Ray2d ray = in_mask1.Get_Transformed_Ray();
	Triangle triangle = in_mask2.Get_Transformed_Triangle();

	Vector2d corners[4];
	triangle.Get_Points(corners);

	LineSegment2CollisionMask lineSegmentMasks[3] = {
		LineSegment2CollisionMask(LineSegment2d::From_Points(corners[0], corners[1]), true),
		LineSegment2CollisionMask(LineSegment2d::From_Points(corners[1], corners[2]), true),
		LineSegment2CollisionMask(LineSegment2d::From_Points(corners[2], corners[0]), true),
	};

	Ray2CollisionMask rayMask = Ray2CollisionMask(ray, true);

	Collision2d testCollision;
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

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(RectangleCollisionMask& in_mask1, RectangleCollisionMask& in_mask2) {
	Collision2d collision;

	Rectangled rectangle1 = in_mask1.Get_Transformed_Rectangle();
	Rectangled rectangle2 = in_mask2.Get_Transformed_Rectangle();

	Vector2d corners[2][4];
	Vector2d projectees[2][2];
	Ranged projections[2][2][2];

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
				return collision;
			}
		}
	}

	RectangleCollisionMask rectangleMasks[2] = {
		RectangleCollisionMask(rectangle1, true),
		RectangleCollisionMask(rectangle2, true)
	};

	for (uint i = 0; i < 2; i++) {
		bool isInside = true;
		i2 = (i == 0) ? 1 : 0;

		for (uint j = 0; j < 4; j++) {
			if (!Evaluate_Typed(Point2CollisionMask(corners[i][j], true), rectangleMasks[i2]).didCollide) {
				isInside = false;
				break;
			}
		}

		if (isInside) {
			collision.didCollide = true;
			collision.collisionPoint = (corners[i][0] + corners[i][1] + corners[i][2] + corners[i][3]) / 4.0;
		}
	}

	for (uint j = 0; j < 4; j++) {
		LineSegment2d edge = LineSegment2d::From_Points(corners[0][j], corners[0][(j + 1) % 4]);
		collision = Evaluate_Typed(LineSegment2CollisionMask(edge), rectangleMasks[1]);
		if (collision.didCollide) {
			return collision;
		}
	}

	throw ProcessFailureException();
	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(Triangle2CollisionMask& in_mask1, Triangle2CollisionMask& in_mask2) {
	Collision2d collision;

	Triangle2d triangle1 = in_mask1.Get_Transformed_Triangle();
	Triangle2d triangle2 = in_mask2.Get_Transformed_Triangle();

	Vector2d corners[2][3];
	Vector2d projectees[2][3];
	Ranged projections[2][3][2];

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
			projectees[i][j] = projectees[i][j].Orthogonal().Normalized();

			projections[i][j][i] = Range(
				projectees[i][j].Dot(corners[i][j]), 
				projectees[i][j].Dot(corners[i][(j + 2) % 3])
			);

			// for each side of the other triangle
			for (uint k = 0; k < 3; k++) {
				projections[i][j][i2].Expand_To(projectees[i][j].Dot(corners[i2][k]));
			}

			if (!projections[i][j][0].Intersection(projections[i][j][1])) {
				return collision;
			}
		}
	}

	Triangle2CollisionMask triangleMasks[2] = {
		Triangle2CollisionMask(triangle1, true),
		Triangle2CollisionMask(triangle2, true)
	};

	for (uint i = 0; i < 2; i++) {
		bool isInside = true;
		i2 = (i == 0) ? 1 : 0;
		
		for (uint j = 0; j < 3; j++) {
			if (!Evaluate_Typed(Point2CollisionMask(corners[i][j], true), triangleMasks[i2]).didCollide) {
				isInside = false;
				break;
			}
		}

		if (isInside) {
			collision.didCollide = true;
			collision.collisionPoint = (corners[i][0] + corners[i][1] + corners[i][2]) / 3.0;
		}
	}

	for (uint j = 0; j < 3; j++) {
		LineSegment2d edge = LineSegment2d::From_Points(corners[0][j], corners[0][(j + 1) % 3]);
		collision = Evaluate_Typed(LineSegment2CollisionMask(edge), triangleMasks[1]);
		if (collision.didCollide) {
			return collision;
		}
	}

	throw ProcessFailureException();
	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(Line2CollisionMask& in_mask1, Line2CollisionMask& in_mask2) {
	Collision2d collision;

	Line2d line1 = in_mask1.Get_Transformed_Line();
	Line2d line2 = in_mask2.Get_Transformed_Line();

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

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(Line2CollisionMask& in_mask1, LineSegment2CollisionMask& in_mask2) {
	Collision2d collision;

	LineSegment2d lineSegment = in_mask2.Get_Transformed_Line_Segment();

	Collision2d testCollision = Evaluate_Typed(
		in_mask1,
		Line2CollisionMask(Line2d::From_Points(lineSegment.Get_Point1(), lineSegment.Get_Point2()), true)
	);

	if (testCollision.didCollide && 
		Is_Between_Inc(
			lineSegment.Get_Projection_Coefficient(testCollision.collisionPoint),
			lineSegment.Get_Projection_Coefficient1(),
			lineSegment.Get_Projection_Coefficient2())
		) {

		collision = testCollision;
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(Line2CollisionMask& in_mask1, Point2CollisionMask& in_mask2) {
	Collision2d collision;

	Line2d line = in_mask1.Get_Transformed_Line();
	Vector2d point = in_mask2.Get_Transformed_Point();

	Vector2d pointOffset = point - line.Get_Point();
	Vector2d direction = line.Get_Direction();

	if (pointOffset.X() / direction.X() == pointOffset.Y() / direction.Y()) {
		collision.didCollide = true;
		collision.collisionPoint = point;
	}

	return collision;
}

Collision2d InPlaceCollisionEvaluator2::Evaluate_Typed(Line2CollisionMask& in_mask1, Ray2CollisionMask& in_mask2) {
	Collision2d collision;

	Ray2d ray = in_mask2.Get_Transformed_Ray();

	Collision2d testCollision = Evaluate_Typed(
		in_mask1,
		Line2CollisionMask(Line2d::From_Point_Direction(ray.Get_Point(), ray.Get_Direction()), true)
	);

	if (testCollision.didCollide && ray.Get_Projection_Coefficient(testCollision.collisionPoint) >= ray.Get_Projection_Coefficient()) {
		collision = testCollision;
	}

	return collision;
}
