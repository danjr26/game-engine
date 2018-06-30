#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vector.h"
#include "matrix.h"

class Transform {
private:
	Transform* parent;
	Matrix4d rotation;
	Matrix4d scaleTranslation;

public:
	Transform();

	Transform* Get_Parent();
	void Set_Parent(Transform* in_parent);

	Matrix4d Get_Rotation();
	void Set_Rotation(const Matrix4d& in_rotation);
	void Set_Rotation(const Vector3d& in_axis, double in_angle);
	void Set_Rotation(const Vector3d& in_xAxis, const Vector3d& in_yAxis, const Vector3d& in_zAxis);
	void Rotate(const Matrix4d& in_rotation);
	void Rotate(const Vector3d& in_axis, double in_angle);

	Vector3d Get_Scale_Factor() const;
	void Set_Scale_Factor(const Vector3d& in_factor);
	void Scale(const Vector3d& in_factor);

	Vector3d Get_Position() const;
	void Set_Position(const Vector3d& in_position);
	void Translate(const Vector3d& in_translation);

	void Invert();
	Transform Inverted() const;

	Matrix4d Get_Matrix() const;
	Matrix4d Get_World_Matrix() const;
};

#endif

