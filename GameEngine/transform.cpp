#include "transform.h"
#include <vector>

Transform::Transform() :
	parent(nullptr),
	translation(),
	scale(1.0, 1.0, 1.0),
	rotation() {}

Transform* Transform::Get_Parent() {
	return parent;
}

void Transform::Set_Parent(Transform* in_parent) {
	parent = in_parent;
}

bool Transform::Is_Local_Identity() const {
	return translation.Is_Zero() && scale.Is_Zero() && rotation.Is_Identity();
}

bool Transform::Is_Global_Identity() const {
	for (Transform const* transform = parent; transform != nullptr; transform = transform->parent) {
		if (!transform->Is_Local_Identity()) return false;
	}
	return true;
}

Rotationd Transform::Get_Local_Rotation() {
	return rotation;
}

Rotationd Transform::Get_World_Rotation() {
	if (parent == nullptr) return rotation;
	else return parent->Local_To_World_Rotation(rotation);
}

void Transform::Set_Local_Rotation(const Rotationd& in_rotation) {
	rotation = in_rotation;
}

void Transform::Set_World_Rotation(const Rotationd& in_rotation) {
	if (parent == nullptr) rotation = in_rotation;
	else rotation = parent->World_To_Local_Rotation(in_rotation);
}

void Transform::Rotate_Local(const Rotationd& in_rotation) {
	rotation = in_rotation.Followed_By(rotation);
}

void Transform::Rotate_World(const Rotationd& in_rotation) {
	if (parent == nullptr) rotation = rotation.Followed_By(in_rotation);
	else rotation = rotation.Followed_By(parent->World_To_Local_Rotation(in_rotation));
}

void Transform::Rotate_Local_Around_Local_Point(const Rotationd& in_rotation, const Vector3d& in_point) {
	Vector3d tempPoint = in_point;
	rotation = in_rotation.Followed_By(rotation);
	translation = in_rotation.Apply_To(translation - tempPoint) + tempPoint;
}

void Transform::Rotate_World_Around_Local_Point(const Rotationd& in_rotation, const Vector3d& in_point) {
	Vector3d tempPoint = in_point;
	Rotationd tempRotation =
		(parent == nullptr) ?
		in_rotation :
		parent->World_To_Local_Rotation(in_rotation);

	rotation = rotation.Followed_By(tempRotation);
	translation = tempRotation.Apply_To(translation - tempPoint) + tempPoint;
}

void Transform::Rotate_Local_Around_World_Point(const Rotationd& in_rotation, const Vector3d& in_point) {
	Vector3d tempPoint =
		(parent == nullptr) ?
		in_point :
		parent->World_To_Local_Point(in_point);

	rotation = in_rotation.Followed_By(rotation);
	translation = in_rotation.Apply_To(translation - tempPoint) + tempPoint;
}

void Transform::Rotate_World_Around_World_Point(const Rotationd& in_rotation, const Vector3d& in_point) {
	Vector3d tempPoint =
		(parent == nullptr) ?
		in_point :
		parent->World_To_Local_Point(in_point);

	Rotationd tempRotation =
		(parent == nullptr) ?
		in_rotation :
		parent->World_To_Local_Rotation(in_rotation);
		
	rotation = rotation.Followed_By(tempRotation);
	translation = tempRotation.Apply_To(translation - tempPoint) + tempPoint;
}

Vector3d Transform::Get_Local_Scale() const {
	return scale;
}

void Transform::Set_Local_Scale(const Vector3d& in_factor) {
	scale = in_factor;
}

void Transform::Scale_Local(const Vector3d& in_factor) {
	scale = scale.Compwise(in_factor);
}

Vector3d Transform::Get_Local_Position() const {
	return translation;
}

Vector3d Transform::Get_World_Position() const {
	return Local_To_World_Point(translation);
}

void Transform::Set_Local_Position(const Vector3d& in_position) {
	translation = in_position;
}

void Transform::Translate_Local(const Vector3d& in_translation) {
	translation += in_translation;
}

void Transform::Translate_World(const Vector3d& in_translation) {
	translation +=
		(parent == nullptr) ?
		in_translation :
		parent->World_To_Local_Vector(in_translation);
}

Vector3d Transform::Apply_To_Local_Point(const Vector3d& in_point) const {
	Vector3d tempPoint = in_point;
	tempPoint = tempPoint.Compwise(scale);
	tempPoint = rotation.Apply_To(tempPoint);
	tempPoint += translation;
	return tempPoint;
}

Vector3d Transform::Apply_To_World_Point(const Vector3d& in_point) const {
	Vector3d tempPoint = World_To_Local_Point(in_point);
	tempPoint = Apply_To_Local_Point(tempPoint);
	return Local_To_World_Point(tempPoint);
}

Vector3d Transform::Apply_To_Local_Direction(const Vector3d& in_direction) const {
	Vector3d tempDirection = in_direction;
	tempDirection = rotation.Apply_To(tempDirection);
	return tempDirection;
}

Vector3d Transform::Apply_To_World_Direction(const Vector3d& in_direction) const {
	Vector3d tempDirection = World_To_Local_Direction(in_direction);
	tempDirection = Apply_To_Local_Direction(tempDirection);
	return Local_To_World_Direction(tempDirection);
}

Vector3d Transform::Apply_To_Local_Vector(const Vector3d& in_vector) const {
	Vector3d tempVector = in_vector;
	tempVector = tempVector.Compwise(scale);
	tempVector = rotation.Apply_To(tempVector);
	return tempVector;
}

Vector3d Transform::Apply_To_World_Vector(const Vector3d& in_vector) const {
	Vector3d tempVector = World_To_Local_Vector(in_vector);
	tempVector = Apply_To_Local_Vector(tempVector);
	return Local_To_World_Vector(tempVector);
}

Vector3d Transform::World_To_Local_Point(const Vector3d& in_point) const {
	std::vector<Transform*> chain;
	for (Transform* transform = parent; transform != nullptr; transform = transform->parent) {
		chain.push_back(transform);
	}

	Vector3d tempPoint = in_point;
	for (int i = chain.size() - 1; i >= 0; i--) {
		tempPoint -= chain[i]->translation;
		tempPoint = chain[i]->rotation.Get_Inverse().Apply_To(tempPoint);
		tempPoint = tempPoint.Compwise(chain[i]->scale.Component_Inverted());
	}
	return tempPoint;
}

Vector3d Transform::Local_To_World_Point(const Vector3d& in_point) const {
	Vector3d tempPoint = in_point;
	for (Transform const* transform = parent; transform != nullptr; transform = transform->parent) {
		tempPoint = tempPoint.Compwise(transform->scale);
		tempPoint = transform->rotation.Apply_To(tempPoint);
		tempPoint += transform->translation;
	}
	return tempPoint;
}

Vector3d Transform::World_To_Local_Direction(const Vector3d& in_direction) const {
	std::vector<Transform*> chain;
	for (Transform* transform = parent; transform != nullptr; transform = transform->parent) {
		chain.push_back(transform);
	}

	Vector3d tempDirection = in_direction;
	for (int i = chain.size() - 1; i >= 0; i--) {
		tempDirection = chain[i]->rotation.Get_Inverse().Apply_To(tempDirection);
	}
	return tempDirection;
}

Vector3d Transform::Local_To_World_Direction(const Vector3d& in_direction) const {
	Vector3d tempDirection = in_direction;
	for (Transform const* transform = parent; transform != nullptr; transform = transform->parent) {
		tempDirection = transform->rotation.Apply_To(tempDirection);
	}
	return tempDirection;
}

Vector3d Transform::World_To_Local_Vector(const Vector3d& in_vector) const {
	std::vector<Transform*> chain;
	for (Transform* transform = parent; transform != nullptr; transform = transform->parent) {
		chain.push_back(transform);
	}

	Vector3d tempVector = in_vector;
	for (int i = chain.size() - 1; i >= 0; i--) {
		tempVector = chain[i]->rotation.Get_Inverse().Apply_To(tempVector);
		tempVector = tempVector.Compwise(chain[i]->scale.Component_Inverted());
	}
	return tempVector;
}

Vector3d Transform::Local_To_World_Vector(const Vector3d & in_vector) const {
	Vector3d tempVector = in_vector;
	for (Transform const* transform = parent; transform != nullptr; transform = transform->parent) {
		tempVector = tempVector.Compwise(transform->scale);
		tempVector = transform->rotation.Apply_To(tempVector);
	}
	return tempVector;
}

Rotationd Transform::World_To_Local_Rotation(const Rotationd& in_rotation) const {
	Rotationd tempRotation = in_rotation;
	for (Transform const* transform = parent; transform != nullptr; transform = transform->parent) {
		tempRotation = tempRotation.Followed_By(transform->rotation.Get_Inverse());
	}
	return tempRotation;
}

Rotationd Transform::Local_To_World_Rotation(const Rotationd& in_rotation) const {
	Rotationd tempRotation = in_rotation;
	for (Transform const* transform = parent; transform != nullptr; transform = transform->parent) {
		tempRotation = transform->rotation.Followed_By(tempRotation);
	}
	return tempRotation;
}

Matrix4d Transform::Get_Local_Matrix() const {
	return Matrix4d::Translation(translation) * rotation.Get_Matrix() * Matrix4d::Scale(scale);
}

Matrix4d Transform::Get_World_Matrix() const {
	Matrix4d tempMatrix = Get_Local_Matrix();
	for (Transform const* transform = parent; transform != nullptr; transform = transform->parent) {
		tempMatrix = transform->Get_Local_Matrix() * tempMatrix;
	}
	return tempMatrix;
}

Matrix4d Transform::Get_Local_Inverse_Matrix() const {
	return Matrix4d::Translation(-translation) * rotation.Get_Inverse().Get_Matrix() * 
		Matrix4d::Scale(scale.Component_Inverted());
}

Matrix4d Transform::Get_World_Inverse_Matrix() const {
	Matrix4d tempMatrix = Get_Local_Inverse_Matrix();
	for (Transform const* transform = parent; transform != nullptr; transform = transform->parent) {
		tempMatrix = transform->Get_Local_Inverse_Matrix() * tempMatrix;
	}
	return tempMatrix;
}
