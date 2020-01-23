#include "collider_basis.h"

template<class T>
void CollisionBasisWrapper<AxisAlignedHalfSpace<T, 2>, T>::applyTransform(const Transform<T, 2>& i_transform) {
	mBasis.applyTransform(i_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<AxisAlignedHalfSpace<T, 2>, T>::getClosestPoint(const basis_t& i_basis, const Vector<T, 2>& i_point) {
	Vector<T, 2> out = i_point;
	out[i_basis.getDimension()] = i_basis.getValue();
	return out;
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<AxisAlignedHalfSpace<T, 2>, T>::getClosestNormal(const basis_t& i_basis, const Vector<T, 2>& i_point, pnp_t i_policy) {
	return -i_basis.getDirection();
}

template<class T>
void CollisionBasisWrapper<AxisAlignedLine<T, 2>, T>::applyTransform(const Transform<T, 2>& i_transform) {
	mBasis.applyTransform(i_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<AxisAlignedLine<T, 2>, T>::getClosestPoint(const basis_t& i_basis, const Vector<T, 2>& i_point) {
	Vector<T, 2> out = i_point;
	out[i_basis.getDimension()] = i_basis.getValue();
	return out;
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<AxisAlignedLine<T, 2>, T>::getClosestNormal(const basis_t& i_basis, const Vector<T, 2>& i_point, pnp_t i_policy) {
	return i_basis.getDirection().orthogonal();
}

template<class T>
void CollisionBasisWrapper<AxisAlignedBox<T, 2>, T>::applyTransform(const Transform<T, 2>& i_transform) {
	mBasis.applyTransform(i_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<AxisAlignedBox<T, 2>, T>::getClosestPoint(const basis_t& i_basis, const Vector<T, 2>& i_point) {
	Vector<T, 2> center = i_basis.getCenter();
	Vector<T, 2> offset = i_point - center;

	Vector<T, 2> minima = i_basis.getMinima();
	Vector<T, 2> maxima = i_basis.getMaxima();

	Range<T> xRange(minima.x(), maxima.x());
	Range<T> yRange(minima.y(), maxima.y());

	if (xRange.containsInc(i_point.x())) {
		if (yRange.containsInc(i_point.y())) {
			if(abs(offset.x()) > abs(offset.y())) {
				return Vector<T, 2>(
					(offset.x() > 0) ? maxima.x() : minima.x(),
					i_point.y()
					);
			}
			else {
				return Vector<T, 2>(
					i_point.x(),
					(offset.y() > 0) ? maxima.y() : minima.y()
					);
			}
		}
		else {
			return Vector<T, 2>(
				i_point.x(),
				(offset.y() > 0) ? maxima.y() : minima.y()
				);
		}
	}
	else {
		if (yRange.containsInc(i_point.y())) {
			return Vector<T, 2>(
				(offset.x() > 0) ? maxima.x() : minima.x(),
				i_point.y()
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
Vector<T, 2> CollisionBasisWrapper<AxisAlignedBox<T, 2>, T>::getClosestNormal(const basis_t& i_basis, const Vector<T, 2>& i_point, pnp_t i_policy){
	Vector<T, 2> center = i_basis.getCenter();
	Vector<T, 2> offset = i_point - center;

	Vector<T, 2> minima = i_basis.getMinima();
	Vector<T, 2> maxima = i_basis.getMaxima();

	Range<T> xRange(minima.x(), maxima.x());
	Range<T> yRange(minima.y(), maxima.y());

	if (xRange.containsInc(i_point.x()) || yRange.containsInc(i_point.y())) {
		return (abs(offset.x()) > abs(offset.y())) ?
			Vector<T, 2>(1, 0) * geutil::sign(offset.x()) :
			Vector<T, 2>(0, 1) * geutil::sign(offset.y());
	}

	Vector<T, 2> corners[4];
	i_basis.getCorners(corners);

	switch (i_policy) {
	case pnp_t::zero:
		return Vector<T, 2>();
		break;
	case pnp_t::nearest_edge:
		return (abs(offset.x()) > abs(offset.y())) ?
			Vector<T, 2>(1, 0) * geutil::random(offset.x()) :
			Vector<T, 2>(0, 1) * geutil::random(offset.y());
		break;
	case pnp_t::towards_point: {
		uint index = geutil::minIndex({
			(i_point - corners[0]).dotSelf(),
			(i_point - corners[1]).dotSelf(),
			(i_point - corners[2]).dotSelf(),
			(i_point - corners[3]).dotSelf()
			});
		return (i_point - corners[index]).normalized();
	}
										   break;
	default:
		throw InvalidArgumentException();
	}
}

template<class T>
void CollisionBasisWrapper<Sphere<T, 2>, T>::applyTransform(const Transform<T, 2>& i_transform) {
	mBasis.applyTransform(i_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Sphere<T, 2>, T>::getClosestPoint(const basis_t& i_basis, const Vector<T, 2>& i_point) {
	return (i_point - i_basis.getCenter()).normalized() * i_basis.getRadius();
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Sphere<T, 2>, T>::getClosestNormal(const basis_t& i_basis, const Vector<T, 2>& i_point, pnp_t i_policy) {
	return (i_point - i_basis.getCenter()).normalized();
}

template<class T>
void CollisionBasisWrapper<HalfSpace<T, 2>, T>::applyTransform(const Transform<T, 2>& i_transform) {
	mBasis.applyTransform(i_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<HalfSpace<T, 2>, T>::getClosestPoint(const basis_t& i_basis, const Vector<T, 2>& i_point) {
	return i_point.projection(i_basis.getDirection().orthogonal()) + i_basis.getPoint();
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<HalfSpace<T, 2>, T>::getClosestNormal(const basis_t& i_basis, const Vector<T, 2>& i_point, pnp_t i_policy) {
	return -i_basis.getDirection();
}

template<class T>
void CollisionBasisWrapper<LineSegment<T, 2>, T>::applyTransform(const Transform<T, 2>& i_transform) {
	mBasis.applyTransform(i_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<LineSegment<T, 2>, T>::getClosestPoint(const basis_t& i_basis, const Vector<T, 2>& i_point) {
	T proj1 = i_basis.getProjectionCoefficient1();
	T proj2 = i_basis.getProjectionCoefficient2();
	T projPoint = i_basis.getProjectionCoefficient(i_point);

	if (projPoint <= proj1) {
		return i_basis.getPoint1();
	}
	if (projPoint >= proj2) {
		return i_basis.getPoint2();
	}

	return i_basis.getProjection(i_point);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<LineSegment<T, 2>, T>::getClosestNormal(const basis_t& i_basis, const Vector<T, 2>& i_point, pnp_t i_policy) {
	Vector<T, 2> normal = i_basis.getDirection().orthogonal();
	Vector<T, 2> points[2] = {
		i_basis.getPoint1(),
		i_basis.getPoint2()
	};
	Vector<T, 2> direction = i_basis.getDirection();

	if (geutil::betwInc<T>(
		(i_point - points[0]).dot(direction),
		0, (points[1] - points[0]).dot(direction)
		)) {

		return (normal.dot(i_point) >= normal.dot(points[0])) ? normal : -normal;
	}
	switch (i_policy) {
	case pnp_t::zero:
		return Vector<T, 2>();
		break;
	case pnp_t::nearest_edge:
		return (normal.dot(i_point) >= normal.dot(points[0])) ? normal : -normal;
		break;
	case pnp_t::towards_point: {
		uint index = geutil::minIndex({
			(i_point - points[0]).dotSelf(),
			(i_point - points[1]).dotSelf(),
			});
		return (i_point - points[index]).normalized();
	}
		break;
	default:
		throw InvalidArgumentException();
	}
}

template<class T>
void CollisionBasisWrapper<Line<T, 2>, T>::applyTransform(const Transform<T, 2>& i_transform) {
	mBasis.applyTransform(i_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Line<T, 2>, T>::getClosestPoint(const basis_t& i_basis, const Vector<T, 2>& i_point) {
	return i_basis.getProjection(i_point);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Line<T, 2>, T>::getClosestNormal(const basis_t& i_basis, const Vector<T, 2>& i_point, pnp_t i_policy) {
	Vector<T, 2> normal = i_basis.getDirection().orthogonal();
	return (normal.dot(i_point) >= normal.dot(i_basis.getPoint())) ? normal : -normal;
}

template<class T>
void CollisionBasisWrapper<MeshSphereTree<T, 2>, T>::applyTransform(const Transform<T, 2>& i_transform) {
	mBasis.applyTransform(i_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<MeshSphereTree<T, 2>, T>::getClosestPoint(const basis_t& i_basis, const Vector<T, 2>& i_point) {
	using iter_t = typename MeshSphereTree<T, 2>::Iterator;
	using Triangle2Wrapper = CollisionBasisWrapper<Triangle<T, 2>, T>;

	std::vector<iter_t> itStack;
	std::vector<iter_t> leaves;
	itStack.reserve(i_basis.getMeshData().getNumberFaces() / 2);
	leaves.reserve(i_basis.getMeshData().getNumberFaces());
	itStack.push_back(i_basis.getIterator());
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
		T distance = (it->getSphere().getCenter() - i_point).dotSelf();
		if (distance < bestDistance) {
			distance = bestDistance;
			bestIt = it;
		}
	}

	Vector<T, 2> bestPoint = Triangle2Wrapper::getClosestPoint(bestIt->getTriangle(), i_point);
	bestDistance = (bestPoint - i_point).magnitude();
	for (auto it = leaves.begin(); it != leaves.end(); it++) {
		T distance = (it->getSphere().getCenter() - i_point).magnitude() - it->getSphere().getRadius();
		if (distance < bestDistance) {
			Vector<T, 2> point = Triangle2Wrapper::getClosestPoint(it->getTriangle(), i_point);
			distance = (point - i_point).magnitude();
			if (distance < bestDistance) {
				bestPoint = point;
				bestIt = it;
			}
		}
	}

	return bestPoint;
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<MeshSphereTree<T, 2>, T>::getClosestNormal(const basis_t& i_basis, const Vector<T, 2>& i_point, pnp_t i_policy) {
	using iter_t = typename MeshSphereTree<T, 2>::Iterator;
	using Triangle2Wrapper = CollisionBasisWrapper<Triangle<T, 2>, T>;

	std::vector<iter_t> itStack;
	std::vector<iter_t> leaves;
	itStack.reserve(i_basis.getMeshData().getNumberFaces() / 2);
	leaves.reserve(i_basis.getMeshData().getNumberFaces());
	itStack.push_back(i_basis.getIterator());
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
		T distance = (it->getSphere().getCenter() - i_point).dotSelf();
		if (distance < bestDistance) {
			distance = bestDistance;
			bestIt = it;
		}
	}

	Vector<T, 2> bestPoint = Triangle2Wrapper::getClosestPoint(bestIt->getTriangle(), i_point);
	bestDistance = (bestPoint - i_point).magnitude();
	for (auto it = leaves.begin(); it != leaves.end(); it++) {
		T distance = (it->getSphere().getCenter() - i_point).magnitude() - it->getSphere().getRadius();
		if (distance < bestDistance) {
			Vector<T, 2> point = Triangle2Wrapper::getClosestPoint(it->getTriangle(), i_point);
			distance = (point - i_point).magnitude();
			if (distance < bestDistance) {
				bestPoint = point;
				bestIt = it;
			}
		}
	}

	return Triangle2Wrapper::getClosestNormal(bestIt->getTriangle(), i_point, i_policy);
}

template<class T>
void CollisionBasisWrapper<Vector<T, 2>, T>::applyTransform(const Transform<T, 2>& i_transform) {
	mBasis = i_transform.localToWorldPoint(mBasis);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Vector<T, 2>, T>::getClosestPoint(const basis_t& i_basis, const Vector<T, 2>& i_point) {
	return i_basis;
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Vector<T, 2>, T>::getClosestNormal(const basis_t& i_basis, const Vector<T, 2>& i_point, pnp_t i_policy) {
	switch (i_policy) {
	case pnp_t::zero:
		return Vector<T, 2>();
		break;
	case pnp_t::nearest_edge:
	case pnp_t::towards_point:
		return i_point - i_basis;
		break;
	default:
		throw InvalidArgumentException();
	}
}

template<class T>
void CollisionBasisWrapper<Ray<T, 2>, T>::applyTransform(const Transform<T, 2>& i_transform) {
	mBasis.applyTransform(i_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Ray<T, 2>, T>::getClosestPoint(const basis_t& i_basis, const Vector<T, 2>& i_point) {
	T projRay = i_basis.getProjectionCoefficient();
	T projPoint = i_basis.getProjectionCoefficient(i_point);

	if (projPoint <= projRay) {
		return i_basis.getPoint();
	}

	return i_basis.getProjection(i_point);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Ray<T, 2>, T>::getClosestNormal(const basis_t& i_basis, const Vector<T, 2>& i_point, pnp_t i_policy) {
	Vector<T, 2> normal = i_basis.getDirection().orthogonal();
	if ((i_point - i_basis.getPoint()).dot(i_basis.getDirection()) >= 0) {
		(normal.dot(i_point) >= normal.dot(i_basis.getPoint())) ? normal : -normal;
	}
	switch (i_policy) {
	case pnp_t::zero:
		return Vector<T, 2>();
		break;
	case pnp_t::nearest_edge:
		return (normal.dot(i_point) >= normal.dot(i_basis.getPoint())) ? normal : -normal;
		break;
	case pnp_t::towards_point:
		return (i_point - i_basis.getPoint()).normalized();
		break;
	default:
		throw InvalidArgumentException();
	}
}

template<class T>
void CollisionBasisWrapper<Box<T, 2>, T>::applyTransform(const Transform<T, 2>& i_transform) {
	mBasis.applyTransform(i_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Box<T, 2>, T>::getClosestPoint(const basis_t& i_basis, const Vector<T, 2>& i_point) {
	Vector<T, 2> axes[2];
	i_basis.getAxes(axes);

	Vector<T, 2> normAxes[2];
	for (uint i = 0; i < 2; i++) {
		normAxes[i] = axes[i].normalized();
	}

	Vector<T, 2> origin = i_basis.getOrigin();

	Range<T> projRanges[2];
	T projPoints[2];
	T projOffsets[2];

	for (uint i = 0; i < 2; i++) {
		projRanges[i] = Range<T>(origin.projectionCoeff(axes[i]), (origin + axes[i]).projectionCoeff(axes[i]));
		projPoints[i] = i_point.projectionCoeff(axes[i]);
		projOffsets[i] = projRanges[i].getMean() - projPoints[i];
	}

	if (projRanges[0].containsInc(i_point.x())) {
		if (projRanges[1].containsInc(i_point.y())) {
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
		if (projRanges[1].containsInc(i_point.y())) {
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
Vector<T, 2> CollisionBasisWrapper<Box<T, 2>, T>::getClosestNormal(const basis_t& i_basis, const Vector<T, 2>& i_point, pnp_t i_policy) {
	Vector<T, 2> corners[4];
	Vector<T, 2> axes[2];
	i_basis.getAxes(axes);
	i_basis.getCorners(corners);
	Vector<T, 2> center = i_basis.getCenter();
	Vector<T, 2> offset = i_point - center;

	Range<T> range1(axes[0].dot(corners[0]), axes[0].dot(corners[3]));
	Range<T> range2(axes[1].dot(corners[0]), axes[1].dot(corners[3]));

	T pointDot1 = axes[0].dot(i_point);
	T pointDot2 = axes[1].dot(i_point);

	T offsetDot1 = axes[0].dot(offset);
	T offsetDot2 = axes[1].dot(offset);

	if (range1.containsInc(pointDot1)) {
		if (range2.containsInc(pointDot2)) {
			return (abs(offsetDot1) > abs(offsetDot2)) ?
				axes[0] * geutil::random(offsetDot1) :
				axes[1] * geutil::random(offsetDot2);
		}
		else {
			return axes[0] * geutil::random(offsetDot1);
		}
	}
	else {
		if (range2.containsInc(pointDot2)) {
			return axes[1] * geutil::random(offsetDot2);
		}
		else {
			switch (i_policy) {
			case pnp_t::zero:
				return Vector<T, 2>();
				break;
			case pnp_t::nearest_edge:
				return (abs(offsetDot1) > abs(offsetDot2)) ?
					axes[0] * geutil::random(offsetDot1) :
					axes[1] * geutil::random(offsetDot2);
				break;
			case pnp_t::towards_point: {
				uint index = geutil::minIndex({
					(i_point - corners[0]).dotSelf(),
					(i_point - corners[1]).dotSelf(),
					(i_point - corners[2]).dotSelf(),
					(i_point - corners[3]).dotSelf()
					});
				return (i_point - corners[index]).normalized();
			}
												   break;
			default:
				throw InvalidArgumentException();
			}
		}
	}
}

template<class T>
void CollisionBasisWrapper<Triangle<T, 2>, T>::applyTransform(const Transform<T, 2>& i_transform) {
	mBasis.applyTransform(i_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Triangle<T, 2>, T>::getClosestPoint(const basis_t& i_basis, const Vector<T, 2>& i_point) {
	Vector<T, 2> corners[3];
	Vector<T, 2> normals[3];
	i_basis.getPoints(corners);
	i_basis.getNormals(normals);
	Vector<T, 2> cornerNormals[3];
	Vector<T, 2> cornerTangents[3];
	bool alley[3];
	bool front[3];

	for (uint i = 0; i < 3; i++) {
		cornerNormals[i] = (normals[i] + normals[(i + 2) % 3]) / 2;
		cornerTangents[i] = cornerNormals[i].orthogonal();

		Vector<T, 2> offset = corners[(i + 1) % 3] - corners[i];
		alley[i] = geutil::betwExc(
			i_point.dot(offset),
			corners[i].dot(offset),
			corners[(i + 1) % 3].dot(offset)
		);

		front[i] = (i_point - corners[i]).dot(normals[i]) >= 0.0;

		if (front[i] && alley[i]) {
			return corners[i] + (i_point - corners[i]).projection(offset);
		}
	}

	uint index = geutil::minIndex({
			(i_point - corners[0]).dotSelf(),
			(i_point - corners[1]).dotSelf(),
			(i_point - corners[2]).dotSelf()
		});

	return corners[index];
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Triangle<T, 2>, T>::getClosestNormal(const basis_t& i_basis, const Vector<T, 2>& i_point, pnp_t i_policy) {
	Vector<T, 2> corners[3];
	Vector<T, 2> normals[3];
	i_basis.getNormals(normals);
	i_basis.getPoints(corners);
	Vector<T, 2> cornerNormals[3];
	Vector<T, 2> cornerTangents[3];
	bool slice[3];
	bool alley[3];
	bool front[3];
	for (uint i = 0; i < 3; i++) {
		cornerNormals[i] = (normals[i] + normals[(i + 2) % 3]) / 2;
		cornerTangents[i] = cornerNormals[i].orthogonal();

		slice[i] = cornerTangents[i].dot(i_point) >= cornerTangents[i].dot(corners[i]);

		Vector<T, 2> offset = corners[(i + 1) % 3] - corners[i];
		alley[i] = geutil::betwExc(
			i_point.dot(offset),
			corners[i].dot(offset),
			corners[(i + 1) % 3].dot(offset)
		);

		front[i] = (i_point - corners[i]).dot(normals[i]) >= 0.0;

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

	switch (i_policy) {
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
		uint index = geutil::minIndex({
			(i_point - corners[0]).dotSelf(),
			(i_point - corners[1]).dotSelf(),
			(i_point - corners[2]).dotSelf()
			});

		return (i_point - corners[index]).normalized();
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