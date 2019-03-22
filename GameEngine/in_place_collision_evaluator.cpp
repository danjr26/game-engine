#include "in_place_collision_evaluator.h"
#include "exceptions.h"
#include "misc.h"
#include "range.h"
#include "projection_overlap.h"
#include "log.h"
#include "basic_collision_mask.h"

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(AARectangleCollisionMask<T>& in_mask1, AARectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	AxisAlignedBox<T, 2> const& rectangle1 = *basis1;
	AxisAlignedBox<T, 2> const& rectangle2 = *basis2;

	Range<T> xRange1 = Range<T>(rectangle1.getMinima().x(), rectangle1.getMaxima().x());
	Range<T> yRange1 = Range<T>(rectangle1.getMinima().y(), rectangle1.getMaxima().y());
	Range<T> xRange2 = Range<T>(rectangle2.getMinima().x(), rectangle2.getMaxima().x());
	Range<T> yRange2 = Range<T>(rectangle2.getMinima().y(), rectangle2.getMaxima().y());

	Range<T> xCollRange, yCollRange;

	bool xColl = xRange1.intersection(xRange2, xCollRange);
	bool yColl = yRange2.intersection(yRange2, yCollRange);

	collision.mDid = xColl && yColl;

	if (collision.mDid && mReturnPoint) {
		collision.mPoint = Vector<T, 2>(xCollRange.getMean(), yCollRange.getMean());
	}

	if (!collision.mDid && mReturnSeparator) {
		if (yColl) {
			collision.mSeparator = (xRange1.getHigh() < xRange2.getHigh()) ? Vector<T, 2>(1, 0) : Vector<T, 2>(-1, 0);
		}
		else {
			collision.mSeparator = (yRange1.getHigh() < yRange2.getHigh()) ? Vector<T, 2>(0, 1) : Vector<T, 2>(0, -1);
		}
		collision.mOwner = &in_mask1;
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(AARectangleCollisionMask<T>& in_mask1, CircleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	AxisAlignedBox<T, 2> const& rectangle = *basis1;
	Circle<T> const& circle = *basis2;

	Vector<T, 2> corners[4];
	rectangle.getCorners(corners);
	Vector<T, 2> rectCenter = rectangle.getCenter();

	AARectangleCollisionMask<T> xRectangleMask = AARectangleCollisionMask<T>(
		AxisAlignedBox<T, 2>::fromExtrema(
			rectangle.getMinima() - Vector<T, 2>(circle.getRadius(), 0), 
			rectangle.getMaxima() + Vector<T, 2>(circle.getRadius(), 0)
		),
		true
	);

	AARectangleCollisionMask<T> yRectangleMask = AARectangleCollisionMask<T>(
		AxisAlignedBox<T, 2>::fromExtrema(
			rectangle.getMinima() - Vector<T, 2>(0, circle.getRadius()),
			rectangle.getMaxima() + Vector<T, 2>(0, circle.getRadius())
		),
		true
	);

	Point2CollisionMask<T> pointMask = Point2CollisionMask<T>(circle.getCenter(), true);
	CircleCollisionMask<T> transformedCircleMask = CircleCollisionMask<T>(circle, true);

	collision = evaluateTyped(xRectangleMask, pointMask);
	if (collision.mDid) {
		if (mReturnPoint) {
			collision.mPoint[0] += GEUtil::sign(rectCenter.x() - collision.mPoint.x()) *
				GEUtil::min<T>(circle.getRadius(), xRectangleMask.getBasis().getDimensions().x() / 2);
		}

		return collision;
	}


	collision = evaluateTyped(yRectangleMask, pointMask);
	if (collision.mDid) {
		if (mReturnPoint) {
			collision.mPoint[1] += GEUtil::sign(rectCenter.y() - collision.mPoint.y()) *
				GEUtil::min<T>(circle.getRadius(), yRectangleMask.getBasis().getDimensions().y() / 2);
		}

		return collision;
	}

	for (uint i = 0; i < 4; i++) {
		collision = evaluateTyped(transformedCircleMask, Point2CollisionMask<T>(corners[i], true));
		if (collision.mDid) break;
	}

	if (!collision.mDid && mReturnSeparator) {
		if (GEUtil::betwInc(circle.getCenter().x(), rectangle.getMinima().x(), rectangle.getMaxima().x())) {
			collision.mSeparator = (rectangle.getMaxima().y() < circle.getCenter().y()) ? Vector<T, 2>(0, 1) : Vector<T, 2>(0, -1);
			collision.mOwner = &in_mask1;
		}
		else if (GEUtil::betwInc(circle.getCenter().y(), rectangle.getMinima().y(), rectangle.getMaxima().y())) {
			collision.mSeparator = (rectangle.getMaxima().x() < circle.getCenter().x()) ? Vector<T, 2>(1, 0) : Vector<T, 2>(-1, 0);
			collision.mOwner = &in_mask1;
		}
		else {
			T cornerDistances[4];
			for (uint i = 0; i < 4; i++) {
				cornerDistances[i] = (circle.getCenter() - corners[i]).dotSelf();
			}

			uint index = GEUtil::minIndex(cornerDistances, 4);
			collision.mSeparator = (circle.getCenter() - corners[index]);
			collision.mOwner = &in_mask1;
		}
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(AARectangleCollisionMask<T>& in_mask1, HalfSpace2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	AxisAlignedBox<T, 2> const& rectangle = *basis1;
	HalfSpace<T, 2> const& halfSpace = *basis2;

	Vector<T, 2> corners[4];
	rectangle.getCorners(corners);

	HalfSpace2CollisionMask<T> transformedHalfSpaceMask = HalfSpace2CollisionMask<T>(halfSpace, true);
	for (uint i = 0; i < 4; i++) {
		collision = evaluateTyped(transformedHalfSpaceMask, Point2CollisionMask<T>(corners[i], true));
		if (collision.mDid) break;
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = -halfSpace.getDirection();
		collision.mOwner = &in_mask2;
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(AARectangleCollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	AxisAlignedBox<T, 2> const& rectangle = *basis1;
	Line<T, 2> const& line = *basis2;

	Vector<T, 2> center = rectangle.getCenter();
	Vector<T, 2> minima = rectangle.getMinima();
	Vector<T, 2> maxima = rectangle.getMaxima();
	Vector<T, 2> point = line.getPoint();
	Vector<T, 2> direction = line.getDirection();

	if (direction.isZero()) {
		AARectangleCollisionMask<T> newRectangleMask(rectangle, true);
		Point2CollisionMask<T> newPointMask(point, true);
		collision = evaluateTyped(newRectangleMask, newPointMask);

		if (collision.mOwner == &newRectangleMask) collision.mOwner = &in_mask1;
		if (collision.mOwner == &newPointMask) collision.mOwner = &in_mask2;

		return collision;
	}

	if (direction.x() == 0 || direction.y() == 0) {
		uint i1 = (direction.x() == 0) ? 0 : 1;
		uint i2 = (i1 == 1) ? 0 : 1;

		if (GEUtil::betwInc(point[i2], minima[i2], maxima[i2])) {
			collision.mDid = true;
			if (mReturnPoint) {
				collision.mPoint[i1] = GEUtil::clamp(point[i1], minima[i1], maxima[i1]);
				collision.mPoint[i2] = point[i2];
			}
		}
		else if (mReturnSeparator) {
			collision.mSeparator[i1] = 0;
			collision.mSeparator[i2] = (point[i2] < minima[i2]) ? (T)1 : (T)-1;
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

		if (GEUtil::betwInc(point[i2] + direction[i2] * ts[i1], minima[i2], maxima[i2])) {
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
		Vector<T, 2> ortho = direction.orthogonal();
		collision.mSeparator = ortho * ((ortho.dot(center - point) < 0) ? -1 : 1);
		collision.mOwner = &in_mask2;
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(AARectangleCollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	AxisAlignedBox<T, 2> const& rectangle = *basis1;
	LineSegment<T, 2> const& lineSegment = *basis2;

	Vector<T, 2> points[2] = {
		lineSegment.getPoint1(),
		lineSegment.getPoint2()
	};

	Vector<T, 2> minima = rectangle.getMinima();
	Vector<T, 2> maxima = rectangle.getMaxima();

	Line2CollisionMask<T> newLineMask(Line<T, 2>::fromPointDirection(lineSegment.getPoint1(), lineSegment.getDirection()), true);

	collision = evaluateTyped(in_mask1, newLineMask);

	if (collision.mDid) {
		if (!GEUtil::betwInc(
			lineSegment.getProjectionCoefficient(collision.mPoint),
			lineSegment.getProjectionCoefficient1(),
			lineSegment.getProjectionCoefficient2())) {

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
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(AARectangleCollisionMask<T>& in_mask1, MST2CollisionMask<T>& in_mask2) {
	using iter_t = typename MeshSphereTree<T, 2>::Iterator;

	Collision<T, 2> collision;

	auto basis2 = in_mask2.getTransformedBasis();

	MeshSphereTree<T, 2> const& tree = *basis2;

	std::vector<iter_t> itStack;
	itStack.push_back(iter_t(tree));
	while (!collision.mDid && !itStack.empty()) {
		iter_t it = itStack.back();
		itStack.pop_back();

		if (evaluateTyped(in_mask1, CircleCollisionMask<T>(it.getSphere(), true)).mDid) {
			if (it.isLeaf()) {
				collision = evaluateTyped(in_mask1, Triangle2CollisionMask<T>(it.getTriangle(), true));
			}
			else {
				itStack.push_back(it.goBoth());
				itStack.push_back(it);
			}
		}
	}

	collision.mSeparator = Vector<T, 2>();
	collision.mOwner = nullptr;

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(AARectangleCollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	AxisAlignedBox<T, 2> const& rectangle = *basis1;
	Vector<T, 2> const& point = *basis2;

	Vector<T, 2> minima = rectangle.getMinima();
	Vector<T, 2> maxima = rectangle.getMaxima();

	bool xColl = GEUtil::betwInc(point.x(), minima.x(), maxima.x());
	bool yColl = GEUtil::betwInc(point.y(), minima.y(), maxima.y());

	collision.mDid = xColl && yColl;

	if (collision.mDid && mReturnPoint) {
		collision.mPoint = point;
	}

	if (!collision.mDid && mReturnSeparator) {
		if (point.x() > maxima.x()) collision.mSeparator = Vector<T, 2>(1, 0);
		else if (point.x() < minima.x()) collision.mSeparator = Vector<T, 2>(-1, 0);
		else if (point.y() > maxima.y()) collision.mSeparator = Vector<T, 2>(0, 1);
		else if (point.y() < minima.y()) collision.mSeparator = Vector<T, 2>(0, -1);
		else throw ProcessFailureException();

		collision.mOwner = &in_mask1;
	}
	
	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(AARectangleCollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	AxisAlignedBox<T, 2> const& rectangle = *basis1;
	Ray<T, 2> const& ray = *basis2;

	Vector<T, 2> point = ray.getPoint();
	Vector<T, 2> direction = ray.getDirection();

	Vector<T, 2> minima = rectangle.getMinima();
	Vector<T, 2> maxima = rectangle.getMaxima();

	Line2CollisionMask<T> newRayMask(Line<T, 2>::fromPointDirection(point, direction), true);

	collision = evaluateTyped(in_mask1, newRayMask);

	if (collision.mDid) {
		if (ray.getProjectionCoefficient(collision.mPoint) < ray.getProjectionCoefficient()) {
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
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(AARectangleCollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	AxisAlignedBox<T, 2> const& aaRectangle = *basis1;
	Box<T, 2> const& rectangle = *basis2;

	AARectangleCollisionMask<T> aaRectangleMask = AARectangleCollisionMask<T>(aaRectangle, true);
	RectangleCollisionMask<T> rectangleMask = RectangleCollisionMask<T>(rectangle, true);

	Vector<T, 2> corners[4];
	rectangle.getCorners(corners);

	for (uint i = 0; i < 4; i++) {
		collision = evaluateTyped(aaRectangleMask, LineSegment2CollisionMask<T>(LineSegment<T, 2>::fromPoints(corners[i], corners[(i + 1) % 3]), true));
		if (collision.mDid) {
			return collision;
		}
	}

	collision = evaluateTyped(Point2CollisionMask<T>(aaRectangle.getCenter(), true), rectangleMask);

	if (!collision.mDid && mReturnSeparator) {
		Vector<T, 2> aaCorners[4];
		aaRectangle.getCorners(aaCorners);

		Vector<T, 2> projectTo[4] = {
			Vector<T, 2>(1, 0),
			Vector<T, 2>(0, 1)
		};
		rectangle.getAxes(&projectTo[2]);
		uint index = findProjectionSeparation(corners, 4, aaCorners, 4, projectTo, 4);
		if (index == 4) {
			throw ProcessFailureException();
		}
		collision.mSeparator = projectTo[index];
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(AARectangleCollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	AxisAlignedBox<T, 2> const& rectangle = *basis1;
	Triangle<T, 2> const& triangle = *basis2;

	AARectangleCollisionMask<T> aaRectangleMask = AARectangleCollisionMask<T>(rectangle, true);
	Triangle2CollisionMask<T> triangleMask = Triangle2CollisionMask<T>(triangle, true);

	Vector<T, 2> corners[3];
	triangle.getPoints(corners);

	for (uint i = 0; i < 3; i++) {
		collision = evaluateTyped(aaRectangleMask, LineSegment2CollisionMask<T>(LineSegment<T, 2>::fromPoints(corners[i], corners[(i + 1) % 3]), true));
		if (collision.mDid) {
			return collision;
		}
	}

	collision = evaluateTyped(Point2CollisionMask<T>(rectangle.getCenter(), true), triangleMask);

	if (!collision.mDid && mReturnSeparator) {
		Vector<T, 2> aaCorners[4];
		rectangle.getCorners(aaCorners);

		Vector<T, 2> projectTo[5] = {
			Vector<T, 2>(1, 0),
			Vector<T, 2>(0, 1)
		};
		triangle.getLazyNormals(&projectTo[2]);
		uint index = findProjectionSeparation(corners, 3, aaCorners, 4, projectTo, 5);
		if (index == 5) {
			throw ProcessFailureException();
		}
		collision.mSeparator = projectTo[index];
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(CircleCollisionMask<T>& in_mask1, CircleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Circle<T> const& circle1 = *basis1;
	Circle<T> const& circle2 = *basis2;

	const T radii = circle1.getRadius() + circle2.getRadius();
	const T radiiSquared = radii * radii;
	const Vector<T, 2> offset = circle2.getCenter() - circle1.getCenter();
	const T offsetDistanceSquared = offset.dotSelf();

	collision.mDid = offsetDistanceSquared <= radiiSquared;
	if (collision.mDid) {
		collision.mPoint = circle1.getCenter() + offset / 2.0;
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mOwner = &in_mask1;
		collision.mSeparator = offset;
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(CircleCollisionMask<T>& in_mask1, HalfSpace2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Circle<T> const& circle = *basis1;
	HalfSpace<T, 2> const& halfSpace = *basis2;

	T circleProjectionCoefficient = halfSpace.getDirection().dot(circle.getCenter());
	T halfSpaceProjectionCoefficient = halfSpace.getProjectionCoefficient();
	T radius = circle.getRadius();

	if (halfSpaceProjectionCoefficient < circleProjectionCoefficient + radius) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = halfSpace.getDirection() * (GEUtil::max<T>(circleProjectionCoefficient - radius, halfSpaceProjectionCoefficient) + circleProjectionCoefficient + radius) / 2.0;
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace.getDirection();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(CircleCollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Circle<T> const& circle = *basis1;
	Line<T, 2> const& line = *basis2;

	Point2CollisionMask<T> pointMask = Point2CollisionMask<T>(line.getProjection(circle.getCenter()), true);
	CircleCollisionMask<T> circleMask = CircleCollisionMask<T>(circle, true);

	collision = evaluateTyped(circleMask, pointMask);

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(CircleCollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Circle<T> const& circle = *basis1;
	LineSegment<T, 2> const& lineSegment = *basis2;

	CircleCollisionMask<T> circleMask = CircleCollisionMask<T>(circle, true);

	Vector<T, 2> projectionPoint = lineSegment.getProjection(circle.getCenter());

	Point2CollisionMask<T> pointMask1 = Point2CollisionMask<T>(projectionPoint, true);
	Point2CollisionMask<T> pointMask2 = Point2CollisionMask<T>(lineSegment.getPoint1(), true);
	Point2CollisionMask<T> pointMask3 = Point2CollisionMask<T>(lineSegment.getPoint2(), true);
	
	if (GEUtil::betwInc<T>(
		lineSegment.getProjectionCoefficient(projectionPoint),
		lineSegment.getProjectionCoefficient1(),
		lineSegment.getProjectionCoefficient2())) {

		collision = evaluateTyped(circleMask, pointMask1);
	}
	else {
		collision = evaluateTyped(circleMask, pointMask2);

		if (!collision.mDid) {
			collision = evaluateTyped(circleMask, pointMask3);
		}
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(CircleCollisionMask<T>& in_mask1, MST2CollisionMask<T>& in_mask2) {
	return Collision<T, 2>();
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(CircleCollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Circle<T> const& circle = *basis1;
	Vector<T, 2> const& point = *basis2;

	if ((point - circle.getCenter()).dotSelf() <= circle.getRadius() * circle.getRadius()) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = point;
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = point - circle.getCenter();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(CircleCollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Circle<T> const& circle = *basis1;
	Ray<T, 2> const& ray = *basis2;

	Ray2CollisionMask<T> rayMask = Ray2CollisionMask<T>(ray, true);

	Vector<T, 2> projectionPoint = ray.getProjection(circle.getCenter());

	Point2CollisionMask<T> pointMask1 = Point2CollisionMask<T>(projectionPoint, true);
	Point2CollisionMask<T> pointMask2 = Point2CollisionMask<T>(ray.getPoint(), true);

	if (ray.getProjectionCoefficient(projectionPoint) >= ray.getProjectionCoefficient()) {
		collision = evaluateTyped(pointMask1, rayMask);
	}

	if (!collision.mDid) {
		collision = evaluateTyped(pointMask2, rayMask);
	}
	
	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(CircleCollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Circle<T> const& circle = *basis1;
	Box<T, 2> const& rectangle = *basis2;

	Vector<T, 2> center = rectangle.getCenter();
	CircleCollisionMask<T> circleMask = CircleCollisionMask<T>(circle, true);

	Vector<T, 2> corners[4];
	rectangle.getCorners(corners);

	Vector<T, 2> axes[2];
	rectangle.getAxes(axes);

	Vector<T, 2> newAxes[2][2] = {
		{axes[0], axes[1]},
		{axes[0], axes[1]}
	};
	newAxes[0][0].addToMagnitude(circle.getRadius() * 2);
	newAxes[1][1].addToMagnitude(circle.getRadius() * 2);

	RectangleCollisionMask<T> rectangleMasks[2] = {
		RectangleCollisionMask<T>(Box<T, 2>::fromCenterAxes(center, &newAxes[0][0]), true),
		RectangleCollisionMask<T>(Box<T, 2>::fromCenterAxes(center, &newAxes[1][0]), true)
	};

	Point2CollisionMask<T> pointMask = Point2CollisionMask<T>(circle.getCenter(), true);

	Collision<T, 2> testCollision;
	for (uint i = 0; i < 2; i++) {
		if ((testCollision = evaluateTyped(pointMask, rectangleMasks[i])).mDid) {
			collision = testCollision;
			if (mReturnPoint) {
				collision.mPoint[i] += newAxes[i][i].normalized()[i] *
					GEUtil::sign((center - collision.mPoint).dot(newAxes[i][i])) *
					GEUtil::min<T>(circle.getRadius(), newAxes[i][i].magnitude() / (T)2);
			}
			return collision;
		}
	}

	if (!collision.mDid) {
		for (uint i = 0; i < 4; i++) {
			pointMask = Point2CollisionMask<T>(corners[i]);
			if ((testCollision = evaluateTyped(circleMask, pointMask)).mDid) {
				collision = testCollision;
				break;
			}
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		Vector<T, 2> normAxes[2] = {
			axes[0].normalized(),
			axes[1].normalized()
		};

		for (uint i = 0; i < 2; i++) {
			Range<T> rectRange(corners[0].projectionCoeff(axes[i]), corners[3].projectionCoeff(axes[i]));
			Range<T> circleRange(
				(circle.getCenter() - normAxes[i] * circle.getRadius()).projectionCoeff(axes[i]),
				(circle.getCenter() + normAxes[i] * circle.getRadius()).projectionCoeff(axes[i])
			);
			if (!rectRange.intersection(circleRange)) {
				collision.mSeparator = axes[i];
				return collision;
			}
		}

		throw ProcessFailureException();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(CircleCollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Circle<T> const& circle = *basis1;
	Triangle<T, 2> const& triangle = *basis2;

	CircleCollisionMask<T> circleMask = CircleCollisionMask<T>(circle, true);

	Vector<T, 2> corners[3];
	triangle.getPoints(corners);

	LineSegment<T, 2> lineSegments[3] = {
		LineSegment<T, 2>::fromPoints(corners[0], corners[1]),
		LineSegment<T, 2>::fromPoints(corners[1], corners[2]),
		LineSegment<T, 2>::fromPoints(corners[2], corners[0])
	};

	Vector<T, 2> centerProjection;
	for (uint i = 0; i < 3; i++) {
		centerProjection = lineSegments[i].getProjection(circle.getCenter());
		collision = evaluateTyped(circleMask, Point2CollisionMask<T>(centerProjection, true));
		if (collision.mDid &&
			GEUtil::betwInc(
				lineSegments[i].getProjectionCoefficient(centerProjection),
				lineSegments[i].getProjectionCoefficient1(),
				lineSegments[i].getProjectionCoefficient2()
			)) {

			return collision;
		}
	}

	for (uint i = 0; i < 3; i++) {
		collision = evaluateTyped(circleMask, Point2CollisionMask<T>(corners[i], true));
		if (collision.mDid) {
			return collision;
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		Vector<T, 2> axes[6];
		triangle.getLazyNormals(axes);

		for (uint i = 0; i < 3; i++) {
			axes[i + 3] = corners[i] - circle.getCenter();
		}

		Vector<T, 2> normAxes[6];

		for (uint i = 0; i < 6; i++) {
			normAxes[i] = axes[i].normalized();
		}

		for (uint i = 0; i < 6; i++) {
			Range<T> triangleRange({ 
				corners[0].dot(normAxes[i]),
				corners[1].dot(normAxes[i]),
				corners[2].dot(normAxes[i])
				});

			Range<T> circleRange(
				(circle.getCenter() - normAxes[i] * circle.getRadius() * (T)0.999).dot(normAxes[i]),
				(circle.getCenter() + normAxes[i] * circle.getRadius() * (T)0.999).dot(normAxes[i])
			);

			if (!triangleRange.intersection(circleRange)) {
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
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(HalfSpace2CollisionMask<T>& in_mask1, HalfSpace2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	HalfSpace<T, 2> const& halfSpace1 = *basis1;;
	HalfSpace<T, 2> const& halfSpace2 = *basis1;;

	Line<T, 2> line1 = Line<T, 2>::fromPointDirection(halfSpace1.getPoint(), halfSpace1.getDirection().orthogonal());
	Line<T, 2> line2 = Line<T, 2>::fromPointDirection(halfSpace2.getPoint(), halfSpace2.getDirection().orthogonal());

	Line2CollisionMask<T> lineMask1 = Line2CollisionMask<T>(line1, true);
	Line2CollisionMask<T> lineMask2 = Line2CollisionMask<T>(line2, true);

	Collision<T, 2> testCollision = evaluateTyped(lineMask1, lineMask2);

	if (testCollision.mDid) {
		collision = testCollision;
	}
	else if (halfSpace1.getDirection().dot(halfSpace2.getDirection()) >= 0.0) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint =
				(halfSpace1.getDirection().dot(halfSpace1.getPoint()) >= halfSpace1.getDirection().dot(halfSpace2.getPoint())) ?
				halfSpace1.getPoint() : halfSpace2.getPoint();
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace1.getDirection();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(HalfSpace2CollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	HalfSpace<T, 2> const& halfSpace = *basis1;

	Line<T, 2> const& line1 = *basis2;
	Line<T, 2> line2 = Line<T, 2>::fromPointDirection(halfSpace.getPoint(), halfSpace.getDirection().orthogonal());

	Line2CollisionMask<T> lineMask1 = Line2CollisionMask<T>(line1, true);
	Line2CollisionMask<T> lineMask2 = Line2CollisionMask<T>(line2, true);
	HalfSpace2CollisionMask<T> halfSpaceMask = HalfSpace2CollisionMask<T>(halfSpace);

	collision = evaluateTyped(lineMask1, lineMask2);

	if (!collision.mDid) {
		collision = evaluateTyped(in_mask1, Point2CollisionMask<T>(line1.getPoint()));
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace.getDirection();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(HalfSpace2CollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	HalfSpace<T, 2> const& halfSpace = *basis1;
	LineSegment<T, 2> const& lineSegment = *basis2;

	HalfSpace2CollisionMask<T> halfSpaceMask = HalfSpace2CollisionMask<T>(halfSpace, true);
	Line2CollisionMask<T> lineMask = Line2CollisionMask<T>(
		Line<T, 2>::fromPointDirection(halfSpace.getPoint(), halfSpace.getDirection()), true);
	LineSegment2CollisionMask<T> lineSegmentMask = LineSegment2CollisionMask<T>(lineSegment, true);

	Collision<T, 2> testCollision = evaluateTyped(lineMask, lineSegmentMask);
	if (testCollision.mDid) {
		collision = testCollision;
	}
	else {
		collision = evaluateTyped(halfSpaceMask, Point2CollisionMask<T>(lineSegment.getPoint1(), true));
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace.getDirection();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(HalfSpace2CollisionMask<T>& in_mask1, MST2CollisionMask<T>& in_mask2) {
	return Collision<T, 2>();
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(HalfSpace2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	HalfSpace<T, 2> const& halfSpace = *basis1;
	Vector<T, 2> const& point = *basis2;

	if (halfSpace.getProjectionCoefficient() <= halfSpace.getDirection().dot(point)) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = point;
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace.getDirection();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(HalfSpace2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	HalfSpace<T, 2> const& halfSpace = *basis1;
	Ray<T, 2> const& ray = *basis2;

	HalfSpace2CollisionMask<T> halfSpaceMask = HalfSpace2CollisionMask<T>(halfSpace, true);
	Line2CollisionMask<T> lineMask = Line2CollisionMask<T>(
		Line<T, 2>::fromPointDirection(halfSpace.getPoint(), halfSpace.getDirection().orthogonal()), true);
	Ray2CollisionMask<T> rayMask = Ray2CollisionMask<T>(ray, true);

	Collision<T, 2> testCollision = evaluateTyped(lineMask, rayMask);
	if (testCollision.mDid) {
		collision = testCollision;
	}
	else {
		collision = evaluateTyped(halfSpaceMask, Point2CollisionMask<T>(ray.getPoint(), true));
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace.getDirection();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(HalfSpace2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	HalfSpace<T, 2> const& halfSpace = *basis1;
	Box<T, 2> const& rectangle = *basis2;

	Vector<T, 2> center = rectangle.getCenter();

	HalfSpace2CollisionMask<T> halfSpaceMask = HalfSpace2CollisionMask<T>(halfSpace, true);
	Line2CollisionMask<T> lineMask = Line2CollisionMask<T>(
		Line<T, 2>::fromPointDirection(halfSpace.getPoint(), halfSpace.getPoint().orthogonal()), true);
	RectangleCollisionMask<T> rectangleMask = RectangleCollisionMask<T>(rectangle, true);

	collision = evaluateTyped(lineMask, rectangleMask);
	
	if (!collision.mDid) {
		collision = evaluateTyped(halfSpaceMask, Point2CollisionMask<T>(center, true));
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace.getDirection();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(HalfSpace2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	HalfSpace<T, 2> const& halfSpace = *basis1;
	Triangle<T, 2> const& triangle = *basis2;

	Vector<T, 2> corners[4];
	triangle.getPoints(corners);

	HalfSpace2CollisionMask<T> halfSpaceMask = HalfSpace2CollisionMask<T>(halfSpace, true);
	Point2CollisionMask<T> pointMasks[3] = {
		Point2CollisionMask<T>(corners[0], true),
		Point2CollisionMask<T>(corners[1], true),
		Point2CollisionMask<T>(corners[2], true)
	};

	Collision<T, 2> testCollision;
	for (uint i = 0; i < 3; i++) {
		testCollision = evaluateTyped(halfSpaceMask, pointMasks[i]);
		if (testCollision.mDid) {
			collision = testCollision;
			break;
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace.getDirection();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(LineSegment2CollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	LineSegment<T, 2> const& lineSegment1 = *basis1;
	LineSegment<T, 2> const& lineSegment2 = *basis2;

	bool thismReturnPoint = mReturnPoint;
	mReturnPoint = true;
	Collision<T, 2> testCollision = evaluateTyped(
		Line2CollisionMask<T>(Line<T, 2>::fromPoints(lineSegment1.getPoint1(), lineSegment1.getPoint2()), true),
		Line2CollisionMask<T>(Line<T, 2>::fromPoints(lineSegment2.getPoint1(), lineSegment2.getPoint2()), true)
	);
	mReturnPoint = thismReturnPoint;

	if (testCollision.mDid &&
		GEUtil::betwInc(
			lineSegment1.getProjectionCoefficient(testCollision.mPoint), 
			lineSegment1.getProjectionCoefficient1(), 
			lineSegment1.getProjectionCoefficient2()) &&
		GEUtil::betwInc(
			lineSegment2.getProjectionCoefficient(testCollision.mPoint), 
			lineSegment2.getProjectionCoefficient1(), 
			lineSegment2.getProjectionCoefficient2())
		) {

		collision = testCollision;
	}

	if (!collision.mDid && mReturnSeparator) {
		Vector<T, 2> points1[2] = {
			lineSegment1.getPoint1(),
			lineSegment1.getPoint2()
		};

		Vector<T, 2> points2[2] = {
			lineSegment2.getPoint1(),
			lineSegment2.getPoint2()
		};

		Vector<T, 2> projectTo[2] = {
			lineSegment1.getDirection().orthogonal(),
			lineSegment2.getDirection().orthogonal()
		};
		uint index = findProjectionSeparation(points1, 2, points2, 2, projectTo, 2);
		if (index == 2) {
			throw ProcessFailureException();
		}
		collision.mSeparator = projectTo[index];
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(LineSegment2CollisionMask<T>& in_mask1, MST2CollisionMask<T>& in_mask2) {
	return Collision<T, 2>();
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(LineSegment2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	LineSegment<T, 2> const& lineSegment = *basis1;
	Vector<T, 2> const& point = *basis2;

	Line<T, 2> line = Line<T, 2>::fromPointDirection(lineSegment.getPoint1(), lineSegment.getOffset());

	T projectionCoefficient = lineSegment.getDirection().dot(point);

	if (GEUtil::betwInc<T>(projectionCoefficient, lineSegment.getProjectionCoefficient1(), lineSegment.getProjectionCoefficient2())) {
		collision = evaluateTyped(Line2CollisionMask<T>(line, true), Point2CollisionMask<T>(point, true));
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = point - (lineSegment.getPoint1() + lineSegment.getDirection() * projectionCoefficient);
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(LineSegment2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	LineSegment<T, 2> const& lineSegment = *basis1;
	Ray<T, 2> const& ray = *basis2;

	bool thismReturnPoint = mReturnPoint;
	mReturnPoint = true;
	Collision<T, 2> testCollision = evaluateTyped(
		Line2CollisionMask<T>(Line<T, 2>::fromPoints(lineSegment.getPoint1(), lineSegment.getPoint2()), true),
		Line2CollisionMask<T>(Line<T, 2>::fromPointDirection(ray.getPoint(), ray.getDirection()), true)
	);
	mReturnPoint = thismReturnPoint;

	if (testCollision.mDid &&
		GEUtil::betwInc(
			lineSegment.getProjectionCoefficient(testCollision.mPoint),
			lineSegment.getProjectionCoefficient1(),
			lineSegment.getProjectionCoefficient2()) &&
		ray.getProjectionCoefficient(testCollision.mPoint) >= ray.getProjectionCoefficient()) {

		collision = testCollision;
	}

	if (!collision.mDid && mReturnSeparator) {
		Vector<T, 2> rayNormal = ray.getDirection().orthogonal();
		if (GEUtil::sign((lineSegment.getPoint1() - ray.getPoint()).dot(rayNormal) -
				ray.getPoint().dot(rayNormal)) ==
			GEUtil::sign((lineSegment.getPoint2() - ray.getPoint()).dot(rayNormal) -
				ray.getPoint().dot(rayNormal))) {

			collision.mSeparator = rayNormal;
		}
		else {
			collision.mSeparator = lineSegment.getDirection().orthogonal();
		}
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(LineSegment2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	LineSegment<T, 2> const& lineSegment = *basis1;
	Box<T, 2> const& rectangle = *basis2;

	RectangleCollisionMask<T> rectangleMask = RectangleCollisionMask<T>(rectangle, true);

	Vector<T, 2> corners[4];
	rectangle.getCorners(corners);

	Vector<T, 2> projectionPoint;
	Collision<T, 2> testCollision;
	for (uint i = 0; i < 4; i++) {
		projectionPoint = lineSegment.getProjection(corners[i]);
		if (GEUtil::betwInc(
			lineSegment.getProjectionCoefficient(projectionPoint), 
			lineSegment.getProjectionCoefficient1(), 
			lineSegment.getProjectionCoefficient2())) {

			testCollision = evaluateTyped(Point2CollisionMask<T>(projectionPoint, true), rectangleMask);
				if (testCollision.mDid) {
					collision = testCollision;
					break;
				}
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		Vector<T, 2> points[2] = {
			lineSegment.getPoint1(),
			lineSegment.getPoint2()
		};

		Vector<T, 2> projectTo[3] = {
			lineSegment.getDirection().orthogonal()
		};
		rectangle.getAxes(&projectTo[1]);
		uint index = findProjectionSeparation(points, 2, corners, 4, projectTo, 3);
		if (index == 3) {
			throw ProcessFailureException();
		}
		collision.mSeparator = projectTo[index];
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(LineSegment2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	LineSegment<T, 2> const& lineSegment = *basis1;
	Triangle<T, 2> const& triangle = *basis2;

	Vector<T, 2> corners[3];
	triangle.getPoints(corners);

	Vector<T, 2> projectionPoints[3];
	for (uint i = 0; i < 3; i++) {
		projectionPoints[i] = lineSegment.getProjection(corners[i]);
	}

	Vector<T, 2> diff1, diff2;
	Vector<T, 2> testPoint;
	for (uint i = 0; i < 3; i++) {
		diff1 = corners[i] - projectionPoints[i];
		diff2 = corners[(i + 1) % 3] - projectionPoints[(i + 1) % 3];
		if (diff1.dot(diff2) < 0.0) {
			testPoint = projectionPoints[i].lerp(projectionPoints[(i + 1) % 3], diff2.magnitude() / diff1.magnitude());
			if (GEUtil::betwInc(
					lineSegment.getProjectionCoefficient(testPoint),
					lineSegment.getProjectionCoefficient1(),
					lineSegment.getProjectionCoefficient2()
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
			lineSegment.getPoint1(),
			lineSegment.getPoint2()
		};

		Vector<T, 2> projectTo[3] = {
			lineSegment.getDirection().orthogonal()
		};
		triangle.getLazyNormals(&projectTo[1]);
		uint index = findProjectionSeparation(points, 2, corners, 3, projectTo, 4);
		if (index == 4) {
			throw ProcessFailureException();
		}
		collision.mSeparator = projectTo[index];
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(MST2CollisionMask<T>& in_mask1, MST2CollisionMask<T>& in_mask2) {
	return Collision<T, 2>();
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(MST2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) {
	return Collision<T, 2>();
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(MST2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	return Collision<T, 2>();
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(MST2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	return Collision<T, 2>();
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(MST2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	return Collision<T, 2>();
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(Point2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

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
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(Point2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Vector<T, 2> const& point = *basis1;
	Ray<T, 2> const& ray = *basis2;

	Collision<T, 2> testCollision = evaluateTyped(
		Line2CollisionMask<T>(Line<T, 2>::fromPointDirection(ray.getPoint(), ray.getDirection()), true),
		Point2CollisionMask<T>(point, true)
	);

	if (testCollision.mDid && ray.getProjectionCoefficient(point) >= ray.getProjectionCoefficient()) {
		collision = testCollision;
	}

	if (!collision.mDid && mReturnSeparator) {
		if (ray.getProjectionCoefficient(point) >= ray.getProjectionCoefficient()) {
			collision.mSeparator = testCollision.mSeparator;
		}
		else {
			collision.mSeparator = point - ray.getPoint();
		}
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(Point2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Box<T, 2> const& rectangle = *basis2;
	Vector<T, 2> const& point = *basis1;

	Vector<T, 2> origin = rectangle.getOrigin();
	Vector<T, 2> axes[2];
	rectangle.getAxes(axes);

	T pointDotAxis1 = axes[0].dot(point - origin);
	T pointDotAxis2 = axes[1].dot(point - origin);

	T axisMax1 = axes[0].dotSelf();
	T axisMax2 = axes[1].dotSelf();

	if (GEUtil::betwInc<T>(pointDotAxis1, 0, axisMax1) && GEUtil::betwInc<T>(pointDotAxis2, 0, axisMax2)) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = point;
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		if (GEUtil::betwInc<T>(pointDotAxis1, 0, axisMax1)) {
			collision.mSeparator = axes[1];
		}
		else if (GEUtil::betwInc<T>(pointDotAxis2, 0, axisMax2)) {
			collision.mSeparator = axes[0];
		}
		else {
			collision.mSeparator = point - rectangle.getCenter();
		}
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(Point2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Triangle<T, 2> const& triangle = *basis2;
	Vector<T, 2> const& point = *basis1;

	Vector<T, 2> points[3];
	triangle.getPoints(points);

	Vector<T, 2> lazyNormals[3];
	triangle.getLazyNormals(lazyNormals);

	Point2CollisionMask<T> pointMask = Point2CollisionMask<T>(point, true);

	for (uint i = 0; i < 3; i++) {
		if (GEUtil::sign(lazyNormals[i].dot(point - points[i])) > 0) {
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
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(Ray2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Ray<T, 2> const& ray1 = *basis1;
	Ray<T, 2> const& ray2 = *basis2;

	bool thismReturnPoint = mReturnPoint;
	mReturnPoint = true;
	Collision<T, 2> testCollision = evaluateTyped(
		Line2CollisionMask<T>(Line<T, 2>::fromPointDirection(ray1.getPoint(), ray1.getDirection()), true),
		Line2CollisionMask<T>(Line<T, 2>::fromPointDirection(ray2.getPoint(), ray2.getDirection()), true)
	);
	mReturnPoint = thismReturnPoint;

	if (testCollision.mDid &&
		ray1.getProjectionCoefficient(testCollision.mPoint) >= ray1.getProjectionCoefficient() &&
		ray2.getProjectionCoefficient(testCollision.mPoint) >= ray2.getProjectionCoefficient()) {

		collision = testCollision;
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mPoint = ray1.getPoint() - ray2.getPoint();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(Ray2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Ray<T, 2> const& ray = *basis1;
	Box<T, 2> const& rectangle = *basis2;

	Vector<T, 2> corners[4];
	rectangle.getCorners(corners);

	bool thismReturnPoint = mReturnPoint;
	mReturnPoint = true;
	LineSegment2CollisionMask<T> lineSegmentMasks[4] = {
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::fromPoints(corners[0], corners[1]), true),
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::fromPoints(corners[1], corners[3]), true),
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::fromPoints(corners[2], corners[3]), true),
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::fromPoints(corners[2], corners[0]), true)
	};
	mReturnPoint = thismReturnPoint;

	Ray2CollisionMask<T> rayMask = Ray2CollisionMask<T>(ray, true);

	Collision<T, 2> testCollision;
	for (uint i = 0; i < 4; i++) {
		testCollision = evaluateTyped(lineSegmentMasks[i], rayMask);
		if (testCollision.mDid) {
			if (!collision.mDid ||
				ray.getProjectionCoefficient(testCollision.mPoint) <
				ray.getProjectionCoefficient(collision.mPoint)) {

				collision = testCollision;
			}
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		Vector<T, 2> axes[2];
		rectangle.getAxes(axes);
		bool oneSided = true;
		Vector<T, 2> rayNormal = ray.getDirection().orthogonal();
		auto sign = GEUtil::sign((corners[0] - ray.getPoint()).dot(rayNormal));
		for (uint i = 1; i < 4; i++) {
			if (GEUtil::sign((corners[0] - ray.getPoint()).dot(rayNormal)) != sign) {
				oneSided = false;
			}
		}

		if (oneSided) {
			collision.mSeparator = rayNormal;
		}
		else {
			if (GEUtil::betwInc(
				axes[0].dot(ray.getPoint()),
				axes[0].dot(corners[0]),
				axes[0].dot(corners[3])
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
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(Ray2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Ray<T, 2> const& ray = *basis1;
	Triangle<T, 2> const& triangle = *basis2;

	Vector<T, 2> corners[3];
	triangle.getPoints(corners);

	bool thismReturnPoint = mReturnPoint;
	mReturnPoint = true;
	LineSegment2CollisionMask<T> lineSegmentMasks[3] = {
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::fromPoints(corners[0], corners[1]), true),
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::fromPoints(corners[1], corners[2]), true),
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::fromPoints(corners[2], corners[0]), true),
	};
	mReturnPoint = thismReturnPoint;

	Ray2CollisionMask<T> rayMask = Ray2CollisionMask<T>(ray, true);

	Collision<T, 2> testCollision;
	for (uint i = 0; i < 3; i++) {
		testCollision = evaluateTyped(lineSegmentMasks[i], rayMask);
		if (testCollision.mDid) {
			if (!collision.mDid ||
				ray.getProjectionCoefficient(testCollision.mPoint) <
				ray.getProjectionCoefficient(collision.mPoint)) {

				collision = testCollision;
			}
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		if (!collision.mDid && mReturnSeparator) {
			Vector<T, 2> axes[3];
			triangle.getLazyNormals(axes);
			bool oneSided = true;
			Vector<T, 2> rayNormal = ray.getDirection().orthogonal();
			auto sign = GEUtil::sign((corners[0] - ray.getPoint()).dot(rayNormal));
			for (uint i = 1; i < 3; i++) {
				if (GEUtil::sign((corners[0] - ray.getPoint()).dot(rayNormal)) != sign) {
					oneSided = false;
				}
			}

			if (oneSided) {
				collision.mSeparator = rayNormal;
			}
			else {
				Vector<T, 2> point = ray.getPoint();
				uint index = findProjectionSeparation(&point, 1, corners, 3, axes, 3);
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
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(RectangleCollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Box<T, 2> const& rectangle1 = *basis1;
	Box<T, 2> const& rectangle2 = *basis2;

	Vector<T, 2> corners[2][4];
	Vector<T, 2> projectees[2][2];
	Range<T> projections[2][2][2];

	rectangle1.getCorners(&corners[0][0]);
	rectangle2.getCorners(&corners[1][0]);
	rectangle1.getAxes(&projectees[0][0]);
	rectangle2.getAxes(&projectees[1][0]);

	uint i2;

	for (uint i = 0; i < 2; i++) {
		i2 = (i == 0) ? 1 : 0;

		for (uint j = 0; j < 2; j++) {
			projectees[i][j] = projectees[i][j].orthogonal().normalized();

			projections[i][j][i] = Range(
				projectees[i][j].dot(corners[i][0]),
				projectees[i][j].dot(corners[i][j + 1])
			);

			for (uint k = 0; k < 4; k++) {
				projections[i][j][i2].expandTo(projectees[i][j].dot(corners[i2][k]));
			}

			if (!projections[i][j][0].intersection(projections[i][j][1])) {
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
			if (!evaluateTyped(Point2CollisionMask<T>(corners[i][j], true), rectangleMasks[i2]).mDid) {
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
		LineSegment<T, 2> edge = LineSegment<T, 2>::fromPoints(corners[0][j], corners[0][(j + 1) % 4]);
		collision = evaluateTyped(LineSegment2CollisionMask<T>(edge), rectangleMasks[1]);
		if (collision.mDid) {
			return collision;
		}
	}

	throw ProcessFailureException();
	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(RectangleCollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Box<T, 2> const& rectangle = *basis1;
	Triangle<T, 2> const& triangle = *basis2;

	RectangleCollisionMask<T> rectangleMask = RectangleCollisionMask<T>(rectangle, true);
	Triangle2CollisionMask<T> triangleMask = Triangle2CollisionMask<T>(triangle, true);

	Vector<T, 2> triangleCorners[3];
	triangle.getPoints(triangleCorners);

	for (uint i = 0; i < 3; i++) {
		collision = evaluateTyped(LineSegment2CollisionMask<T>(LineSegment<T, 2>::fromPoints(triangleCorners[i], triangleCorners[(i + 1) % 3]), true), rectangleMask);
		if (collision.mDid) {
			return collision;
		}
	}

	collision = evaluateTyped(Point2CollisionMask<T>(rectangle.getCenter(), true), triangleMask);
	if (collision.mDid) {
		return collision;
	}

	collision = evaluateTyped(Point2CollisionMask<T>(triangleCorners[0], true), rectangleMask);

	if (!collision.mDid && mReturnSeparator) {
		Vector<T, 2> rectCorners[4];
		rectangle.getCorners(rectCorners);

		Vector<T, 2> projectTo[5];
		rectangle.getAxes(projectTo);
		triangle.getLazyNormals(&projectTo[2]);

		uint index = findProjectionSeparation(rectCorners, 4, triangleCorners, 3, projectTo, 5);
		if (index == 5) {
			throw ProcessFailureException();
		}
		collision.mSeparator = projectTo[index];
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(Triangle2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Triangle<T, 2> const& triangle1 = *basis1;
	Triangle<T, 2> const& triangle2 = *basis2;

	Vector<T, 2> corners[2][3];
	Vector<T, 2> projectees[2][3];
	Range<T> projections[2][3][2];

	triangle1.getPoints(&corners[0][0]);
	triangle2.getPoints(&corners[1][0]);
	triangle1.getPointOffsets(&projectees[0][0]);
	triangle2.getPointOffsets(&projectees[1][0]);

	uint i2;

	// for each triangle
	for (uint i = 0; i < 2; i++) {
		i2 = (i == 0) ? 1 : 0;

		// for each side of that triangle
		for (uint j = 0; j < 3; j++) {
			projectees[i][j] = projectees[i][j].orthogonal();

			projections[i][j][i] = Range(
				projectees[i][j].dot(corners[i][j]), 
				projectees[i][j].dot(corners[i][(j + 2) % 3])
			);

			// for each side of the other triangle
			for (uint k = 0; k < 3; k++) {
				projections[i][j][i2].expandTo(projectees[i][j].dot(corners[i2][k]));
			}

			if (!projections[i][j][0].intersection(projections[i][j][1])) {
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
			if (evaluateTyped(Point2CollisionMask<T>(corners[i][j], true), triangleMasks[i2]).mDid) {
				collision.mDid = true;
				collision.mPoint = corners[i][j];
				return collision;
			}
		}
	}

	for (uint j = 0; j < 3; j++) {
		LineSegment<T, 2> edge = LineSegment<T, 2>::fromPoints(corners[0][j], corners[0][(j + 1) % 3]);
		collision = evaluateTyped(LineSegment2CollisionMask<T>(edge), triangleMasks[1]);
		if (collision.mDid) {
			return collision;
		}
	}

	throw ProcessFailureException();
	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(Line2CollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Line<T, 2> const& line1 = *basis1;
	Line<T, 2> const& line2 = *basis2;

	Matrix<T, 2, 3> matrix;
	matrix.getColumn(0, line1.getDirection());
	matrix.getColumn(1, -line2.getDirection());
	matrix.getColumn(2, line2.getPoint() - line2.getPoint());
	matrix.RREFify();

	if (!matrix.isRowZero(1)) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = line1.getPoint() + line1.getDirection() * matrix.getElement(0, 2);
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = line1.getDirection().orthogonal();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(Line2CollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Line<T, 2> const& line = *basis1;
	LineSegment<T, 2> const& lineSegment = *basis2;

	Line2CollisionMask<T> lineMask(line, true);

	bool thismReturnPoint = mReturnPoint;
	mReturnPoint = true;
	Collision<T, 2> testCollision = evaluateTyped(
		lineMask,
		Line2CollisionMask<T>(Line<T, 2>::fromPoints(lineSegment.getPoint1(), lineSegment.getPoint2()), true)
	);
	mReturnPoint = thismReturnPoint;

	if (testCollision.mDid && 
		GEUtil::betwInc(
			lineSegment.getProjectionCoefficient(testCollision.mPoint),
			lineSegment.getProjectionCoefficient1(),
			lineSegment.getProjectionCoefficient2())
		) {

		collision = testCollision;
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = line.getDirection().orthogonal();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(Line2CollisionMask<T>& in_mask1, MST2CollisionMask<T>& in_mask2) {
	return Collision<T, 2>();
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(Line2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Line<T, 2> const& line = *basis1;
	Vector<T, 2> const& point = *basis2;

	Vector<T, 2> pointOffset = point - line.getPoint();
	Vector<T, 2> direction = line.getDirection();

	if (pointOffset.x() / direction.x() == pointOffset.y() / direction.y()) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = point;
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = line.getDirection().orthogonal();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(Line2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Line<T, 2> const& line = *basis1;
	Ray<T, 2> const& ray = *basis2;

	Line2CollisionMask<T> lineMask(line, true);

	bool thismReturnPoint = mReturnPoint;
	mReturnPoint = true;
	Collision<T, 2> testCollision = evaluateTyped(
		lineMask,
		Line2CollisionMask<T>(Line<T, 2>::fromPointDirection(ray.getPoint(), ray.getDirection()), true)
	);
	mReturnPoint = thismReturnPoint;

	if (testCollision.mDid && ray.getProjectionCoefficient(testCollision.mPoint) >= ray.getProjectionCoefficient()) {
		collision = testCollision;
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = line.getDirection().orthogonal();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(Line2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Line<T, 2> const& line = *basis1;
	Box<T, 2> const& rectangle = *basis2;

	RectangleCollisionMask<T> rectangleMask = RectangleCollisionMask<T>(rectangle, true);

	Vector<T, 2> corners[4];
	rectangle.getCorners(corners);
	
	Vector<T, 2> projectionPoint;
	for (uint i = 0; i < 4; i++) {
		projectionPoint = line.getProjection(corners[i]);
		collision = evaluateTyped(Point2CollisionMask<T>(projectionPoint, true), rectangleMask);
		if (collision.mDid) {
			return collision;
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = line.getDirection().orthogonal();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(Line2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Line<T, 2> const& line = *basis1;
	Triangle<T, 2> const& triangle = *basis2;

	Vector<T, 2> corners[3];
	triangle.getPoints(corners);

	Vector<T, 2> projectionPoints[3];
	for (uint i = 0; i < 3; i++) {
		projectionPoints[i] = line.getProjection(corners[i]);
	}

	Vector<T, 2> diff1, diff2;
	for (uint i = 0; i < 3; i++) {
		diff1 = corners[i] - projectionPoints[i];
		diff2 = corners[(i + 1) % 3] - projectionPoints[(i + 1) % 3];
		if (diff1.dot(diff2) < 0.0) {
			collision.mDid = true;
			if (mReturnPoint) {
				collision.mPoint = projectionPoints[i].lerp(projectionPoints[(i + 1) % 3], diff2.magnitude() / diff1.magnitude());
			}
			break;
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = line.getDirection().orthogonal();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(AAHalfSpace2CollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	AxisAlignedHalfSpace<T, 2> const& halfSpace = *basis1;
	LineSegment<T, 2> const& lineSegment = *basis2;

	uint dimension = halfSpace.getDimension();
	uint otherDimension = (dimension == 0) ? 1 : 0;

	Vector<T, 2> point1 = lineSegment.getPoint1();
	Vector<T, 2> point2 = lineSegment.getPoint2();

	Range<T> range = Range<T>(point1[dimension], point2[dimension]);

	if (GEUtil::betwInc(
		halfSpace.getValue(),
		range.getLow(),
		range.getHigh()
	)) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint[dimension] = halfSpace.getValue();
			collision.mPoint[otherDimension] = GEUtil::lerp(
				point1[otherDimension], 
				point2[otherDimension], 
				halfSpace.getValue(), 
				point1[dimension], 
				point2[dimension]
			);
		}
	}
	else if (
		GEUtil::ceqSwitch(halfSpace.getValue(), point1[dimension], !halfSpace.isPositive()) ||
		GEUtil::ceqSwitch(halfSpace.getValue(), point2[dimension], !halfSpace.isPositive())) {

		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = lineSegment.getCenter();
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mOwner = &in_mask1;
		collision.mSeparator = -halfSpace.getDirection();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(AAHalfSpace2CollisionMask<T>& in_mask1, MST2CollisionMask<T>& in_mask2) {
	return Collision<T, 2>();
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(AAHalfSpace2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	AxisAlignedHalfSpace<T, 2> const& halfSpace = *basis1;
	Vector<T, 2> const& point = *basis2;

	T pointValue = point[halfSpace.getDimension()];
	T halfSpaceValue = halfSpace.getValue();

	if (GEUtil::ceqSwitch(halfSpaceValue, pointValue, !halfSpace.isPositive())) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = point;
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace.getDirection();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(AAHalfSpace2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	AxisAlignedHalfSpace<T, 2> const& halfSpace = *basis1;
	Ray<T, 2> const& ray = *basis2;

	AAHalfSpace2CollisionMask<T> halfSpaceMask = AAHalfSpace2CollisionMask<T>(halfSpace, true);
	Line2CollisionMask<T> lineMask = Line2CollisionMask<T>(Line<T, 2>::fromPointDirection(ray.getPoint(), ray.getDirection()), true);

	bool thismReturnPoint = mReturnPoint;
	mReturnPoint = true;
	Collision<T, 2> testCollision = evaluateTyped(halfSpaceMask, lineMask);
	mReturnPoint = thismReturnPoint;

	if (testCollision.mDid && ray.getProjectionCoefficient(testCollision.mPoint) >= ray.getProjectionCoefficient()) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = testCollision.mPoint;
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace.getDirection();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(AAHalfSpace2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) {

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Box<T, 2> const& rectangle = *basis2;

	Vector<T, 2> corners[4];
	rectangle.getCorners(corners);

	uint dimension = in_mask1.getBasis().getDimension();

	uint minIndex = 0;
	uint maxIndex = 0;
	for (uint i = 1; i < 4; i++) {
		if (corners[i][dimension] < corners[minIndex][dimension]) minIndex = i;
		if (corners[i][dimension] > corners[maxIndex][dimension]) maxIndex = i;
	}

	return evaluateTyped(
		in_mask1,
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::fromPoints(corners[minIndex], corners[maxIndex]), true)
	);
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(AAHalfSpace2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	AxisAlignedHalfSpace<T, 2> const& halfSpace = *basis1;;
	Triangle<T, 2> const& triangle = *basis2;

	Vector<T, 2> corners[3];
	triangle.getPoints(corners);

	uint dimension = halfSpace.getDimension();

	uint minIndex = 0;
	uint maxIndex = 0;
	for (uint i = 0; i < 3; i++) {
		if (!collision.mDid && GEUtil::ceqSwitch(halfSpace.getValue(), corners[i][dimension], !halfSpace.isPositive())) {
			collision.mDid = true;
			if (!mReturnPoint) {
				return collision;
			}
		}
		if (corners[i][dimension] < corners[minIndex][dimension]) minIndex = i;
		if (corners[i][dimension] > corners[maxIndex][dimension]) maxIndex = i;
	}

	if (mReturnPoint && collision.mDid) {
		collision = evaluateTyped(
			AAHalfSpace2CollisionMask<T>(halfSpace, true),
			LineSegment2CollisionMask<T>(LineSegment<T, 2>::fromPoints(corners[minIndex], corners[maxIndex]), true)
		);
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mOwner = &in_mask1;
		collision.mSeparator = -halfSpace.getDirection();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(AAHalfSpace2CollisionMask<T>& in_mask1, AAHalfSpace2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	AxisAlignedHalfSpace<T, 2> const& halfSpace1 = *basis1;
	AxisAlignedHalfSpace<T, 2> const& halfSpace2 = *basis2;

	if (halfSpace1.getDimension() == halfSpace2.getDimension()) {
		uint dimension = halfSpace1.getDimension();
		if (GEUtil::ceqSwitch(halfSpace1.getValue(), halfSpace2.getValue(), !halfSpace1.isPositive())) {
			collision.mDid = true;
			if (mReturnPoint) {
				collision.mPoint[dimension] = halfSpace2.getValue();
			}
		}
		else if (GEUtil::ceqSwitch(halfSpace2.getValue(), halfSpace1.getValue(), !halfSpace2.isPositive())) {
			collision.mDid = true;
			if (mReturnPoint) {
				collision.mPoint[dimension] = halfSpace1.getValue();
			}
		}
	}
	else {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint[halfSpace1.getDimension()] = halfSpace1.getValue();
			collision.mPoint[halfSpace2.getDimension()] = halfSpace2.getValue();
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace1.getDirection();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(AAHalfSpace2CollisionMask<T>& in_mask1, AALine2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	AxisAlignedHalfSpace<T, 2> const& halfSpace = *basis1;
	AxisAlignedLine<T, 2> const& line = *basis2;

	if (halfSpace.getDimension() == line.getDimension()) {
		uint dimension = halfSpace.getDimension();
		if (GEUtil::ceqSwitch(halfSpace.getValue(), line.getValue(), !halfSpace.isPositive())) {
			collision.mDid = true;
			if (mReturnPoint) {
				collision.mPoint[dimension] = line.getValue();
			}
		}
	}
	else {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint[halfSpace.getDimension()] = halfSpace.getValue();
			collision.mPoint[line.getDimension()] = line.getValue();
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace.getDirection();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(AAHalfSpace2CollisionMask<T>& in_mask1, AARectangleCollisionMask<T>& in_mask2) {

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	AxisAlignedBox<T, 2> const& rectangle = *basis2;

	return evaluateTyped(
		in_mask1,
		LineSegment2CollisionMask<T>(LineSegment<T, 2>::fromPoints(rectangle.getMinima(), rectangle.getMaxima()), true)
	);
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(AAHalfSpace2CollisionMask<T>& in_mask1, CircleCollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	Circle<T> const& circle = *basis2;

	uint dimension = in_mask1.getBasis().getDimension();

	LineSegment2CollisionMask<T> newLineSegmentMask(LineSegment<T, 2>::fromPoints(circle.getExtrema(dimension, false), circle.getExtrema(dimension, true)), true);

	collision = evaluateTyped(in_mask1, newLineSegmentMask);

	if (!collision.mDid && mReturnSeparator) {
		if (collision.mOwner == &newLineSegmentMask) collision.mOwner = &in_mask2;
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(AAHalfSpace2CollisionMask<T>& in_mask1, HalfSpace2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	AxisAlignedHalfSpace<T, 2> const& aaHalfSpace = *basis1;
	HalfSpace<T, 2> const& halfSpace = *basis2;

	Line<T, 2> line = Line<T, 2>::fromPointDirection(halfSpace.getPoint(), halfSpace.getDirection().orthogonal());

	AAHalfSpace2CollisionMask<T> aaHalfSpaceMask = AAHalfSpace2CollisionMask<T>(aaHalfSpace, true);
	Line2CollisionMask<T> lineMask = Line2CollisionMask<T>(line, true);

	collision = evaluateTyped(aaHalfSpaceMask, lineMask);

	if (!collision.mDid && GEUtil::ceqSwitch(aaHalfSpace.getValue(), halfSpace.getPoint()[aaHalfSpace.getDimension()], !aaHalfSpace.isPositive())) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = halfSpace.getPoint();
		}
	}

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace.getDirection();
	}

	return collision;
}

template<class T>
Collision<T, 2> InPlaceCollisionEvaluator<T, 2>::evaluateTyped(AAHalfSpace2CollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) {
	Collision<T, 2> collision;

	auto basis1 = in_mask1.getTransformedBasis();
	auto basis2 = in_mask2.getTransformedBasis();

	AxisAlignedHalfSpace<T, 2> const& halfSpace = *basis1;
	Line<T, 2> const& line = *basis2;

	uint dimension = halfSpace.getDimension();

	if (line.getDirection()[dimension] != 0.0) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = line.getPoint();
			collision.mPoint += line.getDirection() * (halfSpace.getValue() - line.getPoint()[dimension]) / line.getDirection()[dimension];
		}
	}
	else if (GEUtil::ceqSwitch(halfSpace.getValue(), line.getPoint()[dimension], !halfSpace.isPositive())) {
		collision.mDid = true;
		if (mReturnPoint) {
			collision.mPoint = line.getPoint();
		}
	} 

	if (!collision.mDid && mReturnSeparator) {
		collision.mSeparator = halfSpace.getDirection();
	}

	return collision;
}

template class InPlaceCollisionEvaluator<float, 2>;
template class InPlaceCollisionEvaluator<double, 2>;