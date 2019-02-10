#include "collision_basis_wrapper.h"

template<class T>
void CollisionBasisWrapper<AxisAlignedHalfSpace<T, 2>, T>::Apply_Transform(const Transform<T, 2>& in_transform) {
	mBasis.Apply_Transform(in_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<AxisAlignedHalfSpace<T, 2>, T>::Get_Closest_Point(const basis_t& in_basis, const Vector<T, 2>& in_point) {
	Vector<T, 2> out = in_point;
	out[in_basis.Get_Dimension()] = in_basis.Get_Value();
	return out;
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<AxisAlignedHalfSpace<T, 2>, T>::Get_Closest_Normal(const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy) {
	return -in_basis.Get_Direction();
}

template<class T>
void CollisionBasisWrapper<AxisAlignedLine<T, 2>, T>::Apply_Transform(const Transform<T, 2>& in_transform) {
	mBasis.Apply_Transform(in_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<AxisAlignedLine<T, 2>, T>::Get_Closest_Point(const basis_t& in_basis, const Vector<T, 2>& in_point) {
	Vector<T, 2> out = in_point;
	out[in_basis.Get_Dimension()] = in_basis.Get_Value();
	return out;
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<AxisAlignedLine<T, 2>, T>::Get_Closest_Normal(const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy) {
	return in_basis.Get_Direction().Orthogonal();
}

template<class T>
void CollisionBasisWrapper<AxisAlignedBox<T, 2>, T>::Apply_Transform(const Transform<T, 2>& in_transform) {
	mBasis.Apply_Transform(in_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<AxisAlignedBox<T, 2>, T>::Get_Closest_Point(const basis_t& in_basis, const Vector<T, 2>& in_point) {
	Vector<T, 2> center = in_basis.Get_Center();
	Vector<T, 2> offset = in_point - center;

	Vector<T, 2> minima = in_basis.Get_Minima();
	Vector<T, 2> maxima = in_basis.Get_Maxima();

	Range<T> xRange(minima.X(), maxima.X());
	Range<T> yRange(minima.Y(), maxima.Y());

	if (xRange.Contains_Inc(in_point.X())) {
		if (yRange.Contains_Inc(in_point.Y())) {
			if(abs(offset.X()) > abs(offset.Y())) {
				return Vector<T, 2>(
					(offset.X() > 0) ? maxima.X() : minima.X(),
					in_point.Y()
					);
			}
			else {
				return Vector<T, 2>(
					in_point.X(),
					(offset.Y() > 0) ? maxima.Y() : minima.Y()
					);
			}
		}
		else {
			return Vector<T, 2>(
				in_point.X(),
				(offset.Y() > 0) ? maxima.Y() : minima.Y()
				);
		}
	}
	else {
		if (yRange.Contains_Inc(in_point.Y())) {
			return Vector<T, 2>(
				(offset.X() > 0) ? maxima.X() : minima.X(),
				in_point.Y()
				);
		}
		else {
			return Vector<T, 2>(
				(offset.X() > 0) ? maxima.X() : minima.X(),
				(offset.Y() > 0) ? maxima.Y() : minima.Y()
				);
		}
	}
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<AxisAlignedBox<T, 2>, T>::Get_Closest_Normal(const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy){
	Vector<T, 2> center = in_basis.Get_Center();
	Vector<T, 2> offset = in_point - center;

	Vector<T, 2> minima = in_basis.Get_Minima();
	Vector<T, 2> maxima = in_basis.Get_Maxima();

	Range<T> xRange(minima.X(), maxima.X());
	Range<T> yRange(minima.Y(), maxima.Y());

	if (xRange.Contains_Inc(in_point.X()) || yRange.Contains_Inc(in_point.Y())) {
		return (abs(offset.X()) > abs(offset.Y())) ?
			Vector<T, 2>(1, 0) * Sign(offset.X()) :
			Vector<T, 2>(0, 1) * Sign(offset.Y());
	}

	Vector<T, 2> corners[4];
	in_basis.Get_Corners(corners);

	switch (in_policy) {
	case pnp_t::zero:
		return Vector<T, 2>();
		break;
	case pnp_t::nearest_edge:
		return (abs(offset.X()) > abs(offset.Y())) ?
			Vector<T, 2>(1, 0) * Sign(offset.X()) :
			Vector<T, 2>(0, 1) * Sign(offset.Y());
		break;
	case pnp_t::towards_point: {
		uint index = Min_Index({
			(in_point - corners[0]).Dot_Self(),
			(in_point - corners[1]).Dot_Self(),
			(in_point - corners[2]).Dot_Self(),
			(in_point - corners[3]).Dot_Self()
			});
		return (in_point - corners[index]).Normalized();
	}
										   break;
	default:
		throw InvalidArgumentException();
	}
}

template<class T>
void CollisionBasisWrapper<Sphere<T, 2>, T>::Apply_Transform(const Transform<T, 2>& in_transform) {
	mBasis.Apply_Transform(in_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Sphere<T, 2>, T>::Get_Closest_Point(const basis_t& in_basis, const Vector<T, 2>& in_point) {
	return (in_point - in_basis.Get_Center()).Normalized() * in_basis.Get_Radius();
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Sphere<T, 2>, T>::Get_Closest_Normal(const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy) {
	return (in_point - in_basis.Get_Center()).Normalized();
}

template<class T>
void CollisionBasisWrapper<HalfSpace<T, 2>, T>::Apply_Transform(const Transform<T, 2>& in_transform) {
	mBasis.Apply_Transform(in_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<HalfSpace<T, 2>, T>::Get_Closest_Point(const basis_t& in_basis, const Vector<T, 2>& in_point) {
	return in_point.Projection(in_basis.Get_Direction().Orthogonal()) + in_basis.Get_Point();
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<HalfSpace<T, 2>, T>::Get_Closest_Normal(const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy) {
	return -in_basis.Get_Direction();
}

template<class T>
void CollisionBasisWrapper<LineSegment<T, 2>, T>::Apply_Transform(const Transform<T, 2>& in_transform) {
	mBasis.Apply_Transform(in_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<LineSegment<T, 2>, T>::Get_Closest_Point(const basis_t& in_basis, const Vector<T, 2>& in_point) {
	T proj1 = in_basis.Get_Projection_Coefficient1();
	T proj2 = in_basis.Get_Projection_Coefficient2();
	T projPoint = in_basis.Get_Projection_Coefficient(in_point);

	if (projPoint <= proj1) {
		return in_basis.Get_Point1();
	}
	if (projPoint >= proj2) {
		return in_basis.Get_Point2();
	}

	return in_basis.Get_Projection(in_point);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<LineSegment<T, 2>, T>::Get_Closest_Normal(const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy) {
	Vector<T, 2> normal = in_basis.Get_Direction().Orthogonal();
	Vector<T, 2> points[2] = {
		in_basis.Get_Point1(),
		in_basis.Get_Point2()
	};
	Vector<T, 2> direction = in_basis.Get_Direction();

	if (Between_Inc<T>(
		(in_point - points[0]).Dot(direction),
		0, (points[1] - points[0]).Dot(direction)
		)) {

		return (normal.Dot(in_point) >= normal.Dot(points[0])) ? normal : -normal;
	}
	switch (in_policy) {
	case pnp_t::zero:
		return Vector<T, 2>();
		break;
	case pnp_t::nearest_edge:
		return (normal.Dot(in_point) >= normal.Dot(points[0])) ? normal : -normal;
		break;
	case pnp_t::towards_point: {
		uint index = Min_Index({
			(in_point - points[0]).Dot_Self(),
			(in_point - points[1]).Dot_Self(),
			});
		return (in_point - points[index]).Normalized();
	}
		break;
	default:
		throw InvalidArgumentException();
	}
}

template<class T>
void CollisionBasisWrapper<Line<T, 2>, T>::Apply_Transform(const Transform<T, 2>& in_transform) {
	mBasis.Apply_Transform(in_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Line<T, 2>, T>::Get_Closest_Point(const basis_t& in_basis, const Vector<T, 2>& in_point) {
	return in_basis.Get_Projection(in_point);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Line<T, 2>, T>::Get_Closest_Normal(const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy) {
	Vector<T, 2> normal = in_basis.Get_Direction().Orthogonal();
	return (normal.Dot(in_point) >= normal.Dot(in_basis.Get_Point())) ? normal : -normal;
}

template<class T>
void CollisionBasisWrapper<MeshSphereTree<T, 2>, T>::Apply_Transform(const Transform<T, 2>& in_transform) {
	mBasis.Apply_Transform(in_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<MeshSphereTree<T, 2>, T>::Get_Closest_Point(const basis_t& in_basis, const Vector<T, 2>& in_point) {
	using iter_t = typename MeshSphereTree<T, 2>::Iterator;
	using Triangle2Wrapper = CollisionBasisWrapper<Triangle<T, 2>, T>;

	std::vector<iter_t> itStack;
	std::vector<iter_t> leaves;
	itStack.reserve(in_basis.Get_Mesh_Data().Get_Number_Faces() / 2);
	leaves.reserve(in_basis.Get_Mesh_Data().Get_Number_Faces());
	itStack.push_back(in_basis.Get_Iterator());
	while (!itStack.empty()) {
		iter_t it = itStack.back();
		itStack.pop_back();

		if (it.Is_Leaf()) {
			leaves.push_back(it);
		}
		else {
			itStack.push_back(it.Go_Both());
			itStack.push_back(it);
		}
	}

	auto bestIt = leaves.begin();
	T bestDistance = std::numeric_limits<T>::infinity();
	for (auto it = leaves.begin(); it != leaves.end(); it++) {
		T distance = (it->Get_Sphere().Get_Center() - in_point).Dot_Self();
		if (distance < bestDistance) {
			distance = bestDistance;
			bestIt = it;
		}
	}

	Vector<T, 2> bestPoint = Triangle2Wrapper::Get_Closest_Point(bestIt->Get_Triangle(), in_point);
	bestDistance = (bestPoint - in_point).Magnitude();
	for (auto it = leaves.begin(); it != leaves.end(); it++) {
		T distance = (it->Get_Sphere().Get_Center() - in_point).Magnitude() - it->Get_Sphere().Get_Radius();
		if (distance < bestDistance) {
			Vector<T, 2> point = Triangle2Wrapper::Get_Closest_Point(it->Get_Triangle(), in_point);
			distance = (point - in_point).Magnitude();
			if (distance < bestDistance) {
				bestPoint = point;
				bestIt = it;
			}
		}
	}

	return bestPoint;
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<MeshSphereTree<T, 2>, T>::Get_Closest_Normal(const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy) {
	using iter_t = typename MeshSphereTree<T, 2>::Iterator;
	using Triangle2Wrapper = CollisionBasisWrapper<Triangle<T, 2>, T>;

	std::vector<iter_t> itStack;
	std::vector<iter_t> leaves;
	itStack.reserve(in_basis.Get_Mesh_Data().Get_Number_Faces() / 2);
	leaves.reserve(in_basis.Get_Mesh_Data().Get_Number_Faces());
	itStack.push_back(in_basis.Get_Iterator());
	while (!itStack.empty()) {
		iter_t it = itStack.back();
		itStack.pop_back();

		if (it.Is_Leaf()) {
			leaves.push_back(it);
		}
		else {
			itStack.push_back(it.Go_Both());
			itStack.push_back(it);
		}
	}

	auto bestIt = leaves.begin();
	T bestDistance = std::numeric_limits<T>::infinity();
	for (auto it = leaves.begin(); it != leaves.end(); it++) {
		T distance = (it->Get_Sphere().Get_Center() - in_point).Dot_Self();
		if (distance < bestDistance) {
			distance = bestDistance;
			bestIt = it;
		}
	}

	Vector<T, 2> bestPoint = Triangle2Wrapper::Get_Closest_Point(bestIt->Get_Triangle(), in_point);
	bestDistance = (bestPoint - in_point).Magnitude();
	for (auto it = leaves.begin(); it != leaves.end(); it++) {
		T distance = (it->Get_Sphere().Get_Center() - in_point).Magnitude() - it->Get_Sphere().Get_Radius();
		if (distance < bestDistance) {
			Vector<T, 2> point = Triangle2Wrapper::Get_Closest_Point(it->Get_Triangle(), in_point);
			distance = (point - in_point).Magnitude();
			if (distance < bestDistance) {
				bestPoint = point;
				bestIt = it;
			}
		}
	}

	return Triangle2Wrapper::Get_Closest_Normal(bestIt->Get_Triangle(), in_point, in_policy);
}

template<class T>
void CollisionBasisWrapper<Vector<T, 2>, T>::Apply_Transform(const Transform<T, 2>& in_transform) {
	mBasis = in_transform.Local_To_World_Point(mBasis);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Vector<T, 2>, T>::Get_Closest_Point(const basis_t& in_basis, const Vector<T, 2>& in_point) {
	return in_basis;
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Vector<T, 2>, T>::Get_Closest_Normal(const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy) {
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
void CollisionBasisWrapper<Ray<T, 2>, T>::Apply_Transform(const Transform<T, 2>& in_transform) {
	mBasis.Apply_Transform(in_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Ray<T, 2>, T>::Get_Closest_Point(const basis_t& in_basis, const Vector<T, 2>& in_point) {
	T projRay = in_basis.Get_Projection_Coefficient();
	T projPoint = in_basis.Get_Projection_Coefficient(in_point);

	if (projPoint <= projRay) {
		return in_basis.Get_Point();
	}

	return in_basis.Get_Projection(in_point);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Ray<T, 2>, T>::Get_Closest_Normal(const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy) {
	Vector<T, 2> normal = in_basis.Get_Direction().Orthogonal();
	if ((in_point - in_basis.Get_Point()).Dot(in_basis.Get_Direction()) >= 0) {
		(normal.Dot(in_point) >= normal.Dot(in_basis.Get_Point())) ? normal : -normal;
	}
	switch (in_policy) {
	case pnp_t::zero:
		return Vector<T, 2>();
		break;
	case pnp_t::nearest_edge:
		return (normal.Dot(in_point) >= normal.Dot(in_basis.Get_Point())) ? normal : -normal;
		break;
	case pnp_t::towards_point:
		return (in_point - in_basis.Get_Point()).Normalized();
		break;
	default:
		throw InvalidArgumentException();
	}
}

template<class T>
void CollisionBasisWrapper<Box<T, 2>, T>::Apply_Transform(const Transform<T, 2>& in_transform) {
	mBasis.Apply_Transform(in_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Box<T, 2>, T>::Get_Closest_Point(const basis_t& in_basis, const Vector<T, 2>& in_point) {
	Vector<T, 2> axes[2];
	in_basis.Get_Axes(axes);

	Vector<T, 2> normAxes[2];
	for (uint i = 0; i < 2; i++) {
		normAxes[i] = axes[i].Normalized();
	}

	Vector<T, 2> origin = in_basis.Get_Origin();

	Range<T> projRanges[2];
	T projPoints[2];
	T projOffsets[2];

	for (uint i = 0; i < 2; i++) {
		projRanges[i] = Range<T>(origin.Projection_Coeff(axes[i]), (origin + axes[i]).Projection_Coeff(axes[i]));
		projPoints[i] = in_point.Projection_Coeff(axes[i]);
		projOffsets[i] = projRanges[i].Get_Mean() - projPoints[i];
	}

	if (projRanges[0].Contains_Inc(in_point.X())) {
		if (projRanges[1].Contains_Inc(in_point.Y())) {
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
		if (projRanges[1].Contains_Inc(in_point.Y())) {
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
Vector<T, 2> CollisionBasisWrapper<Box<T, 2>, T>::Get_Closest_Normal(const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy) {
	Vector<T, 2> corners[4];
	Vector<T, 2> axes[2];
	in_basis.Get_Axes(axes);
	in_basis.Get_Corners(corners);
	Vector<T, 2> center = in_basis.Get_Center();
	Vector<T, 2> offset = in_point - center;

	Range<T> range1(axes[0].Dot(corners[0]), axes[0].Dot(corners[3]));
	Range<T> range2(axes[1].Dot(corners[0]), axes[1].Dot(corners[3]));

	T pointDot1 = axes[0].Dot(in_point);
	T pointDot2 = axes[1].Dot(in_point);

	T offsetDot1 = axes[0].Dot(offset);
	T offsetDot2 = axes[1].Dot(offset);

	if (range1.Contains_Inc(pointDot1)) {
		if (range2.Contains_Inc(pointDot2)) {
			return (abs(offsetDot1) > abs(offsetDot2)) ?
				axes[0] * Sign(offsetDot1) :
				axes[1] * Sign(offsetDot2);
		}
		else {
			return axes[0] * Sign(offsetDot1);
		}
	}
	else {
		if (range2.Contains_Inc(pointDot2)) {
			return axes[1] * Sign(offsetDot2);
		}
		else {
			switch (in_policy) {
			case pnp_t::zero:
				return Vector<T, 2>();
				break;
			case pnp_t::nearest_edge:
				return (abs(offsetDot1) > abs(offsetDot2)) ?
					axes[0] * Sign(offsetDot1) :
					axes[1] * Sign(offsetDot2);
				break;
			case pnp_t::towards_point: {
				uint index = Min_Index({
					(in_point - corners[0]).Dot_Self(),
					(in_point - corners[1]).Dot_Self(),
					(in_point - corners[2]).Dot_Self(),
					(in_point - corners[3]).Dot_Self()
					});
				return (in_point - corners[index]).Normalized();
			}
												   break;
			default:
				throw InvalidArgumentException();
			}
		}
	}
}

template<class T>
void CollisionBasisWrapper<Triangle<T, 2>, T>::Apply_Transform(const Transform<T, 2>& in_transform) {
	mBasis.Apply_Transform(in_transform);
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Triangle<T, 2>, T>::Get_Closest_Point(const basis_t& in_basis, const Vector<T, 2>& in_point) {
	Vector<T, 2> corners[3];
	Vector<T, 2> normals[3];
	in_basis.Get_Points(corners);
	in_basis.Get_Normals(normals);
	Vector<T, 2> cornerNormals[3];
	Vector<T, 2> cornerTangents[3];
	bool alley[3];
	bool front[3];

	for (uint i = 0; i < 3; i++) {
		cornerNormals[i] = (normals[i] + normals[(i + 2) % 3]) / 2;
		cornerTangents[i] = cornerNormals[i].Orthogonal();

		Vector<T, 2> offset = corners[(i + 1) % 3] - corners[i];
		alley[i] = Between_Exc(
			in_point.Dot(offset),
			corners[i].Dot(offset),
			corners[(i + 1) % 3].Dot(offset)
		);

		front[i] = (in_point - corners[i]).Dot(normals[i]) >= 0.0;

		if (front[i] && alley[i]) {
			return corners[i] + (in_point - corners[i]).Projection(offset);
		}
	}

	uint index = Min_Index({
			(in_point - corners[0]).Dot_Self(),
			(in_point - corners[1]).Dot_Self(),
			(in_point - corners[2]).Dot_Self()
		});

	return corners[index];
}

template<class T>
Vector<T, 2> CollisionBasisWrapper<Triangle<T, 2>, T>::Get_Closest_Normal(const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy) {
	Vector<T, 2> corners[3];
	Vector<T, 2> normals[3];
	in_basis.Get_Normals(normals);
	in_basis.Get_Points(corners);
	Vector<T, 2> cornerNormals[3];
	Vector<T, 2> cornerTangents[3];
	bool slice[3];
	bool alley[3];
	bool front[3];
	for (uint i = 0; i < 3; i++) {
		cornerNormals[i] = (normals[i] + normals[(i + 2) % 3]) / 2;
		cornerTangents[i] = cornerNormals[i].Orthogonal();

		slice[i] = cornerTangents[i].Dot(in_point) >= cornerTangents[i].Dot(corners[i]);

		Vector<T, 2> offset = corners[(i + 1) % 3] - corners[i];
		alley[i] = Between_Exc(
			in_point.Dot(offset),
			corners[i].Dot(offset),
			corners[(i + 1) % 3].Dot(offset)
		);

		front[i] = (in_point - corners[i]).Dot(normals[i]) >= 0.0;

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
		uint index = Min_Index({
			(in_point - corners[0]).Dot_Self(),
			(in_point - corners[1]).Dot_Self(),
			(in_point - corners[2]).Dot_Self()
			});

		return (in_point - corners[index]).Normalized();
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