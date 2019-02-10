#include "collision_basis_wrapper.h"

template<class T>
void CollisionBasisWrapper<AxisAlignedHalfSpace<T, 2>, T>::applyTransform(const Transform<T, 2>& in_transform) {
	mBasis.applyTransform(in_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<AxisAlignedHalfSpace<T, 2>, T>::getClosestPoint(const basis_t& in_basis, const Vector<T, 2>& in_point) {
	Vector<T, 2> out = in_point;
	out[in_basis.getDimension()] = in_basis.getValue();
	return out;
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<AxisAlignedHalfSpace<T, 2>, T>::getClosestNormal(const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy) {
	return -in_basis.getDirection();
}

template<class T>
void CollisionBasisWrapper<AxisAlignedLine<T, 2>, T>::applyTransform(const Transform<T, 2>& in_transform) {
	mBasis.applyTransform(in_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<AxisAlignedLine<T, 2>, T>::getClosestPoint(const basis_t& in_basis, const Vector<T, 2>& in_point) {
	Vector<T, 2> out = in_point;
	out[in_basis.getDimension()] = in_basis.getValue();
	return out;
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<AxisAlignedLine<T, 2>, T>::getClosestNormal(const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy) {
	return in_basis.getDirection().orthogonal();
}

template<class T>
void CollisionBasisWrapper<AxisAlignedBox<T, 2>, T>::applyTransform(const Transform<T, 2>& in_transform) {
	mBasis.applyTransform(in_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<AxisAlignedBox<T, 2>, T>::getClosestPoint(const basis_t& in_basis, const Vector<T, 2>& in_point) {
	Vector<T, 2> center = in_basis.getCenter();
	Vector<T, 2> offset = in_point - center;

	Vector<T, 2> minima = in_basis.getMinima();
	Vector<T, 2> maxima = in_basis.getMaxima();

	Range<T> xRange(minima.x(), maxima.x());
	Range<T> yRange(minima.y(), maxima.y());

	if (xRange.containsInc(in_point.x())) {
		if (yRange.containsInc(in_point.y())) {
			if(abs(offset.x()) > abs(offset.y())) {
				return Vector<T, 2>(
					(offset.x() > 0) ? maxima.x() : minima.x(),
					in_point.y()
					);
			}
			else {
				return Vector<T, 2>(
					in_point.x(),
					(offset.y() > 0) ? maxima.y() : minima.y()
					);
			}
		}
		else {
			return Vector<T, 2>(
				in_point.x(),
				(offset.y() > 0) ? maxima.y() : minima.y()
				);
		}
	}
	else {
		if (yRange.containsInc(in_point.y())) {
			return Vector<T, 2>(
				(offset.x() > 0) ? maxima.x() : minima.x(),
				in_point.y()
				);
		}
		else {
			return Vector<T, 2>(
				(offset.x() > 0) ? maxima.x() : minima.x(),
				(offset.y() > 0) ? maxima.y() : minima.y()
				);
		}
	}
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<AxisAlignedBox<T, 2>, T>::getClosestNormal(const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy){
	Vector<T, 2> center = in_basis.getCenter();
	Vector<T, 2> offset = in_point - center;

	Vector<T, 2> minima = in_basis.getMinima();
	Vector<T, 2> maxima = in_basis.getMaxima();

	Range<T> xRange(minima.x(), maxima.x());
	Range<T> yRange(minima.y(), maxima.y());

	if (xRange.containsInc(in_point.x()) || yRange.containsInc(in_point.y())) {
		return (abs(offset.x()) > abs(offset.y())) ?
			Vector<T, 2>(1, 0) * GEUtil::sign(offset.x()) :
			Vector<T, 2>(0, 1) * GEUtil::sign(offset.y());
	}

	Vector<T, 2> corners[4];
	in_basis.getCorners(corners);

	switch (in_policy) {
	case pnp_t::zero:
		return Vector<T, 2>();
		break;
	case pnp_t::nearest_edge:
		return (abs(offset.x()) > abs(offset.y())) ?
			Vector<T, 2>(1, 0) * GEUtil::random(offset.x()) :
			Vector<T, 2>(0, 1) * GEUtil::random(offset.y());
		break;
	case pnp_t::towards_point: {
		uint index = GEUtil::minIndex({
			(in_point - corners[0]).dotSelf(),
			(in_point - corners[1]).dotSelf(),
			(in_point - corners[2]).dotSelf(),
			(in_point - corners[3]).dotSelf()
			});
		return (in_point - corners[index]).normalized();
	}
										   break;
	default:
		throw InvalidArgumentException();
	}
}

template<class T>
void CollisionBasisWrapper<Sphere<T, 2>, T>::applyTransform(const Transform<T, 2>& in_transform) {
	mBasis.applyTransform(in_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Sphere<T, 2>, T>::getClosestPoint(const basis_t& in_basis, const Vector<T, 2>& in_point) {
	return (in_point - in_basis.getCenter()).normalized() * in_basis.getRadius();
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Sphere<T, 2>, T>::getClosestNormal(const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy) {
	return (in_point - in_basis.getCenter()).normalized();
}

template<class T>
void CollisionBasisWrapper<HalfSpace<T, 2>, T>::applyTransform(const Transform<T, 2>& in_transform) {
	mBasis.applyTransform(in_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<HalfSpace<T, 2>, T>::getClosestPoint(const basis_t& in_basis, const Vector<T, 2>& in_point) {
	return in_point.projection(in_basis.getDirection().orthogonal()) + in_basis.getPoint();
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<HalfSpace<T, 2>, T>::getClosestNormal(const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy) {
	return -in_basis.getDirection();
}

template<class T>
void CollisionBasisWrapper<LineSegment<T, 2>, T>::applyTransform(const Transform<T, 2>& in_transform) {
	mBasis.applyTransform(in_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<LineSegment<T, 2>, T>::getClosestPoint(const basis_t& in_basis, const Vector<T, 2>& in_point) {
	T proj1 = in_basis.getProjectionCoefficient1();
	T proj2 = in_basis.getProjectionCoefficient2();
	T projPoint = in_basis.getProjectionCoefficient(in_point);

	if (projPoint <= proj1) {
		return in_basis.getPoint1();
	}
	if (projPoint >= proj2) {
		return in_basis.getPoint2();
	}

	return in_basis.getProjection(in_point);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<LineSegment<T, 2>, T>::getClosestNormal(const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy) {
	Vector<T, 2> normal = in_basis.getDirection().orthogonal();
	Vector<T, 2> points[2] = {
		in_basis.getPoint1(),
		in_basis.getPoint2()
	};
	Vector<T, 2> direction = in_basis.getDirection();

	if (GEUtil::betwInc<T>(
		(in_point - points[0]).dot(direction),
		0, (points[1] - points[0]).dot(direction)
		)) {

		return (normal.dot(in_point) >= normal.dot(points[0])) ? normal : -normal;
	}
	switch (in_policy) {
	case pnp_t::zero:
		return Vector<T, 2>();
		break;
	case pnp_t::nearest_edge:
		return (normal.dot(in_point) >= normal.dot(points[0])) ? normal : -normal;
		break;
	case pnp_t::towards_point: {
		uint index = GEUtil::minIndex({
			(in_point - points[0]).dotSelf(),
			(in_point - points[1]).dotSelf(),
			});
		return (in_point - points[index]).normalized();
	}
		break;
	default:
		throw InvalidArgumentException();
	}
}

template<class T>
void CollisionBasisWrapper<Line<T, 2>, T>::applyTransform(const Transform<T, 2>& in_transform) {
	mBasis.applyTransform(in_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Line<T, 2>, T>::getClosestPoint(const basis_t& in_basis, const Vector<T, 2>& in_point) {
	return in_basis.getProjection(in_point);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Line<T, 2>, T>::getClosestNormal(const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy) {
	Vector<T, 2> normal = in_basis.getDirection().orthogonal();
	return (normal.dot(in_point) >= normal.dot(in_basis.getPoint())) ? normal : -normal;
}

template<class T>
void CollisionBasisWrapper<MeshSphereTree<T, 2>, T>::applyTransform(const Transform<T, 2>& in_transform) {
	mBasis.applyTransform(in_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<MeshSphereTree<T, 2>, T>::getClosestPoint(const basis_t& in_basis, const Vector<T, 2>& in_point) {
	using iter_t = typename MeshSphereTree<T, 2>::Iterator;
	using Triangle2Wrapper = CollisionBasisWrapper<Triangle<T, 2>, T>;

	std::vector<iter_t> itStack;
	std::vector<iter_t> leaves;
	itStack.reserve(in_basis.getMeshData().getNumberFaces() / 2);
	leaves.reserve(in_basis.getMeshData().getNumberFaces());
	itStack.push_back(in_basis.getIterator());
	while (!itStack.empty()) {
		iter_t it = itStack.back();
		itStack.pop_back();

		if (it.isLeaf()) {
			leaves.push_back(it);
		}
		else {
			itStack.push_back(it.goBoth());
			itStack.push_back(it);
		}
	}

	auto bestIt = leaves.begin();
	T bestDistance = std::numeric_limits<T>::infinity();
	for (auto it = leaves.begin(); it != leaves.end(); it++) {
		T distance = (it->getSphere().getCenter() - in_point).dotSelf();
		if (distance < bestDistance) {
			distance = bestDistance;
			bestIt = it;
		}
	}

	Vector<T, 2> bestPoint = Triangle2Wrapper::getClosestPoint(bestIt->getTriangle(), in_point);
	bestDistance = (bestPoint - in_point).magnitude();
	for (auto it = leaves.begin(); it != leaves.end(); it++) {
		T distance = (it->getSphere().getCenter() - in_point).magnitude() - it->getSphere().getRadius();
		if (distance < bestDistance) {
			Vector<T, 2> point = Triangle2Wrapper::getClosestPoint(it->getTriangle(), in_point);
			distance = (point - in_point).magnitude();
			if (distance < bestDistance) {
				bestPoint = point;
				bestIt = it;
			}
		}
	}

	return bestPoint;
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<MeshSphereTree<T, 2>, T>::getClosestNormal(const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy) {
	using iter_t = typename MeshSphereTree<T, 2>::Iterator;
	using Triangle2Wrapper = CollisionBasisWrapper<Triangle<T, 2>, T>;

	std::vector<iter_t> itStack;
	std::vector<iter_t> leaves;
	itStack.reserve(in_basis.getMeshData().getNumberFaces() / 2);
	leaves.reserve(in_basis.getMeshData().getNumberFaces());
	itStack.push_back(in_basis.getIterator());
	while (!itStack.empty()) {
		iter_t it = itStack.back();
		itStack.pop_back();

		if (it.isLeaf()) {
			leaves.push_back(it);
		}
		else {
			itStack.push_back(it.goBoth());
			itStack.push_back(it);
		}
	}

	auto bestIt = leaves.begin();
	T bestDistance = std::numeric_limits<T>::infinity();
	for (auto it = leaves.begin(); it != leaves.end(); it++) {
		T distance = (it->getSphere().getCenter() - in_point).dotSelf();
		if (distance < bestDistance) {
			distance = bestDistance;
			bestIt = it;
		}
	}

	Vector<T, 2> bestPoint = Triangle2Wrapper::getClosestPoint(bestIt->getTriangle(), in_point);
	bestDistance = (bestPoint - in_point).magnitude();
	for (auto it = leaves.begin(); it != leaves.end(); it++) {
		T distance = (it->getSphere().getCenter() - in_point).magnitude() - it->getSphere().getRadius();
		if (distance < bestDistance) {
			Vector<T, 2> point = Triangle2Wrapper::getClosestPoint(it->getTriangle(), in_point);
			distance = (point - in_point).magnitude();
			if (distance < bestDistance) {
				bestPoint = point;
				bestIt = it;
			}
		}
	}

	return Triangle2Wrapper::getClosestNormal(bestIt->getTriangle(), in_point, in_policy);
}

template<class T>
void CollisionBasisWrapper<Vector<T, 2>, T>::applyTransform(const Transform<T, 2>& in_transform) {
	mBasis = in_transform.localToWorldPoint(mBasis);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Vector<T, 2>, T>::getClosestPoint(const basis_t& in_basis, const Vector<T, 2>& in_point) {
	return in_basis;
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Vector<T, 2>, T>::getClosestNormal(const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy) {
	switch (in_policy) {
	case pnp_t::zero:
		return Vector<T, 2>();
		break;
	case pnp_t::nearest_edge:
	case pnp_t::towards_point:
		return in_point - in_basis;
		break;
	default:
		throw InvalidArgumentException();
	}
}

template<class T>
void CollisionBasisWrapper<Ray<T, 2>, T>::applyTransform(const Transform<T, 2>& in_transform) {
	mBasis.applyTransform(in_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Ray<T, 2>, T>::getClosestPoint(const basis_t& in_basis, const Vector<T, 2>& in_point) {
	T projRay = in_basis.getProjectionCoefficient();
	T projPoint = in_basis.getProjectionCoefficient(in_point);

	if (projPoint <= projRay) {
		return in_basis.getPoint();
	}

	return in_basis.getProjection(in_point);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Ray<T, 2>, T>::getClosestNormal(const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy) {
	Vector<T, 2> normal = in_basis.getDirection().orthogonal();
	if ((in_point - in_basis.getPoint()).dot(in_basis.getDirection()) >= 0) {
		(normal.dot(in_point) >= normal.dot(in_basis.getPoint())) ? normal : -normal;
	}
	switch (in_policy) {
	case pnp_t::zero:
		return Vector<T, 2>();
		break;
	case pnp_t::nearest_edge:
		return (normal.dot(in_point) >= normal.dot(in_basis.getPoint())) ? normal : -normal;
		break;
	case pnp_t::towards_point:
		return (in_point - in_basis.getPoint()).normalized();
		break;
	default:
		throw InvalidArgumentException();
	}
}

template<class T>
void CollisionBasisWrapper<Box<T, 2>, T>::applyTransform(const Transform<T, 2>& in_transform) {
	mBasis.applyTransform(in_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Box<T, 2>, T>::getClosestPoint(const basis_t& in_basis, const Vector<T, 2>& in_point) {
	Vector<T, 2> axes[2];
	in_basis.getAxes(axes);

	Vector<T, 2> normAxes[2];
	for (uint i = 0; i < 2; i++) {
		normAxes[i] = axes[i].normalized();
	}

	Vector<T, 2> origin = in_basis.getOrigin();

	Range<T> projRanges[2];
	T projPoints[2];
	T projOffsets[2];

	for (uint i = 0; i < 2; i++) {
		projRanges[i] = Range<T>(origin.projectionCoeff(axes[i]), (origin + axes[i]).projectionCoeff(axes[i]));
		projPoints[i] = in_point.projectionCoeff(axes[i]);
		projOffsets[i] = projRanges[i].getMean() - projPoints[i];
	}

	if (projRanges[0].containsInc(in_point.x())) {
		if (projRanges[1].containsInc(in_point.y())) {
			if (abs(projOffsets[0]) > abs(projOffsets[1])) {
				return origin +
					((projOffsets[0] > 0) ? axes[0] : Vector<T, 2>()) +
					normAxes[1] * projPoints[1];
			}
			else {
				return origin +
					normAxes[0] * projPoints[0] +
					((projOffsets[1] > 0) ? axes[1] : Vector<T, 2>());
			}
		}
		else {
			return origin +
				normAxes[0] * projPoints[0] +
				((projOffsets[1] > 0) ? axes[1] : Vector<T, 2>());
		}
	}
	else {
		if (projRanges[1].containsInc(in_point.y())) {
			return origin +
				((projOffsets[0] > 0) ? axes[0] : Vector<T, 2>()) +
				normAxes[1] * projPoints[1];
		}
		else {
			return origin +
				((projOffsets[0] > 0) ? axes[0] : Vector<T, 2>()) +
				((projOffsets[1] > 0) ? axes[1] : Vector<T, 2>());
		}
	}
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Box<T, 2>, T>::getClosestNormal(const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy) {
	Vector<T, 2> corners[4];
	Vector<T, 2> axes[2];
	in_basis.getAxes(axes);
	in_basis.getCorners(corners);
	Vector<T, 2> center = in_basis.getCenter();
	Vector<T, 2> offset = in_point - center;

	Range<T> range1(axes[0].dot(corners[0]), axes[0].dot(corners[3]));
	Range<T> range2(axes[1].dot(corners[0]), axes[1].dot(corners[3]));

	T pointDot1 = axes[0].dot(in_point);
	T pointDot2 = axes[1].dot(in_point);

	T offsetDot1 = axes[0].dot(offset);
	T offsetDot2 = axes[1].dot(offset);

	if (range1.containsInc(pointDot1)) {
		if (range2.containsInc(pointDot2)) {
			return (abs(offsetDot1) > abs(offsetDot2)) ?
				axes[0] * GEUtil::random(offsetDot1) :
				axes[1] * GEUtil::random(offsetDot2);
		}
		else {
			return axes[0] * GEUtil::random(offsetDot1);
		}
	}
	else {
		if (range2.containsInc(pointDot2)) {
			return axes[1] * GEUtil::random(offsetDot2);
		}
		else {
			switch (in_policy) {
			case pnp_t::zero:
				return Vector<T, 2>();
				break;
			case pnp_t::nearest_edge:
				return (abs(offsetDot1) > abs(offsetDot2)) ?
					axes[0] * GEUtil::random(offsetDot1) :
					axes[1] * GEUtil::random(offsetDot2);
				break;
			case pnp_t::towards_point: {
				uint index = GEUtil::minIndex({
					(in_point - corners[0]).dotSelf(),
					(in_point - corners[1]).dotSelf(),
					(in_point - corners[2]).dotSelf(),
					(in_point - corners[3]).dotSelf()
					});
				return (in_point - corners[index]).normalized();
			}
												   break;
			default:
				throw InvalidArgumentException();
			}
		}
	}
}

template<class T>
void CollisionBasisWrapper<Triangle<T, 2>, T>::applyTransform(const Transform<T, 2>& in_transform) {
	mBasis.applyTransform(in_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Triangle<T, 2>, T>::getClosestPoint(const basis_t& in_basis, const Vector<T, 2>& in_point) {
	Vector<T, 2> corners[3];
	Vector<T, 2> normals[3];
	in_basis.getPoints(corners);
	in_basis.getNormals(normals);
	Vector<T, 2> cornerNormals[3];
	Vector<T, 2> cornerTangents[3];
	bool alley[3];
	bool front[3];

	for (uint i = 0; i < 3; i++) {
		cornerNormals[i] = (normals[i] + normals[(i + 2) % 3]) / 2;
		cornerTangents[i] = cornerNormals[i].orthogonal();

		Vector<T, 2> offset = corners[(i + 1) % 3] - corners[i];
		alley[i] = GEUtil::betwExc(
			in_point.dot(offset),
			corners[i].dot(offset),
			corners[(i + 1) % 3].dot(offset)
		);

		front[i] = (in_point - corners[i]).dot(normals[i]) >= 0.0;

		if (front[i] && alley[i]) {
			return corners[i] + (in_point - corners[i]).projection(offset);
		}
	}

	uint index = GEUtil::minIndex({
			(in_point - corners[0]).dotSelf(),
			(in_point - corners[1]).dotSelf(),
			(in_point - corners[2]).dotSelf()
		});

	return corners[index];
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Triangle<T, 2>, T>::getClosestNormal(const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy) {
	Vector<T, 2> corners[3];
	Vector<T, 2> normals[3];
	in_basis.getNormals(normals);
	in_basis.getPoints(corners);
	Vector<T, 2> cornerNormals[3];
	Vector<T, 2> cornerTangents[3];
	bool slice[3];
	bool alley[3];
	bool front[3];
	for (uint i = 0; i < 3; i++) {
		cornerNormals[i] = (normals[i] + normals[(i + 2) % 3]) / 2;
		cornerTangents[i] = cornerNormals[i].orthogonal();

		slice[i] = cornerTangents[i].dot(in_point) >= cornerTangents[i].dot(corners[i]);

		Vector<T, 2> offset = corners[(i + 1) % 3] - corners[i];
		alley[i] = GEUtil::betwExc(
			in_point.dot(offset),
			corners[i].dot(offset),
			corners[(i + 1) % 3].dot(offset)
		);

		front[i] = (in_point - corners[i]).dot(normals[i]) >= 0.0;

		if (front[i] && alley[i]) {
			return normals[i];
		}
	}

	if (!front[0] && !front[1] && !front[2]) {
		for (uint i = 0; i < 3; i++) {
			if (slice[i] && !slice[(i + 1) % 3]) {
				return normals[i];
			}
		}
		throw ProcessFailureException();
	}

	switch (in_policy) {
	case pnp_t::zero:
		return Vector<T, 2>();
		break;
	case pnp_t::nearest_edge: {
		for (uint i = 0; i < 3; i++) {
			if (slice[i] && !slice[(i + 1) % 3]) {
				return normals[i];
			}
		}
		throw ProcessFailureException();
	}
										  break;
	case pnp_t::towards_point: {
		uint index = GEUtil::minIndex({
			(in_point - corners[0]).dotSelf(),
			(in_point - corners[1]).dotSelf(),
			(in_point - corners[2]).dotSelf()
			});

		return (in_point - corners[index]).normalized();
	}
										   break;
	default:
		throw InvalidArgumentException();
	}
}


template class CollisionBasisWrapper<AxisAlignedHalfSpace2f, float>;
template class CollisionBasisWrapper<AxisAlignedHalfSpace2d, double>;
template class CollisionBasisWrapper<AxisAlignedLine2f, float>;
template class CollisionBasisWrapper<AxisAlignedLine2d, double>;
template class CollisionBasisWrapper<AxisAlignedRectanglef, float>;
template class CollisionBasisWrapper<AxisAlignedRectangled, double>;
template class CollisionBasisWrapper<Circlef, float>;
template class CollisionBasisWrapper<Circled, double>;
template class CollisionBasisWrapper<HalfSpace2f, float>;
template class CollisionBasisWrapper<HalfSpace2d, double>;
template class CollisionBasisWrapper<LineSegment2f, float>;
template class CollisionBasisWrapper<LineSegment2d, double>;
template class CollisionBasisWrapper<Line2f, float>;
template class CollisionBasisWrapper<Line2d, double>;
template class CollisionBasisWrapper<MeshSphereTree2f, float>;
template class CollisionBasisWrapper<MeshSphereTree2d, double>;
template class CollisionBasisWrapper<Vector2f, float>;
template class CollisionBasisWrapper<Vector2d, double>;
template class CollisionBasisWrapper<Ray2f, float>;
template class CollisionBasisWrapper<Ray2d, double>;
template class CollisionBasisWrapper<Rectanglef, float>;
template class CollisionBasisWrapper<Rectangled, double>;
template class CollisionBasisWrapper<Triangle2f, float>;
template class CollisionBasisWrapper<Triangle2d, double>;