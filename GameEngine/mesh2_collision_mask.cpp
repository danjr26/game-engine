#include "mesh2_collision_mask.h"

template<class T>
Mesh2CollisionMask<T>::Mesh2CollisionMask(const MeshVertexData& in_mesh, bool in_ignoreTransform) :
	CollisionMask<T, 2>(in_ignoreTransform),
	mesh(in_mesh)
{}

template<class T>
MeshVertexData& Mesh2CollisionMask<T>::Get_Mesh() {
	return mesh;
}

template<class T>
MeshVertexData Mesh2CollisionMask<T>::Get_Transformed_Mesh() const {
	if (ignoreTransform) {
		return mesh;
	}
	else {
		MeshVertexData out = mesh;
		out.Apply_Transform_Points(transform, out.Get_Member_Index_By_ID(MeshVertexData::position));
		return out;
	}
}

template<class T>
void Mesh2CollisionMask<T>::Apply_Transform() {
	mesh.Apply_Transform_Points(transform, mesh.Get_Member_Index_By_ID(MeshVertexData::position));
}

template<class T>
Mesh2CollisionMask<T>* Mesh2CollisionMask<T>::Clone() const {
	return new Mesh2CollisionMask<T>(*this);
}

template<class T>
Vector<T, 2> Mesh2CollisionMask<T>::Get_Closest_Point(const Vector<T, 2>& in_point) const {
	throw NotImplementedException();
	return Vector<T, 2>();
}

template<class T>
Vector<T, 2> Mesh2CollisionMask<T>::Get_Closest_Normal(const Vector<T, 2>& in_point, PointNormalPolicy in_policy) const {
	throw NotImplementedException();
	return Vector<T, 2>();
}

template<class T>
Collision<T, 2> Mesh2CollisionMask<T>::Accept_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CollisionMask<T, 2>& in_other) {
	return in_other.Accept_Secondhand_Evaluator(in_evaluator, *this);
}

template<class T>
Collision<T, 2> Mesh2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedHalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Mesh2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedLine2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Mesh2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedRectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Mesh2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CircleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Mesh2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, HalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Mesh2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Line2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Mesh2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, LineSegment2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Mesh2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Mesh2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> Mesh2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Point2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> Mesh2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Ray2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> Mesh2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, RectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> Mesh2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Triangle2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}



template class Mesh2CollisionMask<float>;
template class Mesh2CollisionMask<double>;