#include "in_place_collision_evaluator.h"
#include "exceptions.h"
#include "misc.h"
#include "range.h"
#include "projection_overlap.h"
#include "log.h"
#include "basic_collision_mask.h"

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AARectangleCollisionMask<T>& in_mask1, AARectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	AxisAlignedBox<T, 2> const& rectangle1 = *basis1;
	AxisAlignedBox<T, 2> const& rectangle2 = *basis2;

	Range<T> xRange1 = Range<T>(rectangle1.Get_Minima().X(), rectangle1.Get_Maxima().X());
	Range<T> yRange1 = Range<T>(rectangle1.Get_Minima().Y(), rectangle1.Get_Maxima().Y());
	Range<T> xRange2 = Range<T>(rectangle2.Get_Minima().X(), rectangle2.Get_Maxima().X());
	Range<T> yRange2 = Range<T>(rectangle2.Get_Minima().Y(), rectangle2.Get_Maxima().Y());

	Range<T> xCollRange, yCollRange;

	bool xColl = xRange1.Intersection(xRange2, xCollRange);
	bool yColl = yRange2.Intersection(yRange2, yCollRange);

	collision.mDid = xColl && yColl;

	if (collision.mDid && mReturnPoint) {
		collision.mPoint = Vector<T, 2>(xCollRange.Get_Mean(), yCollRange.Get_Mean());
	}

	if (!collision.mDid && mReturnSeparator) {
		if (yColl) {
			collision.mSeparator = (xRange1.Get_High() < xRange2.Get_High()) ? Vector<T, 2>(1, 0) : Vector<T, 2>(-1, 0);
		}
		else {
			collision.mSeparator = (yRange1.Get_High() < yRange2.Get_High()) ? Vector<T, 2>(0, 1) : Vector<T, 2>(0, -1);
		}
		collision.mOwner = &in_mask1;
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AARectangleCollisionMask<T>& in_mask1, CircleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	AxisAlignedBox<T, 2> const& rectangle = *basis1;
	Circle<T> const& circle = *basis2;

	Vector<T, 2> corners[4];
	rectangle.Get_Corners(corners);
	Vector<T, 2> rectCenter = rectangle.Get_Center();

	AARectangleCollisionMask<T> xRectangleMask = AARectangleCollisionMask<T>(
		AxisAlignedBox<T, 2>::From_Extrema(
			rectangle.Get_Minima() - Vector<T, 2>(circle.Get_Radius(), 0), 
			rectangle.Get_Maxima() + Vector<T, 2>(circle.Get_Radius(), 0)
		),
		true
	);

	AARectangleCollisionMask<T> yRectangleMask = AARectangleCollisionMask<T>(
		AxisAlignedBox<T, 2>::From_Extrema(
			rectangle.Get_Minima() - Vector<T, 2>(0, circle.Get_Radius()),
			rectangle.Get_Maxima() + Vector<T, 2>(0, circle.Get_Radius())
		),
		true
	);

	Point2CollisionMask<T> pointMask = Point2CollisionMask<T>(circle.Get_Center(), true);
	CircleCollisionMask<T> transformedCircleMask = CircleCollisionMask<T>(circle, true);

	collision = Evaluate_Typed(xRectangleMask, pointMask);
	if (collision.mDid) {
		if (mReturnPoint) {
			collision.mPoint[0] += Sign(rectCenter.X() - collision.mPoint.X()) *
				Min<T>(circle.Get_Radius(), xRectangleMask.Get_Basis().Get_Dimensions().X() / 2);
		}

		return collision;
	}


	collision = Evaluate_Typed(yRectangleMask, pointMask);
	if (collision.mDid) {
		if (mReturnPoint) {
			collision.mPoint[1] += Sign(rectCenter.Y() - collision.mPoint.Y()) *
				Min<T>(circle.Get_Radius(), yRectangleMask.Get_Basis().Get_Dimensions().Y() / 2);
		}

		return collision;
	}

	for (uint i = 0; i < 4; i++) {
		collision = Evaluate_Typed(transformedCircleMask, Point2CollisionMask<T>(corners[i], true));
		if (collision.mDid) break;
	}

	if (!collision.mDid && mReturnSeparator) {
		if (Between_Inc(circle.Get_Center().X(), rectangle.Get_Minima().X(), rectangle.Get_Maxima().X())) {
			collision.mSeparator = (rectangle.Get_Maxima().Y() < circle.Get_Center().Y()) ? Vector<T, 2>(0, 1) : Vector<T, 2>(0, -1);
			collision.mOwner = &in_mask1;
		}
		else if (Between_Inc(circle.Get_Center().Y(), rectangle.Get_Minima().Y(), rectangle.Get_Maxima().Y())) {
			collision.mSeparator = (rectangle.Get_Maxima().X() < circle.Get_Center().X()) ? Vector<T, 2>(1, 0) : Vector<T, 2>(-1, 0);
			collision.mOwner = &in_mask1;
		}
		else {
			T cornerDistances[4];
			for (uint i = 0; i < 4; i++) {
				cornerDistances[i] = (circle.Get_Center() - corners[i]).Dot_Self();
			}

			uint index = Min_Index(cornerDistances, 4);
			collision.mSeparator = (circle.Get_Center() - corners[index]);
			collision.mOwner = &in_mask1;
		}
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AARectangleCollisionMask<T>& in_mask1, HalfSpace2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	AxisAlignedBox<T, 2> const& rectangle = *basis1;
	HalfSpace<T, 2> const& halfSpace = *basis2;

	Vector<T, 2> corners[4];
	rectangle.Get_Corners(corners);

	HalfSpace2CollisionMask<T> transformedHalfSpaceMask = HalfSpace2CollisionMask<T>(halfSpace, true);
	for (uint i = 0; i < 4; i++) {
		collision = Evaluate_Typed(transformedHalfSpaceMask, Point2CollisionMask<T>(corners[i], true));
		if (collision.mDid) break;
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = -halfSpace.Get_Direction();
		collision.mOwner = &in_mask2;
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AARectangleCollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	AxisAlignedBox<T, 2> const& rectangle = *basis1;
	Line<T, 2> const& line = *basis2;

	Vector<T, 2> center = rectangle.Get_Center();
	Vector<T, 2> minima = rectangle.Get_Minima();
	Vector<T, 2> maxima = rectangle.Get_Maxima();
	Vector<T, 2> point = line.Get_Point();
	Vector<T, 2> direction = line.Get_Direction();

	if (direction.Is_Zero()) {
		AARectangleCollisionMask<T> newRectangleMask(rectangle, true);
		Point2CollisionMask<T> newPointMask(point, true);
		collision = Evaluate_Typed(newRectangleMask, newPointMask);

		if (collision.mOwner == &newRectangleMask) collision.mOwner = &in_mask1;
		if (collision.mOwner == &newPointMask) collision.mOwner = &in_mask2;

		return collision;
	}

	if (direction.X() == 0 || direction.Y() == 0) {
		uint i1 = (direction.X() == 0) ? 0 : 1;
		uint i2 = (i1 == 1) ? 0 : 1;

		if (Between_Inc(point[i2], minima[i2], maxima[i2])) {
			collision.mDid = true;
			if (mReturnPoint) {
				collision.mPoint[i1] = Clamp(point[i1], minima[i1], maxima[i1]);
				collision.mPoint[i2] = point[i2];
			}
		}
		else if (mReturnSeparator) {
			collision.mSeparator[i1] = 0;
			collision.mSeparator[i2] = (point[i2] < minima[i2]) ? 1 : -1;
			collision.mOwner = &in_mask2;
		}

		return collision;
	}

	Collision<T, 2> collisions[2];
	T ts[2];

	for (uint i1 = 0; i1 < 2; i1++) {
		uint i2 = (i1 == 1) ? 0 : 1;

		ts[i1] = (point[i1] < center[i1]) ? minima[i1] : maxima[i1];
		ts[i1] = (ts[i1] - point[i1]) / direction[i1];

		if (Between_Inc(point[i2] + direction[i2] * ts[i1], minima[i2], maxima[i2])) {
			collisions[i1].mDid = true;
			if (mReturnPoint) {
				collisions[i1].mPoint = point + direction * ts[i1];
			}
 		}
	}

	T minT = std::numeric_limits<T>::infinity();
	for (uint i = 0; i < 2; i++) {
		if (collisions[i].mDid && ((minT < 0 && ts[i] >= 0) || abs(ts[i]) < minT)) {
			collision = collisions[i];
			minT = abs(ts[i]);
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		Vector<T, 2> ortho = direction.Orthogonal();
		collision.mSeparator = ortho * ((ortho.Dot(center - point) < 0) ? -1 : 1);
		collision.mOwner = &in_mask2;
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AARectangleCollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	AxisAlignedBox<T, 2> const& rectangle = *basis1;
	LineSegment<T, 2> const& lineSegment = *basis2;

	Vector<T, 2> points[2] = {
		lineSegment.Get_Point1(),
		lineSegment.Get_Point2()
	};

	Vector<T, 2> minima = rectangle.Get_Minima();
	Vector<T, 2> maxima = rectangle.Get_Maxima();

	Line2CollisionMask<T> newLineMask(Line<T, 2>::From_Point_Direction(lineSegment.Get_Point1(), lineSegment.Get_Direction()), true);

	collision = Evaluate_Typed(in_mask1, newLineMask);

	if (collision.mDid) {
		if (!Between_Inc(
			lineSegment.Get_Projection_Coefficient(collision.mPoint),
			lineSegment.Get_Projection_Coefficient1(),
			lineSegment.Get_Projection_Coefficient2())) {

			collision.mDid = false;
			if (mReturnSeparator) {
				for (uint i1 = 0; i1 < 2; i1++) {
					uint i2 = (i1 == 1) ? 0 : 1;
					if (points[0][i1] > maxima[i1] && points[1][i1] > maxima[i1]) {
						collision.mSeparator[i1] = 0;
						collision.mSeparator[i2] = 1;
					}
					else if (points[0][i1] < minima[i1] && points[1][i1] < minima[i1]) {
						collision.mSeparator[i1] = 0;
						collision.mSeparator[i2] = -1;
					}
				}

				collision.mOwner = &in_mask1;
			}
		}
	}
	else {
		if (mReturnSeparator) {
			if (collision.mOwner == &newLineMask) collision.mOwner = &in_mask2;
		}
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AARectangleCollisionMask<T>& in_mask1, MST2CollisionMask<T>& in_mask2) {
	using iter_t = typename MeshSphereTree<T, 2>::Iterator;

	Collision<T, 2> collision;

	auto basis2 = in_mask2.Get_Transformed_Basis();

	MeshSphereTree<T, 2> const& tree = *basis2;

	std::vector<iter_t> itStack;
	itStack.push_back(iter_t(tree));
	while (!collision.mDid && !itStack.empty()) {
		iter_t it = itStack.back();
		itStack.pop_back();

		if (Evaluate_Typed(in_mask1, CircleCollisionMask<T>(it.Get_Sphere(), true)).mDid) {
			if (it.Is_Leaf()) {
				collision = Evaluate_Typed(in_mask1, Triangle2CollisionMask<T>(it.Get_Triangle(), true));
			}
			else {
				itStack.push_back(it.Go_Both());
				itStack.push_back(it);
			}
		}
	}

	collision.mSeparator = Vector<T, 2>();
	collision.mOwner = nullptr;

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AARectangleCollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	AxisAlignedBox<T, 2> const& rectangle = *basis1;
	Vector<T, 2> const& point = *basis2;

	Vector<T, 2> minima = rectangle.Get_Minima();
	Vector<T, 2> maxima = rectangle.Get_Maxima();

	bool xColl = Between_Inc(point.X(), minima.X(), maxima.X());
	bool yColl = Between_Inc(point.Y(), minima.Y(), maxima.Y());

	collision.mDid = xColl && yColl;

	if (collision.mDid && mReturnPoint) {
		collision.mPoint = point;
	}

	if (!collision.mDid && mReturnSeparator) {
		if (point.X() > maxima.X()) collision.mSeparator = Vector<T, 2>(1, 0);
		else if (point.X() < minima.X()) collision.mSeparator = Vector<T, 2>(-1, 0);
		else if (point.Y() > maxima.Y()) collision.mSeparator = Vector<T, 2>(0, 1);
		else if (point.Y() < minima.Y()) collision.mSeparator = Vector<T, 2>(0, -1);
		else throw ProcessFailureException();

		collision.mOwner = &in_mask1;
	}
	
	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AARectangleCollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	AxisAlignedBox<T, 2> const& rectangle = *basis1;
	Ray<T, 2> const& ray = *basis2;

	Vector<T, 2> point = ray.Get_Point();
	Vector<T, 2> direction = ray.Get_Direction();

	Vector<T, 2> minima = rectangle.Get_Minima();
	Vector<T, 2> maxima = rectangle.Get_Maxima();

	Line2CollisionMask<T> newRayMask(Line<T, 2>::From_Point_Direction(point, direction), true);

	collision = Evaluate_Typed(in_mask1, newRayMask);

	if (collision.mDid) {
		if (ray.Get_Projection_Coefficient(collision.mPoint) < ray.Get_Projection_Coefficient()) {
			collision.mDid = false;
			if (mReturnSeparator) {
				for (uint i1 = 0; i1 < 2; i1++) {
					uint i2 = (i1 == 1) ? 0 : 1;
					if (point[i1] > maxima[i1]) {
						collision.mSeparator[i1] = 0;
						collision.mSeparator[i2] = 1;
					}
					else if (point[i1] < minima[i1]) {
						collision.mSeparator[i1] = 0;
						collision.mSeparator[i2] = -1;
					}
				}

				collision.mOwner = &in_mask1;
			}
		}
	}
	else {
		if (mReturnSeparator) {
			if (collision.mOwner == &newRayMask) collision.mOwner = &in_mask2;
		}
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AARectangleCollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	AxisAlignedBox<T, 2> const& aaRectangle = *basis1;
	Box<T, 2> const& rectangle = *basis2;

	AARectangleCollisionMask<T> aaRectangleMask = AARectangleCollisionMask<T>(aaRectangle, true);
	RectangleCollisionMask<T> rectangleMask = RectangleCollisionMask<T>(rectangle, true);

	Vector<T, 2> corners[4];
	rectangle.Get_Corners(corners);

	for (uint i = 0; i < 4; i++) {
		collision = Evaluate_Typed(aaRectangleMask, LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(corners[i], corners[(i + 1) % 3]), true));
		if (collision.mDid) {
			return collision;
		}
	}

	collision = Evaluate_Typed(Point2CollisionMask<T>(aaRectangle.Get_Center(), true), rectangleMask);

	if (!collision.mDid && mReturnSeparator) {
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
		collision.mSeparator = projectTo[index];
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AARectangleCollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	AxisAlignedBox<T, 2> const& rectangle = *basis1;
	Triangle<T, 2> const& triangle = *basis2;

	AARectangleCollisionMask<T> aaRectangleMask = AARectangleCollisionMask<T>(rectangle, true);
	Triangle2CollisionMask<T> triangleMask = Triangle2CollisionMask<T>(triangle, true);

	Vector<T, 2> corners[3];
	triangle.Get_Points(corners);

	for (uint i = 0; i < 3; i++) {
		collision = Evaluate_Typed(aaRectangleMask, LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(corners[i], corners[(i + 1) % 3]), true));
		if (collision.mDid) {
			return collision;
		}
	}

	collision = Evaluate_Typed(Point2CollisionMask<T>(rectangle.Get_Center(), true), triangleMask);

	if (!collision.mDid && mReturnSeparator) {
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
		collision.mSeparator = projectTo[index];
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(CircleCollisionMask<T>& in_mask1, CircleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Circle<T> const& circle1 = *basis1;
	Circle<T> const& circle2 = *basis2;

	const T radii = circle1.Get_Radius() + circle2.Get_Radius();
	const T radiiSquared = radii * radii;
	const Vector<T, 2> offset = circle2.Get_Center() - circle1.Get_Center();
	const T offsetDistanceSquared = offset.Dot_Self();

	collision.mDid = offsetDistanceSquared <= radiiSquared;
	if (collision.mDid) {
		collision.mPoint = circle1.Get_Center() + offset / 2.0;
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mOwner = &in_mask1;
		collision.mSeparator = offset;
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(CircleCollisionMask<T>& in_mask1, HalfSpace2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Circle<T> const& circle = *basis1;
	HalfSpace<T, 2> const& halfSpace = *basis2;

	T circleProjectionCoefficient = halfSpace.Get_Direction().Dot(circle.Get_Center());
	T halfSpaceProjectionCoefficient = halfSpace.Get_Projection_Coefficient();
	T radius = circle.Get_Radius();

	if (halfSpaceProjectionCoefficient < circleProjectionCoefficient + radius) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = halfSpace.Get_Direction() * (Max<T>(circleProjectionCoefficient - radius, halfSpaceProjectionCoefficient) + circleProjectionCoefficient + radius) / 2.0;
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(CircleCollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Circle<T> const& circle = *basis1;
	Line<T, 2> const& line = *basis2;

	Point2CollisionMask<T> pointMask = Point2CollisionMask<T>(line.Get_Projection(circle.Get_Center()), true);
	CircleCollisionMask<T> circleMask = CircleCollisionMask<T>(circle, true);

	collision = Evaluate_Typed(circleMask, pointMask);

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(CircleCollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Circle<T> const& circle = *basis1;
	LineSegment<T, 2> const& lineSegment = *basis2;

	CircleCollisionMask<T> circleMask = CircleCollisionMask<T>(circle, true);

	Vector<T, 2> projectionPoint = lineSegment.Get_Projection(circle.Get_Center());

	Point2CollisionMask<T> pointMask1 = Point2CollisionMask<T>(projectionPoint, true);
	Point2CollisionMask<T> pointMask2 = Point2CollisionMask<T>(lineSegment.Get_Point1(), true);
	Point2CollisionMask<T> pointMask3 = Point2CollisionMask<T>(lineSegment.Get_Point2(), true);
	
	if (Between_Inc<T>(
		lineSegment.Get_Projection_Coefficient(projectionPoint),
		lineSegment.Get_Projection_Coefficient1(),
		lineSegment.Get_Projection_Coefficient2())) {

		collision = Evaluate_Typed(circleMask, pointMask1);
	}
	else {
		collision = Evaluate_Typed(circleMask, pointMask2);

		if (!collision.mDid) {
			collision = Evaluate_Typed(circleMask, pointMask3);
		}
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(CircleCollisionMask<T>& in_mask1, MST2CollisionMask<T>& in_mask2) {
	return Collision<T, 2>();
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(CircleCollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Circle<T> const& circle = *basis1;
	Vector<T, 2> const& point = *basis2;

	if ((point - circle.Get_Center()).Dot_Self() <= circle.Get_Radius() * circle.Get_Radius()) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = point;
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = point - circle.Get_Center();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(CircleCollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Circle<T> const& circle = *basis1;
	Ray<T, 2> const& ray = *basis2;

	Ray2CollisionMask<T> rayMask = Ray2CollisionMask<T>(ray, true);

	Vector<T, 2> projectionPoint = ray.Get_Projection(circle.Get_Center());

	Point2CollisionMask<T> pointMask1 = Point2CollisionMask<T>(projectionPoint, true);
	Point2CollisionMask<T> pointMask2 = Point2CollisionMask<T>(ray.Get_Point(), true);

	if (ray.Get_Projection_Coefficient(projectionPoint) >= ray.Get_Projection_Coefficient()) {
		collision = Evaluate_Typed(pointMask1, rayMask);
	}

	if (!collision.mDid) {
		collision = Evaluate_Typed(pointMask2, rayMask);
	}
	
	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(CircleCollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Circle<T> const& circle = *basis1;
	Box<T, 2> const& rectangle = *basis2;

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
		if ((testCollision = Evaluate_Typed(pointMask, rectangleMasks[i])).mDid) {
			collision = testCollision;
			if (mReturnPoint) {
				collision.mPoint[i] += newAxes[i][i].Normalized()[i] *
					Sign((center - collision.mPoint).Dot(newAxes[i][i])) *
					Min<T>(circle.Get_Radius(), newAxes[i][i].Magnitude() / (T)2);
			}
			return collision;
		}
	}

	if (!collision.mDid) {
		for (uint i = 0; i < 4; i++) {
			pointMask = Point2CollisionMask<T>(corners[i]);
			if ((testCollision = Evaluate_Typed(circleMask, pointMask)).mDid) {
				collision = testCollision;
				break;
			}
		}
	}

	if (!collision.mDid && mReturnSeparator) {
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
				collision.mSeparator = axes[i];
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

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Circle<T> const& circle = *basis1;
	Triangle<T, 2> const& triangle = *basis2;

	CircleCollisionMask<T> circleMask = CircleCollisionMask<T>(circle, true);

	Vector<T, 2> corners[3];
	triangle.Get_Points(corners);

	LineSegment<T, 2> lineSegments[3] = {
		LineSegment<T, 2>::From_Points(corners[0], corners[1]),
		LineSegment<T, 2>::From_Points(corners[1], corners[2]),
		LineSegment<T, 2>::From_Points(corners[2], corners[0])
	};

	Vector<T, 2> centerProjection;
	for (uint i = 0; i < 3; i++) {
		centerProjection = lineSegments[i].Get_Projection(circle.Get_Center());
		collision = Evaluate_Typed(circleMask, Point2CollisionMask<T>(centerProjection, true));
		if (collision.mDid &&
			Between_Inc(
				lineSegments[i].Get_Projection_Coefficient(centerProjection),
				lineSegments[i].Get_Projection_Coefficient1(),
				lineSegments[i].Get_Projection_Coefficient2()
			)) {

			return collision;
		}
	}

	for (uint i = 0; i < 3; i++) {
		collision = Evaluate_Typed(circleMask, Point2CollisionMask<T>(corners[i], true));
		if (collision.mDid) {
			return collision;
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		Vector<T, 2> axes[6];
		triangle.Get_Lazy_Normals(axes);

		for (uint i = 0; i < 3; i++) {
			axes[i + 3] = corners[i] - circle.Get_Center();
		}

		Vector<T, 2> normAxes[6];

		for (uint i = 0; i < 6; i++) {
			normAxes[i] = axes[i].Normalized();
		}

		for (uint i = 0; i < 6; i++) {
			Range<T> triangleRange({ 
				corners[0].Dot(normAxes[i]),
				corners[1].Dot(normAxes[i]),
				corners[2].Dot(normAxes[i])
				});

			Range<T> circleRange(
				(circle.Get_Center() - normAxes[i] * circle.Get_Radius() * (T)0.999).Dot(normAxes[i]),
				(circle.Get_Center() + normAxes[i] * circle.Get_Radius() * (T)0.999).Dot(normAxes[i])
			);

			if (!triangleRange.Intersection(circleRange)) {
				collision.mOwner = (i <= 2) ? (void*)&in_mask2 : (void*)&in_mask1;
				collision.mSeparator = normAxes[i];
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

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	HalfSpace<T, 2> const& halfSpace1 = *basis1;;
	HalfSpace<T, 2> const& halfSpace2 = *basis1;;

	Line<T, 2> line1 = Line<T, 2>::From_Point_Direction(halfSpace1.Get_Point(), halfSpace1.Get_Direction().Orthogonal());
	Line<T, 2> line2 = Line<T, 2>::From_Point_Direction(halfSpace2.Get_Point(), halfSpace2.Get_Direction().Orthogonal());

	Line2CollisionMask<T> lineMask1 = Line2CollisionMask<T>(line1, true);
	Line2CollisionMask<T> lineMask2 = Line2CollisionMask<T>(line2, true);

	Collision<T, 2> testCollision = Evaluate_Typed(lineMask1, lineMask2);

	if (testCollision.mDid) {
		collision = testCollision;
	}
	else if (halfSpace1.Get_Direction().Dot(halfSpace2.Get_Direction()) >= 0.0) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint =
				(halfSpace1.Get_Direction().Dot(halfSpace1.Get_Point()) >= halfSpace1.Get_Direction().Dot(halfSpace2.Get_Point())) ?
				halfSpace1.Get_Point() : halfSpace2.Get_Point();
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace1.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(HalfSpace2CollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	HalfSpace<T, 2> const& halfSpace = *basis1;

	Line<T, 2> const& line1 = *basis2;
	Line<T, 2> line2 = Line<T, 2>::From_Point_Direction(halfSpace.Get_Point(), halfSpace.Get_Direction().Orthogonal());

	Line2CollisionMask<T> lineMask1 = Line2CollisionMask<T>(line1, true);
	Line2CollisionMask<T> lineMask2 = Line2CollisionMask<T>(line2, true);
	HalfSpace2CollisionMask<T> halfSpaceMask = HalfSpace2CollisionMask<T>(halfSpace);

	collision = Evaluate_Typed(lineMask1, lineMask2);

	if (!collision.mDid) {
		collision = Evaluate_Typed(in_mask1, Point2CollisionMask<T>(line1.Get_Point()));
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(HalfSpace2CollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	HalfSpace<T, 2> const& halfSpace = *basis1;
	LineSegment<T, 2> const& lineSegment = *basis2;

	HalfSpace2CollisionMask<T> halfSpaceMask = HalfSpace2CollisionMask<T>(halfSpace, true);
	Line2CollisionMask<T> lineMask = Line2CollisionMask<T>(
		Line<T, 2>::From_Point_Direction(halfSpace.Get_Point(), halfSpace.Get_Direction()), true);
	LineSegment2CollisionMask<T> lineSegmentMask = LineSegment2CollisionMask<T>(lineSegment, true);

	Collision testCollision = Evaluate_Typed(lineMask, lineSegmentMask);
	if (testCollision.mDid) {
		collision = testCollision;
	}
	else {
		collision = Evaluate_Typed(halfSpaceMask, Point2CollisionMask<T>(lineSegment.Get_Point1(), true));
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(HalfSpace2CollisionMask<T>& in_mask1, MST2CollisionMask<T>& in_mask2) {
	return Collision<T, 2>();
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(HalfSpace2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	HalfSpace<T, 2> const& halfSpace = *basis1;
	Vector<T, 2> const& point = *basis2;

	if (halfSpace.Get_Projection_Coefficient() <= halfSpace.Get_Direction().Dot(point)) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = point;
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(HalfSpace2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	HalfSpace<T, 2> const& halfSpace = *basis1;
	Ray<T, 2> const& ray = *basis2;

	HalfSpace2CollisionMask<T> halfSpaceMask = HalfSpace2CollisionMask<T>(halfSpace, true);
	Line2CollisionMask<T> lineMask = Line2CollisionMask<T>(
		Line<T, 2>::From_Point_Direction(halfSpace.Get_Point(), halfSpace.Get_Direction().Orthogonal()), true);
	Ray2CollisionMask<T> rayMask = Ray2CollisionMask<T>(ray, true);

	Collision testCollision = Evaluate_Typed(lineMask, rayMask);
	if (testCollision.mDid) {
		collision = testCollision;
	}
	else {
		collision = Evaluate_Typed(halfSpaceMask, Point2CollisionMask<T>(ray.Get_Point(), true));
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(HalfSpace2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	HalfSpace<T, 2> const& halfSpace = *basis1;
	Box<T, 2> const& rectangle = *basis2;

	Vector<T, 2> center = rectangle.Get_Center();

	HalfSpace2CollisionMask<T> halfSpaceMask = HalfSpace2CollisionMask<T>(halfSpace, true);
	Line2CollisionMask<T> lineMask = Line2CollisionMask<T>(
		Line<T, 2>::From_Point_Direction(halfSpace.Get_Point(), halfSpace.Get_Point().Orthogonal()), true);
	RectangleCollisionMask<T> rectangleMask = RectangleCollisionMask<T>(rectangle, true);

	collision = Evaluate_Typed(lineMask, rectangleMask);
	
	if (!collision.mDid) {
		collision = Evaluate_Typed(halfSpaceMask, Point2CollisionMask<T>(center, true));
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(HalfSpace2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	HalfSpace<T, 2> const& halfSpace = *basis1;
	Triangle<T, 2> const& triangle = *basis2;

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
		if (testCollision.mDid) {
			collision = testCollision;
			break;
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(LineSegment2CollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	LineSegment<T, 2> const& lineSegment1 = *basis1;
	LineSegment<T, 2> const& lineSegment2 = *basis2;

	bool thismReturnPoint = mReturnPoint;
	mReturnPoint = true;
	Collision<T, 2> testCollision = Evaluate_Typed(
		Line2CollisionMask<T>(Line<T, 2>::From_Points(lineSegment1.Get_Point1(), lineSegment1.Get_Point2()), true),
		Line2CollisionMask<T>(Line<T, 2>::From_Points(lineSegment2.Get_Point1(), lineSegment2.Get_Point2()), true)
	);
	mReturnPoint = thismReturnPoint;

	if (testCollision.mDid &&
		Between_Inc(
			lineSegment1.Get_Projection_Coefficient(testCollision.mPoint), 
			lineSegment1.Get_Projection_Coefficient1(), 
			lineSegment1.Get_Projection_Coefficient2()) &&
		Between_Inc(
			lineSegment2.Get_Projection_Coefficient(testCollision.mPoint), 
			lineSegment2.Get_Projection_Coefficient1(), 
			lineSegment2.Get_Projection_Coefficient2())
		) {

		collision = testCollision;
	}

	if (!collision.mDid && mReturnSeparator) {
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
		collision.mSeparator = projectTo[index];
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(LineSegment2CollisionMask<T>& in_mask1, MST2CollisionMask<T>& in_mask2) {
	return Collision<T, 2>();
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(LineSegment2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	LineSegment<T, 2> const& lineSegment = *basis1;
	Vector<T, 2> const& point = *basis2;

	Line<T, 2> line = Line<T, 2>::From_Point_Direction(lineSegment.Get_Point1(), lineSegment.Get_Offset());

	T projectionCoefficient = lineSegment.Get_Direction().Dot(point);

	if (Between_Inc<T>(projectionCoefficient, lineSegment.Get_Projection_Coefficient1(), lineSegment.Get_Projection_Coefficient2())) {
		collision = Evaluate_Typed(Line2CollisionMask<T>(line, true), Point2CollisionMask<T>(point, true));
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = point - (lineSegment.Get_Point1() + lineSegment.Get_Direction() * projectionCoefficient);
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(LineSegment2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	LineSegment<T, 2> const& lineSegment = *basis1;
	Ray<T, 2> const& ray = *basis2;

	bool thismReturnPoint = mReturnPoint;
	mReturnPoint = true;
	Collision<T, 2> testCollision = Evaluate_Typed(
		Line2CollisionMask<T>(Line<T, 2>::From_Points(lineSegment.Get_Point1(), lineSegment.Get_Point2()), true),
		Line2CollisionMask<T>(Line<T, 2>::From_Point_Direction(ray.Get_Point(), ray.Get_Direction()), true)
	);
	mReturnPoint = thismReturnPoint;

	if (testCollision.mDid &&
		Between_Inc(
			lineSegment.Get_Projection_Coefficient(testCollision.mPoint),
			lineSegment.Get_Projection_Coefficient1(),
			lineSegment.Get_Projection_Coefficient2()) &&
		ray.Get_Projection_Coefficient(testCollision.mPoint) >= ray.Get_Projection_Coefficient()) {

		collision = testCollision;
	}

	if (!collision.mDid && mReturnSeparator) {
		Vector<T, 2> rayNormal = ray.Get_Direction().Orthogonal();
		if (Sign((lineSegment.Get_Point1() - ray.Get_Point()).Dot(rayNormal) -
				ray.Get_Point().Dot(rayNormal)) ==
			Sign((lineSegment.Get_Point2() - ray.Get_Point()).Dot(rayNormal) -
				ray.Get_Point().Dot(rayNormal))) {

			collision.mSeparator = rayNormal;
		}
		else {
			collision.mSeparator = lineSegment.Get_Direction().Orthogonal();
		}
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(LineSegment2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	LineSegment<T, 2> const& lineSegment = *basis1;
	Box<T, 2> const& rectangle = *basis2;

	RectangleCollisionMask<T> rectangleMask = RectangleCollisionMask<T>(rectangle, true);

	Vector<T, 2> corners[4];
	rectangle.Get_Corners(corners);

	Vector<T, 2> projectionPoint;
	Collision<T, 2> testCollision;
	for (uint i = 0; i < 4; i++) {
		projectionPoint = lineSegment.Get_Projection(corners[i]);
		if (Between_Inc(
			lineSegment.Get_Projection_Coefficient(projectionPoint), 
			lineSegment.Get_Projection_Coefficient1(), 
			lineSegment.Get_Projection_Coefficient2())) {

			testCollision = Evaluate_Typed(Point2CollisionMask<T>(projectionPoint, true), rectangleMask);
				if (testCollision.mDid) {
					collision = testCollision;
					break;
				}
		}
	}

	if (!collision.mDid && mReturnSeparator) {
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
		collision.mSeparator = projectTo[index];
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(LineSegment2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	LineSegment<T, 2> const& lineSegment = *basis1;
	Triangle<T, 2> const& triangle = *basis2;

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
			if (Between_Inc(
					lineSegment.Get_Projection_Coefficient(testPoint),
					lineSegment.Get_Projection_Coefficient1(),
					lineSegment.Get_Projection_Coefficient2()
				)) {
				collision.mDid = true;
				if (mReturnPoint) {
					collision.mPoint = testPoint;
				}
				break;
			}
		}
	}

	if (!collision.mDid && mReturnSeparator) {
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
		collision.mSeparator = projectTo[index];
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(MST2CollisionMask<T>& in_mask1, MST2CollisionMask<T>& in_mask2) {
	return Collision<T, 2>();
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(MST2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) {
	return Collision<T, 2>();
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(MST2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	return Collision<T, 2>();
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(MST2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	return Collision<T, 2>();
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(MST2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	return Collision<T, 2>();
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Point2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Vector<T, 2> const& point1 = *basis1;
	Vector<T, 2> const& point2 = *basis2;

	if (point1 == point2) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = point1;
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = point1 - point2;
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Point2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Vector<T, 2> const& point = *basis1;
	Ray<T, 2> const& ray = *basis2;

	Collision<T, 2> testCollision = Evaluate_Typed(
		Line2CollisionMask<T>(Line<T, 2>::From_Point_Direction(ray.Get_Point(), ray.Get_Direction()), true),
		Point2CollisionMask<T>(point, true)
	);

	if (testCollision.mDid && ray.Get_Projection_Coefficient(point) >= ray.Get_Projection_Coefficient()) {
		collision = testCollision;
	}

	if (!collision.mDid && mReturnSeparator) {
		if (ray.Get_Projection_Coefficient(point) >= ray.Get_Projection_Coefficient()) {
			collision.mSeparator = testCollision.mSeparator;
		}
		else {
			collision.mSeparator = point - ray.Get_Point();
		}
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Point2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Box<T, 2> const& rectangle = *basis2;
	Vector<T, 2> const& point = *basis1;

	Vector<T, 2> origin = rectangle.Get_Origin();
	Vector<T, 2> axes[2];
	rectangle.Get_Axes(axes);

	T pointDotAxis1 = axes[0].Dot(point - origin);
	T pointDotAxis2 = axes[1].Dot(point - origin);

	T axisMax1 = axes[0].Dot_Self();
	T axisMax2 = axes[1].Dot_Self();

	if (Between_Inc<T>(pointDotAxis1, 0, axisMax1) && Between_Inc<T>(pointDotAxis2, 0, axisMax2)) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = point;
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		if (Between_Inc<T>(pointDotAxis1, 0, axisMax1)) {
			collision.mSeparator = axes[1];
		}
		else if (Between_Inc<T>(pointDotAxis2, 0, axisMax2)) {
			collision.mSeparator = axes[0];
		}
		else {
			collision.mSeparator = point - rectangle.Get_Center();
		}
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Point2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Triangle<T, 2> const& triangle = *basis2;
	Vector<T, 2> const& point = *basis1;

	Vector<T, 2> points[3];
	triangle.Get_Points(points);

	Vector<T, 2> lazyNormals[3];
	triangle.Get_Lazy_Normals(lazyNormals);

	Point2CollisionMask<T> pointMask = Point2CollisionMask<T>(point, true);

	for (uint i = 0; i < 3; i++) {
		if (Sign(lazyNormals[i].Dot(point - points[i])) > 0) {
			if (mReturnSeparator) {
				collision.mSeparator = lazyNormals[i];
				return collision;
			}
		}
	}

	collision.mDid = true;
	if (mReturnPoint) {
		collision.mPoint = point;
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Ray2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Ray<T, 2> const& ray1 = *basis1;
	Ray<T, 2> const& ray2 = *basis2;

	bool thismReturnPoint = mReturnPoint;
	mReturnPoint = true;
	Collision<T, 2> testCollision = Evaluate_Typed(
		Line2CollisionMask<T>(Line<T, 2>::From_Point_Direction(ray1.Get_Point(), ray1.Get_Direction()), true),
		Line2CollisionMask<T>(Line<T, 2>::From_Point_Direction(ray2.Get_Point(), ray2.Get_Direction()), true)
	);
	mReturnPoint = thismReturnPoint;

	if (testCollision.mDid &&
		ray1.Get_Projection_Coefficient(testCollision.mPoint) >= ray1.Get_Projection_Coefficient() &&
		ray2.Get_Projection_Coefficient(testCollision.mPoint) >= ray2.Get_Projection_Coefficient()) {

		collision = testCollision;
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mPoint = ray1.Get_Point() - ray2.Get_Point();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Ray2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Ray<T, 2> const& ray = *basis1;
	Box<T, 2> const& rectangle = *basis2;

	Vector<T, 2> corners[4];
	rectangle.Get_Corners(corners);

	bool thismReturnPoint = mReturnPoint;
	mReturnPoint = true;
	LineSegment2CollisionMask<T> lineSegmentMasks[4] = {
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(corners[0], corners[1]), true),
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(corners[1], corners[3]), true),
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(corners[2], corners[3]), true),
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(corners[2], corners[0]), true)
	};
	mReturnPoint = thismReturnPoint;

	Ray2CollisionMask<T> rayMask = Ray2CollisionMask<T>(ray, true);

	Collision<T, 2> testCollision;
	for (uint i = 0; i < 4; i++) {
		testCollision = Evaluate_Typed(lineSegmentMasks[i], rayMask);
		if (testCollision.mDid) {
			if (!collision.mDid ||
				ray.Get_Projection_Coefficient(testCollision.mPoint) <
				ray.Get_Projection_Coefficient(collision.mPoint)) {

				collision = testCollision;
			}
		}
	}

	if (!collision.mDid && mReturnSeparator) {
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
			collision.mSeparator = rayNormal;
		}
		else {
			if (Between_Inc(
				axes[0].Dot(ray.Get_Point()),
				axes[0].Dot(corners[0]),
				axes[0].Dot(corners[3])
			)) {
				collision.mSeparator = axes[1];
			}
			else {
				collision.mSeparator = axes[0];
			}
		}
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Ray2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Ray<T, 2> const& ray = *basis1;
	Triangle<T, 2> const& triangle = *basis2;

	Vector<T, 2> corners[3];
	triangle.Get_Points(corners);

	bool thismReturnPoint = mReturnPoint;
	mReturnPoint = true;
	LineSegment2CollisionMask<T> lineSegmentMasks[3] = {
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(corners[0], corners[1]), true),
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(corners[1], corners[2]), true),
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(corners[2], corners[0]), true),
	};
	mReturnPoint = thismReturnPoint;

	Ray2CollisionMask<T> rayMask = Ray2CollisionMask<T>(ray, true);

	Collision<T, 2> testCollision;
	for (uint i = 0; i < 3; i++) {
		testCollision = Evaluate_Typed(lineSegmentMasks[i], rayMask);
		if (testCollision.mDid) {
			if (!collision.mDid ||
				ray.Get_Projection_Coefficient(testCollision.mPoint) <
				ray.Get_Projection_Coefficient(collision.mPoint)) {

				collision = testCollision;
			}
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		if (!collision.mDid && mReturnSeparator) {
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
				collision.mSeparator = rayNormal;
			}
			else {
				Vector<T, 2> point = ray.Get_Point();
				uint index = Find_Projection_Separation(&point, 1, corners, 3, axes, 3);
				if (index == 3) {
					throw ProcessFailureException();
				}
				collision.mSeparator = axes[index];
			}
		}
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(RectangleCollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Box<T, 2> const& rectangle1 = *basis1;
	Box<T, 2> const& rectangle2 = *basis2;

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
				if (mReturnSeparator) {
					collision.mSeparator = projectees[i][j];
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
			if (!Evaluate_Typed(Point2CollisionMask<T>(corners[i][j], true), rectangleMasks[i2]).mDid) {
				isInside = false;
				break;
			}
		}

		if (isInside) {
			collision.mDid = true;
			if (mReturnPoint) {
				collision.mPoint = (corners[i][0] + corners[i][1] + corners[i][2] + corners[i][3]) / 4.0;
			}
		}
	}

	for (uint j = 0; j < 4; j++) {
		LineSegment<T, 2> edge = LineSegment<T, 2>::From_Points(corners[0][j], corners[0][(j + 1) % 4]);
		collision = Evaluate_Typed(LineSegment2CollisionMask<T>(edge), rectangleMasks[1]);
		if (collision.mDid) {
			return collision;
		}
	}

	throw ProcessFailureException();
	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(RectangleCollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Box<T, 2> const& rectangle = *basis1;
	Triangle<T, 2> const& triangle = *basis2;

	RectangleCollisionMask<T> rectangleMask = RectangleCollisionMask<T>(rectangle, true);
	Triangle2CollisionMask<T> triangleMask = Triangle2CollisionMask<T>(triangle, true);

	Vector<T, 2> triangleCorners[3];
	triangle.Get_Points(triangleCorners);

	for (uint i = 0; i < 3; i++) {
		collision = Evaluate_Typed(LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(triangleCorners[i], triangleCorners[(i + 1) % 3]), true), rectangleMask);
		if (collision.mDid) {
			return collision;
		}
	}

	collision = Evaluate_Typed(Point2CollisionMask<T>(rectangle.Get_Center(), true), triangleMask);
	if (collision.mDid) {
		return collision;
	}

	collision = Evaluate_Typed(Point2CollisionMask<T>(triangleCorners[0], true), rectangleMask);

	if (!collision.mDid && mReturnSeparator) {
		Vector<T, 2> rectCorners[4];
		rectangle.Get_Corners(rectCorners);

		Vector<T, 2> projectTo[5];
		rectangle.Get_Axes(projectTo);
		triangle.Get_Lazy_Normals(&projectTo[2]);

		uint index = Find_Projection_Separation(rectCorners, 4, triangleCorners, 3, projectTo, 5);
		if (index == 5) {
			throw ProcessFailureException();
		}
		collision.mSeparator = projectTo[index];
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Triangle2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Triangle<T, 2> const& triangle1 = *basis1;
	Triangle<T, 2> const& triangle2 = *basis2;

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
				if (mReturnSeparator) {
					collision.mSeparator = projectees[i][j];
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
			if (Evaluate_Typed(Point2CollisionMask<T>(corners[i][j], true), triangleMasks[i2]).mDid) {
				collision.mDid = true;
				collision.mPoint = corners[i][j];
				return collision;
			}
		}
	}

	for (uint j = 0; j < 3; j++) {
		LineSegment<T, 2> edge = LineSegment<T, 2>::From_Points(corners[0][j], corners[0][(j + 1) % 3]);
		collision = Evaluate_Typed(LineSegment2CollisionMask<T>(edge), triangleMasks[1]);
		if (collision.mDid) {
			return collision;
		}
	}

	throw ProcessFailureException();
	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Line2CollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Line<T, 2> const& line1 = *basis1;
	Line<T, 2> const& line2 = *basis2;

	Matrix<T, 2, 3> matrix;
	matrix.Column(0, line1.Get_Direction());
	matrix.Column(1, -line2.Get_Direction());
	matrix.Column(2, line2.Get_Point() - line2.Get_Point());
	matrix.RREFify();

	if (!matrix.Row_Is_Zero(1)) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = line1.Get_Point() + line1.Get_Direction() * matrix.Element(0, 2);
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = line1.Get_Direction().Orthogonal();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Line2CollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Line<T, 2> const& line = *basis1;
	LineSegment<T, 2> const& lineSegment = *basis2;

	Line2CollisionMask<T> lineMask(line, true);

	bool thismReturnPoint = mReturnPoint;
	mReturnPoint = true;
	Collision<T, 2> testCollision = Evaluate_Typed(
		lineMask,
		Line2CollisionMask<T>(Line<T, 2>::From_Points(lineSegment.Get_Point1(), lineSegment.Get_Point2()), true)
	);
	mReturnPoint = thismReturnPoint;

	if (testCollision.mDid && 
		Between_Inc(
			lineSegment.Get_Projection_Coefficient(testCollision.mPoint),
			lineSegment.Get_Projection_Coefficient1(),
			lineSegment.Get_Projection_Coefficient2())
		) {

		collision = testCollision;
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = line.Get_Direction().Orthogonal();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Line2CollisionMask<T>& in_mask1, MST2CollisionMask<T>& in_mask2) {
	return Collision<T, 2>();
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Line2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Line<T, 2> const& line = *basis1;
	Vector<T, 2> const& point = *basis2;

	Vector<T, 2> pointOffset = point - line.Get_Point();
	Vector<T, 2> direction = line.Get_Direction();

	if (pointOffset.X() / direction.X() == pointOffset.Y() / direction.Y()) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = point;
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = line.Get_Direction().Orthogonal();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Line2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Line<T, 2> const& line = *basis1;
	Ray<T, 2> const& ray = *basis2;

	Line2CollisionMask<T> lineMask(line, true);

	bool thismReturnPoint = mReturnPoint;
	mReturnPoint = true;
	Collision<T, 2> testCollision = Evaluate_Typed(
		lineMask,
		Line2CollisionMask<T>(Line<T, 2>::From_Point_Direction(ray.Get_Point(), ray.Get_Direction()), true)
	);
	mReturnPoint = thismReturnPoint;

	if (testCollision.mDid && ray.Get_Projection_Coefficient(testCollision.mPoint) >= ray.Get_Projection_Coefficient()) {
		collision = testCollision;
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = line.Get_Direction().Orthogonal();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Line2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Line<T, 2> const& line = *basis1;
	Box<T, 2> const& rectangle = *basis2;

	RectangleCollisionMask<T> rectangleMask = RectangleCollisionMask<T>(rectangle, true);

	Vector<T, 2> corners[4];
	rectangle.Get_Corners(corners);
	
	Vector<T, 2> projectionPoint;
	for (uint i = 0; i < 4; i++) {
		projectionPoint = line.Get_Projection(corners[i]);
		collision = Evaluate_Typed(Point2CollisionMask<T>(projectionPoint, true), rectangleMask);
		if (collision.mDid) {
			return collision;
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = line.Get_Direction().Orthogonal();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(Line2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Line<T, 2> const& line = *basis1;
	Triangle<T, 2> const& triangle = *basis2;

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
			collision.mDid = true;
			if (mReturnPoint) {
				collision.mPoint = projectionPoints[i].Lerp(diff2.Magnitude() / diff1.Magnitude(), projectionPoints[(i + 1) % 3]);
			}
			break;
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = line.Get_Direction().Orthogonal();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AAHalfSpace2CollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	AxisAlignedHalfSpace<T, 2> const& halfSpace = *basis1;
	LineSegment<T, 2> const& lineSegment = *basis2;

	uint dimension = halfSpace.Get_Dimension();
	uint otherDimension = (dimension == 0) ? 1 : 0;

	Vector<T, 2> point1 = lineSegment.Get_Point1();
	Vector<T, 2> point2 = lineSegment.Get_Point2();

	Range<T> range = Range<T>(point1[dimension], point2[dimension]);

	if (Between_Inc(
		halfSpace.Get_Value(),
		range.Get_Low(),
		range.Get_High()
	)) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint[dimension] = halfSpace.Get_Value();
			collision.mPoint[otherDimension] = Lerp(
				point1[otherDimension], 
				point2[otherDimension], 
				halfSpace.Get_Value(), 
				point1[dimension], 
				point2[dimension]
			);
		}
	}
	else if (
		Ceq_Switch(halfSpace.Get_Value(), point1[dimension], !halfSpace.Is_Positive()) ||
		Ceq_Switch(halfSpace.Get_Value(), point2[dimension], !halfSpace.Is_Positive())) {

		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = lineSegment.Get_Center();
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mOwner = &in_mask1;
		collision.mSeparator = -halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AAHalfSpace2CollisionMask<T>& in_mask1, MST2CollisionMask<T>& in_mask2) {
	return Collision<T, 2>();
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AAHalfSpace2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	AxisAlignedHalfSpace<T, 2> const& halfSpace = *basis1;
	Vector<T, 2> const& point = *basis2;

	T pointValue = point[halfSpace.Get_Dimension()];
	T halfSpaceValue = halfSpace.Get_Value();

	if (Ceq_Switch(halfSpaceValue, pointValue, halfSpace.Is_Positive())) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = point;
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AAHalfSpace2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	AxisAlignedHalfSpace<T, 2> const& halfSpace = *basis1;
	Ray<T, 2> const& ray = *basis2;

	AAHalfSpace2CollisionMask<T> halfSpaceMask = AAHalfSpace2CollisionMask<T>(halfSpace, true);
	Line2CollisionMask<T> lineMask = Line2CollisionMask<T>(Line<T, 2>::From_Point_Direction(ray.Get_Point(), ray.Get_Direction()), true);

	bool thismReturnPoint = mReturnPoint;
	mReturnPoint = true;
	Collision<T, 2> testCollision = Evaluate_Typed(halfSpaceMask, lineMask);
	mReturnPoint = thismReturnPoint;

	if (testCollision.mDid && ray.Get_Projection_Coefficient(testCollision.mPoint) >= ray.Get_Projection_Coefficient()) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = testCollision.mPoint;
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AAHalfSpace2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Box<T, 2> const& rectangle = *basis2;

	Vector<T, 2> corners[4];
	rectangle.Get_Corners(corners);

	uint dimension = in_mask1.Get_Basis().Get_Dimension();

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
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AAHalfSpace2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	AxisAlignedHalfSpace<T, 2> const& halfSpace = *basis1;;
	Triangle<T, 2> const& triangle = *basis2;

	Vector<T, 2> corners[3];
	triangle.Get_Points(corners);

	uint dimension = halfSpace.Get_Dimension();

	uint minIndex = 0;
	uint maxIndex = 0;
	for (uint i = 0; i < 3; i++) {
		if (!collision.mDid && Ceq_Switch(halfSpace.Get_Value(), corners[i][dimension], !halfSpace.Is_Positive())) {
			collision.mDid = true;
			if (!mReturnPoint) {
				return collision;
			}
		}
		if (corners[i][dimension] < corners[minIndex][dimension]) minIndex = i;
		if (corners[i][dimension] > corners[maxIndex][dimension]) maxIndex = i;
	}

	if (mReturnPoint && collision.mDid) {
		collision = Evaluate_Typed(
			AAHalfSpace2CollisionMask<T>(halfSpace, true),
			LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(corners[minIndex], corners[maxIndex]), true)
		);
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mOwner = &in_mask1;
		collision.mSeparator = -halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AAHalfSpace2CollisionMask<T>& in_mask1, AAHalfSpace2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	AxisAlignedHalfSpace<T, 2> const& halfSpace1 = *basis1;
	AxisAlignedHalfSpace<T, 2> const& halfSpace2 = *basis2;

	if (halfSpace1.Get_Dimension() == halfSpace2.Get_Dimension()) {
		uint dimension = halfSpace1.Get_Dimension();
		if (Ceq_Switch(halfSpace1.Get_Value(), halfSpace2.Get_Value(), !halfSpace1.Is_Positive())) {
			collision.mDid = true;
			if (mReturnPoint) {
				collision.mPoint[dimension] = halfSpace2.Get_Value();
			}
		}
		else if (Ceq_Switch(halfSpace2.Get_Value(), halfSpace1.Get_Value(), !halfSpace2.Is_Positive())) {
			collision.mDid = true;
			if (mReturnPoint) {
				collision.mPoint[dimension] = halfSpace1.Get_Value();
			}
		}
	}
	else {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint[halfSpace1.Get_Dimension()] = halfSpace1.Get_Value();
			collision.mPoint[halfSpace2.Get_Dimension()] = halfSpace2.Get_Value();
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace1.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AAHalfSpace2CollisionMask<T>& in_mask1, AALine2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	AxisAlignedHalfSpace<T, 2> const& halfSpace = *basis1;
	AxisAlignedLine<T, 2> const& line = *basis2;

	if (halfSpace.Get_Dimension() == line.Get_Dimension()) {
		uint dimension = halfSpace.Get_Dimension();
		if (Ceq_Switch(halfSpace.Get_Value(), line.Get_Value(), !halfSpace.Is_Positive())) {
			collision.mDid = true;
			if (mReturnPoint) {
				collision.mPoint[dimension] = line.Get_Value();
			}
		}
	}
	else {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint[halfSpace.Get_Dimension()] = halfSpace.Get_Value();
			collision.mPoint[line.Get_Dimension()] = line.Get_Value();
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AAHalfSpace2CollisionMask<T>& in_mask1, AARectangleCollisionMask<T>& in_mask2) {

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	AxisAlignedBox<T, 2> const& rectangle = *basis2;

	return Evaluate_Typed(
		in_mask1,
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::From_Points(rectangle.Get_Minima(), rectangle.Get_Maxima()), true)
	);
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AAHalfSpace2CollisionMask<T>& in_mask1, CircleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	Circle<T> const& circle = *basis2;

	uint dimension = in_mask1.Get_Basis().Get_Dimension();

	LineSegment2CollisionMask<T> newLineSegmentMask(LineSegment<T, 2>::From_Points(circle.Get_Extrema(dimension, false), circle.Get_Extrema(dimension, true)), true);

	collision = Evaluate_Typed(in_mask1, newLineSegmentMask);

	if (!collision.mDid && mReturnSeparator) {
		if (collision.mOwner == &newLineSegmentMask) collision.mOwner = &in_mask2;
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AAHalfSpace2CollisionMask<T>& in_mask1, HalfSpace2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	AxisAlignedHalfSpace<T, 2> const& aaHalfSpace = *basis1;
	HalfSpace<T, 2> const& halfSpace = *basis2;

	Line<T, 2> line = Line<T, 2>::From_Point_Direction(halfSpace.Get_Point(), halfSpace.Get_Direction().Orthogonal());

	AAHalfSpace2CollisionMask<T> aaHalfSpaceMask = AAHalfSpace2CollisionMask<T>(aaHalfSpace, true);
	Line2CollisionMask<T> lineMask = Line2CollisionMask<T>(line, true);

	collision = Evaluate_Typed(aaHalfSpaceMask, lineMask);

	if (!collision.mDid && Ceq_Switch(aaHalfSpace.Get_Value(), halfSpace.Get_Point()[aaHalfSpace.Get_Dimension()], !aaHalfSpace.Is_Positive())) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = halfSpace.Get_Point();
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace.Get_Direction();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::Evaluate_Typed(AAHalfSpace2CollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.Get_Transformed_Basis();
	auto basis2 = in_mask2.Get_Transformed_Basis();

	AxisAlignedHalfSpace<T, 2> const& halfSpace = *basis1;
	Line<T, 2> const& line = *basis2;

	uint dimension = halfSpace.Get_Dimension();

	if (line.Get_Direction()[dimension] != 0.0) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = line.Get_Point();
			collision.mPoint += line.Get_Direction() * (halfSpace.Get_Value() - line.Get_Point()[dimension]) / line.Get_Direction()[dimension];
		}
	}
	else if (Ceq_Switch(halfSpace.Get_Value(), line.Get_Point()[dimension], !halfSpace.Is_Positive())) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = line.Get_Point();
		}
	} 

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace.Get_Direction();
	}

	return collision;
}

template class InPlaceCollisionEvaluator<float, 2>;
template class InPlaceCollisionEvaluator<double, 2>;