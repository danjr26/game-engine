#include "transform.h"

Transform::Transform() :
	parent(nullptr),
	rotation(Matrix4d::Identity()),
	scaleTranslation(Matrix4d::Identity())
{}

Transform* Transform::Get_Parent() {
	return parent;
}

void Transform::Set_Parent(Transform* in_parent) {
	parent = in_parent;
}

Matrix4d Transform::Get_Rotation() {
	return rotation;
}

void Transform::Set_Rotation(const Matrix4d& in_rotation) {
	rotation = in_rotation;
}

void Transform::Set_Rotation(const Vector3d& in_axis, double in_angle) {
	rotation = Matrix4d::Rotation(in_axis, in_angle);
}

void Transform::Set_Rotation(const Vector3d& in_xAxis, const Vector3d& in_yAxis, 
	const Vector3d& in_zAxis) {

	Vector3d xAxis = in_xAxis.Normalized();
	Vector3d yAxis = in_yAxis.Normalized();
	Vector3d zAxis = in_zAxis.Normalized();

	rotation = Matrix4d({
		xAxis.X(), yAxis.X(), zAxis.X(), 0.0,
		xAxis.Y(), yAxis.Y(), zAxis.Y(), 0.0,
		xAxis.Z(), yAxis.Z(), zAxis.Z(), 0.0,
		0.0, 0.0, 0.0, 1.0
		});
}

void Transform::Rotate(const Matrix4d& in_rotation) {
	rotation = in_rotation * rotation;
}

void Transform::Rotate(const Vector3d& in_axis, double in_angle) {
	rotation = Matrix4d::Rotation(in_axis, in_angle) * rotation;
}

Vector3d Transform::Get_Scale_Factor() const {
	return Vector3d(
		scaleTranslation.Element(0, 0),
		scaleTranslation.Element(1, 1),
		scaleTranslation.Element(2, 2)
	);
}

void Transform::Set_Scale_Factor(const Vector3d& in_factor) {
	scaleTranslation.Element(0, 0, in_factor.Get(0));
	scaleTranslation.Element(1, 1, in_factor.Get(1));
	scaleTranslation.Element(2, 2, in_factor.Get(2));
}

void Transform::Scale(const Vector3d& in_factor) {
	scaleTranslation.Element(0, 0, scaleTranslation.Element(0, 0) * in_factor.Get(0));
	scaleTranslation.Element(1, 1, scaleTranslation.Element(1, 1) * in_factor.Get(1));
	scaleTranslation.Element(2, 2, scaleTranslation.Element(2, 2) * in_factor.Get(2));
}

Vector3d Transform::Get_Position() const {
	return Vector3d(
		scaleTranslation.Element(0, 3),
		scaleTranslation.Element(1, 3),
		scaleTranslation.Element(2, 3)
	);
}

void Transform::Set_Position(const Vector3d& in_position) {
	scaleTranslation.Element(0, 3, in_position.X());
	scaleTranslation.Element(1, 3, in_position.Y());
	scaleTranslation.Element(2, 3, in_position.Z());
}

void Transform::Translate(const Vector3d& in_translation) {
	scaleTranslation.Element(0, 3, scaleTranslation.Element(0, 3) + in_translation.X());
	scaleTranslation.Element(1, 3, scaleTranslation.Element(1, 3) + in_translation.Y());
	scaleTranslation.Element(2, 3, scaleTranslation.Element(2, 3) + in_translation.Z());
}

void Transform::Invert() {
	Set_Position(-Get_Position());
	rotation.Transpose();
	Set_Scale_Factor(Get_Scale_Factor().Component_Inverted());
}

Transform Transform::Inverted() const {
	Transform output = (*this);
	output.Invert();
	return output;
}

Matrix4d Transform::Get_Matrix() const {
	return scaleTranslation * rotation;
}

Matrix4d Transform::Get_World_Matrix() const {
	if (parent == nullptr) {
		return Get_Matrix();
	}
	else {
		return Get_Matrix() * parent->Get_World_Matrix();
	}
}



