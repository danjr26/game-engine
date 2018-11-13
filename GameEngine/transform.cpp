#include "transform.h"
#include <vector>

template<class T, uint n>
Transform<T, n>::Transform() :
	parent(nullptr),
	translation(),
	scale(),
	rotation() {
	
	for (uint i = 0; i < n; i++) scale[i] = 1;
}

template<class T, uint n>
Transform<T, n>* Transform<T, n>::Get_Parent() {
	return parent;
}

template<class T, uint n>
Transform<T, n> const* Transform<T, n>::Get_Const_Parent() const {
	return parent;
}

template<class T, uint n>
void Transform<T, n>::Set_Parent(Transform* in_parent) {
	parent = in_parent;
}

template<class T, uint n>
bool Transform<T, n>::Is_Local_Identity() const {
	return translation.Is_Zero() && scale.Is_Zero() && rotation.Is_Identity();
}

template<class T, uint n>
bool Transform<T, n>::Is_World_Identity() const {
	for (Transform const* transform = parent; transform != nullptr; transform = transform->parent) {
		if (!transform->Is_Local_Identity()) return false;
	}
	return true;
}

template<class T, uint n>
Rotation<T, n> Transform<T, n>::Get_Local_Rotation() {
	return rotation;
}

template<class T, uint n>
Rotation<T, n> Transform<T, n>::Get_World_Rotation() {
	if (parent == nullptr) return rotation;
	else return parent->Local_To_World_Rotation(rotation);
}

template<class T, uint n>
void Transform<T, n>::Set_Local_Rotation(const Rotation<T, n>& in_rotation) {
	rotation = in_rotation;
}

template<class T, uint n>
void Transform<T, n>::Set_World_Rotation(const Rotation<T, n>& in_rotation) {
	if (parent == nullptr) rotation = in_rotation;
	else rotation = parent->World_To_Local_Rotation(in_rotation);
}

template<class T, uint n>
void Transform<T, n>::Rotate_Local(const Rotation<T, n>& in_rotation) {
	rotation = in_rotation.Followed_By(rotation);
}

template<class T, uint n>
void Transform<T, n>::Rotate_World(const Rotation<T, n>& in_rotation) {
	if (parent == nullptr) rotation = rotation.Followed_By(in_rotation);
	else rotation = rotation.Followed_By(parent->World_To_Local_Rotation(in_rotation));
}

template<class T, uint n>
void Transform<T, n>::Rotate_Local_Around_Local_Point(const Rotation<T, n>& in_rotation, const Vector<T, n>& in_point) {
	Vector<T, n> tempPoint = in_point;
	rotation = in_rotation.Followed_By(rotation);
	translation = in_rotation.Apply_To(translation - tempPoint) + tempPoint;
}

template<class T, uint n>
void Transform<T, n>::Rotate_World_Around_Local_Point(const Rotation<T, n>& in_rotation, const Vector<T, n>& in_point) {
	Vector<T, n> tempPoint = in_point;
	Rotation<T, n> tempRotation =
		(parent == nullptr) ?
		in_rotation :
		parent->World_To_Local_Rotation(in_rotation);

	rotation = rotation.Followed_By(tempRotation);
	translation = tempRotation.Apply_To(translation - tempPoint) + tempPoint;
}

template<class T, uint n>
void Transform<T, n>::Rotate_Local_Around_World_Point(const Rotation<T, n>& in_rotation, const Vector<T, n>& in_point) {
	Vector<T, n> tempPoint =
		(parent == nullptr) ?
		in_point :
		parent->World_To_Local_Point(in_point);

	rotation = in_rotation.Followed_By(rotation);
	translation = in_rotation.Apply_To(translation - tempPoint) + tempPoint;
}

template<class T, uint n>
void Transform<T, n>::Rotate_World_Around_World_Point(const Rotation<T, n>& in_rotation, const Vector<T, n>& in_point) {
	Vector<T, n> tempPoint =
		(parent == nullptr) ?
		in_point :
		parent->World_To_Local_Point(in_point);

	Rotation<T, n> tempRotation =
		(parent == nullptr) ?
		in_rotation :
		parent->World_To_Local_Rotation(in_rotation);
		
	rotation = rotation.Followed_By(tempRotation);
	translation = tempRotation.Apply_To(translation - tempPoint) + tempPoint;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::Get_Local_Scale() const {
	return scale;
}

template<class T, uint n>
void Transform<T, n>::Set_Local_Scale(const Vector<T, n>& in_factor) {
	scale = in_factor;
}

template<class T, uint n>
void Transform<T, n>::Scale_Local(const Vector<T, n>& in_factor) {
	scale = scale.Compwise(in_factor);
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::Get_Local_Position() const {
	return translation;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::Get_World_Position() const {
	return Local_To_World_Point(translation);
}

template<class T, uint n>
void Transform<T, n>::Set_Local_Position(const Vector<T, n>& in_position) {
	translation = in_position;
}

template<class T, uint n>
void Transform<T, n>::Translate_Local(const Vector<T, n>& in_translation) {
	translation += in_translation;
}

template<class T, uint n>
void Transform<T, n>::Translate_World(const Vector<T, n>& in_translation) {
	translation +=
		(parent == nullptr) ?
		in_translation :
		parent->World_To_Local_Vector(in_translation);
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::Apply_To_Local_Point(const Vector<T, n>& in_point) const {
	Vector<T, n> tempPoint = in_point;
	tempPoint = tempPoint.Compwise(scale);
	tempPoint = rotation.Apply_To(tempPoint);
	tempPoint += translation;
	return tempPoint;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::Apply_To_World_Point(const Vector<T, n>& in_point) const {
	Vector<T, n> tempPoint = World_To_Local_Point(in_point);
	tempPoint = Apply_To_Local_Point(tempPoint);
	return Local_To_World_Point(tempPoint);
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::Apply_To_Local_Direction(const Vector<T, n>& in_direction) const {
	Vector<T, n> tempDirection = in_direction;
	tempDirection = rotation.Apply_To(tempDirection);
	return tempDirection;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::Apply_To_World_Direction(const Vector<T, n>& in_direction) const {
	Vector<T, n> tempDirection = World_To_Local_Direction(in_direction);
	tempDirection = Apply_To_Local_Direction(tempDirection);
	return Local_To_World_Direction(tempDirection);
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::Apply_To_Local_Vector(const Vector<T, n>& in_vector) const {
	Vector<T, n> tempVector = in_vector;
	tempVector = tempVector.Compwise(scale);
	tempVector = rotation.Apply_To(tempVector);
	return tempVector;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::Apply_To_World_Vector(const Vector<T, n>& in_vector) const {
	Vector<T, n> tempVector = World_To_Local_Vector(in_vector);
	tempVector = Apply_To_Local_Vector(tempVector);
	return Local_To_World_Vector(tempVector);
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::World_To_Local_Point(const Vector<T, n>& in_point) const {
	std::vector<Transform*> chain;
	for (Transform* transform = parent; transform != nullptr; transform = transform->parent) {
		chain.push_back(transform);
	}

	Vector<T, n> tempPoint = in_point;
	for (int i = (int)chain.size() - 1; i >= 0; i--) {
		tempPoint -= chain[i]->translation;
		tempPoint = chain[i]->rotation.Get_Inverse().Apply_To(tempPoint);
		tempPoint = tempPoint.Compwise(chain[i]->scale.Component_Inverted());
	}
	return tempPoint;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::Local_To_World_Point(const Vector<T, n>& in_point) const {
	Vector<T, n> tempPoint = in_point;
	for (Transform const* transform = parent; transform != nullptr; transform = transform->parent) {
		tempPoint = tempPoint.Compwise(transform->scale);
		tempPoint = transform->rotation.Apply_To(tempPoint);
		tempPoint += transform->translation;
	}
	return tempPoint;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::World_To_Local_Direction(const Vector<T, n>& in_direction) const {
	std::vector<Transform*> chain;
	for (Transform* transform = parent; transform != nullptr; transform = transform->parent) {
		chain.push_back(transform);
	}

	Vector<T, n> tempDirection = in_direction;
	for (int i = (int)chain.size() - 1; i >= 0; i--) {
		tempDirection = chain[i]->rotation.Get_Inverse().Apply_To(tempDirection);
	}
	return tempDirection;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::Local_To_World_Direction(const Vector<T, n>& in_direction) const {
	Vector<T, n> tempDirection = in_direction;
	for (Transform const* transform = parent; transform != nullptr; transform = transform->parent) {
		tempDirection = transform->rotation.Apply_To(tempDirection);
	}
	return tempDirection;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::World_To_Local_Vector(const Vector<T, n>& in_vector) const {
	std::vector<Transform*> chain;
	for (Transform* transform = parent; transform != nullptr; transform = transform->parent) {
		chain.push_back(transform);
	}

	Vector<T, n> tempVector = in_vector;
	for (int i = (int)chain.size() - 1; i >= 0; i--) {
		tempVector = chain[i]->rotation.Get_Inverse().Apply_To(tempVector);
		tempVector = tempVector.Compwise(chain[i]->scale.Component_Inverted());
	}
	return tempVector;
}

template<class T, uint n>
Vector<T, n> Transform<T, n>::Local_To_World_Vector(const Vector<T, n>& in_vector) const {
	Vector<T, n> tempVector = in_vector;
	for (Transform const* transform = parent; transform != nullptr; transform = transform->parent) {
		tempVector = tempVector.Compwise(transform->scale);
		tempVector = transform->rotation.Apply_To(tempVector);
	}
	return tempVector;
}

template<class T, uint n>
Rotation<T, n> Transform<T, n>::World_To_Local_Rotation(const Rotation<T, n>& in_rotation) const {
	Rotation<T, n> tempRotation = in_rotation;
	for (Transform const* transform = parent; transform != nullptr; transform = transform->parent) {
		tempRotation = tempRotation.Followed_By(transform->rotation.Get_Inverse());
	}
	return tempRotation;
}

template<class T, uint n>
Rotation<T, n> Transform<T, n>::Local_To_World_Rotation(const Rotation<T, n>& in_rotation) const {
	Rotation<T, n> tempRotation = in_rotation;
	for (Transform const* transform = parent; transform != nullptr; transform = transform->parent) {
		tempRotation = transform->rotation.Followed_By(tempRotation);
	}
	return tempRotation;
}

template<class T, uint n>
Matrix<T, 4, 4> Transform<T, n>::Get_Local_Matrix() const {
	return Matrix<T, 4, 4>::Translation(Vector<T, 3>(translation)) * rotation.Get_Matrix() * Matrix<T, 4, 4>::Scale(Vector<T, 3>(scale));
}

template<class T, uint n>
Matrix<T, 4, 4> Transform<T, n>::Get_World_Matrix() const {
	Matrix<T, 4, 4> tempMatrix = Get_Local_Matrix();
	for (Transform const* transform = parent; transform != nullptr; transform = transform->parent) {
		tempMatrix = transform->Get_Local_Matrix() * tempMatrix;
	}
	return tempMatrix;
}

template<class T, uint n>
Matrix<T, 4, 4> Transform<T, n>::Get_Local_Inverse_Matrix() const {
	return Matrix<T, 4, 4>::Translation(-Vector<T, 3>(translation)) * rotation.Get_Inverse().Get_Matrix() * 
		Matrix<T, 4, 4>::Scale(Vector<T, 3>(scale.Component_Inverted()));
}

template<class T, uint n>
Matrix<T, 4, 4> Transform<T, n>::Get_World_Inverse_Matrix() const {
	Matrix<T, 4, 4> tempMatrix = Get_Local_Inverse_Matrix();
	for (Transform const* transform = parent; transform != nullptr; transform = transform->parent) {
		tempMatrix = transform->Get_Local_Inverse_Matrix() * tempMatrix;
	}
	return tempMatrix;
}

template class Transform<float, 2>;
template class Transform<double, 2>;
template class Transform<float, 3>;
template class Transform<double, 3>;